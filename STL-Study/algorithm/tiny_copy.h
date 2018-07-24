#pragma once

#include "../tiny_config.h"
#include "../tiny_type_traits.h"

TINY_STL_BEGIN_NAMESPACE

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

		return __copy_by_iter_type(first, last, result, iterator_category(first));
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
		typedef typename type_traits<T>::has_default_assignment_operator t;
		return __copy_by_default_assignment_operator_type(first, last, result, t());
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
		typedef typename type_traits<T>::has_default_assignment_operator t;
		return __copy_by_default_assignment_operator_type(first, last, result, t());
	}
};
#pragma endregion

#pragma region[copy_iterator]
// InputIterator 版本
template<typename InputIterator, typename OutputIterator>
inline OutputIterator __copy_by_iter_type(InputIterator first, InputIterator last,
										  OutputIterator result, input_iterator_tag)
{
#ifdef MORE_DEBUG_INFO
	printf("Input iterator copy... \n");
#endif // MORE_DEBUG_INFO

	for (; first != last; ++result, ++first)
		*result = *first;

	return result;
}

// RandomAccessIterator 版本
template<typename RandomAccessIterator, typename OutputIterator>
inline OutputIterator __copy_by_iter_type(RandomAccessIterator first, RandomAccessIterator last,
										  OutputIterator result, random_access_iterator_tag)
{
	return __copy_by_random_access_iter(first, last, result, distance_type(first));
}

template<typename RandomAccessIterator, typename OutputIterator, typename Distance>
inline OutputIterator __copy_by_random_access_iter(RandomAccessIterator first, RandomAccessIterator last,
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
// 指针所指的对象具备 default assignment operator 默认赋值运算符
template<typename T>
inline T* __copy_by_default_assignment_operator_type(const T* first, const T* last,
													 T* result, true_type)
{
#ifdef MORE_DEBUG_INFO
	printf("\t\t default assignment operator copy... \n");
#endif // MORE_DEBUG_INFO

	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}

// 指针所指的对象具备 non-default assignment operator 用户自定义赋值运算符
template<typename T>
inline T* __copy_by_default_assignment_operator_type(const T* first, const T* last,
													 T* result, false_type)
{
#ifdef MORE_DEBUG_INFO
	printf("\t\t non-default assignment operator copy... \n");
#endif // MORE_DEBUG_INFO

	return __copy_by_random_access_iter(first, last, result, (ptrdiff_t*)(0));
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