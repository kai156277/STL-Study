#pragma once

#include "tiny_config.h"
#include "tiny_iterator.h"

TINY_STL_BEGIN_NAMESPACE

// accumulate 用于计算 init 和 [first, last) 内所有元素的总和，将每个元素累加到初值 init 上
// 初始值 init 的意义在于当[first, last)为空区间时仍能获得一个明确定义的值
#pragma region[accumulate]
template<typename InputIterator, typename T>
T accumulate(InputIterator first, InputIterator last, T init)
{
	for (; first != last; ++first)
		init = init + *first;

	return init;
}

// accumulate 用于计算 init 和 [first, last) 内所有元素的总和，对每个元素执行二元操作 binary_op(init, *first)
template<typename InputIterator, typename T, typename BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op)
{
	for (; first != last; ++first)
		init = binary_op(init, *first);

	return init;
}
#pragma endregion

// adjacent_difference 用于计算 [first, last) 中相邻元素的差额。
// 第一个元素将被置于第一个位置，这样做在使用 partial_sum 函数时便可重建序列。
// 如果令 result 等于 first， 那么它将成为一个质变算法。
#pragma region[adjacent_difference]
template<typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result)
{
	if (first == last)
		return result;

	*result = *first;	// 记录第一个元素
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

// 采用外界提供的二元仿函数
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

// inner_product 能够计算 [first1, last1) 和 [first2, first2 + (last1 - first1)) 的一般内积。
// init 的意义在于当 [first, last) 为空时，仍可获得一个明确定义的结果
#pragma region[inner_product]
template<typename InputIterator1, typename InputIterator2, typename T>
T inner_product(InputIterator1 first1, InputIterator1 last1, 
				InputIterator2 first2, T init)
{
	for (; first1 != last1; ++first1, ++first2)
		init += ((*first1) * (*first2));

	return init;
}

// binary_op1 用于初始化 init 值
// binary_op2 用于处理 first1 和 first2
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

// partial_sum 用来计算局部总和
// 当first == result 时，这时一个质变算法
// 本算法返回输出区间最尾端的位置。
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
		value += *first; // 求和
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
		value = binary_op(value, *first); // 求和
		*(++result) = value;
	}
	return ++result;
}
#pragma endregion

// iota 设定某个区间的内容，使其内的每一个元素从指定的 value 开始，呈现递增状态。
template<typename ForwardIterator, typename T>
void iota(ForwardIterator first, ForwardIterator last, T value)
{
	while (first != last)
		*first++ = value++;
}

TINY_STL_END_NAMESPACE
