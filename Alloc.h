/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-01-29T17:15:56+08:00
 * @Email:  guang334419520@126.com
 * @Filename: stl_simple_alloc.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-21T17:45:07+08:00
 */

#ifndef __GSTL_ALLOC_H
#define __GSTL_ALLOC_H

#if 0
  #include <new>
  #define __THROW_BAD_ALLOC throw bad_alloc
#else
  # include <iostream>
  # define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; \
           exit(1)
#endif

#include <cstdlib>
#include <cstring>
#include "config.h"

__GSTL_BEGIN_NAMESPACE
//namespace simple_stl {

template <int inst>
class MallocAlloc {
public:
  typedef void (*new_handler)();
public:
  static void* Allocate(size_t n)
  {
    void *result = malloc(n);

    if (result == 0) {
      result = oom_malloc(n);
    }
    return result;
  }

  static void Deallocate(void *ptr, size_t /* n */)
  {
    free(ptr);
  }

  static void* Reallocate(void *ptr, size_t old_size, size_t new_size)
  {
    void *result = realloc(ptr, new_size);
    if (0 == result) result = oom_realloc(ptr, new_size);

    return result;
  }

  static new_handler set_malloc_handler(new_handler f)
  {
    new_handler old = __malloc__alloc_oom_handler;
    __malloc__alloc_oom_handler = f;
    return old;
  }

private:
  static void* oom_malloc(size_t);
  static void* oom_realloc(void*, size_t);
  static new_handler __malloc__alloc_oom_handler;
};

template <int inst>
typename MallocAlloc<inst>::new_handler           \
MallocAlloc<inst>::__malloc__alloc_oom_handler = 0;

template <int inst>
void* MallocAlloc<inst>::oom_malloc(size_t n)
{
  new_handler my_malloc_handler;
  void *result = 0;

  for (; ;) {
    my_malloc_handler = __malloc__alloc_oom_handler;
    if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }

    (*my_malloc_handler)();
    result = malloc(n);
    if (result != 0) return result;
  }
}

template <int inst>
void* MallocAlloc<inst>::oom_realloc(void * ptr, size_t n)
{
  new_handler my_realloc_handler;
  void *result = 0;

  for (; ;) {
    my_realloc_handler = __malloc__alloc_oom_handler;
    if(0 == my_realloc_handler) { __THROW_BAD_ALLOC; }

    (*my_realloc_handler)();
    result = realloc(ptr, n);
    if(result != 0) return result;
  }
}


typedef MallocAlloc<0> malloc_alloc;



template <int inst>
class DefaultAlloc {
public:
  static void* Allocate(size_t n);
  static void  Deallocate(void *ptr, size_t n);
  static void* Reallocate(void *ptr, size_t old_sz, size_t new_sz);
private:
  enum {__ALIGN = 8};
  enum {__MAX_BYTES = 128};
  enum {__NFRELISTS = __MAX_BYTES / __ALIGN};
private:
  union obj {
    union obj *free_list_link;
    char client_data[1];
  };
private:
  static size_t ROUND_UP(size_t bytes)
  {
    return ( (bytes) + __ALIGN - 1) & ~(__ALIGN - 1);
  }

  static size_t FREELIST_INDEX(size_t bytes)
  {
    return (bytes + __ALIGN - 1) / __ALIGN - 1;
  }
private:
  static void* refill(size_t n);
  static char* chunk_alloc(size_t size, int &nobjs);

private:
  static obj * volatile free_list[__NFRELISTS];
  static char *start_free;
  static char *end_free;
  static size_t heap_size;
};

template<int inst>
char * DefaultAlloc<inst>::start_free = 0;

template <int inst>
char * DefaultAlloc<inst>::end_free = 0;

template <int inst>
size_t DefaultAlloc<inst>::heap_size = 0;

template <int inst>
typename DefaultAlloc<inst>::obj* volatile      \
DefaultAlloc<inst>::free_list[__NFRELISTS] =    \
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


template <int inst>
void* DefaultAlloc<inst>::Allocate(size_t n)
{
  obj * volatile * my_free_list;
  obj *result = 0;

  if (n > __MAX_BYTES) {
    return malloc_alloc::Allocate(n);
  }

  my_free_list = free_list + FREELIST_INDEX(n);
  result = *my_free_list;

  if (0 == result) {
    void *r = refill(ROUND_UP(n));
    return r;
  }

  *my_free_list = result->free_list_link;

  return result;
}

