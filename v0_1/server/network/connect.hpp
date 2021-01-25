#ifndef _SCONNECT_HPP_
#define _SCONNECT_HPP_

#include "../../inc/data_type.hpp"
#include "../../inc/global_config.hpp"
#include "../../tool/for_simple.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <iostream>
#include <thread>
#include "../account/account_manage.hpp"
#include "../message/message.hpp"
#include "../database/database.hpp"

class Sconnect
{
public:
	Sconnect();
	~Sconnect();
	void init();
	void set_tcp_fd(int fd);	// 设置监听端口
	void set_udp_fd(int fd);	// 同上
	void work();
	int  Accept(int fd, sockaddr *sa, socklen_t *len);
	
	friend void Init(Sconnect &con);
	friend void Work(Sconnect &con);
	friend void Receive(Sconnect *con);
	friend void Sendto(Sconnect *con);
private:
	char server_ip[32];			// 服务器IP，目前只适用IPv4
	int  tcp_fd;				// TCP连接句柄
	int  udp_fd;				// UDP连接句柄
	int  epoll_fd;				// 连接集合
	bool enable;				// 服务器是否启用成功

	epoll_event evs[2048];		// epoll事件集
	epoll_event ev;				// 跟节点事件
	
	Maccount *acnt;
	Message *msg;
	database *acc_mag;
	//database ofl_msg;
};

void Init(Sconnect&);
void Work(Sconnect&);
void Receive(Sconnect*);
void Sendto(Sconnect*);

#endif
