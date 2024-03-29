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
#include <tools/file.hpp>

namespace simtalk
{
namespace part
{

using namespace simtalk::tools;

// const size define
inline const int ONE_LOG_SIZE = 1024*8;
inline const int LOG_BUF_SIZE = ONE_LOG_SIZE*1024;

enum plv
{// print level define
	PL_UNDEF = -1,// 未定义
	PL_ASSERT = 0,// 断言
	PL_ERROR = 1,// 错误
	PL_WARNING = 2,// 警告
	PL_INFO	= 3,// 信息
	PL_DEBUG = 4,// 调试
	PL_VERBOSE = 5,// 详细
	PL_UNUSED = 6,// 未使用
	PL_EXCEPT = 7,// 日志级别异常
};

class easylog : noncopyable
{
	using log_buffer = buffer<LOG_BUF_SIZE>;

public:
	explicit easylog(const char* _conf_path);
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
	void print(const int _level, const char* _format, Args... _others);

	/*
	 * format print with C-style
	 * [in 1]: print level
	 * [in 2]: format string
	 * [in ...]: mutable length parameters
	 */
	// void print(const int _level, const char* _format, ...);

	const char* msg_data() { return out_buff_.current()->data(); }

private:
	// suit with print()
	template<typename T, typename... Args>
	void msg_print(log_buffer& _buff, const char* _format, T _first, Args... _others);

	/*
	 * print to buffer
	 * [in 1]: buffer string data
	 * [in 2]: size of data
	 */
	void buffer_print(const char* _data, int _size);

private:
	std::mutex lock_;
	cycle<log_buffer> out_buff_;
	fileopt out_file_;
};

/*
 * get the now time format string
 * [out]: format string
 */
buffer1KB time_now();

/*
 * get the name of print level
 * [in 1]: print level
 * [out]: level name
 */
const char* plv_name(plv level);

#define CALL_INFOMATION() \
({\
	buffer1KB buff; \
	buff << '|' << __FILE__ << ':' << __func__ << ':' << __LINE__ << '|'; \
	(buff.data()); \
})

#define LOG_PRINT(_level, _format, args...) \
do{ \
	buffer1KB buff; \
	buff << time_now(); \
	buff << plv_name(_level); \
	buff << CALL_INFOMATION(); \
	G_LOG.print(_level, "%" _format, buff.data(), args); \
}while(0);

#define LOG_ASSERT(_format, args...) LOG_PRINT(plv::PL_ASSERT, _format, args)
#define LOG_ERROR(_format, args...) LOG_PRINT(plv::PL_ERROR, _format, args)
#define LOG_WARN(_format, args...) LOG_PRINT(plv::PL_WARNING, _format, args)
#define LOG_INFO(_format, args...) LOG_PRINT(plv::PL_INFO, _format, args)
#define LOG_DEBUG(_format, args...) LOG_PRINT(plv::PL_DEBUG, _format, args)
#define LOG_VERBOSE(_format, args...) LOG_PRINT(plv::PL_VERBOSE, _format, args)

/*member function defination*/

template<typename... Args>
void easylog::print(int _level, const char* _format, Args... _others)
{
#ifdef DEBUG
	if(_level <= plv::PL_UNDEF || _level >= plv::PL_UNUSED) return;
#else
	if(_level <= plv::PL_UNDEF || _level >= plv::PL_DEBUG) return;
#endif
	log_buffer buff;
	if constexpr (sizeof...(_others) == 0)
	{
		buff << _format;
	}
	else
	{
		msg_print(buff, _format, _others...);
	}
	buff << '\n';
	buffer_print(buff.data(), buff.size());
}

template<typename T, typename... Args>
void easylog::msg_print(log_buffer& _buff, const char* _format, T _first, Args... _others)
{
	if(_format == nullptr) return;
	static_assert(sizeof...(_others) < 10);// avoid stack overflow
	for( ; *_format != '\0'; ++_format)
	{
		if(*_format == '%')
		{
			_buff << _first;
			if constexpr (sizeof...(_others) > 0)
			{
				msg_print(_buff, _format+1, _others...);
				return;
			}
		}
		else _buff << *_format;
	}
}

/*extern declare*/
extern easylog G_LOG;

}// namespace:part
}// namespace:simtalk

#endif// _EASYLOG_HPP
