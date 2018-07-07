#pragma once

#include <iostream>
#include "tiny_config.h"
#include "tiny_alloc.h"
#include "tiny_uninitialized.h"
#include <initializer_list>	// 考虑自行实现

TINY_STL_BEGIN_NAMESPACE

template<typename T, typename Alloc = alloc>
class vector
{
public:
// vector 嵌套型别定义
	typedef T			value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

	typedef const value_type* const_pointer;
	typedef const value_type* const_iterator;
	typedef const value_type& const_reference;
private:
	// vector 的空间配置器
	typedef simple_alloc<T, Alloc> data_allocator;

	iterator start;		// 表示目前使用空间的头
	iterator finish;	// 表示目前使用空间的尾
	iterator end_of_storage; // 表示目前可用空间的尾

	void insert_aux(iterator position, const T& x);
	void deallocate();
	void fill_initalize(size_type n, const T& value);

	template<typename InputIterator>
	void copy_initalize(InputIterator first, InputIterator last);

	template<typename InputIterator>
	void __initalize_aux(InputIterator count, InputIterator value, true_type);

	template<typename InputIterator>
	void __initalize_aux(InputIterator first, InputIterator last, false_type);
public:
	vector() 
		:start(nullptr), finish(nullptr), end_of_storage(nullptr) 
	{
	}
	vector(size_t count, const T& value) 
	{ 
		fill_initalize(count, value);
	}
	explicit vector(size_t count) 
	{ 
		fill_initalize(count, T());
	}

	template<typename InputIterator>
	vector(InputIterator first, InputIterator last) 
	{
		printf("vector construct: input, last\n");
		typedef typename is_integer<InputIterator>::Integral is_POD;
		__initalize_aux(first, last, is_POD());
	}
	vector(const vector& other) 
	{
		copy_initalize(other.cbegin(), other.cend());
	}
#ifdef MOVE_CONSTRUCTOR
	vector(vector&& other) {} // 移动构造需要alloc的额外支持
#endif //MOVE_CONSTRUCTOR 
	vector(std::initializer_list<T> init) 
	{ 
		copy_initalize(init.begin(), init.end()); 
	}
	~vector() 
	{ 
		destroy(start, finish);
		deallocate();
	}

	vector& operator=(const vector& rhs)
	{
		destroy(start, finish);
		copy_initalize(rhs.begin(), rhs.end());
	}

	iterator begin() 
	{ 
		return start;
	}
	const_iterator cbegin() const 
	{
		return start;
	}
	iterator end() 
	{
		return finish;
	}
	const_iterator cend() const 
	{
		return finish;
	}

	size_type size() const 
	{ 
		return size_type(cend() - cbegin());
	}
	size_type capacity() const 
	{ 
		return size_type(end_of_storage - begin());
	}
	bool empty() const 
	{
		return begin() - end();
	}

	reference operator[](size_type n) 
	{
		return *(begin() + n);
	}
	const_reference operator[](size_type n) const 
	{
		return *(begin() + n);
	}

	reference front()
	{ 
		return *begin();
	}
	reference back() 
	{ 
		return *(end() - 1);
	}

	template<typename InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		destroy(start, finish);
		copy_initalize(first, last);
	}
	void assign(size_type count, const T& value) 
	{ 
		destroy(start, finish);
		fill_initalize(count, value);
	}
	void assign(std::initializer_list<T> list)
	{
		assign(list.begin(), list.end());
	}
};

template <typename T, typename Alloc>
inline void vector<T, Alloc>::insert_aux(iterator position, const T& x)
{

}

template<typename T, typename Alloc>
inline void vector<T, Alloc>::deallocate()
{
	if (start)
		data_allocator::deallocate(start, end_of_storage - start);
}

template<typename T, typename Alloc>
inline void vector<T, Alloc>::fill_initalize(size_type n, const T & value)
{
	start = data_allocator::allocate(n);
	uninitialized_fill_n(begin(), n, value);
	finish = start + n;
	end_of_storage = finish;
}

template<typename T, typename Alloc>
template<typename InputIterator>
inline void vector<T, Alloc>::copy_initalize(InputIterator first, InputIterator last)
{
	size_type n = (size_type)distance(first, last);
	start = data_allocator::allocate(n);
	uninitialized_copy(first, last, start);
	finish = start + n;
	end_of_storage = finish;
}

template<typename T, typename Alloc>
template<typename InputIterator>
inline void vector<T, Alloc>::__initalize_aux(InputIterator first, InputIterator last, false_type)
{
	copy_initalize(first, last);
}
template<typename T, typename Alloc>
template<typename InputIterator>
inline void vector<T, Alloc>::__initalize_aux(InputIterator count, InputIterator value, true_type)
{
	fill_initalize(count, value);
}


TINY_STL_END_NAMESPACE

