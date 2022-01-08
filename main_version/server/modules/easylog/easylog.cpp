#include <part/easylog.hpp>
#include <comm/configure_define.hpp>
#include <chrono>
#include <iomanip>
#include <stdarg.h>

using namespace simtalk::part;
using namespace simtalk::tools;

#define CURRDIR "test.log"

namespace simtalk::part
{

static const char* PL_NAME[PL_UNUSED] = 
{// print level name define
	"[ASSERT]",
	"[ERROR]",
	"[WARNING]",
	"[INFO]",
	"[DEBUG]",
	"[VERBOSE]"
};

const char* plv_name(const int _level)
{
	return PL_NAME[_level % PL_UNUSED];
}

buffer64B time_now()
{
	using namespace std::chrono;
	buffer64B buff;
	tm tm_fmt;
	auto now_ = system_clock::now();
	auto micros = duration_cast<microseconds>(now_.time_since_epoch()).count() - duration_cast<seconds>(now_.time_since_epoch()).count()*1000000;
	auto t_c = system_clock::to_time_t(now_);
	buff << std::put_time(localtime_r(&t_c, &tm_fmt), "%F %T.");
	buff << micros;
	return buff;
}

easylog G_LOG(CURRDIR);// global logInstance define

}// namespace:part

easylog::easylog(const char* _conf_path) : out_file_(_conf_path)
{
	out_buff_.insert(new buffer<LOG_BUF_SIZE>);// first buffer
	out_buff_.insert(new buffer<LOG_BUF_SIZE>);// second buffer
}

easylog::~easylog()
{}

void easylog::buffer_print(const char* _data, int _size)
{
	std::lock_guard _lock(lock_);
	if(out_buff_.current()->append(_data, _size) != 0)
	{
		buffer<LOG_BUF_SIZE>* output = out_buff_.current();
		out_file_.write(output->data(), output->size());
		out_buff_.next();// TODO
		out_buff_.current()->append(_data,_size);
	}
}

/*
void easylog::print(const int _level, const char* _format, ...)
{
#ifdef DEBUG
	if(_level <= PL_UNDEF || _level >= PL_UNUSED) return;
#else
	if(_level <= PL_UNDEF || _level >= PL_DEBUG) return;
#endif
	char* msg = new char[ONE_LOG_SIZE];
    va_list ap;
    va_start(ap, _format);
    int len = vsprintf(msg, _format, ap);
    va_end(ap);
    buffer<ONE_LOG_SIZE> buff(msg, len);
    buff << '\n';
	buffer_print(buff.data(), buff.size());
}
*/
