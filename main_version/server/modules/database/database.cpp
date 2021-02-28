#include "database.hpp"

database::database()
{
	this->init();
}

database::database(const char *db_name)
{
	this->init();
	if(!this->connect(db_name))
		DEBUG("数据库连接失败");
	if(!this->test())
		std::cout << "[failed] query test" << std::endl;
}

database::~database()
{
	mysql_close(db_handle);
	std::cout << "[success] database distructor running" << std::endl;
}

void database::init()
{
	strcpy(host, "localhost");
	strcpy(user, "root");
	strcpy(pwd, "xjz150608");
	port = 3306;
	DEBUG("数据库连接信息初始化成功");
}

bool database::test()
{
	char sql[256] = {"select account_num from account_info limit 1"};
	size_t len = strlen(sql);
	if(mysql_real_query(this->db_handle, sql, len) != 0)
	{
		std::cout << mysql_errno(this->db_handle) << mysql_error(this->db_handle) << std::endl;
		return false;
	}
	this->result = mysql_store_result(this->db_handle);
	mysql_free_result(this->result);
	return true;
}
bool database::setdb(const char *name)
{
	strcpy(db_name, name);
	if(this->db_handle != NULL)
		mysql_close(db_handle);

	//重新打开数据库
	if(!this->connect(this->db_name))
		return false;
	return true;
}

bool database::connect(const char *_db_name)
{
	if(db_handle != NULL)
		return false;
	// 初始化数据库
	if(mysql_library_init(0, NULL, NULL) != 0)
	{
		DEBUG("数据库初始化失败");
		goto LIB_END;
	}
	// 初始化mysql
	db_handle = mysql_init(NULL);
	if(db_handle == NULL)
	{
		DEBUG("初始化数据库失败");
		goto FALSE;
	}
	if(mysql_real_connect(db_handle, host, user, pwd, _db_name, port, NULL, 0) == NULL)
	{
		std::cout << mysql_errno(this->db_handle) << mysql_error(this->db_handle) << std::endl;
		DEBUG("连接数据库失败");
		goto LIB_END;
	}
	DEBUG("连接数据库成功");
	return true;
LIB_END:
	db_handle = NULL;
	mysql_library_end();
FALSE:
	return false;
}

/*
 * 执行增，删，改
 * @para sql: 要执行的SQL语句
 */
bool database::modify(const char *sql)
{
	QUERY(sql);
	if(mysql_real_query(db_handle, QUERY_SQL, QUERY_LEN) != 0)
	{
		std::cout << mysql_errno(this->db_handle) << ' ' << mysql_error(this->db_handle) << std::endl;
		DEBUG("操作失败");
		return false;
	}
	return true;
}

/*
 * 执行查询
 * @para sql: 要执行的SQL语句
 */
bool database:: query(const char *sql, fetch_data_t fetch, void *container)
{
	QUERY(sql);
	if(mysql_real_query(db_handle, QUERY_SQL, QUERY_LEN) != 0)
	{
		std::cout << mysql_errno(this->db_handle) << ' ' << mysql_error(this->db_handle) << std::endl;
		return false;
	}
	// 存储结果集
	result = mysql_store_result(db_handle);
	if(result)
	{
		/* 获取结果集中域(字段)的数量
		unsigned int field_nums = mysql_num_fields(result);

		// 获取结果集的行值
		while(sql_row = mysql_fetch_row(result))
		{
			for(int i = 0; i < field_nums; i ++)
				printf("%12s", sql_row[i]);
			printf("\n");
		}*/
		if(!fetch(result, container))
		{
			mysql_free_result(result);
			return false;
		}
	}
	mysql_free_result(result);
	DEBUG("[success] query success");
	return true;
}

/*
 * 根据请求类型查询不同的数据
 * @para type: 需要查询的数据类型
 */
void database::req_type(MSG_TYPE type)
{
	char sql[256] = {};
	switch(type)
	{
	case account:
		strcpy(sql, "select account_num, account_name, account_type from account_info where account_num=");
		break;
	default:
		DEBUG("类型错误");
		break;
	}
}
