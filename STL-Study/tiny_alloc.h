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
	static void* allocate(size_t n);
	static void deallocate(void *ptr);
	static void deallocate(void *ptr, size_t n);
	static void* reallocate(void *ptr, size_t new_size);
	static malloc_alloc_oom_handler set_malloc_handler(malloc_alloc_oom_handler f);

private:
    // oom: out of memory
    static void* oom_malloc(size_t n);
    static void* oom_realloc(void *ptr, size_t n);
    static malloc_alloc_oom_handler mHandler;
};

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
	static size_t freelist_index(size_t bytes);
	// 调整上界至 8的倍数
	static size_t round_up(size_t bytes);

	// 返回一个大小为n的对象，并可能加入大小为n的其它区块到free list
	static void* refill(size_t n);

	// 配置一大块空间，可容纳nobjs个大小为 size 的区块
	// 如果配置 nobjs 个区块有所不便，nobjs 可能会降低
	static char* chunk_alloc(size_t size, int* nobjs);

	static char *start_free;
	static char *end_free;
	static size_t heap_size;
};


#ifdef SIMPLE_MALLOC
	typedef malloc_alloc alloc;
#else
	typedef default_alloc alloc;
#endif // SIMPLE_MALLOC
TINY_STL_END_NAMESPACE
