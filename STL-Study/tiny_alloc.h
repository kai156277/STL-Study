/*
	模仿 SGI-STL 的设计哲学，去掉了多线程状态的处理
	SGI-STL 设计哲学:
	- 向 system heap 要求空间
	- 考虑多线程状态
	- 考虑内存不足时的应变措施
	- 考虑过多”小型区块“可能造成的内存碎片

	考虑到小型区块可能造成内存破碎的问题，SGI设计了双层配置器，第一级配置器直接使用
	malloc()和free()，第二级配置器视不同情况而定：
		1. 当配置区块大于128 bytes时，直接调用第一级配置器。
		2. 当配置区块小于128 bytes时，采用memory pool整理方式来处理。
*/
#pragma once

#include "tiny_config.h"

#include <new>
#include <cstdlib>


TINY_STL_BEGIN_NAMESPACE

template<typename T, typename Alloc>
class simple_alloc
{
public:
	static T* allocate(size_t n)
	{
		return (0 == n) ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}

	static T* allocate(void)
	{
		return (T*)Alloc::allocate(sizeof(T));
	}

	static void deallocate(T *ptr, size_t n)
	{
		if (0 != n) Alloc::deallocate(ptr, n * sizeof(T));
	}

	static void deallocate(T *ptr)
	{
		Alloc::deallocate(ptr, sizeof(T));
	}
};

typedef void (* malloc_alloc_oom_handler)();

// 第一级配置器
class malloc_alloc
{
public:
    static void* allocate(size_t n)
    {
        // 如果malloc 不成功，则调用oom_malloc尝试处理“内存不足”的问题
        void *result = malloc(n);
        if(result == nullptr)
            result = oom_malloc(n);

        return result;
    }

	static void deallocate(void *ptr)
	{
		free(ptr);
	}

    static void deallocate(void *ptr, size_t n)
    {
        free(ptr);
    }

    static void* reallocate(void *ptr, size_t new_size)
    {
        void *result = realloc(ptr, new_size);
        if(result == nullptr)
            result = oom_realloc(ptr, new_size);

        return result;
    }

    static malloc_alloc_oom_handler set_malloc_handler(malloc_alloc_oom_handler f)
    {
        malloc_alloc_oom_handler old = mHandler;
        mHandler = f;
        return old;
    }

private:
    // oom: out of memory
    static void* oom_malloc(size_t n);
    static void* oom_realloc(void *ptr, size_t n);
    static malloc_alloc_oom_handler mHandler;
};


malloc_alloc_oom_handler malloc_alloc::mHandler = nullptr;

// 如果没用使用set_malloc_handler()来配置“内存不足”的处理函数，那么将会抛出bad_alloc异常信息
// 注意，设计“内存不足”处理函数是客户端的责任，设定也是。
void* malloc_alloc::oom_malloc(size_t n)
{
    void* result = nullptr;

    for(;;) // 反复尝试释放、配置
    {
        if(nullptr == mHandler)
            throw std::bad_alloc();

        mHandler(); 	// 调用处理函数,企图释放内存
        result = malloc(n); 	// 再次尝试配置内存

        if(result)
            return result;
    }
}

void* malloc_alloc::oom_realloc(void *ptr, size_t n)
{
    void* result = nullptr;

    for(;;)
    {
        if(nullptr == mHandler)
            throw std::bad_alloc();

        mHandler();
        result = realloc(ptr, n);

        if(result)
            return result;
    }
}

/* 第二级配置器
   
   如果区块大于 128bytes 时，就移交给第一级配置器处理。
   当区块小于 128bytes 时，则以 memory pool 管理，每次配置一大块内存，并维护对应的
   free-list下次如果再有相同大小的内存需求，就直接从自由链表中拨出，如果客户端释放小
   额区域，就由配置器收回到 free-list 中。

   第二级配置器会主动将任何小额区块的内存需求量上调至8的倍数，并维护16个 free-list，
   各自管理大小分别为8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112,
   120, 128 bytes 的小额区域
*/

enum { align = 8 }; // 小型区块上调边界
enum { max_bytes = 128 }; // 小型区块的最大值
enum { num_free_lists = 16 }; // free-lists 个数


class default_alloc
{
public:
	static void *allocate(size_t n);
	static void deallocate(void *ptr, size_t n);
	static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
private:
	union obj // free-list 的节点结构
	{
		union obj *free_list_link;
		char client_data[1];
	};

	static obj* free_list[num_free_lists];

	// 根据区块大小，决定使用第n号free-list, n从0开始
	static size_t freelist_index(size_t bytes)
	{
		return ((bytes + align - 1) / align - 1);
	}
	// 调整上界至 8的倍数
	static size_t round_up(size_t bytes)
	{
		return (((bytes)+align - 1) & ~(align - 1));
	}

	// 返回一个大小为n的对象，并可能加入大小为n的其它区块到free list
	static void* refill(size_t n);

	// 配置一大块空间，可容纳nobjs个大小为 size 的区块
	// 如果配置 nobjs 个区块有所不便，nobjs 可能会降低
	static char* chunk_alloc(size_t size, int* nobjs);

