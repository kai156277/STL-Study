#pragma once
#include "../tiny_config.h"
#include "../tiny_type_traits.h"
#include "../tiny_iterator.h"

TINY_STL_BEGIN_NAMESPACE

template<typename RandomAccessIterator, typename BidirectionIterator, typename Distance>
inline BidirectionIterator __copy_backward_by_random_access_iter(RandomAccessIterator first, RandomAccessIterator last,
																 BidirectionIterator result, Distance*)
{
#ifdef MORE_DEBUG_INFO
	printf("\t\t Random access iterator copy... \n");
#endif // MORE_DEBUG_INFO
	Distance n = last - first;
	while (n > 0)
	{
		--n;
		--result;
		--last;
		*result = *last;
	}
	return result;
}

// 指针所指的对象具备 default assignment operator 默认赋值运算符
template<typename T>
inline T* __copy_backward_by_default_assignment_operator_type(const T* first, const T* last,
															  T* result, true_type)
{
#ifdef MORE_DEBUG_INFO
	printf("\t\t default assignment operator copy... \n");
#endif // MORE_DEBUG_INFO

	T* dst = result - (last - first);
	memmove(dst, first, sizeof(T) * (last - first));
	return dst;
}

// 指针所指的对象具备 non-default assignment operator 用户自定义赋值运算符
template<typename T>
inline T* __copy_backward_by_default_assignment_operator_type(const T* first, const T* last,
															  T* result, false_type)
{
#ifdef MORE_DEBUG_INFO
	printf("\t\t non-default assignment operator copy... \n");
#endif // MORE_DEBUG_INFO

	return __copy_backward_by_random_access_iter(first, last, result, (ptrdiff_t*)(0));
}

// BidirectionIterator 版本
template<typename BidirectionIterator1, typename BidirectionIterator2>
inline BidirectionIterator2 __copy_backward_by_iter_type(BidirectionIterator1 first, BidirectionIterator1 last,
														 BidirectionIterator2 result, input_iterator_tag)
{
#ifdef MORE_DEBUG_INFO
	printf("Input iterator copy... \n");
#endif // MORE_DEBUG_INFO

	while (first != last)
	{
		--last;
		--result;
		*result = *last;
	}

	return result;
}

// RandomAccessIterator 版本
template<typename RandomAccessIterator, typename BidirectionIterator>
inline BidirectionIterator __copy_backward_by_iter_type(RandomAccessIterator first, RandomAccessIterator last,
														BidirectionIterator result, random_access_iterator_tag)
{
	return __copy_backward_by_random_access_iter(first, last, result, distance_type(first));
}

template<typename BidirectionIterator1, typename BidirectionIterator2>
struct __copy_backward_dispatch
{
	BidirectionIterator2 operator()(BidirectionIterator1 first,
									BidirectionIterator1 last,
									BidirectionIterator2 result)
	{
	#ifdef MORE_DEBUG_INFO
		printf("\t__copy_backward_dispatch copy...\n");
	#endif // MORE_DEBUG_INFO

		return __copy_backward_by_iter_type(first, last, result, iterator_category(first));
	}
};

template<typename T>
struct __copy_backward_dispatch<T*, T*>
{
	T* operator()(T* first, T* last, T* result)
	{
	#ifdef MORE_DEBUG_INFO
		printf("\t __copy_backward_dispatch<T*, T*> copy...\n");
	#endif // MORE_DEBUG_INFO
		typedef typename type_traits<T>::has_default_assignment_operator t;
		return __copy_backward_by_default_assignment_operator_type(first, last, result, t());
	}
};

template<typename T>
struct __copy_backward_dispatch<const T*, T*>
{
	T* operator()(const T* first, const T* last, T* result)
	{
	#ifdef MORE_DEBUG_INFO
		printf("\t __copy_backward_dispatch<const T*, T*> copy...\n");
	#endif // MORE_DEBUG_INFO
		typedef typename type_traits<T>::has_default_assignment_operator t;
		return __copy_backward_by_default_assignment_operator_type(first, last, result, t());
	}
};


template<typename BidirectionIterator1, typename BidirectionIterator2>
inline BidirectionIterator2 copy_backward(BidirectionIterator1 first,
										  BidirectionIterator1 last,
										  BidirectionIterator2 result)
{
#ifdef MORE_DEBUG_INFO
	printf("template copy_backward...\n");
#endif // MORE_DEBUG_INFO
	return __copy_backward_dispatch<BidirectionIterator1, BidirectionIterator2>()(first, last, result);
}

// specialization for char
inline char* copy_backward(const char* first, const char* last, char* result)
{
#ifdef MORE_DEBUG_INFO
	printf("const char copy_backward... \n");
#endif // MORE_DEBUG_INFO
	
	char* dst = result - (last - first);
	memmove(dst, first, last - first);
	return dst;
}

// specialization for wchar
inline wchar_t* copy_backward(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
#ifdef MORE_DEBUG_INFO
	printf("const wchar_t copy_backward... \n");
#endif // MORE_DEBUG_INFO

	wchar_t* dst = result - (last - first);
	wmemmove(dst, first, sizeof(wchar_t) * (last - first));
	return dst;
}
TINY_STL_END_NAMESPACE
