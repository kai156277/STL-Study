#pragma once
#ifndef TINY_VECTOR_DEF
#define TINY_VECTOR_DEF

#include "tiny_algobase.h"
#include "..\tiny_vector.h"

TINY_STL_BEGIN_NAMESPACE

#pragma region[constructor]

template<typename T, typename Alloc>
inline vector<T, Alloc>::vector()
	:start(nullptr), finish(nullptr), end_of_storage(nullptr)
{
#ifdef MORE_DEBUG_INFO
	printf("\n default constructor... size: %d \n", size());
#endif // MORE_DEBUG_INFO
}

template<typename T, typename Alloc>
inline vector<T, Alloc>::vector(size_type count, const T & value)
{
	fill_initalize(count, value);
#ifdef MORE_DEBUG_INFO
	printf("\n vector(size_t %d, const %s& value)... size: %d \n", count, typeid(T).name(), size());
#endif // MORE_DEBUG_INFO
}

template<typename T, typename Alloc>
inline vector<T, Alloc>::vector(size_type count)
{
	fill_initalize(count, T());
#ifdef MORE_DEBUG_INFO
	printf("\n explicit vector(size_t %d)... size: %d \n", count, size());
#endif // MORE_DEBUG_INFO
}

template<typename T, typename Alloc>
inline vector<T, Alloc>::vector(const vector & other)
{
	copy_initalize(other.cbegin(), other.cend());
#ifdef MORE_DEBUG_INFO
	printf("\n vector<%s>(const vector& other)... size: %d \n", typeid(T).name(), size());
#endif // MORE_DEBUG_INFO
}

template<typename T, typename Alloc>
template<typename InputIterator>
inline vector<T, Alloc>::vector(InputIterator first, InputIterator last)
{
	typedef typename is_integer<InputIterator>::Integral is_POD;
	__initalize_aux(first, last, is_POD());
#ifdef MORE_DEBUG_INFO
	// 这个地方写的有点问题
	//typedef typename iterator_traits<InputIterator*>::value_type input_type;
	const char* type_name = typeid(InputIterator).name();
	printf("\n vector(%s first, %s last)... size: %d \n", type_name, type_name, size());
#endif // MORE_DEBUG_INFO
}

template<typename T, typename Alloc>
inline vector<T, Alloc>::vector(std::initializer_list<T> init)
{
	copy_initalize(init.begin(), init.end());
#ifdef MORE_DEBUG_INFO
	printf("\n vector(std::initializer_list<%s> init)... size: %d \n", typeid(T).name(), size());
#endif // MORE_DEBUG_INFO
}

#pragma endregion

template<typename T, typename Alloc>
inline vector<T, Alloc>::~vector()
{
	destroy(start, finish);
	deallocate();
}

template<typename T, typename Alloc>
inline vector<T, Alloc>& vector<T, Alloc>::operator=(const vector & rhs)
{
	destroy(start, finish);
	copy_initalize(rhs.begin(), rhs.end());
}

template<typename T, typename Alloc>
inline void vector<T, Alloc>::assign(size_type count, const T & value)
{
	destroy(start, finish);
	deallocate();
	fill_initalize(count, value);
}

template<typename T, typename Alloc>
inline void vector<T, Alloc>::assign(std::initializer_list<T> list)
{
	assign(list.begin(), list.end());
}

#pragma region[iterators]

template<typename T, typename Alloc>
inline typename vector<T, Alloc>::iterator vector<T, Alloc>::begin()
{
	return start;
}

template<typename T, typename Alloc>
inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::cbegin() const
{
	return start;
}

template<typename T, typename Alloc>
inline typename vector<T, Alloc>::iterator vector<T, Alloc>::end()
{
	return finish;
}

template<typename T, typename Alloc>
inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::cend() const
{
	return finish;
}

#pragma endregion

#pragma region capacity

template<typename T, typename Alloc>
inline typename vector<T, Alloc>::size_type vector<T, Alloc>::size() const
{
	return size_type(cend() - cbegin());
}

