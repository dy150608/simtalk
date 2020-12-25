#ifndef _CCONNECT_HPP_
#define _CCONNECT_HPP_

/*自定义头文件*/
#include "inc/data_type.hpp"
#include "tool/for_simple.hpp"
#include "inc/global_config.hpp"
/*cpp*/
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <utility>
#include <queue>
#include <vector>
#include <thread>
/*c*/
#include <cstring>
#include <cstdio>
#include <ctime>
/*system call*/
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <semaphore.h>
/*QT*/
#include <QDebug>

using std::cout;
using std::cin;
using std::endl;

typedef struct _acc
{
    char id[MAX_ACCOUNT_LEN] = {};     /*帐号ID*/
    char name[MAX_USERNAME_LEN] = {};  /*帐号名称*/
}acc;

/*account_num, msg_list*/
typedef std::map<std::string, std::vector<msg*>> msg_type;

// 连接类
class Cconnect
{
public:
    Cconnect();
	~Cconnect();
    void init();
    bool heart_beats(char *target_ip);       /*心跳机制*/
	bool link(char *target_ip);				 /*与目标主机建立连接*/
	bool req_send(int req_type);			 /*发送数据请求*/
	bool req_recv();						 /*接收数据消息*/
    void send();                             /*分发发送*/
    void recv();                             /*分发接收*/

    bool login(const char *account, const char *passwd);     /*登录*/
    bool registered(const char *account, const char *passwd);/*注册*/
    void quit();                             /*与目标主机断开连接*/
    bool list_online();                      /*列出除本机外的在线用户*/
    bool send_msg(msg *_msg, int len);       /*向服务器发送消息数据*/

    /*data*/
    sem_t sem1, sem2;
    char BUF[1500] = {};
    int BUFLEN = 0;
    int aim_ip = 0;     		/*对端IP，目前只支持IPv4*/
    char ip_str[16] = {};       /*对端IP*/
    bool is_link = false;       /*是否与对端保持连接*/
    int tcp_fd = 0;				/*TCP连接句柄*/
    int udp_fd = 0;				/*UDP连接句柄*/
    account_info acnt;          /*本机帐号信息*/
    /*id, name*/
    std::map<std::string, std::string> online_list;         /*在线用户列表*/
    msg_type msg_list;          /*消息列表*/
    msg_type::iterator cu = msg_list.begin();               /*当前对话用户*/
    std::vector<msg*>::reverse_iterator cu_msg = (cu->second).rbegin();/*当前消息索引*/
};

#endif
