/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-01-27T15:14:53+08:00
 * @Email:  guang334419520@126.com
 * @Filename: stl_simple_construct.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-01-27T16:47:41+08:00
 */

#ifndef __STL_SIMPLE_CONSTRUCT_H
#define __STL_SIMPLE_CONSTRUCT_H

#include <new.h>

__STL_SIMPLE_BEGIN_NAMESPACE    //namespace name

// Call The destructor
template <class T>
inline void Destroy(T *pointer)
{
  pointer->~T();
}

// Call The Constructor
template <class T1, class T2>
inline void Construct(T1 *pointer, T2 &value)
{
  new (p) T(value);
}

// another version of Destroy, receive two Iterator.
template <class ForwordIterator>
inline void Destroy(ForwordIterator first, ForwordIterator last)
{
  __destroy(first, last, value_type(first));
}

//judge the type
template <class ForwordIterator, class T>
inline void __destroy(ForwordIterator first, ForwordIterator last, T*)
{
  typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;

  __destroy_aux(first, last, trivial_destructor());
}

//value type have non-trivial destructor
template <class ForwordIterator>
inline void
__destroy_aux(ForwordIterator first, ForwordIterator last, __false_type)
{
  while (first != last) {
    Destroy(&*first);
    first++;
  }
}

// value type have trivial destructor
template <class ForwordIterator>
inline void
__destroy_aux(ForwordIterator first, ForwordIterator last, __true_type)
{
}

// specialized version of the Destroy, For Iterator is char*, char*
__STL_TEMPLATE_NULL
inline void Destroy(char*, char*)
{
}

// specialized version of the Destroy, For Iterator is w_char_t*, w_char_t*
__STL_TEMPLATE_NULL
inline void Destroy(w_char_t*, wchar_t*)
{
}

__STL_SIMPLE_END_NAMESPACE
#endif
