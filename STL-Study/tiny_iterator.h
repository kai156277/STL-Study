#pragma once

#include "tiny_config.h"

TINY_STL_BEGIN_NAMESPACE

// 作为重载决议标志的五个类型
// 以class来定义迭代器的各种分类标签，不仅可以促成重载机制的成功运行，
// 另一个好处是，通过继承，我们可以不必为算法实现每一个迭代器的重载。
// 当一个算法支持forward迭代器时，那么喂给它bidrectional迭代器时也能正常工作

struct input_iterator_tag {};	// 只读迭代器标志
struct output_iterator_tag {};	// 只写迭代器标志

// Forward迭代器是Input/Output迭代器的结合，具有Input迭代器的全部功能和Output迭代器的大部分功能。
// Forward迭代器能多次指向同一容器中的同一元素，并能多次处理同一元素
struct forward_iterator_tag : public input_iterator_tag {}; // 前向迭代器标志

// 双向迭代器在Forward迭代器的基础上增加了反向遍历的能力
struct bidirectional_iterator_tag : public forward_iterator_tag {}; // 双向迭代器标志
struct random_access_iterator_tag : public bidirectional_iterator_tag {}; // 随机访问迭代器标志

// iterator 的基础样式
template<typename Category, typename T,
		 typename Distance = ptrdiff_t,
		 typename Pointer = T*,
		 typename Reference = T&>
struct iterator
{
	typedef Category	iterator_category; // 迭代器类型
	typedef T			value_type; // 迭代器所指对象的类型
	typedef Distance	difference_type; // 表示两个迭代器之间的距离
	typedef Pointer		pointer; // 指向迭代器所指之物
	typedef Reference	reference; // 
};

// 用于专门提取迭代器的特性，作为一个模板，可以将模板参数指定为原生指针类型(T*)，所以原生指针T*
// 虽然不是一种class type，亦可通过traits取其value type.
template<typename Iterator>
struct iterator_traits
{
	typedef typename Iterator::iterator_category	iterator_category;
	typedef typename Iterator::value_type			value_type;
	typedef typename Iterator::difference_type		difference_type;
	typedef typename Iterator::pointer				pointer;
	typedef typename Iterator::reference			reference;
};

// 对原生指针而设计的traits偏特化版
template<typename T>
struct iterator_traits<T*>
{
	typedef random_access_iterator_tag	iterator_category;
	typedef T							value_type;
	typedef ptrdiff_t					difference_type;
	typedef T*							pointer;
	typedef T&							reference;
};

// 对原生的 pointer-to-const 而设计的traits偏特化版
template<typename T>
struct iterator_traits<const T*>
{
	typedef random_access_iterator_tag	iterator_category;
	typedef T							value_type;
	typedef ptrdiff_t					difference_type;
	typedef const T*					pointer;
	typedef const T&					reference;
};

// 判断迭代器的类型
template<typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
	return typename iterator_traits<Iterator>::iterator_category();
}

// 决定迭代器的distance type
template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 决定迭代器的value type
template<typename Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template<typename T>
inline T* value_type(const T*)
{
	return (T*)(0);
}

// distance() 可以接受任何类型的迭代器，为了遵循STL算法的命名规则：以算法所能接受的最初级的类型
// 为其迭代器参数命名
template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
	return __distance(first, last, iterator_category(first));
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag)
{
	typename iterator_traits<InputIterator>::difference_type n = 0;
	while (first != last) 
	{
		++first;
		++n;
	}
	printf("input iterator\n");
	return n;
}

template <typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
	printf("randowm iterator\n");
	return (last - first);
}

template<typename InputIterator, typename Distance>
inline void advance(InputIterator& i, Distance n)
{
	__advance(i, n, iterator_category(i));
}

template<typename InputIterator, typename Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag)
{
	while (n--) ++i;
}

template<typename BidirectionalIterator, typename Distance>
inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
{
	if (n >= 0)
		while (n--) ++i;
	else
		while (n--) --i;
}

template<typename RandomAccessIterator, typename Distance>
inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag)
{
	i += n;
}

TINY_STL_END_NAMESPACE