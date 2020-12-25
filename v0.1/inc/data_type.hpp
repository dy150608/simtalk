// 消息格式: (protocol{data_type(4bytes)|msg_type(4bytes)|msg_len(2bytes)|placeholder(2bytes)})|message
// 读写消息：|receiver(4bytes)|protocol{data_type(4bytes)|msg_type(4bytes)|msg_len(2bytes)|placeholder(2bytes)}|message
#ifndef _DATA_TYPE_HPP_
#define _DATA_TYPE_HPP_

#include <string>
#include <queue>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

namespace dt
{
// 消息链表节点的插入方式
enum INSERT
{
	// 在读取文件中的消息记录时一般用头插
	head,		/*头插法*/
	// 在插入新的消息节点时一般用尾插
    tail		/*尾插法*/
};
}

// 帐号用户名及聊天室名称最大长度
const int MAX_USERNAME_LEN = 16;
// 文字消息最大长度
const int MAX_MESSAGE_LEN = 1460 - 60;
// 帐号ID最大长度
const int MAX_ACCOUNT_LEN = 16;
// 密码最大长度
const int MAX_PASSWORD_LEN = 16;

// 终端请求的消息类型
enum MSG_TYPE
{
	account,	/* 帐号 */
	contact,	/* 联系人 */
    chatroom,	/* 聊天室 */
    dynamic,	/* 动态 */
    offline		/* 离线消息 */
};

// 系统语言
enum LANG
{
    ch,    /* 中文 */
    en     /* 英文 */
};

// 数据类型
enum DATA_TYPE
{
/*
 * 注：除文字消息外的数据类型都为文件
 * 文件类消息保存文件路径（默认路径就行）
 */
	word,		/*消息*/
    file		/*文件*/
};

// 账户类型
enum ACCOUNT_TYPE
{
    user,       /*用户*/
    group       /*聊天室*/
};

// 消息节点
typedef struct message
{
    char date[24] = {};					/*消息日期*/
    //char pret[16] = {};	 			/*时间前缀 prefix_time*/
    //char suft[16] = {};	 			/*时间后缀 suffix_time*/
	char sender[MAX_ACCOUNT_LEN] = {};	/*发送者*/
	char receiver[MAX_ACCOUNT_LEN] = {};/*接收者*/
	char cont[1300] = {};				/*消息内容*/
}msg;

// 未读消息集
typedef struct unread_message
{
    std::queue<msg*> ur_set;				/*未读消息队列*/
	char account_num[MAX_ACCOUNT_LEN] = {};	/*接收者帐号*/
	int  count = 0;							/*未读消息数量*/
}um;

// 帐号
typedef struct account
{
    char name[MAX_USERNAME_LEN];        /*帐号名称*/
    char id[MAX_ACCOUNT_LEN];           /*帐号ID*/
    ACCOUNT_TYPE  type;                 /*帐号类型*/
	char pwd[MAX_PASSWORD_LEN];			/*帐号密码*/
}account_info;

// 协议
typedef struct protocol
{
	int data_type = 0;
    int msg_type  = 0;
    short msg_len = 0;
    short placeholder = 0;
}protocol;

//data_type:
#define ACCOUNT_INFO (0X0001)     //1.帐号
#define MESSAGE (0X0002)          //2.消息
//msg_type:
#define REGISTER_ACCOUNT (0X0001) //1.注册帐号
#define LOGIN_ACCOUNT (0X0002)    //２.登录
#define MODIFY_PASSWD (0X0004)    //３.修改密码
#define LOGOUT_ACCOUNT (0X0008)   //４.注销
#define SEARCH (0X00010)          //５.搜索
#define SEND_MSG (0X00020)        //６.发送消息
#define ADD_CONTACT (0X0040)      //７.添加联系人
#define LIST_ONLINE (0X0080)	  //８．获取在线用户列表

typedef struct mq_msg
{
    long mtype = 0;
    char buf[1500] = {};
}MSG;

#define MSG_PATH "/home/li/document/codes/simple-chat/cache/local_message/"

#endif
