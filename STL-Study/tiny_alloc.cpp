#include "tiny_alloc.h"

TINY_STL_BEGIN_NAMESPACE

malloc_alloc_oom_handler malloc_alloc::mHandler = nullptr;

inline void * malloc_alloc::allocate(size_t n)
{
	// ���malloc ���ɹ��������oom_malloc���Դ����ڴ治�㡱������
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

// ���û��ʹ��set_malloc_handler()�����á��ڴ治�㡱�Ĵ���������ô�����׳�bad_alloc�쳣��Ϣ
// ע�⣬��ơ��ڴ治�㡱�������ǿͻ��˵����Σ��趨Ҳ�ǡ�
void* malloc_alloc::oom_malloc(size_t n)
{
    void* result = nullptr;

    for(;;) // ���������ͷš�����
    {
        if(nullptr == mHandler)
            throw std::bad_alloc();

        mHandler(); 	// ���ô�����,��ͼ�ͷ��ڴ�
        result = malloc(n); 	// �ٴγ��������ڴ�

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

	// size_t > 180 ʹ�õ�һ��������
	if (n > (size_t)max_bytes)
	{
		return (malloc_alloc::allocate(n));
	}

	// Ѱ��16��free-list���ʺϵ�һ��
	my_free_list = free_list + freelist_index(n);
	result = *my_free_list;
	if (result == 0)
	{
		void *r = refill(round_up(n));
		return r;
	}

	// ����free-list
	*my_free_list = result->free_list_link;
	return (result);
}

// ptr �������� nullptr
void default_alloc::deallocate(void *ptr, size_t n)
{
	obj *quit = (obj*)ptr;
	obj **my_free_list;

	// ����128�͵��õ�һ��������
	if (n > (size_t)max_bytes)
	{
		malloc_alloc::deallocate(ptr, n);
		return;
	}

	// Ѱ�Ҷ�Ӧ��freelist
	my_free_list = free_list + freelist_index(n);

	//����freelist, ��������
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

// ���������С������ʹ�õ�n��free-list, n��0��ʼ
inline size_t default_alloc::freelist_index(size_t bytes)
{
	return ((bytes + align - 1) / align - 1);
}

// �����Ͻ��� 8�ı���
inline size_t default_alloc::round_up(size_t bytes)
{
	return (((bytes)+align - 1) & ~(align - 1));
}

// ����һ����СΪn�Ķ��� ������ʱ���Ϊ�ʵ���freelist ���ӽڵ�
// ���� n �Ѿ��� 8 �ı���
void* default_alloc::refill(size_t n)
{
	int nobjs = 20;

	// ����chunk_alloc(), ���Ի�ȥnojs��������Ϊfreelist���½ڵ�
	char *chunk = chunk_alloc(n, &nobjs);
	obj **my_free_list;
	obj *result, *current_obj, *next_obj;
	int i = 1;

	// ���ֻ��һ�����飬�������ͷ�����������ã�free list ���½ڵ�
	if (i == nobjs)
		return (chunk);
	// �������free list �������½ڵ�
	size_t tmp = freelist_index(n);
	my_free_list = free_list + tmp;

	// ������ chunk �ռ��ڽ���free list
	result = (obj*)chunk;

	// ��������free list ָ�������õĿռ䣨ȡ���ڴ�أ�
	*my_free_list = next_obj = (obj*)(chunk + n);

	// ���½� free list �ĸ��ڵ㴮������
	// �ӵ�1��ʼ����0�����ظ��Ͷ�
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

// ���ڴ����ȡ�ռ��free listʹ��
char* default_alloc::chunk_alloc(size_t size, int *nobjs)
{
	char *result = nullptr;
	size_t total_bytes = size * (*nobjs);
	size_t bytes_left = end_free - start_free; // ʣ��ռ�

	if (bytes_left >= total_bytes)
	{
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else if (bytes_left >= size) // �ڴ��ʣ��ռ䣬�㹻һ������������
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
			// �ڴ�ػ���һЩ��ͷ��������ʵ���freelist
			obj **my_free_list = free_list + freelist_index(bytes_left);
			// ����free list�����ڴ���еĲ���ռ����
			((obj*)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}

		// ����heap�ռ䣬���������ڴ��
		start_free = (char*)malloc(bytes_to_get);
		if (nullptr == start_free)
		{
			int i;
			obj **my_free_list, *ptr;

			for (i = size; i <= max_bytes; i += align)
			{
				my_free_list = free_list + freelist_index(i);
				ptr = *my_free_list;
				if (nullptr != ptr) // free list ������δ������
				{
					*my_free_list = ptr->free_list_link;
					start_free = (char*)ptr;
					end_free = start_free + i;
					// �ݹ�����Լ���Ϊ������nobjs
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