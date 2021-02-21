#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include "../../inc/data_type.hpp"
#include "../../tool/for_simple.hpp"
#include <mysql/mysql.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*执行查询操作的回调*/
typedef bool (*fetch_data_t)(MYSQL_RES *query, void *container);

class database 
{
public:
	database();
	database(const char *db_name);
	~database();
	void init();
	bool setdb(const char *name);			// 设置要查询的数据库名
	bool connect(const char *db_name);		// 要连接的数据库
	bool modify(const char *sql);			// 修改数据库
	bool query(const char *sql, fetch_data_t fetch, void *container);// 查询数据库
	void req_type(MSG_TYPE type);			// 终端请求的数据类型
	bool test();
private:
	MYSQL *db_handle;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	MYSQL_FIELD *field;
	
	char host[16];
	char user[MAX_USERNAME_LEN];
	char pwd[MAX_PASSWORD_LEN];
	int  port;
	char db_name[MAX_USERNAME_LEN];
};

#endif
