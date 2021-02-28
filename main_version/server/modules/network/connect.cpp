#include "connect.hpp"
using std::cin;
using std::cout;
using std::endl;

void Work(Sconnect &con)
{
	cout << "server running\n";
	con.work();
}

Sconnect::Sconnect()
{
	cout << "[success] network constructor running\n";
	this->init();
	this->acnt = new Maccount;
	this->msg = new Message;
	this->acc_mag = new database("account_manage");
	this->acnt->set_db(this->acc_mag);
}

Sconnect::~Sconnect()
{
	close(tcp_fd);
	close(epoll_fd);
	delete this->acnt;
	delete this->msg;
	delete this->acc_mag;
	cout << "[success] network distructor running\n";
}

void Sconnect::init()
{
	int ret = 0;
	sockaddr_in tcp_addr;
	tcp_fd = socket(AF_INET,SOCK_STREAM,0);
	if(tcp_fd < 0)
	{
		perror(strerror(errno));
	}
	
	tcp_addr.sin_family = AF_INET;
	tcp_addr.sin_port   = htons(SERVER_PORT);
	tcp_addr.sin_addr.s_addr = 0;		// 本机的ip都绑定
	
	//设置端口复用
    int opt = 1;
    setsockopt(tcp_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));

	// 绑定本机IP
	bind(tcp_fd, (sockaddr*)&tcp_addr, sizeof(tcp_addr));

	epoll_fd = epoll_create(1);
	if(epoll_fd < 0)
	{
		close(tcp_fd);
		goto DISABLE;
	}
	
	ev.events = EPOLLIN;
	ev.data.fd = tcp_fd;
	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_fd, &ev) < 0)
	{
		close(tcp_fd);
		close(epoll_fd);
		goto DISABLE;
	}

	// 创建管道
	ret = mkfifo("send_to_client", 0666);
	if(ret < 0)
	{
		cout << "[failed] fifo exist\n";
	}

	enable = true;
	DEBUG("服务器初始化成功");
	return;
DISABLE:
	perror("连接初始化失败");
	enable = false;
	exit(-1);
}

void Sconnect::set_tcp_fd(int fd)
{
	tcp_fd = fd;
	return;
}

void Sconnect::set_udp_fd(int fd)
{
	udp_fd = fd;
	return;
}

#if 1
void Receive(Sconnect *con)
{
	listen(con->tcp_fd, 128);
	while(1)
	{
		int n = epoll_wait(con->epoll_fd, con->evs, 2048, -1);
		if(n >= 0)
		{
			for(int i = 0; i < n; i ++)
			{
				int fd = con->evs[i].data.fd;
				// 如果是epoll_fd并且是读事件
				if(fd != con->tcp_fd && con->evs[i].events&EPOLLIN)
				{
					char _msg[1500] = {};
					int ret = read(fd, _msg, sizeof(_msg));
					if(ret > 0)
					{
						cout << "connect fd: " << fd << endl;
						/*for(int i = 0; i < ret; i ++)
						    printf(" %d", _msg[i]);
						cout << endl;*/
						
						protocol pro;
						memcpy(&pro, _msg, sizeof(protocol));

						if(pro.data_type & ACCOUNT_INFO)
						{
							cout << "the type of account data..." << endl;
							con->acnt->work(pro.msg_type, _msg+sizeof(protocol), fd);
						}
						else if(pro.data_type & MESSAGE)
						{
							cout << "the type of message data..." << endl;
							con->acnt->work(pro.msg_type, _msg+sizeof(protocol), fd);
						}
						else
						{
							cout << "unknow message\n";
						}
						//错误处理 todo
					}
					// 主动关闭连接的那一方会发送一个0长数据包
					else
					{
					    for(auto it = con->acnt->onlines.begin(); it != con->acnt->onlines.end(); it ++)
					    {
					        if(it->second->fd == fd)
					        {
					            account_info _tmp;
					            memset((void*)&_tmp, 0, sizeof(account_info));
					            memcpy((void*)&_tmp, (void*)&it->second->account_info, sizeof(acc));
						        con->acnt->logout(&_tmp, it->second->fd);
						        break;
					        }
					    }
						
						close(fd);
						epoll_ctl(con->epoll_fd, EPOLL_CTL_DEL, fd, &(con->evs[i]));
						printf("客户端 %d 已关闭连接\n", fd);
					}
				}
				else if(fd == con->tcp_fd && con->evs[i].events&EPOLLIN)
				{
					sockaddr_in cli;
					socklen_t len = sizeof(cli);
					char ip[16] = {};
					int cfd = con->Accept(con->tcp_fd, (sockaddr*)&cli, &len);
					printf("client: %s-%d...connect\n",
							inet_ntop(AF_INET, &cli.sin_addr.s_addr, ip, 16),
							ntohs(cli.sin_port));
					
					con->ev.data.fd = cfd;
					con->ev.events = EPOLLIN;
					epoll_ctl(con->epoll_fd, EPOLL_CTL_ADD, cfd, &(con->ev));
				}
			}
		}
	}
}

void Sendto(Sconnect *con)
{
	int read_fd = open("send_to_client", O_RDONLY);
	if(read_fd < 0)
	{
		cout << "[failed] open fifo_read failed" << endl;
		exit(-1);
	}
	else
		std::cout << "[success] open fifo_read success" << std::endl;

	while(1)
	{
		sleep(1);
		char buf[1400] = {};
		memset(buf, 0, sizeof(buf));
		int ret = read(read_fd, buf, sizeof(buf));
		if(ret > 0)
		{
			int send_fd = 0; 
			memcpy((void*)&send_fd, buf, sizeof(int));
			cout << "send fd: " << send_fd << endl;
			/*for(int i = 0; i < ret; i ++)
				printf("%d ", buf[i]);
			cout << endl;*/

			int _ret = write(send_fd, buf+4, ret);
			if(_ret > 0)
				cout << "[success] send a message to client\n";
			else
			{
				perror("");
				cout << "[failed] a message send to client failed\n";
			}
		}
	}
}
#endif

void Sconnect::work()
{
	std::thread _read(Receive, this);
	std::thread _send(Sendto, this);
	_read.join();
	_send.detach();
}

int Sconnect::Accept(int fd, sockaddr *sa, socklen_t *salenptr)
{
	int n;
again:
	if((n = accept(fd, sa, salenptr)) < 0)
	{
		if((errno == ECONNABORTED) || (errno == EINTR))
			goto again;
		else
		{
			DEBUG("提取失败");
			exit(-1);
		}
	}
	return n;
}
