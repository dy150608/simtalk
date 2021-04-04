#ifndef _EASYLOG_
#define _EASYLOG_

#include <iostream>
#include <ofstream>
#include <sstream>
#include <cstdio>
#include <cstdarg>

using std::string;
using std::ofstream;
using std::stringstream;

class easylog;
// global
static easylog elog;
const int BUFF_SIZE=1024*8;

// enum
enum printlv// print level
{
	ASSERT	= 0,// 断言
	ERROR	= 1,// 错误
	WARNING = 2,// 警告
	INFO	= 3,// 信息
	DEBUG	= 4,// 调试
	VERBOSE = 5,// 详细
};

// assist
const string PL_NAME[] = {// print level name
	{"ASSERT"},
	{"ERROR"},
	{"WARNING"},
	{"INFO"},
	{"DEBUG"},
	{"VERBOSE"}
}

class easylog
{
public:
	/*
	easylog()=delete;
	easylog(easylog&)=delete;
	easylog(easylog&&)=delete;
	easylog& operator=(easylog&&)=delete;
	easylog& operator=(const easylog&)=delete;
	*/

public:
	easylog(string& log_file)=default;
	~easylog();

	/*
	 * set default print level
	 * [in 1]: print level
	 */

	void set_printlv(printlv level) { m_level = level; }

	/*
	 * print a message to log_file or console
	 * [in 1]: print level
	 * [in 2]: message
	 */
	void write(printlv level, stringstream& message);

	/*
	 * format print:
	 *   usecase[ easylog::msg_print("% %", "hello", "world"); ]
	 * [in 1]: format string, '%' is use for format placeholder
	 * [in ...]: mutable length parameters
	 *
	 * and [ void msg_print(const string&); ] is base function in C++11
	 */
	void msg_print(const string& format);
	template<typename T, typename... Args>
	void msg_print(const string& format, T first, Args... others);

	/*
	 * format print is too troublesome to realize,
	 * TODO when using C++17 with std::format,
	 * and here use C-style to realize one ver.
	 * [in 1]: print level
	 * [in 2]: format string
	 */
	int msg_print(printlv level, const char* format, ...);

private:
	/*
	 * get the name of print level
	 * [in 1]: print level
	 * [out 1]: level name
	 */
	const string& lv_name(printlv level);

private:
	const ofstream m_logfile;
	stringstream m_msg;
	printlv m_level = printlv::INFO;
	char* m_msg_buff= nullptr;
};

#define CALL_INFOMATION() \
({ \
	string str = __FILE__ + __func__ + __LINE__; \
	(str.c_str()); \
})

#define LOG_PRINT(level, const char* format, ...) \
do \
{ \
	char msg_buff[BUFF_SIZE] = {}; \
	stringstream ss; \
	va_list varg_list; \
	va_start(varg_list, format); \
	int num = _vsnprintf(msg_buff, BUFF_SIZE, format, varg_list); \
	ss << CALL_INFOMATION() << msg_buff; \
	easylog::write(level, ss); \
}while(0)

#define LOG_ASSERT(const char* format, ...) \
do \
{ \
	LOG_PRINT(printlv::ASSERT, format, ##args); \
}while(0)

#define LOG_ERROR(const char* format, ...) \
do \
{ \
	LOG_PRINT(printlv::ERROR, format, ##args); \
}while(0)

#define LOG_WARING(const char* format, ...) \
do \
{ \
	LOG_PRINT(printlv::WARNING, format, ##args); \
}while(0)

#define LOG_INFO(const char* format, ...) \
do \
{ \
	LOG_PRINT(printlv::INFO, format, ##args); \
}while(0)

#define LOG_DEBUG(const char* format, ...) \
do \
{ \
	LOG_PRINT(printlv::DEBUG, format, ##args); \
}while(0)

#define LOG_VERBOSE(const char* format, ...) \
do \
{ \
	LOG_PRINT(printlv::VERBOSE, format, ##args); \
}while(0)

#endif// _EASYLOG_
