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

static const char* PL_NAME[plv::PL_EXCEPT] = 
{// print level name define
	"[ASSERT]",
	"[ERROR]",
	"[WARNING]",
	"[INFO]",
	"[DEBUG]",
	"[VERBOSE]",
	"[LV_EXCEPT]"
};

buffer1KB time_now()
{
	using namespace std::chrono;
	buffer1KB buff;
	tm tm_fmt;
	auto now_ = system_clock::now();
	auto micros = duration_cast<microseconds>(now_.time_since_epoch()).count() - duration_cast<seconds>(now_.time_since_epoch()).count()*1000000;
	auto t_c = system_clock::to_time_t(now_);
	buff << std::put_time(localtime_r(&t_c, &tm_fmt), "%F %T.");
	buff << micros;
	return buff;
}

const char* plv_name(plv _level)
{
	if(_level <= plv::PL_UNDEF || _level >= plv::PL_UNUSED) return PL_NAME[plv::PL_EXCEPT-1];
	return PL_NAME[_level];
}

/*global logInstance define*/
easylog G_LOG(CURRDIR);

}// namespace:part

easylog::easylog(const char* _conf_path) : out_file_(_conf_path)
{
	out_buff_.insert(new log_buffer);// first buffer
	out_buff_.insert(new log_buffer);// second buffer
}

easylog::~easylog()
{
	log_buffer* output = out_buff_.current();
	out_file_.write(output->data(), output->size());
}

void easylog::buffer_print(const char* _data, int _size)
{
	std::lock_guard<std::mutex> _lock(lock_);
	if(out_buff_.current()->append(_data, _size) != 0)
	{
		log_buffer* output = out_buff_.current();
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
