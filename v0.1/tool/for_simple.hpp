#ifndef _FOR_SIMPLE_H_
#define _FOR_SIMPLE_H_

#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

/*
 * 用于打印错误信息
 * 只在debug模式下使用
 * @para msg: 要打印的错误消息
 */
//void DEBUG(const char *msg);
#define DEBUG(msg) \
	printf("In-%s-%d: %s\n", __FILE__, __LINE__, msg)

 /*
  * 错误退出
  * @para msg: 退出的错误信息
  */
void ERROR(const char *msg);
 
/*
 * 打印由num个'-'组成的线条
 * @para num: '-'的数量
 */
void DRAW_LINE(int num);

/*
 * 拼接字符串，每次拼接后字符串长度都会变长
 * @para src: 存储位置
 * @para dest: 拼接串
 * @para len: [in]原串长度， [out]存储串长度
 * @ret: 存储串位置
 */
char* strcnt(char *src, char *dest, int *len);

/*
 * 截取字符串，每次截取后字符串长度都会变短
 * @para src: 需要截取的字符串
 * @para dest: 存储位置
 * @para len: [in]原串长度, [out]存储串长度
 * @ret: 原串地址
 */
char* strcut(char *src, char *dest, int *len);

/*
 * 获取标准格式表示的字符串
 * @para time: 当前时间戳
 * @ret: 标准格式日期
 */
std::string time_to_date(time_t &time);

/*
 * int mysql_real_query(MYSQL *mysql, const char *query, unsigned int length)
 * 设置query和length参数
 * @para sql: 要查询的SQL语句
 */
extern char QUERY_SQL[256];
extern int  QUERY_LEN;

#define QUERY(sql) do { \
	QUERY_LEN = strlen(sql); \
	strncpy(QUERY_SQL, sql, QUERY_LEN); \
	printf("run: %s\n", sql); \
}while(0)

#endif
