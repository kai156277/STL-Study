#include "tiny_vector_unit_test.h"

#include "tiny_vector.h"
#include "tiny_print.h"
#include <cstdio>

void tiny_vector_clear_unit_test();

void tiny_vector_push_back_unit_test();

void tiny_vector_pop_back_unit_test();

void tiny_vector_erase_unit_test();

void tiny_vector_insert_unit_test();

using namespace tiny;
void tiny_vector_constructor_unit_test()
{
	vector<int> vec;
	for (int n : vec)
		printf("%d ", n);
	putchar('\n');
	
	vector<int> vec1((size_t)20, 10);	/// = { 1, 2, 3 , 4, 5 };
	for (int n : vec1)
		printf("%d ", n);
	putchar('\n');

	vector<int> vec2((size_t)20);	/// = { 1, 2, 3 , 4, 5 };
	putchar('\n');

	vector<int> vec3(10, 20);	/// = { 1, 2, 3 , 4, 5 };
	for (int n : vec3)
		printf("%d ", n);
	putchar('\n');


	int a[10] = { 1, 2, 3 };
	vector<int> vec4(a, a+3);
	for (int n : vec4)
		printf("%d ", n);
	putchar('\n');

	vector<int> vec5(vec4);
	for (int n : vec5)
		printf("%d ", n);
	putchar('\n');

	vector<int> vec6 = { 1, 2, 3, 4, 5, 6 };
	for (int n : vec6)
		printf("%d ", n);
	putchar('\n');
}

void tiny_vector_element_access_unit_test()
{
	vector<int> vec = { 1, 2, 3, 4, 5, 6 };
	for (size_t i = 0; i < vec.size(); ++i)
		printf("operator[%d]: %d\n", i, vec[i]);

	for (size_t i = 0; i < vec.size(); ++i)
		printf("at(%d): %d\n", i, vec.at(i));

	printf("front: %d\n", vec.front());
	printf("back: %d\n", vec.back());
}

void tiny_vector_modifiers_unit_test()
{
#if 0
	tiny_vector_push_back_unit_test();
	tiny_vector_pop_back_unit_test();
	tiny_vector_erase_unit_test();
	tiny_vector_clear_unit_test();
#endif
	tiny_vector_insert_unit_test();
}

void tiny_vector_member_unit_test()
{
	tiny_vector_capacity_unit_test();
	tiny_vector_element_access_unit_test();
	tiny_vector_modifiers_unit_test();
}

void tiny_vector_capacity_unit_test()
{
	vector<int> vec;
	printf("is empty: %s\n", vec.empty() ? "true" : "false");
	printf("size: %d\n", vec.size());
	printf("capacity: %d\n", vec.capacity());
	vec.reserve(10);
	printf("reserve to 10 element: %d\n", vec.size());

	vector<int> vec1(10);

	printf("is empty: %s\n", vec1.empty() ? "true" : "false");
	printf("size: %d\n", vec1.size());
	printf("capacity: %d\n", vec1.capacity());
	vec1.reserve(0);
	printf("reserve to 0 element: %d\n", vec1.size());
}


void tiny_vector_clear_unit_test()
{
	vector<int> vec = { 1, 2, 3, 4, 5, 6 };
	STL_printf(vec);
	vec.clear();
	STL_printf(vec);
}

void tiny_vector_push_back_unit_test()
{
	vector<int> vec6;
	for (int i = 0; i < 10; ++i)
	{
		printf("value: %d \t", i);
		vec6.push_back(i);
	}

	STL_printf(vec6);
}

void tiny_vector_pop_back_unit_test()
{
	vector<int> vec6 = { 1, 2, 3, 4, 5, 6 };
	for (int i = 0; i < 10; ++i)
	{
		STL_printf(vec6);
		vec6.pop_back();
	}
}

void tiny_vector_erase_unit_test()
{
	vector<int> vec6 = { 1, 2, 3, 4, 5, 6 };
	size_t size = vec6.size();
	for (size_t i = 0; i < size; ++i)
	{
		STL_printf(vec6);
		vec6.erase(vec6.begin());
	}

	vector<int> vec5 = { 1, 2, 3, 4, 5, 6 };
	size = vec5.size();
	for (size_t i = 0; i < size; ++i)
	{
		STL_printf(vec5);
		vector<int>::iterator iter = vec5.end();
		vec5.erase(--iter);
	}
	vector<int> vec4 = { 1, 2, 3, 4, 5, 6 };

	for (size_t i = 0; i < 3; ++i)
	{
		STL_printf(vec4);
		vector<int>::iterator iter = vec4.begin();
		vec4.erase(iter + i);
	}
	vector<int>::iterator iter;
	vector<int> vec3 = { 1, 2, 3, 4, 5, 6 };
	STL_printf(vec3);
	iter = vec3.erase(vec3.begin() + 2, vec3.begin() + 4);
	printf("%d\n", *iter);
	STL_printf(vec3);
	iter = vec3.erase(vec3.begin(), vec3.end());
	printf("%d\n", *iter);
	STL_printf(vec3);
}

void tiny_vector_insert_unit_test()
{
	vector<int> vec6 = { 1, 2, 3, 4, 5, 6 };
	size_t size = vec6.size();
	for (size_t i = 0; i < size; ++i)
	{
		vec6.insert(vec6.begin() + i, i);
		STL_printf(vec6);
	}

	size = vec6.size();
	for (size_t i = 0; i < size; ++i)
	{
		vector<int>::const_iterator citer = vec6.insert(vec6.cbegin() + i, i);
		printf("%d ", *citer);
		STL_printf(vec6);
	}

	size = vec6.size();
	for (size_t i = 0; i < size; ++i)
	{
		vec6.insert(vec6.begin() + i, 2, i);
		STL_printf(vec6);
	}

	size = vec6.size();
	for (size_t i = 0; i < size; ++i)
	{
		vector<int>::const_iterator citer = vec6.insert(vec6.cbegin() + i, 2, i);
		STL_printf(vec6);
	}
}
