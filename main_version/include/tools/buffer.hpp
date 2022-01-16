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
	
	buffer() : data_(new char[SIZE]), curr_(data_) {}
	
	buffer(buffer& _buff) : data_(new char[SIZE]), curr_(nullptr)
	{
		memcpy(data_, _buff.data(), _buff.size());
		curr_ = data_ + _buff.size();
	}
	
	buffer(buffer&& _buff) :
		data_(_buff.get_data()),
		curr_(data_ + _buff.size())
	{
		_buff.reset();
	}
	
	buffer(char* _buff, offset_t len) : data_(new char[SIZE]), curr_(nullptr)
	{
		memcpy(data_, _buff, len);
		curr_ = data_ + len;
	}

	~buffer()
	{
		if(data_ != nullptr)
		{
			delete[] data_;
			data_ = curr_ = nullptr;
		}
	}

protected:
	char* get_data() { return data_; }
	void reset() { data_ = curr_ = nullptr; }

public:// data-opt member func
	const char* data() { return data_; }
	const char* tail() { return data_ + SIZE-1; }
	char* current() { return curr_; }
	offset_t size() { return curr_ - data_; }
	void resize() { curr_ = data_; }
	void add(offset_t _len) { curr_ += _len; }
	void clear()
	{
		memset(data_, 0, SIZE);
		resize();
	}

public:// input member func
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
		std::string(std::move(ss.str()));
		append(ss.str().c_str(), ss.str().size());
		return *this;
	}

private:
	char* data_ = nullptr;
	char* curr_ = nullptr;
};

using buffer1KB = buffer<1024>;
using buffer8KB = buffer<8192>;
using buffer1MB = buffer<10240>;
using buffer8MB = buffer<81920>;

template<int SIZE>
std::stringstream& operator<<(std::stringstream& _out, buffer<SIZE>& _buff)
{
	_out << _buff.data();
	return _out;
}

}// namespace:tools
}// namespace:simtalk

#endif// BUFFER_HPP
