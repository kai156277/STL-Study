#pragma once
#include "tiny_config.h"
#include "tiny_iterator.h"

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
TINY_STL_END_NAMESPACE
