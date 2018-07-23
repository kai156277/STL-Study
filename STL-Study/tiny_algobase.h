#pragma once
#include "tiny_config.h"
#include "tiny_iterator.h"
#include "tiny_type_traits.h"
#include <utility>

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

#pragma region[copy]

#pragma region[copy_dispatch]
template<typename InputIterator, typename OutputIterator>
struct __copy_dispatch
{
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
	{
	#ifdef MORE_DEBUG_INFO
		printf("\t__copy_dispatch copy...\n");
	#endif // MORE_DEBUG_INFO

		return __copy(first, last, result, iterator_category(first));
	}
};

template<typename T>
struct __copy_dispatch<T*, T*>
{
	T* operator()(T* first, T* last, T* result)
	{
	#ifdef MORE_DEBUG_INFO
		printf("\t __copy_dispatch<T*, T*> copy...\n");
	#endif // MORE_DEBUG_INFO
		typedef typename type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};

template<typename T>
struct __copy_dispatch<const T*, T*>
{
	T* operator()(const T* first, const T* last, T* result)
	{
	#ifdef MORE_DEBUG_INFO
		printf("\t __copy_dispatch<const T*, T*> copy...\n");
	#endif // MORE_DEBUG_INFO
		typedef typename type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};
#pragma endregion

#pragma region[copy_iterator]
// InputIterator �汾
template<typename InputIterator, typename OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last,
							 OutputIterator result, input_iterator_tag)
{
#ifdef MORE_DEBUG_INFO
	printf("Input iterator copy... \n");
#endif // MORE_DEBUG_INFO

	for (; first != last; ++result, ++first)
		*result = *first;

	return result;
}

// RandomAccessIterator �汾
template<typename RandomAccessIterator, typename OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last,
							 OutputIterator result, random_access_iterator_tag)
{
	return __copy_d(first, last, result, distance_type(first));
}

template<typename RandomAccessIterator, typename OutputIterator, typename Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
							  OutputIterator result, Distance*)
{
#ifdef MORE_DEBUG_INFO
	printf("\t\t Random access iterator copy... \n");
#endif // MORE_DEBUG_INFO

	for (Distance n = last - first; n > 0; --n, ++result, ++first)
		*result = *first;

	return result;
}

#pragma endregion

#pragma region[assignment]
// ָ����ָ�Ķ���߱� default assignment operator Ĭ�ϸ�ֵ�����
template<typename T>
inline T* __copy_t(const T* first, const T* last,
				   T* result, true_type)
{
#ifdef MORE_DEBUG_INFO
	printf("\t\t default assignment operator copy... \n");
#endif // MORE_DEBUG_INFO

	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}

// ָ����ָ�Ķ���߱� non-default assignment operator �û��Զ��帳ֵ�����
template<typename T>
inline T* __copy_t(const T* first, const T* last,
				   T* result, false_type)
{
#ifdef MORE_DEBUG_INFO
	printf("\t\t non-default assignment operator copy... \n");
#endif // MORE_DEBUG_INFO

	return __copy_d(first, last, result, (ptrdiff_t*)(0));
}

#pragma endregion

template<typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
#ifdef MORE_DEBUG_INFO
	printf("template copy...\n");
#endif // MORE_DEBUG_INFO
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

// specialization for char
inline char* copy(const char* first, const char* last, char* result)
{
#ifdef MORE_DEBUG_INFO
	printf("const char copy... \n");
#endif // MORE_DEBUG_INFO
	memmove(result, first, last - first);
	return result + (last - first);
}

// specialization for wchar
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
#ifdef MORE_DEBUG_INFO
	printf("const wchar_t copy... \n");
#endif // MORE_DEBUG_INFO

	wmemmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}
#pragma endregion

TINY_STL_END_NAMESPACE
