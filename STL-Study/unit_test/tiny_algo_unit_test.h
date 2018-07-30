#pragma once

#include "tiny_type_traits.h"
class copy_class
{
public:
	copy_class() : _data(3)
	{}
	int _data;
};

class copy_class_def
{
public:
	copy_class_def() :_data(3), _data1(2)
	{}
	int _data;
	int _data1;
};
namespace tiny {
	template<>
	struct type_traits<copy_class_def>
	{
		typedef true_type	has_default_constructor;
		typedef true_type	has_default_copy_constructor;
		typedef true_type	has_default_assignment_operator;
		typedef true_type	has_default_destructor;
		typedef false_type	is_POD_type; // POD: Plain Old Data
	};
}

void tiny_copy_unit_test();
void tiny_copy_backward_unit_test();
void tiny_STL_print_unit_test();