template<typename T, typename Alloc>
inline typename vector<T, Alloc>::size_type vector<T, Alloc>::capacity() const
{
	return size_type(end_of_storage - cbegin());
}

template<typename T, typename Alloc>
inline bool vector<T, Alloc>::empty() const
{
	return (cend() - cbegin()) == 0;
}

#pragma endregion

#pragma region[element access]

template<typename T, typename Alloc>
inline typename vector<T, Alloc>::reference vector<T, Alloc>::operator[](size_type n)
{
	return *(begin() + n);
}

template<typename T, typename Alloc>
inline typename vector<T, Alloc>::const_reference vector<T, Alloc>::operator[](size_type n) const
{
	return *(cbegin() + n);
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::at(size_type n)
{
	if (n >= size())
	{
		char msg[125];
		sprintf_s(msg, 125, "\"vector<%s>::at(%d)\" is out of range!", typeid(T).name(), n);
		throw std::out_of_range(msg);
	}

	return this->operator[](n);
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::at(size_type n) const
{
	if (n >= size())
	{
		char msg[125];
		sprintf_s(msg, 125, "\"vector<%s>::at(%d) const\" is out of range!", typeid(T).name(), n);
		throw std::out_of_range(msg);
	}

	return this->operator[](n);
}

template<typename T, typename Alloc>
inline typename vector<T, Alloc>::reference vector<T, Alloc>::front()
{
	return *begin();
}

template<typename T, typename Alloc>
inline typename vector<T, Alloc>::reference vector<T, Alloc>::back()
{
	return *(end() - 1);
}

template<typename T, typename Alloc>
inline T* vector<T, Alloc>::data() noexcept
{
	return begin();
}

template<typename T, typename Alloc>
inline const T* vector<T, Alloc>::data() const noexcept
{
	return cbegin();
}

#pragma endregion

#pragma region [modifiers]
template<typename T, typename Alloc>
void vector<T, Alloc>::push_back(const T & value)
{
	if (finish != end_of_storage)
	{
	#ifdef MORE_DEBUG_INFO
		printf("push_back. vector has spare space... size: %d\t capacity: %d\n", size(), capacity());
	#endif // MORE_DEBUG_INFO

		construct(finish, value);
		++finish;
	}
	else
	{
	#ifdef MORE_DEBUG_INFO
		printf("push_back. vector has no spare space... size: %d\t capacity: %d\n", size(), capacity());
	#endif // MORE_DEBUG_INFO
		insert(finish, value);
	}
}

template<typename T, typename Alloc>
void vector<T, Alloc>::pop_back()
{
	if (start != finish)
	{
		--finish;
		destroy(finish);
	}
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator pos)
{
	if (pos + 1 != finish)
		copy(pos + 1, finish, pos);

	--finish;
	destroy(finish);
	return pos;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last)
{
	iterator i = copy(last, finish, first);
	destroy(i, finish);
	finish = finish - (last - first);
	return first;
}

template<typename T, typename Alloc>
inline void vector<T, Alloc>::clear()
{
	erase(begin(), end());
}

template<typename T, typename Alloc>
inline void vector<T, Alloc>::reserve(size_type new_num)
{
	if (new_num > capacity())
	{
		assign(new_num, T());
	}
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const T & value)
{
	if (finish != end_of_storage)	// 还有备用空间
	{
		#ifdef MORE_DEBUG_INFO
			printf("insert. vector has spare space... size: %d\t capacity: %d\n", size(), capacity());
		#endif // MORE_DEBUG_INFO
		if (finish == position)
		{
			construct(finish, value);
			return finish++;
		}

		construct(finish, *(finish - 1)); // 在备用空间起始处构造一个元素，并以最后一个元素值为初值
		copy_backward(position, finish - 1, finish);
		*position = value;
		return finish++;
	}
	else
	{
		#ifdef MORE_DEBUG_INFO
			printf("insert. vector has no spare space... size: %d\t capacity: %d\n", size(), capacity());
		#endif // MORE_DEBUG_INFO
		const size_type old_size = size();
		const size_type new_size = (old_size != 0) ? (2 * old_size) : 1;

		iterator new_start = data_allocator::allocate(new_size);
		iterator new_finish = new_start;
		iterator new_position = new_finish;
		try
		{
			new_finish = uninitialized_copy(start, position, new_start);
			construct(new_finish, value);
			new_position = new_finish++;
			new_finish = uninitialized_copy(position, finish, new_finish);
		}
		catch (const std::exception& )
		{
			// "commit or rollback" semantics.
			destroy(new_start, new_finish);
			data_allocator::deallocate(new_start);
			throw;
		}

		// 析构并释放原 vector
		destroy(begin(), end());
		deallocate();

		// 调整迭代器，指向新 vector
		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + new_size;
		return new_position;
	}
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator position, const T & value)
{
	iterator _pos = const_cast<iterator>(position);
	return insert(_pos, value);
}

template<typename T, typename Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& value)
{
	if (n == 0) return;

	if(size_type(end_of_storage - finish) >= n) // 备用空间充足
	{
		const size_type elems_after = finish - position;
		iterator old_finish = finish;

		if (elems_after > n) // 插入点之后的现有元素个数 大于 新增元素个数
		{
			uninitialized_copy(finish -n, finish, finish);
			finish += n;
			copy_backward(position, old_finish - n, old_finish);
			fill(position, position + n, value);
		}
		else
		{
			uninitialized_fill_n(finish, n - elems_after, value);
			finish += (n - elems_after);
			uninitialized_copy(position, old_finish, finish);
			finish += elems_after;
			fill(position, old_finish, value);
		}
	}
	else
		{
			// 配置新空间
			const size_type old_size = size();
			const size_type new_size = old_size + max(old_size, n);

			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = new_start;
			try
			{
				new_finish = uninitialized_copy(start, position, new_start);
				new_finish = uninitialized_fill_n(new_finish, n, value);
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch (const std::exception&)
			{
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);
				throw;
			}
			destroy(start, finish);
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + new_size;
		}
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator position, size_type n, const T & value)
{
	difference_type dis = position - cbegin();
	iterator _pos = begin() + dis;
	insert(_pos, n, value);
	return begin() + dis;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::swap(vector& other)
{
	tiny::swap(this->start, other.start);
	tiny::swap(this->finish, other.finish);
	tiny::swap(this->end_of_storage, other.end_of_storage);
}

#pragma endregion

template<typename T, typename Alloc>
inline void vector<T, Alloc>::deallocate()
{
	if (start)
		data_allocator::deallocate(start, end_of_storage - start);
}

template<typename T, typename Alloc>
inline void vector<T, Alloc>::fill_initalize(size_type n, const T & value)
{
	start = data_allocator::allocate(n);
	uninitialized_fill_n(begin(), n, value);
	finish = start + n;
	end_of_storage = finish;
}


template<typename T, typename Alloc>
template<typename InputIterator>
inline void vector<T, Alloc>::assign(InputIterator first, InputIterator last)
{
	destroy(start, finish);
	deallocate();
	copy_initalize(first, last);
}

template<typename T, typename Alloc>
template<typename InputIterator>
inline void vector<T, Alloc>::copy_initalize(InputIterator first, InputIterator last)
{
	size_type n = (size_type)distance(first, last);
	start = data_allocator::allocate(n);
	uninitialized_copy(first, last, start);
	finish = start + n;
	end_of_storage = finish;
}

template<typename T, typename Alloc>
template<typename InputIterator>
inline void vector<T, Alloc>::__initalize_aux(InputIterator first, InputIterator last, false_type)
{
	copy_initalize(first, last);
}

template<typename T, typename Alloc>
template<typename InputIterator>
inline void vector<T, Alloc>::__initalize_aux(InputIterator count, InputIterator value, true_type)
{
	fill_initalize(count, value);
}

TINY_STL_END_NAMESPACE

#endif // TINY_VECTOR_DEF

