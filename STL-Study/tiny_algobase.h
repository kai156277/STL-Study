#pragma once
#include "tiny_config.h"
#include "tiny_iterator.h"
#include "tiny_type_traits.h"
#include <utility>

#include "algorithm\tiny_copy.h"
#include "algorithm\tiny_copy_backward.h"

TINY_STL_BEGIN_NAMESPACE

// equal 序列在 [first, last)区间相等，返回 True 。如果第二序列的元素比较多，多出来的元素不予考虑。
#pragma region[equal]
template<typename InputIterator1, typename InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
	for (; first1 != last1; ++first1, ++first2)
		if (*first1 != *first2)
			return false;

	return true;
}

template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred)
{
	for (; first1 != last1; ++first1, ++first2)
		if (!binary_pred(*first1, *first2))
			return false;

	return true;
}
#pragma endregion

// fill 将 [first, last) 内的所有元素改填新值
template<typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
	for (; first != last; ++first)
		*first = value;
}

// fill_n 将 [first, first + n) 内的元素改填新值，返回的迭代器指向被填入的最后一个元素的下一个位置
template<typename OutputIterator, typename Size, typename T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value)
{
	for (; n > 0; --n, ++first)
		*first = value;

	return first;
}

// iter_swap 将两个 ForwardIterator 所指对象对调
template<typename ForwardIterator1, typename ForwardIterator2>
void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
{
	typename iterator_traits<ForwardIterator1>::value_type tmp = *a;
	*a = *b;
	*b = tmp;
}

// lexicographical_compare 以"字典排序方式"对两个序列[first1, last1)和[first2, last2)进行比较。
// 当第一序列比第二序列小时，返回true
#pragma region[lexicographical_compare]
template<typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
							 InputIterator2 first2, InputIterator2 last2)
{
	// 任意一个序列到达尾端，就结束。否则两序列就相应元素一一比较
	for (; first1 != last1 && first2 != last2; ++first1, ++first2)
	{
		if (*first1 < *first2)
			return true; // 第一序列小于第二序列返回 true

		if (*first2 < *first1)
			return false;// 第一序列大于第二序列返回 false
	}
	// 如果第一序列的长度小于第二序列，那么第一序列小于第二序列
	return (first1 == last1) && (first2 != last2);
}

template<typename InputIterator1, typename InputIterator2, typename Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
							 InputIterator2 first2, InputIterator2 last2, Compare comp)
{
	for (; first1 != last1 && first2 != last2; ++first1, ++first2)
	{
		if (comp(*first1, *first2))
			return true; 

		if (comp(*first2, *first1))
			return false;
	}
	return (first1 == last1) && (first2 != last2);
}
#pragma endregion

// max 取两个对象中的较大值。
#pragma region[max]
template<typename T>
inline const T& max(const T& a, const T& b)
{
	return a < b ? b : a;
}

template<typename T, typename Compare>
inline const T& max(const T& a, const T& b, Compare comp)
{
	return comp(a, b) ? b : a;
}
#pragma endregion

// min 取两个对象中的较小值
#pragma region[min]
template<typename T>
inline const T& min(const T& a, const T& b)
{
	return b < a ? b : a;
}

template<typename T, typename Compare>
inline const T& min(const T& a, const T& b, Compare comp)
{
	return comp(b, a) ? b : a;
}
#pragma endregion

// mismatch 用来平行比较两个序列，指出两者之间的第一个不匹配点
#pragma region[mismatch]
template<typename InputIterator1, typename InputIterator2>
std::pair<InputIterator1, InputIterator2> 
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
	while (first1 != last1 && *first1 == *first2)
	{
		++first1;
		++first2;
	}
	return std::pair<InputIterator1, InputIterator2>(first1, first2);
}

template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
std::pair<InputIterator1, InputIterator2> 
mismatch(InputIterator1 first1, InputIterator1 last1, 
		 InputIterator2 first2, BinaryPredicate binary_pred)
{
	while (first1 != last1 && binary_pred(*first1, *first2))
	{
		++first1;
		++first2;
	}
	return std::pair<InputIterator1, InputIterator2>(first1, first2);
}
#pragma endregion

// swap 交换两个对象的内容
template<typename T>
inline void swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

TINY_STL_END_NAMESPACE
