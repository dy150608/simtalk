#ifndef NONCOPYABLE_HPP
#define NONCOPYABLE_HPP

namespace simtalk
{

class noncopyable 
{
public:
	noncopyable(const noncopyable&) = delete;
	void operator=(const noncopyable&) = delete;

protected:
	noncopyable() = default;
	~noncopyable() = default;
};

}// namespace:simtalk
#endif// NONCOPYABLE_HPP
