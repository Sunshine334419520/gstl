/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-01-31T17:41:01+08:00
 * @Email:  guang334419520@126.com
 * @Filename: uninitialized.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-02-28T13:35:32+08:00
 */

#ifndef GSTL_UNINITIALIZED_H
#define GSTL_UNINITIALIZED_H

#include <cstring>
#include "Algorithm.h"

__GSTL_BEGIN_NAMESPACE

//uninitialized_copy
template <class InputIterator, class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last,
                         ForwardIterator result, __true_type)
{
  return copy(first, last, result);
}

template <class InputIterator, class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last,
                         ForwardIterator result, __false_type)
{
  ForwardIterator curr = result;
  for( ; first != last; ++curr, ++first) {
    Construct(&(*curr), *first);
  }

  return curr;
}

template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator
__uninitialized_copy(InputIterator first, InputIterator last,
                     ForwardIterator result, T*)
{
  typedef typename __type_traits<T>::is_POD_type is_POD;
  return __uninitialized_copy_aux(first, last, result, is_POD());
}

template <class InputIterator, class ForwardIterator>
inline ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last,
                   ForwardIterator result)
{
  return __uninitialized_copy(first, last, result, value_type(result));
}

template<>
inline char*
uninitialized_copy(const char *first, const char *last, char *result)
{
  memmove(result, first, last - first);
  return result + (last - first);
}

template<>
inline wchar_t*
uninitialized_copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
{
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}




// uninitialized_fill
template <class ForwardIterator, class T>
inline void
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                         const T& x, __false_type)
{
  ForwardIterator cur = first;
  for ( ; first != last; ++cur) {
    Construct(&*cur, x);
  }
}

template <class ForwardIterator, class T>
inline void
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                         const T& x, __true_type)
{
  fill(first, last, x);
}

template <class ForwardIterator, class T, class T1>
inline void
__uninitialized_fill(ForwardIterator first, ForwardIterator last,
                   const T& x, T1*)
{
  typedef typename __type_traits<T1>::is_POD_type is_POD;
  __uninitialized_fill_aux(first, last, x, is_POD());
}

template <class ForwardIterator, class T>
inline void
uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
  __uninitialized_fill(first, last, x, vaule_type(first));
}




//uninitialized_fill_n

template <class ForwardIterator, class Size, class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type)
{
  return fill_n(first, n, x);
}

template <class ForwardIterator, class Size, class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type)
{
  ForwardIterator cur = first;

  for( ; n > 0; --n, ++cur) {
    Construct(&*cur, x);
  }

  return cur;
}

template <class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator
__uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*)
{
  typedef typename __type_traits<T1>::is_POD_type is_POD;
  return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template <class ForwardIterator, class Size, class T>
inline ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
  return __uninitialized_fill_n(first, n, x, value_type(first));
}






__GSTL_END_NAMESPACE

#endif
