#pragma once
#include "tiny_config.h"
#include "tiny_iterator.h"
#include "tiny_type_traits.h"
#include <utility>

#include "algorithm\tiny_copy.h"
#include "algorithm\tiny_copy_backward.h"

TINY_STL_BEGIN_NAMESPACE

// equal ������ [first, last)������ȣ����� True ������ڶ����е�Ԫ�رȽ϶࣬�������Ԫ�ز��迼�ǡ�
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

// fill �� [first, last) �ڵ�����Ԫ�ظ�����ֵ
template<typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
	for (; first != last; ++first)
		*first = value;
}

// fill_n �� [first, first + n) �ڵ�Ԫ�ظ�����ֵ�����صĵ�����ָ����������һ��Ԫ�ص���һ��λ��
template<typename OutputIterator, typename Size, typename T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value)
{
	for (; n > 0; --n, ++first)
		*first = value;

	return first;
}

// iter_swap ������ ForwardIterator ��ָ����Ե�
template<typename ForwardIterator1, typename ForwardIterator2>
void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
{
	typename iterator_traits<ForwardIterator1>::value_type tmp = *a;
	*a = *b;
	*b = tmp;
}

// lexicographical_compare ��"�ֵ�����ʽ"����������[first1, last1)��[first2, last2)���бȽϡ�
// ����һ���бȵڶ�����Сʱ������true
#pragma region[lexicographical_compare]
template<typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
							 InputIterator2 first2, InputIterator2 last2)
{
	// ����һ�����е���β�ˣ��ͽ��������������о���ӦԪ��һһ�Ƚ�
	for (; first1 != last1 && first2 != last2; ++first1, ++first2)
	{
		if (*first1 < *first2)
			return true; // ��һ����С�ڵڶ����з��� true

		if (*first2 < *first1)
			return false;// ��һ���д��ڵڶ����з��� false
	}
	// �����һ���еĳ���С�ڵڶ����У���ô��һ����С�ڵڶ�����
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

// max ȡ���������еĽϴ�ֵ��
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

// min ȡ���������еĽ�Сֵ
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

// mismatch ����ƽ�бȽ��������У�ָ������֮��ĵ�һ����ƥ���
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

// swap �����������������
template<typename T>
inline void swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

TINY_STL_END_NAMESPACE