template <int inst>
void DefaultAlloc<inst>::Deallocate(void *ptr, size_t n)
{
  obj * volatile * my_free_list;
  obj *p = static_cast<obj *>(ptr);

  if (n > __MAX_BYTES) {
    malloc_alloc::Deallocate(p, n);
    return ;
  }

  my_free_list = free_list + FREELIST_INDEX(n);
  p->free_list_link = *my_free_list;
  *my_free_list = p;
}

template <int inst>
void* DefaultAlloc<inst>::Reallocate(void *ptr, size_t old_sz, size_t new_sz)
{
  obj * result;
  obj * volatile * my_free_list;

  if (new_sz >= __MAX_BYTES) {
    return malloc_alloc::Reallocate(ptr, old_sz, new_sz);
  }

  if (ROUND_UP(old_sz) == ROUND_UP(new_sz)) return ptr;

  result = Allocate(new_sz);
  size_t copy_sz = new_sz < old_sz ? new_sz : old_sz;
  memcpy(result, ptr, copy_sz);
  Deallocate(ptr, old_sz);
  return result;
}

template <int inst>
void* DefaultAlloc<inst>::refill(size_t n)
{
  int nobjs = 20;

  char * chunk = chunk_alloc(n, nobjs);

  obj * volatile * my_free_list;
  obj * result;
  obj * current_obj, * next_obj;

  if (1 == nobjs) {
    return chunk;
  }

  my_free_list = free_list + FREELIST_INDEX(n);
  result = (obj*)chunk;

  *my_free_list = next_obj = (obj*)(chunk + n);

  for (int i = 1; ; i++) {
    current_obj = next_obj;
    next_obj = (obj*)( (char*)current_obj + n);

    if (i == nobjs -1) {
      current_obj->free_list_link = 0;
      break;
    }

    current_obj->free_list_link = next_obj;
  }

  return result;
}

template <int inst>
char* DefaultAlloc<inst>::chunk_alloc(size_t size, int &nobjs)
{
  size_t total_bytes = size * nobjs;
  size_t bytes_left = end_free - start_free;
  char * result;

  if (bytes_left >= total_bytes) {
    result = start_free;
    start_free += total_bytes;

    return result;
  } else if (bytes_left >= size) {
    result = start_free;
    nobjs = bytes_left / size;
    total_bytes = size * nobjs;
    start_free += total_bytes;

    return result;
  } else {
    size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);

    if (bytes_left > 0) {
      obj * volatile * my_free_list;

      my_free_list = free_list + FREELIST_INDEX(bytes_left);
      ( (obj*)start_free)->free_list_link = *my_free_list;
      *my_free_list = (obj*)start_free;
    }

    start_free = (char*)malloc(bytes_to_get);

    if (0 == start_free) {
      obj * volatile * my_free_list, *p;

      for (int i = size; i <= __MAX_BYTES; i += __ALIGN) {
          my_free_list = free_list + FREELIST_INDEX(i);
          p = *my_free_list;

          if(0 != p) {
            *my_free_list = p->free_list_link;
            start_free = (char*)p;
            end_free = start_free + i;

            return chunk_alloc(size, nobjs);
          }

      }
      end_free = 0;
      start_free = (char*)malloc_alloc::Allocate(bytes_to_get);
    }

    heap_size += bytes_to_get;
    end_free = start_free + bytes_to_get;

    return chunk_alloc(size, nobjs);
  }
}

typedef DefaultAlloc<0> alloc;


template <class T, class Alloc>
class SimpleAlloc {
public:
  static T* Allocate(size_t n) {
    return 0 == n ? 0 : static_cast<T *>(Alloc::Allocate(sizeof(T) * n));
  }

  static T* Allocate(void) {
    return static_cast<T *>(Alloc::Allocate(sizeof(T)));
  }

  static void Deallocate(void *p, size_t n) {
    if(0 != n)
      Alloc::Deallocate(p, sizeof(T) * n);
  }

  static void Deallocate(void *p) {
    Alloc::Deallocate(p, sizeof(T));
  }
};

__GSTL_END_NAMESPACE

#endif
