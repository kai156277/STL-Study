#pragma once

#include "tiny_config.h"
#include "tiny_iterator.h"
#include "tiny_type_traits.h"
#include "tiny_construct.h"

TINY_STL_BEGIN_NAMESPACE

// ���ڴ�����������Ĺ�����Ϊ���뿪���������Ϊ���Ŀ�ĵص�[result, result+(last-first)��
// ��Χ�ڵ�ÿһ����������ָ��δ��ʼ��������uninitialized_copy()��ʹ��copy constructor,
// ����Ϊ������Դ��[first, last)��Χ�ڵ�ÿһ���������һ�ݸ���Ʒ���Ž������Χ�С�
// 
// ������ȫ���乹�캯��(range constructor)ͨ���������������:
// - �����ڴ����飬���԰�����Χ�ڵ�����Ԫ��
// - ʹ�� uninitialized_copy()���ڸ��ڴ������Ϲ���Ԫ�ء�
//
// �������"commit or rollback"���⣬��˼��Ҫô����������б���Ԫ�أ�Ҫô�������κ�Ԫ�ء�
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first,
								   InputIterator last,
								   ForwardIterator result)
{
	return __uninitialized_copy(first, last, result, value_type(result));
}

// [first, last) ��Χ�ڵ�ÿ����������ָ��δ��ʼ�����ڴ棬��ôuninitialized_fill()����
// �÷�Χ�ڲ��� x �ĸ���Ʒ��
//
// �������"commit or rollback"����
template<typename ForwardIterator, typename T>
void uninitialized_fill(ForwardIterator first,
						ForwardIterator last,
						const T& x)
{
	__uninitialized_fill(first, last, x, value_type(first));
}

// ���[first, first + n)��Χ�ڵ�ÿһ����������ָ��δ��ʼ�����ڴ棬��ôuninitialized_fill_n()
// ����� copy constructor, �ڸ÷�Χ�ڲ��� x �ĸ���Ʒ
//
// �������"commit or rollback"����
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

// ��� const char* ���ػ��汾
inline char* uninitialized_copy(const char* first, const char* last, char* result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}

// ��� const wchar_t* ���ػ��汾
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