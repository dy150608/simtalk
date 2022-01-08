#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstdint>
#include <sstream>
#include <cstring>
#include <utility>

namespace simtalk
{

namespace tools
{

template<int SIZE>
class buffer
{
	using self = buffer;
public:
	using offset_t = size_t;
	
	// empty construct
	buffer() : data_(new char[SIZE]), curr_(data_) {}
	
	buffer(buffer& _buff) : data_(new char[SIZE]), curr_(nullptr)
	{
		memcpy(data_, _buff.data(), _buff.size());
		curr_ = data_ + _buff.size();
	}
	
	buffer(buffer&& _buff) : data_(const_cast<char*>(_buff.data())), curr_(const_cast<char*>(data_)+_buff.size()) {}
	
	buffer(char* _buff, offset_t len) : data_(std::move(_buff)), curr_(const_cast<char*>(data_)+len) {}
	~buffer()
	{
		delete[] data_;
		data_ = curr_ = nullptr;
	}

	const char* data() { return data_; }
	const char* tail() { return data_ + sizeof(data_)-1; }
	char* current() { return curr_; }
	offset_t size() { return curr_ - data_; }
	void reset() { curr_ = data_; }
	void add(offset_t _len) { curr_ += _len; }

public:
	int append(const char* _buf, offset_t _len)
	{
		if(tail()-current() < _len) return -1;
		memcpy(curr_, _buf, _len);
		curr_ += _len;
		return 0;
	}

	self& operator<<(const self& _buff)
	{
		append(_buff.data(), _buff.size());
		return *this;
	}

	self& operator<<(const char* _buff)
	{
		append(_buff, strlen(_buff));
		return *this;
	}

	self& operator<<(const char _ch)
	{
		append(&_ch, 1);
		return *this;
	}

	template<typename T>
	self& operator<<(T&& _value)
	{
		std::stringstream ss;
		ss.sync_with_stdio(false);
		ss << _value;
		append(ss.str().c_str(), ss.str().size());
		return *this;
	}

private:
	char* data_ = nullptr;
	char* curr_ = nullptr;
};

using buffer32B  = buffer<32>;
using buffer64B  = buffer<64>;
using buffer128B = buffer<128>;
using buffer256B = buffer<256>;
using buffer512B = buffer<512>;
using buffer1KB  = buffer<1024>;
using buffer2KB  = buffer<2048>;

template<int SIZE>
std::stringstream& operator<<(std::stringstream& _out, buffer<SIZE>& _buff)
{
	_out << _buff.data();
	return _out;
}

}// namespace:tools
}// namespace:simtalk
#endif// BUFFER_HPP
