#pragma once

#include <new.h>
#include "tiny_type_traits.h"
#include "tiny_iterator.h"

TINY_STL_BEGIN_NAMESPACE

template<typename T1, typename T2>
inline void construct(T1* ptr, const T2& value)
{
	new (ptr) T1(value);
}

// destroy single object
template<typename T>
inline void destroy(T* ptr)
{
	ptr->~T();
}

// destroy range objects
template<typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	__destroy(first, last, value_type(first));
}

// 通过类型萃取(type_traits)来实现编译时的函数调用决定
template<typename ForwardIterator, typename T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
{
	typedef typename type_traits<T>::has_default_destructor trivial_destructor;
	__destroy_aux(first, last, trivial_destructor());
}

// 使用用户自定义的析构函数
template<typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, false_type)
{
	for (; first < last; ++first)
		destroy(&*first);
}

// 使用默认析构函数
template<typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, true_type)
{}

// destroy 对基本类型的特化
inline void destroy(char*, char*) {}
inline void destroy(int*, int*) {}
inline void destroy(long*, long*) {}
inline void destroy(float*, float*) {}
inline void destroy(double*, double*) {}
inline void destroy(wchar_t*, wchar_t*) {}


TINY_STL_END_NAMESPACE