	static char *start_free;
	static char *end_free;
	static size_t heap_size;
};

char *default_alloc::start_free = 0;
char *default_alloc::end_free = 0;
size_t default_alloc::heap_size = 0;

default_alloc::obj* default_alloc::free_list[num_free_lists] = { 
	nullptr, nullptr, nullptr, nullptr, 
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,};

// n must be > 0
void* default_alloc::allocate(size_t n)
{
	obj** my_free_list;
	obj* result;

	// size_t > 180 使用第一级配置器
	if (n > (size_t)max_bytes)
	{
		return (malloc_alloc::allocate(n));
	}

	// 寻找16个free-list中适合的一个
	my_free_list = free_list + freelist_index(n);
	result = *my_free_list;
	if (result == 0)
	{
		void *r = refill(round_up(n));
		return r;
	}

	// 调整free-list
	*my_free_list = result->free_list_link;
	return (result);
}

// ptr 不可以是 nullptr
void default_alloc::deallocate(void *ptr, size_t n)
{
	obj *quit = (obj*)ptr;
	obj **my_free_list;

	// 大于128就调用第一级配置器
	if (n > (size_t)max_bytes)
	{
		malloc_alloc::deallocate(ptr, n);
		return;
	}

	// 寻找对应的freelist
	my_free_list = free_list + freelist_index(n);

	//调整freelist, 回收区块
	quit->free_list_link = *my_free_list;
	*my_free_list = quit;
}

void* default_alloc::reallocate(void* ptr, size_t old_sz, size_t new_sz)
{
    void* result;
    size_t copy_sz;

    if (old_sz > (size_t) max_bytes && new_sz > (size_t) max_bytes) 
        return realloc(ptr, new_sz);

    if (round_up(old_sz) == round_up(new_sz)) 
		return ptr;

    result = allocate(new_sz);
    copy_sz = (new_sz > old_sz) ? old_sz : new_sz;
    memcpy(result, ptr, copy_sz);
    deallocate(ptr, old_sz);
    return result;
}

// 返回一个大小为n的对象， 并且有时候会为适当的freelist 增加节点
// 假设 n 已经是 8 的倍数
void* default_alloc::refill(size_t n)
{
	int nobjs = 20;

	// 调用chunk_alloc(), 尝试回去nojs个区块作为freelist的新节点
	char *chunk = chunk_alloc(n, &nobjs);
	obj **my_free_list;
	obj *result, *current_obj, *next_obj;
	int i = 1;

	// 如果只有一个区块，这个区块就分配给调用者用，free list 无新节点
	if (i == nobjs)
		return (chunk);
	// 否则调整free list ，纳入新节点
	size_t tmp = freelist_index(n);
	my_free_list = free_list + tmp;

	// 以下在 chunk 空间内建立free list
	result = (obj*)chunk;

	// 以下引导free list 指向新配置的空间（取自内存池）
	*my_free_list = next_obj = (obj*)(chunk + n);

	// 以下将 free list 的各节点串接起来
	// 从第1开始，第0个返回给客端
	for (i = 1; ; i++)
	{
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n);
		if (nobjs - 1 == i)
		{
			current_obj->free_list_link = nullptr;
			break;
		}
		else
		{
			current_obj->free_list_link = next_obj;
		}
	}
	return result;
}

// 从内存池中取空间给free list使用
char* default_alloc::chunk_alloc(size_t size, int *nobjs)
{
	char *result = nullptr;
	size_t total_bytes = size * (*nobjs);
	size_t bytes_left = end_free - start_free; // 剩余空间

	if (bytes_left >= total_bytes)
	{
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else if (bytes_left >= size) // 内存池剩余空间，足够一个及以上区块
	{
		(*nobjs) = bytes_left / size;
		total_bytes = size * (*nobjs);
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else
	{
		size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4);
		if (bytes_left > 0)
		{
			// 内存池还有一些零头，分配给适当的freelist
			obj **my_free_list = free_list + freelist_index(bytes_left);
			// 调整free list，将内存池中的残余空间编入
			((obj*)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}

		// 配置heap空间，用来补充内存池
		start_free = (char*)malloc(bytes_to_get);
		if (nullptr == start_free)
		{
			int i;
			obj **my_free_list, *ptr;

			for (i = size; i <= max_bytes; i += align)
			{
				my_free_list = free_list + freelist_index(i);
				ptr = *my_free_list;
				if (nullptr != ptr) // free list 内尚有未用区块
				{
					*my_free_list = ptr->free_list_link;
					start_free = (char*)ptr;
					end_free = start_free + i;
					// 递归调用自己，为了修正nobjs
					return chunk_alloc(size, nobjs);
				}
			}
			end_free = 0;
			start_free = (char*)malloc_alloc::allocate(bytes_to_get);
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return chunk_alloc(size, nobjs);
	}
}

#ifdef SIMPLE_MALLOC
	typedef malloc_alloc alloc;
#else
	typedef default_alloc alloc;
#endif // SIMPLE_MALLOC
TINY_STL_END_NAMESPACE
