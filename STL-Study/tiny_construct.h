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

// ͨ��������ȡ(type_traits)��ʵ�ֱ���ʱ�ĺ������þ���
template<typename ForwardIterator, typename T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
{
	typedef typename type_traits<T>::has_default_destructor trivial_destructor;
	__destroy_aux(first, last, trivial_destructor());
}

// ʹ���û��Զ������������
template<typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, false_type)
{
	for (; first < last; ++first)
		destroy(&*first);
}

// ʹ��Ĭ����������
template<typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, true_type)
{}

// destroy �Ի������͵��ػ�
inline void destroy(char*, char*) {}
inline void destroy(int*, int*) {}
inline void destroy(long*, long*) {}
inline void destroy(float*, float*) {}
inline void destroy(double*, double*) {}
inline void destroy(wchar_t*, wchar_t*) {}


TINY_STL_END_NAMESPACE