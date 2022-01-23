#ifndef CYCLE_HPP
#define CYCLE_HPP

#include <comm/noncopyable.hpp>
#include <memory>
#include <tuple>
#include <utility>

namespace simtalk
{
namespace tools
{

template<typename T>
class cycle : noncopyable
{
	using cycle_node = 
	struct list_node 
	{
		list_node(T* _res) : res_(std::unique_ptr<T>(_res)), next_(nullptr) {}
		~list_node() { next_ = nullptr; }
		
		std::unique_ptr<T> res_;
		list_node* next_;
	};

public:
	cycle() : curr_(nullptr), size_(0) {}
	cycle(T* _res);
	~cycle();

	T* current() { return curr_->res_.get(); }
	
	T* next() { curr_ = curr_->next_; return current(); }

	uint32_t size() { return size_; }

	std::tuple<T*, uint32_t> insert(T* _res) noexcept;

private:
	cycle_node* curr_;
	uint32_t size_;
};

template<typename T>
cycle<T>::cycle(T* _res) : curr_(nullptr), size_(0)
{
	insert(std::forward<T*>(_res));
}

template<typename T>
cycle<T>::~cycle()
{
	cycle_node** curr_addr = &curr_;
	while(*curr_addr != nullptr)
	{
		curr_ = curr_->next_;
		delete *curr_addr;
		*curr_addr = nullptr;
		--size_;
		if(curr_ != nullptr) curr_addr = &curr_;
	}
}

template<typename T>
std::tuple<T*, uint32_t> cycle<T>::insert(T* _res) noexcept
{
	// TODO: bad_alloc
	cycle_node* ptr= new cycle_node(_res);
	cycle_node* tmp = ptr;

	if(curr_ != nullptr) tmp = curr_->next_;
	else curr_ = ptr;

	curr_->next_ = ptr;
	ptr->next_ = tmp;
	++size_;
	return std::make_tuple(curr_->next_->res_.get(), size());
}

}// namespace:tools
}// namespace:simtalk

#endif// CYCLE_HPP
