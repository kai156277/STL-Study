#pragma once

#include "tiny_config.h"

TINY_STL_BEGIN_NAMESPACE

// ������û���ṩ����Ĺ��졢���������������͸�ֵ�����ʱ������ֱ��ʹ��
// malloc(), memcpy(), memmove(), free()�������Ի�����Ч�ʡ�

// type_traits���ṩ��һ�ֻ��ƣ�������Բ�ͬ�����ͣ��ڱ���ʱ��ɺ����ַ���

// ����iterator_traits �����ľ��飬����ϣ������ʹ��type_traits, ������Ӧ����
// ����һ��������/�����ʵ�"����"����Ϊ������Ҫ����Ӧ��������в����Ƶ���

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

// ������ԭ��ָ����� type_traits ƫ�ػ��汾
// ԭ��ָ���౻��Ϊһ�ֱ�������
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
