/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-02-26T19:01:21+08:00
 * @Email:  guang334419520@126.com
 * @Filename: Algorithm.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-20T17:02:34+08:00
 */

#ifndef GSTL_ALGORITHM_H
#define GSTL_ALGORITHM_H

#ifndef GSTL_CONFIG_H
#include "config.h"
#endif

#ifndef GSTL_ITERATOR_H
#include "iterator.h"
#endif

#include "Traits.h"

#include <cstring>
#include <cstdlib>


__GSTL_BEGIN_NAMESPACE

template <class InputIterator, class T>
inline InputIterator
find(InputIterator first, InputIterator last, const T& x)
{
  while (first != last && *first != x)
    ++first;
  return first;
}

template <class InputIterator1, class InputIterator2>
inline bool
equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
  for ( ; first1 != last1; ++first1, ++first2)
    if (*first1 != *first2)
      return false;

  return true;
}

template <class ForwardIterator, class T>
inline void
fill(ForwardIterator first, ForwardIterator last, const T& value)
{
  for ( ; first != last; ++first) {
    *first = value;
  }
}

template <class OutputIterator, class Size, class T>
inline OutputIterator
fill_n(OutputIterator first, Size n, const T& value)
{
  for ( ; n > 0; --n, ++first)
    *first = value;

  return first;
}


template <class ForwardIterator1, class ForwardIterator2, class T>
inline void
__iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*)
{
  T tmp = *a;
  *a = *b;
  *b = tmp;
}

template <class ForwardIterator1, class ForwardIterator2>
inline void
iter_swap(ForwardIterator1 a, ForwardIterator2 b)
{
  __iter_swap(a, b, value_type(a));
}

template <class T>
inline const T& max(const T& a, const T& b)
{
  return a > b ? a : b;
}

template <class T>
inline const T& min(const T& a, const T& b)
{
  return a < b ? a : b;
}

template <class T>
inline void swap(T& a, T& b)
{
  T tmp = a;
  a = b;
  b = tmp;
}


//用字典排列方式对两个序列进行比较，小于放回true， 大于false
template <class Inputerator1, class Inputerator2>
bool lexicographical_compare(Inputerator1 first1, Inputerator1 last1,
                             Inputerator2 first2, Inputerator2 last2)
{
  for ( ; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (*first1 < *first2)
      return true;
    if (*first2 < *first1)
      return false;
  }


  return first1 ==last1 && first2 != last2;
}


/* copy 大家族 */
template <class InputIterator, class OutputIterator>
inline OutputIterator
__copy(InputIterator first, InputIterator last,
       OutputIterator result, input_iterator_tag)
{
  //对迭代器进行循环，效率低
  for ( ; first != last; ++first, ++result)
    *result = *first;
  return result;
}

template <class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator
__copy_d(RandomAccessIterator first, RandomAccessIterator last,
         OutputIterator result, Distance*)
{
  //对n进行循环，效率高
  for (Distance n = last - first; n > 0; n--, ++result, ++first)
    *result = *first;
  return result;
}

template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator
__copy(RandomAccessIterator first, RandomAccessIterator last,
       OutputIterator result, random_access_iterator_tag)
{
  return __copy_d(first, last, result, distance_type(first));
}

template <class T>
inline T*
__copy_t(const T* first,const T* last, T* result, __true_type)
{
  memmove(result, first, (last - first) * sizeof(T));
  return result + (last - first);
}

template <class T>
inline T*
__copy_t(const T* first, const T* last, T* result, __false_type)
{
  return __copy_d(first, last, result);
}

template <class InputIterator, class OutputIterator>
struct __copy_dispatch {
    OutputIterator operator()(InputIterator first, InputIterator last,
                              OutputIterator result) {
      return __copy(first, last, result, iterator_category(first));
    }
};

template <class T>
struct __copy_dispatch<T*, T*> {
  T* operator()(T* first, T* last, T* result) {
    //判断是否拥有 trivial assignment operator
    typedef typename __type_traits<T>::has_trivial_assignment_operator t;
    return __copy_t(first, last, result, t());
  }
};

template <class T>
struct __copy_dispatch<const T*, T*> {
  T* operator()(const T* first,const T* last, T* result) {
    //判断是否拥有 trivial assignment operator
    typedef typename __type_traits<T>::has_trivial_assignment_operator t;
    return __copy_t(first, last, result, t());
  }
};

