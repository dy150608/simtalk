#include <tools/easylog.hpp>
#include <cstring>

easylog::easylog(string& log_file) : m_logfile(ofstream(log_file.c_str(), std::ios::out|std::ios::app))
{
	m_msg_buff = new char[LOG_BUFF_SIZE];
	m_logfile.sync_with_stdio(false);
}

easylog::~easylog()
{
	delete[] m_msg_buff;
}

int easylog::write(printlv level, string& message)
{
	const char* plv = plv_name(level);
	if(plv == nullptr) return -1;
	m_logfile.write(plv, PL_NAME_SIZE);
	m_logfile.write(message.c_str(), message.length());
	return 0;
}

int easylog::msg_print(printlv level, const char* format, ...)
{
	int num = 0;
	va_list varg_list;
	va_start(varg_list, format);
	num = vsnprintf(m_msg_buff, LOG_BUFF_SIZE, format, varg_list);
	va_end(varg_list);

	const char* plv = plv_name(level);
	if(plv == nullptr) return -1;
	m_logfile.write(plv, PL_NAME_SIZE);
	m_logfile.write(m_msg_buff, LOG_BUFF_SIZE);
	memset(m_msg_buff, LOG_BUFF_SIZE, 0);
	return num;
}

void easylog::msg_print(const char* message)
{
	if(message == nullptr) return;
	m_msg << *message;
	string tmp = m_msg.str();
	this->write(m_level, tmp);
	m_msg.clear();
}
template<typename T, typename... Args>
void easylog::msg_print(const char* format, T first, Args... others)
{
	if(format == nullptr) return;
	for( ; *format != '\0'; format++ )
	{
		if(*format == '%')
		{
			m_msg << first;
			msg_print(format+1, others...);
			return;
		}
		m_msg << *format;
	}
}

const char* easylog::plv_name(printlv level)
{
	if(level >= printlv::PL_UNUSED) return nullptr;
#ifdef RELEASE
	if(level >= printlv::PL_DEBUG) return nullptr;
#elif defined DEBUG
#endif
	return PL_NAME[static_cast<int>(level)];
}
