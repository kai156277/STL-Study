#pragma once

#include "tiny_config.h"
#include "tiny_iterator.h"

TINY_STL_BEGIN_NAMESPACE

// accumulate ���ڼ��� init �� [first, last) ������Ԫ�ص��ܺͣ���ÿ��Ԫ���ۼӵ���ֵ init ��
// ��ʼֵ init ���������ڵ�[first, last)Ϊ������ʱ���ܻ��һ����ȷ�����ֵ
#pragma region[accumulate]
template<typename InputIterator, typename T>
T accumulate(InputIterator first, InputIterator last, T init)
{
	for (; first != last; ++first)
		init = init + *first;

	return init;
}

// accumulate ���ڼ��� init �� [first, last) ������Ԫ�ص��ܺͣ���ÿ��Ԫ��ִ�ж�Ԫ���� binary_op(init, *first)
template<typename InputIterator, typename T, typename BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op)
{
	for (; first != last; ++first)
		init = binary_op(init, *first);

	return init;
}
#pragma endregion

// adjacent_difference ���ڼ��� [first, last) ������Ԫ�صĲ�
// ��һ��Ԫ�ؽ������ڵ�һ��λ�ã���������ʹ�� partial_sum ����ʱ����ؽ����С�
// ����� result ���� first�� ��ô������Ϊһ���ʱ��㷨��
#pragma region[adjacent_difference]
template<typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result)
{
	if (first == last)
		return result;

	*result = *first;	// ��¼��һ��Ԫ��
	typedef typename iterator_traits<InputIterator>::value_type T;
	T value = *first;
	while (++first != last)
	{
		T tmp = *first;
		*(++result) = tmp - value;
		value = tmp;
	}
	return ++result;
}

// ��������ṩ�Ķ�Ԫ�º���
template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
								   OutputIterator result, BinaryOperation binary_op)
{
	if (first == last)
		return result;

	*result = *first;
	typedef typename iterator_traits<InputIterator>::value_type T;
	T value = *first;
	while (++first != last)
	{
		T tmp = *first;
		*(++result) = binary_op(tmp, value);
		value = tmp;
	}
	return ++result;
}
#pragma endregion

// inner_product �ܹ����� [first1, last1) �� [first2, first2 + (last1 - first1)) ��һ���ڻ���
// init ���������ڵ� [first, last) Ϊ��ʱ���Կɻ��һ����ȷ����Ľ��
#pragma region[inner_product]
template<typename InputIterator1, typename InputIterator2, typename T>
T inner_product(InputIterator1 first1, InputIterator1 last1, 
				InputIterator2 first2, T init)
{
	for (; first1 != last1; ++first1, ++first2)
		init += ((*first1) * (*first2));

	return init;
}

// binary_op1 ���ڳ�ʼ�� init ֵ
// binary_op2 ���ڴ��� first1 �� first2
template<typename InputIterator1, typename InputIterator2, typename T,
		 typename BinaryOperation1, typename BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1,
				InputIterator2 first2, T init,
				BinaryOperation1 binary_op1, BinaryOperation2 binary_op2)
{
	for (; first1 != last1; ++first1, ++first2)
		init = binary_op1(init, binary_op2(first1, first2));

	return init;
}
#pragma endregion

// partial_sum ��������ֲ��ܺ�
// ��first == result ʱ����ʱһ���ʱ��㷨
// ���㷨�������������β�˵�λ�á�
#pragma region[partial_sum]
template<typename InputIterator, typename OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result)
{
	if (first == last)
		return result;

	typedef typename iterator_traits<InputIterator>::value_type T;

	T value = *first;
	while (++first != last)
	{
		value += *first; // ���
		*(++result) = value;
	}
	return ++result;
}

template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
						   OutputIterator result, BinaryOperation binary_op)
{
	if (first == last)
		return result;

	typedef typename iterator_traits<InputIterator>::value_type T;

	T value = *first;
	while (++first != last)
	{
		value = binary_op(value, *first); // ���
		*(++result) = value;
	}
	return ++result;
}
#pragma endregion

// iota �趨ĳ����������ݣ�ʹ���ڵ�ÿһ��Ԫ�ش�ָ���� value ��ʼ�����ֵ���״̬��
template<typename ForwardIterator, typename T>
void iota(ForwardIterator first, ForwardIterator last, T value)
{
	while (first != last)
		*first++ = value++;
}

TINY_STL_END_NAMESPACE
