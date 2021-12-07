#include <part/easylog.hpp>
#include <comm/configure_define.hpp>

using namespace simtalk::part;
using namespace simtalk::tools;

static easylog LOG(EASYLOG_CONFIG);

easylog::easylog(const char* conf_path)
{
	out_buff.insert(new buffer<LOG_BUF_SIZE>);// first buffer
	out_buff.insert(new buffer<LOG_BUF_SIZE>);// second buffer
}

easylog::~easylog()
{}

template<typename... Args>
void easylog::print(printlv level, const char* format, Args... others)
{
#ifdef DEBUG
	if (level < printlv::PL_ASSERT || level >= printlv::PL_UNUSED) return;
#elif RELEASE
	if (level < printlv::PL_ASSERT || level >= printlv::PL_DEBUG) return;
#endif
	buffer<ONE_LOG_SIZE> buff;
	buff << msg_prefix(level);
	if constexpr (sizeof...(others) == 0)
	{
		buff << format;
	}
	else
	{
		msg_print(buff, format, others...);
	}
	buff << '\n';

	std::lock_guard _lock(lock);
	// TODO
	// write to out buffer
}

template<typename T, typename... Args>
void easylog::msg_print(buffer<ONE_LOG_SIZE>& buff, const char* format, T first, Args... others)
{
	if(format == nullptr) return;
	for( ; *format != '\0'; ++format)
	{
		if(*format == '%')
		{
			buff << first;
			if constexpr (sizeof...(others) > 0)
			{
				msg_print(buff, format+1, others...);
				return;
			}
		}
		buff << *format;
	}
}

const char* easylog::plv_name(printlv level)
{
	if (level < printlv::PL_ASSERT || level >= printlv::PL_UNUSED) return nullptr;
	return PL_NAME[static_cast<int>(level)];
}

buffer<ONE_LOG_SIZE> easylog::msg_prefix(printlv level)
{
	buffer<ONE_LOG_SIZE> buff;
	// date TODO
	// call function
	buff << CALL_INFOMATION();
	return buff;
}
