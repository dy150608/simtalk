#ifndef _EASYLOG_HPP
#define _EASYLOG_HPP

#include <cstdio>
#include <cstdarg>
#include <mutex>
#include <string>
#include <memory>
#include <comm/noncopyable.hpp>
#include <tools/buffer.hpp>
#include <tools/cycle.hpp>

using namespace simtalk::tools;

namespace simtalk
{

namespace part
{

class easylog;
extern easylog LOG;

constexpr int ONE_LOG_SIZE = 1024*8;
constexpr int LOG_BUF_SIZE = ONE_LOG_SIZE*1024;
constexpr int PL_NAME_SIZE = 16;

enum class printlv// print level
{
	PL_ASSERT	= 0,// 断言
	PL_ERROR	= 1,// 错误
	PL_WARNING	= 2,// 警告
	PL_INFO		= 3,// 信息
	PL_DEBUG	= 4,// 调试
	PL_VERBOSE	= 5,// 详细
	PL_UNUSED
};

const char* PL_NAME[] = 
{// print level name
	"[ASSERT]",
	"[ERROR]",
	"[WARNING]",
	"[INFO]",
	"[DEBUG]",
	"[VERBOSE]"
};

class easylog : noncopyable
{	
public:
	explicit easylog(const char* conf_path);
	~easylog();

	/*
	 * format print:
	 *   usecase[ easylog::msg_print("% %", "hello", "world"); ]
	 * [in 1]: print level
	 * [in 2]: format string, '%' is use for format placeholder
	 * [in 3]: first parameters
	 * [in ...]: mutable length parameters
	 */
	template<typename... Args>
	void print(printlv level, const char* format, Args... others);

private:
	// suit with print()
	template<typename T, typename... Args>
	void msg_print(buffer<ONE_LOG_SIZE>& buff, const char* format, T first, Args... others);

	/*
	 * get the name of print level
	 * [in 1]: print level
	 * [out 1]: level name
	 */
	const char* plv_name(printlv level);

	/*
	 * print prefix
	 * [in 1]: print level
	 * [out]: prefix string
	 */
	buffer<ONE_LOG_SIZE> msg_prefix(printlv level);

private:
	std::mutex lock;
	cycle<buffer<LOG_BUF_SIZE>> out_buff;// buffer write to file
};

#define CALL_INFOMATION() \
({\
	buffer<ONE_LOG_SIZE> buff; \
	buff << '|' << __FILE__ << ':' << __func__ << ':' << __LINE__ << '|'; \
	(buff.data()); \
})

template<typename... Args>
void LOG_PRINT(printlv level, const char* format, Args... others)
{
	if(format == nullptr) return;
	LOG.print(level, format, &others...);
}

template<typename... Args>
void LOG_ASSERT(const char* format, Args... others)
{
	LOG_PRINT(printlv::PL_ASSERT, format, &others...);
}

template<typename... Args>
void LOG_ERROR(const char* format, Args... others)
{
	LOG_PRINT(printlv::PL_ERROR, format, &others...);
}

template<typename... Args>
void LOG_WARN(const char* format, Args... others)
{
	LOG_PRINT(printlv::PL_WARNING, format, &others...);
}

template<typename... Args>
void LOG_INFO(const char* format, Args... others)
{
	LOG_PRINT(printlv::PL_INFO, format, &others...);
}

template<typename... Args>
void LOG_DEBUG(const char* format, Args... others)
{
	LOG_PRINT(printlv::PL_DEBUG, format, &others...);
}

template<typename... Args>
void LOG_VERBOSE(const char* format, Args... others)
{
	LOG_PRINT(printlv::PL_VERBOSE, format, &others...);
}

}// namespace:part
}// namespace:simtalk
#endif// _EASYLOG_HPP
