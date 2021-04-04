#include <tools/easylog.hpp>

easylog::easylog(string& log_file) : m_logfile(ofstream(log_file.c_str(), std::ios::out|std::ios::app))
{
	m_msg_buff = new char[BUFF_SIZE];
}

easylog::~easylog()
{
	delete[] m_msg_buff;
}

void easylog::write(printlv level, stringstream& message)
{
	m_logfile << lv_name(level) << message.str() << std::endl;
	message.clear();
}

int easylog::msg_print(printlv level, const char* format, ...)
{
	int num = 0;
	va_list varg_list;
	va_start(varg_list, format);
	num = _vsnprintf(m_msg_buff, BUFF_SIZE, format, varg_list);
	va_end(varg_list);

	m_logfile << lv_name[level] << m_msg_buff << std::endl;
	memset(m_msg_buff, BUFF_SIZE, 0);
	return num;
}

void msg_print(const string& format)
{
	m_msg << format;
	this->write(m_level, m_msg);
}
template<typename T, typename... Args>
void easylog::msg_print(const string& format, T first, Args... others)
{
	for(auto iter = format.begin(); iter != format.end(); iter++)
	{
		if(*iter == '%')
		{
			m_msg << first;
			msg_print(format, others...);
			return;
		}
		m_msg << *iter;
	}
}

const string& easylog::lv_name(printlv level)
{
#ifdef RELEASE
	if(elevel >= printlv::DEBUG) return "";
#elif define DEBUG
	if(elevel > printlv::VERBOSE) return "";
#endif

	string str = '[' + lv_name(level) + ']';
	return std::move(str);
}
