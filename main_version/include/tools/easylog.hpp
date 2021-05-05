#ifndef _EASYLOG_
#define _EASYLOG_

#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdarg>

using std::string;
using std::ofstream;
using std::stringstream;

class easylog;
// global
extern easylog global_log;
const int LOG_BUFF_SIZE=1024*8;
const int PL_NAME_SIZE=16;

// enum
enum printlv// print level
{
	PL_ASSERT	= 0,// 断言
	PL_ERROR	= 1,// 错误
	PL_WARNING = 2,// 警告
	PL_INFO	= 3,// 信息
	PL_DEBUG	= 4,// 调试
	PL_VERBOSE = 5,// 详细
	PL_UNUSED
};

// assist
const static char PL_NAME[][PL_NAME_SIZE] = {// print level name
	{"[ASSERT]"},
	{"[ERROR]"},
	{"[WARNING]"},
	{"[INFO]"},
	{"[DEBUG]"},
	{"[VERBOSE]"}
};

class easylog
{
public:
	/*
	easylog()=delete;
	easylog(easylog&)=delete;
	easylog(easylog&&)=delete;
	easylog& operator=(easylog&)=delete;
	easylog& operator=(easylog&&)=delete;
	*/

public:
	explicit easylog(string& log_file);
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
	 * [out 1]: 0->succ, other->fail
	 */
	int write(printlv level, string& message);

	/*
	 * format print:
	 *   usecase[ easylog::msg_print("% %", "hello", "world"); ]
	 * [in 1]: format string, '%' is use for format placeholder
	 * [in ...]: mutable length parameters
	 *
	 * and [ void msg_print(const string&); ] is base function in C++11
	 */
	void msg_print(const char* message);
	template<typename T, typename... Args>
	void msg_print(const char* format, T first, Args... others);

	/*
	 * format print is too troublesome to realize,
	 * TODO when using C++17 with std::format,
	 * and here use C-style to realize one ver.
	 * [in 1]: print level
	 * [in 2]: format string
	 * [out 1]: 0->succ, other->fail
	 */
	int msg_print(printlv level, const char* format, ...);

private:
	/*
	 * get the name of print level
	 * [in 1]: print level
	 * [out 1]: level name
	 */
	const char* plv_name(printlv level);

private:
	ofstream m_logfile;
	stringstream m_msg;
	printlv m_level = printlv::PL_INFO;
	char* m_msg_buff= nullptr;
};

#define CALL_INFOMATION() \
({ \
	stringstream ss; \
	ss << '|' << __FILE__ << ':' << __func__ << ':' << __LINE__ << '|'; \
	(ss.str()); \
})

inline void LOG_PRINT(printlv level, const char* format, ...)
{
	char msg_buff[LOG_BUFF_SIZE] = {};
	string str;
	va_list varg_list;
	va_start(varg_list, format);
	int num = vsnprintf(msg_buff, LOG_BUFF_SIZE, format, varg_list);
	str += CALL_INFOMATION();
	str += msg_buff;
	global_log.write(level, str);
}

#define LOG_ASSERT(format, ...) \
do{ \
	LOG_PRINT(printlv::PL_ASSERT, format, ##__VA_ARGS__); \
}while(0)

#define LOG_ERROR(format, ...) \
do{ \
	LOG_PRINT(printlv::PL_ERROR, format, ##__VA_ARGS__); \
}while(0)

#define LOG_WARING(format, ...) \
do{ \
	LOG_PRINT(printlv::PL_WARNING, format, ##__VA_ARGS__); \
}while(0)

#define LOG_INFO(format, ...) \
do{ \
	LOG_PRINT(printlv::PL_INFO, format, ##__VA_ARGS__); \
}while(0)

#define LOG_DEBUG(format, ...) \
do{ \
	LOG_PRINT(printlv::PL_DEBUG, format, ##__VA_ARGS__); \
}while(0)

#define LOG_VERBOSE(format, ...) \
do{ \
	LOG_PRINT(printlv::PL_VERBOSE, format, ##__VA_ARGS__); \
}while(0)

#endif// _EASYLOG_
