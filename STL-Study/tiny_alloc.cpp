#include "tiny_alloc.h"

TINY_STL_BEGIN_NAMESPACE

malloc_alloc_oom_handler malloc_alloc::mHandler = nullptr;

inline void * malloc_alloc::allocate(size_t n)
{
	// 如果malloc 不成功，则调用oom_malloc尝试处理“内存不足”的问题
	void *result = malloc(n);
	if (result == nullptr)
		result = oom_malloc(n);

	return result;
}

inline void malloc_alloc::deallocate(void * ptr)
{
	free(ptr);
}

inline void malloc_alloc::deallocate(void * ptr, size_t n)
{
	free(ptr);
}

inline void * malloc_alloc::reallocate(void * ptr, size_t new_size)
{
	void *result = realloc(ptr, new_size);
	if (result == nullptr)
		result = oom_realloc(ptr, new_size);

	return result;
}

inline malloc_alloc_oom_handler malloc_alloc::set_malloc_handler(malloc_alloc_oom_handler f)
{
	malloc_alloc_oom_handler old = mHandler;
	mHandler = f;
	return old;
}

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

// 根据区块大小，决定使用第n号free-list, n从0开始
inline size_t default_alloc::freelist_index(size_t bytes)
{
	return ((bytes + align - 1) / align - 1);
}

// 调整上界至 8的倍数
inline size_t default_alloc::round_up(size_t bytes)
{
	return (((bytes)+align - 1) & ~(align - 1));
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

TINY_STL_END_NAMESPACE