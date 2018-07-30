#pragma once

#include <iostream>
#include "tiny_config.h"
#include "tiny_alloc.h"
#include "tiny_uninitialized.h"
#include "tiny_iterator.h"
#include <initializer_list>	// 考虑自行实现

TINY_STL_BEGIN_NAMESPACE

template<typename T, typename Alloc = alloc>
class vector
{
public:
// vector 嵌套型别定义
#pragma region[member type]
	typedef T			value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

	typedef const value_type* const_pointer;
	typedef const value_type* const_iterator;
	typedef const value_type& const_reference;
#pragma endregion

	vector();
	vector(size_type count, const T& value);
	explicit vector(size_type count);

	template<typename InputIterator>
	vector(InputIterator first, InputIterator last);
	vector(const vector& other);
#ifdef Rvalue_Reverence
	vector(vector&& other) {} // 移动构造需要alloc的额外支持
#endif //Rvalue_Reverence 
	vector(std::initializer_list<T> init);

	~vector();

	vector& operator=(const vector& rhs);
#if 0
	vector& operator=(vector&& other);
	vector& operator=(std::initializer_list<T> ilist);
#endif

	template<typename InputIterator>
	void assign(InputIterator first, InputIterator last);
	void assign(size_type count, const T& value);
	void assign(std::initializer_list<T> list);

	iterator begin();
	const_iterator cbegin() const;
	iterator end();
	const_iterator cend() const;
#if 0
	iterator rbegin();
	const_iterator crbegin() const;
	iterator rend();
	const_iterator crend() const;
#endif

	size_type size() const;
#if 0
	size_type max_size() const;
	void shrink_to_fit();
#endif
	size_type capacity() const;
	bool empty() const;
	void reserve(size_type new_num);

	reference operator[](size_type n);
	const_reference operator[](size_type n) const;

	reference at(size_type n);
	const_reference at(size_type n) const;

	reference front();
	reference back();

	T* data() noexcept;
	const T* data() const noexcept;

	void push_back(const T& value);
	void pop_back();
	iterator erase(iterator pos);
	iterator erase(iterator first, iterator last);

	iterator insert(iterator position, const T& value);
	iterator insert(const_iterator position, const T& value);
#ifdef Rvalue_Reverence
	iterator insert(const_iterator position, const T&& value);
#endif //Rvalue_Reverence
	void insert(iterator position, size_type n, const T& value);
	iterator insert(const_iterator position, size_type n, const T& value);
#if 0
	template<typename InputIterator>
	iterator insert(iterator position, InputIterator first, InputIterator last);
	template<typename InputIterator>
	iterator insert(const_iterator position, InputIterator first, InputIterator last);
	iterator insert(const_iterator position, std::initializer_list<T> ilist);

	template<typename... Args>
	iterator emplace(const_iterator pos, Args&&... args);
	template<typename... Args>
	void emplace_back(Args&&... args);

	void resize(size_type count, T value = T());
	void resize(size_type count);
	void resize(size_type count, const value_type& value);
#endif //  0

	void clear();
	void swap(vector& other);

private:
	// vector 的空间配置器
	typedef simple_alloc<T, Alloc> data_allocator;

	iterator start;		// 表示目前使用空间的头
	iterator finish;	// 表示目前使用空间的尾
	iterator end_of_storage; // 表示目前可用空间的尾

	void deallocate();
	void fill_initalize(size_type n, const T& value);

	template<typename InputIterator>
	void copy_initalize(InputIterator first, InputIterator last);

	template<typename InputIterator>
	void __initalize_aux(InputIterator count, InputIterator value, true_type);

	template<typename InputIterator>
	void __initalize_aux(InputIterator first, InputIterator last, false_type);
};

#if 0
template<typename T, typename Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template<typename T, typename Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template<typename T, typename Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template<typename T, typename Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template<typename T, typename Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template<typename T, typename Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template<typename T, typename Alloc>
void swap(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
#endif

TINY_STL_END_NAMESPACE

#include "tiny_def\tiny_vector_def.h"