template <class InputIterator, class OutputIterator>
inline OutputIterator
copy(InputIterator first, InputIterator last, OutputIterator result)
{
  return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

__STL_TEMPLATE_NULL inline char*
copy(const char* first, const char* last, char* result)
{
  memmove(result, first, last - first);
  return result + (last - first);
}

__STL_TEMPLATE_NULL inline wchar_t*
copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}


//copy_backward

template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2
__copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
                BidirectionalIterator2 result)
{
  while (first != last) *--result = *--last;
  return result;
}


template <class T>
inline T*
__copy_backward_t(const T* first, const T* last, T* result, __true_type)
{
    const ptrdiff_t n = last - first;
    memmove(result - n, first, sizeof(T) * n);
    return result - n;

}

template <class T>
inline T*
__copy_backward_t(const T* first, const T* last, T* result, __false_type)
{
  return __copy_backward(first, last, result);
}

//copy_backward完全泛化版本
template <class BidirectionalIterator1, class BidirectionalIterator2>
struct __copy_backward_dispatch {
  BidirectionalIterator2 operator()(BidirectionalIterator1 first,
                                    BidirectionalIterator1 last,
                                    BidirectionalIterator2 result) {
    return __copy_backward(first, last, result);
  }
};

//__copy_backward_dispatch偏特花版本
template <class T>
struct __copy_backward_dispatch<T*, T*> {
  T* operator()(T* first, T* last, T* result) {
    typedef typename __type_traits<T>::has_trivial_assignment_operator t;
    return __copy_backward_t(first, last, result, t());
  }
};

//__copy_backward_dispatch偏特花版本
template <class T>
struct __copy_backward_dispatch<const T*, T*> {
  T* operator()(const T* first,const T* last, T* result) {
    typedef typename __type_traits<T>::has_trivial_assignment_operator t;
    return __copy_backward_t(first, last, result, t());
  }
};


template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2
copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
              BidirectionalIterator2 result)
{
  //调用泛化版本
  return __copy_backward_dispatch<BidirectionalIterator1,
                           BidirectionalIterator2>()(first, last, result);

}


/* ------------------------ lower_bound ------------------------ */
template <class ForwardIterator, class Value, class Distance>
inline ForwardIterator
__lower_bound(ForwardIterator first, ForwardIterator last,
              const Value& value, Distance*, forward_iterator_tag)
{
  Distance len = distance(first, last);
  Distance half;
  ForwardIterator middle;

  while (len > 0) {
    middle = first;
    half = len >> 1;
    advance(middle, half);
    if (*middle < value) {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else
      len = half;
  }
  return first;
}

template <class RandomAccessIterator, class Value, class Distance>
inline RandomAccessIterator
__lower_bound(RandomAccessIterator first, RandomAccessIterator last,
              const Value& value, Distance*, random_access_iterator_tag)
{
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (*middle < value) {
      first = middle + 1;
      len = len - half - 1;
    }
    else
      len = half;
  }
  return first;
}

template <class ForwardIterator, class Value,
        class Compare, class Distance>
inline ForwardIterator
__lower_bound(ForwardIterator first, ForwardIterator last,
              const Value& value, Compare comp, Distance*, forward_iterator_tag)
{
  Distance len = distance(first, last);
  Distance half;
  ForwardIterator middle;

  while (len > 0) {
    middle = first;
    half = len >> 1;
    advance(middle, half);
    if (comp(*middle, value)) {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else
      len = half;
  }
  return first;
}

template <class RandomAccessIterator, class Value,
          class Compare, class Distance>
inline RandomAccessIterator
__lower_bound(RandomAccessIterator first, RandomAccessIterator last,
              const Value& value, Compare comp, Distance*, random_access_iterator_tag)
{
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (comp(*middle, value)) {
      first = middle + 1;
      len = len - half - 1;
    }
    else
      len = half;
  }
  return first;
}


template <class ForwardIterator, class Value>
inline ForwardIterator
lower_bound(ForwardIterator first, ForwardIterator last,
            const Value& value)
{
  return __lower_bound(first,last, value, distance_type(first),
                       iterator_category(first));
}

template <class ForwardIterator, class Value, class Compare>
inline ForwardIterator
lower_bound(ForwardIterator first, ForwardIterator last,
            const Value& value, Compare comp)
{
  return __lower_bound(first, last, value, comp, distance_type(first),
                       iterator_category(first));
}


__GSTL_END_NAMESPACE

#endif
