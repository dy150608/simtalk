#ifndef FILE_HPP
#define FILE_HPP

#include <fstream>
#include <cstdio>
#include <mutex>
#include <comm/noncopyable.hpp>

namespace simtalk
{
namespace tools
{

class fileopt : noncopyable
{
public:
	fileopt(const char* _path) : ofs_(_path, std::ios::ate) {}
	~fileopt() { close(); }
	
public:
	/*
	 * output context to file 
	 * [in 1]: string of context
	 * [in 2]: length of context
	 */
	void write(const char* _context, int _len)
	{
		std::lock_guard<std::mutex> lock(lock_);
		ofs_.write(_context, _len);
	}

private:
	/*
	 * close the file connection
	 */
	void close() { ofs_.close(); }

private:
	std::ofstream ofs_;
	std::mutex lock_;
};

}// tools
}// simtalk

#endif// FILE_HPP
