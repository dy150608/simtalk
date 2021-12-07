#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstdint>
#include <sstream>
#include <cstring>

namespace simtalk
{

namespace tools
{

using offset_t = size_t;

template<int SIZE>
class buffer
{
	using self = buffer;
public:
	// empty construct
	buffer() : _data(new char[SIZE]), _curr(_data) {}
	
	~buffer()
	{
		delete[] _data;
		_data = _curr = nullptr;
	}
	
	buffer(buffer& _buff) : _data(new char[SIZE]), _curr(nullptr)
	{
		memcpy(_data, _buff.data(), _buff.size());
		_curr = _data + _buff.size();
	}
	
	buffer(buffer&& _buff) : _data(const_cast<char*>(_buff.data())), _curr(const_cast<char*>(_data)+_buff.size()) {}

	const char* data() { return _data; }
	const char* tail() { return _data + sizeof(_data)-1; }
	char* current() { return _curr; }
	offset_t size() { return _curr - _data; }
	void reset() { _curr = _data; }
	void add(offset_t len) { _curr += len; }

public:
	int append(const char* buf, offset_t len)
	{
		if(tail()-current() < len) return -1;
		memcpy(_curr, buf, len);
		_curr += len;
		return 0;
	}

	self& operator<<(const char* buff)
	{
		append(buff, strlen(buff));
		return *this;
	}

	self& operator<<(const char ch)
	{
		append(&ch, 1);
		return *this;
	}

	template<typename T>
	self& operator<<(T value)
	{
		std::stringstream ss;
		ss.sync_with_stdio(false);
		ss << value;
		append(ss.str().c_str(), ss.str().size());
		return *this;
	}

private:
	char* _data = nullptr;
	char* _curr = nullptr;
};

}// namespace:tools
}// namespace:simtalk
#endif// BUFFER_HPP
