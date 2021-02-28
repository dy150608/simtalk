#include "account_manage.hpp"

Maccount::Maccount()
{
	DEBUG("帐号管理器创建成功");
}

Maccount::~Maccount()
{
	for(auto it = onlines.begin(); it != onlines.end(); it ++)
	{
		acc_info *user = it->second;
		onlines.erase(it);
		delete user;
	}
	std::cout << "[success] account manage distructor running" << std::endl;
}

bool fetch_name_exsit(MYSQL_RES *result, void *container)
{
	MYSQL_ROW sql_row;
	while(sql_row = mysql_fetch_row(result))
		std::cout << sql_row[0] << std::endl;
	return true;
}
bool Maccount::registered(account_info *acnt, int fd)
{
	char sql[256] = {};
	char re[16] = {};
	char msg[1500] = {};
	int len = 0, write_fd = 0, ret = 0;
	protocol pro;
	memset((void*)&pro, 0, sizeof(protocol));

	/*到数据库中搜索是否有同名用户*/
	sprintf(sql, "select account_name from account_info where account_name='%s'", acnt->name);
	if(!this->db->query(sql, fetch_name_exsit, nullptr))
	{
		strcpy(re, "failed");
		goto FAILED;
	}
	/*生成帐号*/
	strncpy(this->number_id, this->generate_num(this->number_id), MAX_ACCOUNT_LEN);
	strncpy(acnt->id, this->number_id, MAX_ACCOUNT_LEN);
	std::cout << "current id: " << this->number_id << std::endl;

	/*将新用户添加到数据库*/
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "insert into account_info (account_num, account_name, account_type) values ('%s', '%s', %d)", acnt->id, acnt->name, acnt->type);
	/*如果添加失败，回退生成的帐号*/
	if(!this->db->modify(sql))
	{
		strcpy(re, "failed");
		this->number -= 1;
		memset(this->number_id, 0, sizeof(this->number_id));
		sprintf(this->number_id, "%d", this->number);
		goto FAILED;
	}
	memset(re, 0, sizeof(re));
	strcpy(re, "success");

FAILED:
	/*将注册信息返回到客户端*/
	pro.data_type = ACCOUNT_INFO;
	pro.msg_type  = REGISTER_ACCOUNT;
	pro.msg_len   = sizeof(account_info) + sizeof(re);

	memcpy(msg, &fd, sizeof(int));
	len += sizeof(int);
	memcpy(msg+len, &pro, sizeof(protocol));
	len += sizeof(protocol);
	memcpy(msg+len, acnt, sizeof(account_info));
	len += sizeof(account_info);
	memcpy(msg+len, re, sizeof(re));
	len += sizeof(re);
	/*for(int i = 0; i < len; i ++)
		printf("%d ", msg[i]);
	std::cout << std::endl;*/

	/*将要发送的内容发送到管道*/
	write_fd = open("send_to_client", O_WRONLY);
	if(write_fd < 0)
	{
		DEBUG("[failed] open fifo_write failed");
		return false;
	}
	ret = write(write_fd, msg, len);
	if(ret < 0)
	{
		close(write_fd);
		DEBUG("[failed] a message send to fifo failed");
		return false;
	}
	close(write_fd);
	return true;
}

/*登录查询回调*/
bool fetch_account_info(MYSQL_RES *result, void *container)
{
	MYSQL_ROW sql_row;
	if(sql_row = mysql_fetch_row(result))
	{
		memcpy(((account_info*)container)->id, sql_row[0], MAX_ACCOUNT_LEN);
		memcpy(((account_info*)container)->name, sql_row[1], MAX_USERNAME_LEN);
		return true;
	}
	else
	{
		std::cout << "null result" << std::endl;
		return false;
	}
}
bool Maccount::login(account_info *acnt, int fd)
{
	/*查询帐号是否正确*/
	char sql[256] = {};
	char re[16] = {};
	acc_info *user;
	protocol pro;
	char msg[1500] = {};
	int len = 0;
	int write_fd, ret;
	sprintf(sql, "select account_num, account_name from account_info where account_num='%s' or account_name='%s' limit 1", acnt->name, acnt->name);
	if(!this->db->query(sql, fetch_account_info, (void*)acnt))
    {
        strcpy(re, "failed");
        goto FAILED;
    }
	else
		strcpy(re, "success");
	std::cout << acnt->id << ' ' << acnt->name << std::endl;

	/*将在线用户信息一起存放*/
	user = new acc_info;
	strncpy(user->account_info.id, acnt->id, MAX_ACCOUNT_LEN);
	strncpy(user->account_info.name, acnt->name, MAX_USERNAME_LEN);
	user->fd = fd;
	ins = this->onlines.insert(std::pair<std::string, acc_info*>(std::string(acnt->id), user));
	if(!ins.second)
	{
		std::cout << "[failed] add to online user" << std::endl;
		return false;
	}

FAILED:
	/*查询到帐号信息后发送到客户端通知登录成功*/
	pro.data_type = ACCOUNT_INFO;
	pro.msg_type  = LOGIN_ACCOUNT;
	pro.msg_len   = sizeof(account_info) + sizeof(re);

	memcpy(msg, &fd, sizeof(int));
	len += sizeof(int);
	memcpy(msg+len, &pro, sizeof(protocol));
	len += sizeof(protocol);
	memcpy(msg+len, acnt, sizeof(account_info));
	len += sizeof(account_info);
	memcpy(msg+len, re, sizeof(re));
	len += sizeof(re);
	/*for(int i = 0; i < len; i ++)
		printf(" %d", msg[i]);
	std::cout << std::endl;*/

	/*将要发送的内容发送到管道*/
	write_fd = open("send_to_client", O_WRONLY);
	if(write_fd < 0)
	{
		DEBUG("[failed] open fifo_write failed");
		return false;
	}
	ret = write(write_fd, msg, len);
	if(ret < 0)
	{
		close(write_fd);
		DEBUG("[failed] a message send to fifo failed");
		return false;
	}
	close(write_fd);
	
	std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
	for(auto it = onlines.begin(); it != onlines.end(); it ++)
	{
	    std::cout << it->second->account_info.id << ':' << it->second->account_info.name << ':' << it->second->fd << std::endl;
	}
	return true;
}

