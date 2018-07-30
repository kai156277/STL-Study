#pragma once
#include "tiny_vector.h"
#include <iostream>

template<typename T>
struct item_printf
{
	void operator()(const T& item)
	{
		std::cout << item << " ";
	}
};

template<typename T, 
	template<typename T, typename Alloc = tiny::alloc > class VECTOR = tiny::vector>
void inline STL_printf(const VECTOR<T>& vec)
{
#ifdef MORE_DEBUG_INFO
	printf("void vector_printf(const %s<%s>&)... \n", typeid(VECTOR).name(), typeid(T).name());
#endif
	printf_by_iter_type(vec, item_printf<T>(), iterator_category(vec.cbegin()));
}

template<typename T>
void inline printf_by_iter_type(const T& container, item_printf<typename T::value_type> print, tiny::random_access_iterator_tag)
{
	for (size_t i = 0; i < container.size(); ++i)
		print(container[i]);
	putchar('\n');
}

template<typename T>
void inline printf_by_iter_type(const T& container, item_printf<typename T::value_type> print, tiny::input_iterator_tag)
{
	for (auto iter = vec.cbegin(); iter != vec.cend(); iter++)
		print(*iter);
	putchar('\n');
}

