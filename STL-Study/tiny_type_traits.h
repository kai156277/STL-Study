#pragma once

#include "tiny_config.h"

TINY_STL_BEGIN_NAMESPACE

// 当类型没有提供特殊的构造、拷贝、析构函数和赋值运算符时，可以直接使用
// malloc(), memcpy(), memmove(), free()来处理，以获得最高效率。

// type_traits，提供了一种机制，允许针对不同的类型，在编译时完成函数分发。

// 根据iterator_traits 得来的经验，我们希望这样使用type_traits, 并且响应我们
// 的是一个有着真/假性质的"对象"，因为我们需要其响应结果来进行参数推导。

// tiny_type_traits<T>::has_default_constructor
// tiny_type_traits<T>::has_default_copy_constructor
// tiny_type_traits<T>::has_default_assignment_operator
// tiny_type_traits<T>::has_default_destructor
// tiny_type_traits<T>::is_POD_type

struct true_type {};
struct false_type {};

template<typename type>
struct type_traits
{
	typedef false_type	has_default_constructor;
	typedef false_type	has_default_copy_constructor;
	typedef false_type	has_default_assignment_operator;
	typedef false_type	has_default_destructor;
	typedef false_type	is_POD_type; // POD: Plain Old Data
};

#pragma region[type_traits]
template<> struct type_traits<bool>
{
	typedef true_type	has_default_constructor;
	typedef true_type	has_default_copy_constructor;
	typedef true_type	has_default_assignment_operator;
	typedef true_type	has_default_destructor;
	typedef true_type	is_POD_type;
};

template<> struct type_traits<char> 
{
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<signed char>
{
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<unsigned char>
{
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<wchar_t>
{
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<short>
{
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<unsigned short> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<int> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<unsigned int> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<long> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<unsigned long> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<long long> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<unsigned long long> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<float> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<double> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<long double> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

// 对以下原生指针设计 type_traits 偏特化版本
// 原生指针亦被视为一种标量类型
template <class _Tp>
struct type_traits<_Tp*> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<char*> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<signed char*> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<unsigned char*> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<const char*> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<const signed char*> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};

template<> struct type_traits<const unsigned char*> {
   typedef true_type    has_default_constructor;
   typedef true_type    has_default_copy_constructor;
   typedef true_type    has_default_assignment_operator;
   typedef true_type    has_default_destructor;
   typedef true_type    is_POD_type;
};
#pragma endregion

template<typename type>
struct is_integer
{
	typedef false_type Integral;
};

#pragma region[is_integer]
template<> struct is_integer<bool>
{
	typedef true_type Integral;
};

template<> struct is_integer<char>
{
	typedef true_type Integral;
};

template<> struct is_integer<signed char>
{
	typedef true_type Integral;
};

template<> struct is_integer<unsigned char>
{
	typedef true_type Integral;
};


template<> struct is_integer<wchar_t>
{
	typedef true_type Integral;
};


template<> struct is_integer<short>
{
	typedef true_type Integral;
};

template<> struct is_integer<unsigned short>
{
	typedef true_type Integral;
};

template<> struct is_integer<int>
{
	typedef true_type Integral;
};

template<> struct is_integer<unsigned int>
{
	typedef true_type Integral;
};

template<> struct is_integer<long>
{
	typedef true_type Integral;
};

template<> struct is_integer<unsigned long>
{
	typedef true_type Integral;
};

template<> struct is_integer<long long>
{
	typedef true_type Integral;
};

template<> struct is_integer<unsigned long long>
{
	typedef true_type Integral;
};
#pragma endregion

TINY_STL_END_NAMESPACE
