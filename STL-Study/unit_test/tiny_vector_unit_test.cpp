#include "tiny_vector_unit_test.h"

//#include "tiny_vector.h"
#include <iostream>

using std::cout;
using std::endl;

//using namespace tiny;
void tiny_vector_constructor_unit_test()
{
#if 0
	// vector() 
	vector<int> vec;
	cout << "vec: " << vec.size() << endl;
	for (int n : vec)
		cout << n << " ";
	cout << endl;
	
	// vector(size_t count, const T& value) 
	vector<int> vec1((size_t)20, 10);	/// = { 1, 2, 3 , 4, 5 };
	cout << "vec1: " << vec1.size() << endl;
	for (int n : vec1)
		cout << n << " ";
	cout << endl;

	// explicit vector(size_t count) 
	vector<int> vec2((size_t)20);	/// = { 1, 2, 3 , 4, 5 };
	cout << "vec2: " << vec.size() << endl;

	cout << "vec3: " << endl;
	// vector(InputIterator first, InputIterator last) 
	vector<int> vec3(10, 20);	/// = { 1, 2, 3 , 4, 5 };
	for (int n: vec3)
		cout << n << " ";
	cout << endl;

	cout << "vec4: " << endl;
	// vector(InputIterator first, InputIterator last) 
	int a[10] = { 1, 2, 3 };
	vector<int> vec4(a, a+3);
	for(int n: vec4)
		cout << n << " ";
	cout << endl;

	cout << "vec5: " << endl;
	// vector(const vector& other) 
	vector<int> vec5(vec4);
	for(int n: vec5)
		cout << n << " ";
	cout << endl;

	cout << "vec6: " << endl;
	vector<int> vec6 = { 1, 2, 3, 4, 5, 6 };
	for(int n: vec6)
		cout << n << " ";
	cout << endl;
#endif
}