bool Maccount::logout(account_info *acnt, int fd)
{
    std::cout << acnt->id << ' ' << acnt->name << "->" << fd << std::endl;
	auto it = onlines.find(std::string(acnt->id));
	if(it != onlines.end())
	{
	    delete it->second;
		this->onlines.erase(it);
	}
	std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
	for(auto it = onlines.begin(); it != onlines.end(); it ++)
	{
	    std::cout << it->second->account_info.id << ':' << it->second->account_info.name << ':' << it->second->fd << std::endl;
	}
	return true;
}

bool Maccount::mod_name(account_info *acnt)
{
	this->db->connect("account_manage");
	char sql[256] = {};
	sprintf(sql, "update account_info set account_name=%s where account_num=%s", acnt->name, acnt->id);
	if(this->db->modify(sql))
		return true;
	return false;
}

bool Maccount::mod_pwd(account_info *acnt)
{
	this->db->connect("account_manage");
	char sql[256] = {};
	if(acnt->pwd == nullptr)
		return false;
	sprintf(sql, "update account_pwd set password=%s where account_num=%s", acnt->pwd, acnt->id);
	this->db->modify(sql);
	return true;
}

char* Maccount::generate_num(const char *number_str)
{
	this->number = atoi(number_str);
	this->number += 1;
	sprintf(this->number_id, "%d", this->number);
	return this->number_id;
}

bool Maccount::work(int msg_type, const char *buf, int fd)
{
	bool ret = false;
	account_info info;
	msg _msg;
	/*结构没做好，消息类型数据要单独处理*/
	if(msg_type & SEND_MSG)
	{
		memset((void*)&_msg, 0, sizeof(msg));
		memcpy(&_msg, buf, sizeof(msg));
	}
	else
	{
		memset((void*)&info, 0, sizeof(account_info));
		memcpy(&info, buf, sizeof(account_info));
	}

	if(msg_type & SEND_MSG)
	{
		std::cout << "message" << std::endl;
		if(send_msg(&_msg, fd))
			ret = true;
	}
	else if(msg_type & REGISTER_ACCOUNT)
	{
		std::cout << "registered" << std::endl;
		if(registered(&info, fd))
			ret = true;
	}
	else if(msg_type & LOGIN_ACCOUNT)
	{
		std::cout << "login" << std::endl;
		if(login(&info, fd))
			ret = true;
	}
	else if(msg_type & LOGOUT_ACCOUNT)
	{
		std::cout << "logout" << std::endl;
		if(logout(&info, fd))
			ret = true;
	}
	else if(msg_type & LIST_ONLINE)
	{
		std::cout << "list online user" << std::endl;
		if(list_online(&info, fd))
			ret = true;
	}
	else if(msg_type & MODIFY_PASSWD)
	{
		std::cout << "mod passwd" << std::endl;
		if(mod_pwd(&info))
			ret = true;
	}
	else
		ret = false;
	return ret;
}

bool fetch_max_account_num(MYSQL_RES *result, void *container)
{
	MYSQL_ROW sql_row;
	if(sql_row = mysql_fetch_row(result))
	{
		memcpy((char*)container, sql_row[0], MAX_ACCOUNT_LEN);
		return true;
	}
	else
	{
		std::cout << "null result" << std::endl;
		return false;
	}
}

