#pragma once

#include "tiny_config.h"
#include "tiny_iterator.h"
#include "tiny_type_traits.h"
#include "tiny_construct.h"

TINY_STL_BEGIN_NAMESPACE

// 将内存的配置与对象的构造行为分离开来，如果作为输出目的地的[result, result+(last-first)）
// 范围内的每一个迭代器都指向未初始化区域，则uninitialized_copy()会使用copy constructor,
// 给身为输入来源的[first, last)范围内的每一个对象产生一份复制品，放进输出范围中。
// 
// 容器的全区间构造函数(range constructor)通常以两个步骤完成:
// - 配置内存区块，足以包含范围内的所有元素
// - 使用 uninitialized_copy()，在该内存区块上构造元素。
//
// 必须具有"commit or rollback"语意，意思是要么“构造出所有必须元素，要么不构造任何元素“
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first,
								   InputIterator last,
								   ForwardIterator result)
{
	return __uninitialized_copy(first, last, result, value_type(result));
}

// [first, last) 范围内的每个迭代器都指向未初始化的内存，那么uninitialized_fill()会在
// 该范围内产生 x 的复制品。
//
// 必须具有"commit or rollback"语意
template<typename ForwardIterator, typename T>
void uninitialized_fill(ForwardIterator first,
						ForwardIterator last,
						const T& x)
{
	__uninitialized_fill(first, last, x, value_type(first));
}

// 如果[first, first + n)范围内的每一个迭代器都指向未初始化的内存，那么uninitialized_fill_n()
// 会调用 copy constructor, 在该范围内产生 x 的复制品
//
// 必须具有"commit or rollback"语意
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
	return __uninitialized_fill_n(first, n, x, value_type(first));
}

#pragma region[__uninitialized_copy]
template<typename InputIterator, typename ForwardIterator, typename T>
inline ForwardIterator __uninitialized_copy(InputIterator first,
											InputIterator last,
											ForwardIterator result,
											T*)
{
	typedef typename type_traits<T>::is_POD_type is_POD;
	return __uninitialized_copy_aux(first, last, result, is_POD());
}


template<typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first,
												InputIterator last,
												ForwardIterator result,
												true_type)
{
	return std::copy(first, last, result);
}

template<typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first,
												InputIterator last,
												ForwardIterator result,
												false_type)
{
	ForwardIterator cur = result;
	try
	{
		for (; first != last; ++first, ++cur)
		{
			construct(&*cur, *first);
		}
		return cur;
	}
	catch (...)
	{
		destroy(result, cur);
	}
}

// 针对 const char* 的特化版本
inline char* uninitialized_copy(const char* first, const char* last, char* result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}

// 针对 const wchar_t* 的特化版本
inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
	memmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}

#pragma endregion

#pragma region[__uninitialized_fill]
template<typename ForwardIterator, typename T, typename T1>
inline void __uninitialized_fill(ForwardIterator first,
								 ForwardIterator last,
								 const T& x, T1*)
{
	typedef typename type_traits<T1>::is_POD_type is_POD;
	__uninitialized_copy_aux(first, last, x, is_POD());
}

template<typename ForwardIterator, typename T>
inline void __uninitialized_fill_aux(ForwardIterator first,
									 ForwardIterator last,
									 const T& x,
									 true_type)
{
	std::fill(first, last, x);
}

template<typename ForwardIterator, typename T>
inline void __uninitialized_fill_aux(ForwardIterator first,
									 ForwardIterator last,
									 const T& x,
									 false_type)
{
	ForwardIterator cur = first;
	try
	{
		for (; cur != last; ++cur)
			construct(&*cur, x);
	}
	catch (...)
	{
		destroy(first, cur);
	}
}
#pragma endregion

#pragma region[__uninitialized_fill_n]
template<typename ForwardIterator, typename Size, typename T, typename T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,
											  Size n, const T& x, T1*)
{
	typedef typename type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
												  Size n, const T& x, true_type)
{
	return std::fill_n(first, n, x);
}

template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
												  Size n, const T& x, false_type)
{
	ForwardIterator cur = first;
	try
	{
		for (; n > 0; --n, ++cur)
			construct(&*cur, x);
		return cur;
	}
	catch (...)
	{
		destroy(first, cur);
	}
}
#pragma endregion

TINY_STL_END_NAMESPACE