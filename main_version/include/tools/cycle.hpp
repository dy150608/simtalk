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
		list_node(T* _res) : res(std::unique_ptr<T>(_res)), next(nullptr) {}
		~list_node() { next = nullptr; }
		
		std::unique_ptr<T> res;
		list_node* next;
	};

public:
	cycle() : curr_(nullptr), size_(0) {}
	cycle(T* _res);
	~cycle();

	T* current() { return curr_->res.get(); }
	
	T* next() { curr_ = curr_->next; return current(); }

	size_t size() { return size_; }

	std::tuple<T*, size_t> insert(T* _res) noexcept;

private:
	std::tuple<T*, size_t>
	first_ins(T* _res)
	{
		cycle_node* _new = new cycle_node(_res);
		_new.next = _new;
		++size_;
		return std::make_tuple<T*, size_t>(curr_->next->res.get(), size_);
	}

private:
	cycle_node* curr_;
	size_t size_;
};

template<typename T>
cycle<T>::cycle(T* _res) : curr_(nullptr), size_(0)
{
	first_ins(std::forward<T>(_res));
}

template<typename T>
cycle<T>::~cycle()
{
	cycle_node**curr_addr = &curr_;
	while(*curr_addr != nullptr)
	{
		curr_ = curr_->next;
		delete *curr_addr;
		curr_addr = &curr_;
		--size_;
	}
}

template<typename T>
std::tuple<T*, size_t> cycle<T>::insert(T* _res) noexcept
{
	// TODO: bad_alloc
	cycle_node* _new = new cycle_node(_res);
	cycle_node* next = curr_->next;
	_new->next = next;
	curr_->next = _new;
	++size_;
	return std::make_tuple<T*, size_t>(curr_->next->res.get(), size());
}

}// namespace:tools
}// namespace:simtalk
#endif// CYCLE_HPP