void Maccount::set_db(database *_db)
{
	db = _db;
	char sql[256] = "select max(account_num) from account_info";
	char number_str[MAX_ACCOUNT_LEN] = {};
	if(!this->db->query(sql, fetch_max_account_num, number_str))
	{
		std::cout << "[failed] read current max account number" << std::endl;
		return;
	}
	this->number = atoi(number_str);
	sprintf(this->number_id, "%d", this->number);
	std::cout << "max account num: " << this->number << std::endl;
	std::cout << "max account str: " << this->number_id << std::endl;
}

bool Maccount::list_online(account_info *acnt, int fd)
{
	char msg[1400] = {};
	int remain_len = sizeof(msg);
	int msg_len = 0;
	int data_len = 0;

	data_len = sizeof(int);
	memcpy((void*)msg, (void*)&fd, data_len);
	msg_len += data_len;
	remain_len -= data_len;

	/*填入协议*/
	data_len = sizeof(protocol);
	protocol pro;
	memset((void*)&pro, 0, data_len);
	pro.data_type = ACCOUNT_INFO;
	pro.msg_type  = LIST_ONLINE;
	pro.msg_len   = 0;
	msg_len += data_len;
	remain_len -= data_len;

	for(auto it = onlines.begin(); it != onlines.end(); it ++)
	{
		acc_info *user = it->second;
		if(strcmp(user->account_info.id, acnt->id) != 0)
		{
			data_len = sizeof(acc);
			memcpy((void*)(msg+msg_len), (void*)&user->account_info, data_len);
			std::cout << user->account_info.id << "->" << user->account_info.name << std::endl;
			msg_len += data_len;
			remain_len -= data_len;
			pro.msg_len += 1;
		}
		if(remain_len <= sizeof(acc))
			break;
	}
	memcpy((void*)(msg+sizeof(int)), (void*)&pro, sizeof(protocol));
	/*for(int i = 0; i < msg_len; i ++)
		printf("%d ", msg[i]);
	std::cout << std::endl;*/

	/*将要发送的内容发送到管道*/
	int write_fd = open("send_to_client", O_WRONLY);
	if(write_fd < 0)
	{
		DEBUG("[failed] open fifo_write failed");
		return false;
	}
	int ret = write(write_fd, msg, msg_len);
	if(ret < 0)
	{
		close(write_fd);
		DEBUG("[failed] a message send to fifo failed");
		return false;
	}
	close(write_fd);
	return true;
}

bool Maccount::send_msg(msg *_msg, int fd)
{
	char re[] = {"[system]对方已离线"};
	char buf[1450] = {};
	int len = sizeof(int);

	protocol pro;
	len += sizeof(protocol);
	memset((void*)&pro, 0, len);
	pro.data_type = MESSAGE;
	pro.msg_type  = SEND_MSG;
	
	std::cout << _msg->date << std::endl;
	std::cout << _msg->sender << std::endl;
	std::cout << _msg->receiver << std::endl;
	std::cout << _msg->cont << std::endl;

	/*如果没找到在线用户，系统返回提示消息*/
	auto it = onlines.find(std::string(_msg->receiver));
	if(it == onlines.end())
	{
		time_t t;
		time(&t);
		std::string time = time_to_date(t);

        /*如果对方已离线，就将消息清空后填入系统提示*/
        std::string sender(_msg->receiver);
        std::string receiver(_msg->sender);
		memset((void*)_msg->date, 0, sizeof(_msg->date));
		memset((void*)_msg->receiver, 0, sizeof(_msg->receiver));
		memset((void*)_msg->sender, 0, sizeof(_msg->sender));
		
		memcpy((void*)_msg->date, (void*)time.c_str(), sizeof(_msg->date));
		memcpy((void*)_msg->receiver, (void*)sender.c_str(), sizeof(_msg->receiver));
		memcpy((void*)_msg->sender, (void*)receiver.c_str(), sizeof(_msg->sender));
		
		memset((void*)_msg->cont, 0, sizeof(_msg->cont));
		memcpy((void*)_msg->cont, (void*)re, strlen(re));

		pro.msg_len = sizeof(msg);
		len += pro.msg_len;
	}
	else
		fd = it->second->fd;

	memcpy((void*)buf, (void*)&fd, sizeof(int));
	memcpy((void*)(buf+sizeof(int)), (void*)&pro, sizeof(protocol));
	memcpy((void*)(buf+sizeof(int)+sizeof(protocol)), (void*)_msg, sizeof(msg));
	len += sizeof(msg);

	/*将要发送的内容发送到管道*/
	int write_fd = open("send_to_client", O_WRONLY);
	if(write_fd < 0)
	{
		DEBUG("[failed] open fifo_write failed");
		return false;
	}
	int ret = write(write_fd, buf, len);
	if(ret < 0)
	{
		close(write_fd);
		DEBUG("[failed] a message send to fifo failed");
		return false;
	}
	close(write_fd);
	return true;
}
