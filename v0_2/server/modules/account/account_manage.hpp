#ifndef _ACCOUNT_MANAGE_HPP_
#define _ACCOUNT_MANAGE_HPP_

#include "../../inc/data_type.hpp"
#include "../database/database.hpp"
#include <map>
#include <utility>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>

typedef struct _acc
{
    char id[MAX_ACCOUNT_LEN] = {};     /*帐号ID*/
	char name[MAX_USERNAME_LEN] = {};  /*帐号名称*/
}acc;
typedef struct acc_info
{
	acc account_info;
	int fd = 0;
}acc_info;

class Maccount
{
public:
	Maccount();
	~Maccount();
	bool work(int type, const char *msg, int fd);// 处理消息
	void set_db(database *db);					 // 设置数据库
	bool registered(account_info *acnt, int fd); // 注册帐号
	bool mod_name(account_info *acnt);			 // 修改帐号信息
	bool login(account_info *acnt, int fd);		 // 登录帐号
	bool logout(account_info *acnt, int fd);	 // 注销帐号
	char* generate_num(const char *numstr);		 // 生成帐号
	bool mod_pwd(account_info *acnt);			 // 修改密码
	bool list_online(account_info *acnt, int fd);// 获取在线用户列表
	bool send_msg(msg *msg, int fd);	 		 // 发送消息到其他客户端

	char number_id[MAX_ACCOUNT_LEN];			 // 下一个生成的id串 int atoi(const char *nptr);
	int number;									 // 下一个生成的id sprintf(...);
	database *db;
	/*pair<string account_num, int fd>*/
	std::map<std::string, acc_info*> onlines;	 // 在线用户
	std::pair<std::map<std::string, acc_info*>::iterator, bool> ins;// 添加用户返回值
};

#endif
