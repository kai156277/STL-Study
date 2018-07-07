#pragma once

#include "tiny_config.h"

TINY_STL_BEGIN_NAMESPACE

// ��Ϊ���ؾ����־���������
// ��class������������ĸ��ַ����ǩ���������Դٳ����ػ��Ƶĳɹ����У�
// ��һ���ô��ǣ�ͨ���̳У����ǿ��Բ���Ϊ�㷨ʵ��ÿһ�������������ء�
// ��һ���㷨֧��forward������ʱ����ôι����bidrectional������ʱҲ����������

struct input_iterator_tag {};	// ֻ����������־
struct output_iterator_tag {};	// ֻд��������־

// Forward��������Input/Output�������Ľ�ϣ�����Input��������ȫ�����ܺ�Output�������Ĵ󲿷ֹ��ܡ�
// Forward�������ܶ��ָ��ͬһ�����е�ͬһԪ�أ����ܶ�δ���ͬһԪ��
struct forward_iterator_tag : public input_iterator_tag {}; // ǰ���������־

// ˫���������Forward�������Ļ����������˷������������
struct bidirectional_iterator_tag : public forward_iterator_tag {}; // ˫���������־
struct random_access_iterator_tag : public bidirectional_iterator_tag {}; // ������ʵ�������־

// iterator �Ļ�����ʽ
template<typename Category, typename T,
		 typename Distance = ptrdiff_t,
		 typename Pointer = T*,
		 typename Reference = T&>
struct iterator
{
	typedef Category	iterator_category; // ����������
	typedef T			value_type; // ��������ָ���������
	typedef Distance	difference_type; // ��ʾ����������֮��ľ���
	typedef Pointer		pointer; // ָ���������ָ֮��
	typedef Reference	reference; // 
};

// ����ר����ȡ�����������ԣ���Ϊһ��ģ�壬���Խ�ģ�����ָ��Ϊԭ��ָ������(T*)������ԭ��ָ��T*
// ��Ȼ����һ��class type�����ͨ��traitsȡ��value type.
template<typename Iterator>
struct iterator_traits
{
	typedef typename Iterator::iterator_category	iterator_category;
	typedef typename Iterator::value_type			value_type;
	typedef typename Iterator::difference_type		difference_type;
	typedef typename Iterator::pointer				pointer;
	typedef typename Iterator::reference			reference;
};

// ��ԭ��ָ�����Ƶ�traitsƫ�ػ���
template<typename T>
struct iterator_traits<T*>
{
	typedef random_access_iterator_tag	iterator_category;
	typedef T							value_type;
	typedef ptrdiff_t					difference_type;
	typedef T*							pointer;
	typedef T&							reference;
};

// ��ԭ���� pointer-to-const ����Ƶ�traitsƫ�ػ���
template<typename T>
struct iterator_traits<const T*>
{
	typedef random_access_iterator_tag	iterator_category;
	typedef T							value_type;
	typedef ptrdiff_t					difference_type;
	typedef const T*					pointer;
	typedef const T&					reference;
};

// �жϵ�����������
template<typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
	return typename iterator_traits<Iterator>::iterator_category();
}

// ������������distance type
template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// ������������value type
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

// distance() ���Խ����κ����͵ĵ�������Ϊ����ѭSTL�㷨�������������㷨���ܽ��ܵ������������
// Ϊ���������������
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