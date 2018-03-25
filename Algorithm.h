/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-02-26T19:01:21+08:00
 * @Email:  guang334419520@126.com
 * @Filename: Algorithm.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-25T16:42:47+08:00
 */

#ifndef GSTL_ALGORITHM_H
#define GSTL_ALGORITHM_H

#ifndef GSTL_CONFIG_H
#include "config.h"
#endif

#ifndef GSTL_ITERATOR_H
#include "Iterator.h"
#endif

#include "Traits.h"
#include "Pair.h"
#include "ReverseIterator.h"

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

template <class Inputerator1, class Inputerator2, class BinaryOperation>
bool lexicographical_compare(Inputerator1 first1, Inputerator1 last1,
                             Inputerator2 first2, Inputerator2 last2,
                             BinaryOperation binary_op)
{
  for ( ; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (binary_op(*first1, *first2))
      return true;
    if (binary_op(*first2, *first1))
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

__GSTL_TEMPLATE_NULL inline char*
copy(const char* first, const char* last, char* result)
{
  memmove(result, first, last - first);
  return result + (last - first);
}

__GSTL_TEMPLATE_NULL inline wchar_t*
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

template <class InputIterator1, class InputIterator2>
Pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
                                              InputIterator1 last1,
                                              InputIterator2 first2)
{
  while (first1 != last1 && *first1 == *first2) {
    ++first1;
    ++first2;
  }
  return Pair<InputIterator1, InputIterator2>(first1, first2);
}

template <class InputIterator1, class InputIterator2, class BinaryPredicate>
Pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
                                              InputIterator1 last1,
                                              InputIterator2 first2,
                                              BinaryPredicate binary_pred)
{
  while (first1 != last1 && binary_pred(*first1, *first2)) {
    ++first1;
    ++first2;
  }
  return Pair<InputIterator1, InputIterator2>(first1, first2);
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

/* ------------------------ upper_bound ------------------------ */
template <class ForwardIterator, class T, class Distance>
ForwardIterator __upper_bound(ForwardIterator first,
                              ForwardIterator last,
                              const T& value, Distance*,
                              forward_iterator_tag)
{
  Distance len = distance(first, last);
  Distance half;
  ForwardIterator middle;
  while (len > 0) {
    middle = first;
    half = len >> 1;
    advance(middle, half);
    if (value < *middle)
      len = half;
    else {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}

template <class RandomAccessIterator, class T, class Distance>
 RandomAccessIterator __upper_bound(RandomAccessIterator first,
                                    RandomAccessIterator last,
                                    const T& value, Distance,
                                    random_access_iterator_tag)
{
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;
  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (value < *middle)
      len = half;
    else {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}

template <class ForwardIterator, class T,
          class Compare, class Distance>
ForwardIterator __upper_bound(ForwardIterator first,
                              ForwardIterator last,
                              const T& value, Compare comp,
                              Distance*,
                              forward_iterator_tag)
{
  Distance len = distance(first, last);
  Distance half;
  ForwardIterator middle;
  while (len > 0) {
    middle = first;
    half = len >> 1;
    advance(middle, half);
    if (comp(value, *middle))
      len = half;
    else {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}

template <class RandomAccessIterator, class T,
          class Compare, class Distance>
 RandomAccessIterator __upper_bound(RandomAccessIterator first,
                                    RandomAccessIterator last,
                                    const T& value, Compare comp,
                                    Distance,
                                    random_access_iterator_tag)
{
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;
  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (comp(value, *middle))
      len = half;
    else {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}

template <class ForwardIterator, class T>
inline ForwardIterator upper_bound(ForwardIterator first,
                                   ForwardIterator last,
                                   const T& value)
{
  return __upper_bound(first, last, value, distance_type(first),
                       iterator_category(first));
}

template <class ForwardIterator, class T, class Compare>
inline ForwardIterator upper_bound(ForwardIterator first,
                                   ForwardIterator last,
                                   const T& value, Compare comp)
{
  return __upper_bound(first, last, value, comp, distance_type(first),
                       iterator_category(first));
}

/* ------------------------ binary_search ------------------------ */
template <class ForwardIterator, class T>
inline bool binary_search(ForwardIterator first, ForwardIterator last,
                          const T& x)
{
  ForwardIterator i = lower_bound(first, last, x);
  return i != last && !(x < *i);
}

template <class BidirectionalIterator>
void reverse(BidirectionalIterator first, BidirectionalIterator last);

/* ------------------------ next_permutation ------------------------ */
template <class BidirectionalIterator>
bool next_permutation(BidirectionalIterator first,
                      BidirectionalIterator last)
{
  if (first == last) return false;
  BidirectionalIterator i = first;
  ++i;
  if (i == last) return false;
  i = last;
  --i;
  while (true) {
    BidirectionalIterator ii = i;
    --i;
    if (*i < *ii) {
      BidirectionalIterator j = last;
      --j;
      while (!(*i < j))
        --j;
      iter_swap(i, j);
      reverse(ii, last);
      return true;
    }
    if (i == first) {
      reverse(first, last);
      return false;
    }
  }
}

/* ------------------------ prev_permutation ------------------------ */
template <class BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first,
                      BidirectionalIterator last)
{
  if (first == last) return false;
  BidirectionalIterator i = first;
  ++i;
  if (i == last) return false;
  i = last;
  --i;
  while (true) {
    BidirectionalIterator ii = i;
    --i;
    if (*ii < *i) {
      BidirectionalIterator j = last;
      --j;
      while (!(j < *i))
        --j;
      iter_swap(i, j);
      reverse(ii, last);
      return true;
    }
    if (i == first) {
      reverse(first, last);
      return false;
    }
  }
}

/* ------------------------ Set_union ------------------------ */
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result)
{
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      *result = *first1;
      ++first1;
    }
    else if (*first2 < *first1) {
      *result = *first2;
      ++first2;
    }
    else {
      *result = *first1;
      ++first1;
      ++first2;
    }
    ++result;
  }
  return copy(first1, last1, copy(first2, last2, result));
}

template <class InputIterator1, class InputIterator2,
          class OutputIterator, class BinaryOperation>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result,BinaryOperation binary_op)
{
  while (first1 != last1 && first2 != last2) {
    if (binary_op(*first1, *first2)) {
      *result = *first1;
      ++first1;
    }
    else if (binary_op(*first2, *first1)) {
      *result = *first2;
      ++first2;
    }
    else {
      *result = *first1;
      ++first1;
      ++first2;
    }
    ++result;
  }
  return copy(first1, last1, copy(first2, last2, result));
}

/* ------------------------ set_intersection ------------------------ */
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result)
{
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2)
      ++first1;
    else if (*first2 < *first1)
      ++first2;
    else {
      *result = *first1;
      ++first1;
      ++first2;
      ++result;
    }
  }
  return result;
}

template <class InputIterator1, class InputIterator2,
          class OutputIterator, class BinaryOperation>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result, BinaryOperation binary_op)
{
  while (first1 != last1 && first2 != last2) {
    if (binary_op(*first1, *first2))
      ++first1;
    else if (binary_op(*first2, *first1))
      ++first2;
    else {
      *result = *first1;
      ++first1;
      ++first2;
      ++result;
    }
  }
  return result;
}


/* ------------------------ set_difference ------------------------ */
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result)
{
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      *result = *first1;
      ++first1;
      ++result;
    }
    else if (*first2 < *first1)
      ++first2;
    else {
      ++first1;
      ++first2;
    }
  }

  return copy(first1, last1, result);
}

template <class InputIterator1, class InputIterator2,
          class OutputIterator, class BinaryOperation>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result,BinaryOperation binary_op)
{
  while (first1 != last1 && first2 != last2) {
    if (binary_op(*first1, *first2)) {
      *result = *first1;
      ++first1;
      ++result;
    }
    else if (binary_op(*first2, *first1))
      ++first2;
    else {
      ++first1;
      ++first2;
    }
  }

  return copy(first1, last1, result);
}


/* ------------------------ adjacent_find ------------------------ */
template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first,
                              ForwardIterator last)
{
  if (first == last) return first;
  ForwardIterator next = first;
  while (++next != last) {
    if (*next == *first)
      return first;
    first = next;
  }
}

template <class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first,
                              ForwardIterator last,
                              BinaryPredicate binary_pred)
{
  if (first == last) return first;
  ForwardIterator next = first;
  while (++next != last) {
    if (binary_pred(*next, *first))
      return first;
    first = next;
  }
}

/* ------------------------ count and count_if ------------------------ */
template <class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value)
{
  typename iterator_traits<InputIterator>::difference_type n = 0;
  while (first != last) {
    if (*first == value)
      ++n;
    ++first;
  }

  return n;

}

template <class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last,
         Predicate pred)
{
  typename iterator_traits<InputIterator>::difference_type n = 0;
  while (first != last) {
    if (pred(*first))
      ++n;
    ++first;
  }
  return n;
}

/* ------------------------ search ------------------------ */
template <class ForwardIterator1, class ForwardIterator2,
          class Distance1, class Distance2>
ForwardIterator1 __search(ForwardIterator1 first1,
                          ForwardIterator1 last1,
                          ForwardIterator2 first2,
                          ForwardIterator2 last2,
                          Distance1*, Distance2*)
{
  Distance1 d1 = distance(first1, last1);
  Distance2 d2 = distance(first2, last2);

  if (d1 < d2) return last1;

  ForwardIterator1 current1 = first1;
  ForwardIterator2 current2 = first2;
  while (current2 != last2) {
    if (*current2 == *current1) {
      ++current2;
      ++current1;
    }
    else {
      if (d1 == d2)
        return last1;
      current1 = ++first1;
      current2 = first2;
      --d1;
    }
  }
  return first1;
}

template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 search(ForwardIterator1 first1,
                        ForwardIterator1 last1,
                        ForwardIterator2 first2,
                        ForwardIterator2 last2)
{
  return __search(first1, last1, first2, last2, distance_type(first1),
                  distance_type(first2));
}

template <class ForwardIterator, class Integer, class T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
                         Integer count, const T& value)
{
  first = find(first, last, value);
  while (first != last) {
    Integer n = count - 1;
    ForwardIterator i = first;
    ++i;
    while (i != last && n != 0 && *i == value) {
      ++i;
      --n;
    }
    if (n == 0)
      return first;
    else
      first = find(i, last, value);
  }
  return last;
}

/* ------------------------ find_if ------------------------ */
template <class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last,
                      Predicate pred)
{
  while (first != last && !pred(*first))
    ++first;
  return first;
}

/* ------------------------ find_end ------------------------ */
template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator1
__find_end(ForwardIterator1 first1, ForwardIterator1 last1,
           ForwardIterator2 first2, ForwardIterator2 last2,
           forward_iterator_tag)
{
  if (first2 == last2)
    return last1;
  else {
    ForwardIterator1 result = last1;
    while (true) {
      ForwardIterator1 new_result = search(first1, last1, first2, last2);
      if (new_result == last1)
        return result;
      else {
        result = new_result;
        first1 = new_result;
        ++first1;
      }
    }
  }
}

template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator1
__find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
           BidirectionalIterator2 first2, BidirectionalIterator2 last2,
           bidirectional_iterator_tag)
{
  typedef reverse_iterator<BidirectionalIterator1> reviter1;
  typedef reverse_iterator<BidirectionalIterator2> reviter2;
  reviter1 rlast1(first1);
  reviter2 rlast2(first2);

  reviter1 rresult = search(reviter1(last1), rlast1,
                            reviter2(last2), rlast2);
  if (rresult == rlast1)
    return last1;
  else {
    BidirectionalIterator1 result = rresult.base();
    advance(result, -distance(first2, last2));
    return result;
  }
}

template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1
find_end(ForwardIterator1 first1, ForwardIterator1 last1,
         ForwardIterator2 first2, ForwardIterator2 last2)
{
  typedef typename iterator_traits<ForwardIterator1>::iterator_category
                    category1;
  typedef typename iterator_traits<ForwardIterator2>::iterator_category
                    category2;

  __find_end(first1, last1, first2, last2, category1(), category2());
}

/* ------------------------ find_first_of ------------------------ */
template <class InputIterator, class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2)
{
  for ( ; first1 != last1; ++first1) {
    for (ForwardIterator cur = first2; cur != last2; ++cur)
      if (*cur == *first1)
        return first1;
  }
  return last1;
}

template <class InputIterator, class ForwardIterator, class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2,
                            BinaryPredicate binary_pred)
{
  for ( ; first1 != last1; ++first1) {
    for (ForwardIterator cur = first2; cur != last2; ++cur)
      if (binary_pred(*first1, *cur))
        return first1;
  }
  return last1;
}

/* ------------------------ for_each ------------------------ */
template <class InputIterator, class Function>
Function for_each(InputIterator first, InputIterator last, Function fun)
{
  while (first != last) {
    fun(*first);
    ++first;
  }
  return fun;
}
/* ------------------------ generate ------------------------ */
template <class ForwardIterator, class Generate>
void generate(ForwardIterator first, ForwardIterator last, Generate gen)
{
  while (first != last)
    *first = gen();
}

template <class ForwardIterator, class Size, class Generate>
void generate_n(ForwardIterator first, Size n, Generate gen)
{
  for ( ; n > 0; --n, ++first)
    *first = gen();
}

/* ------------------------ includes ------------------------ */
template <class InputIterator1, class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2)
{
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2)
      ++first1;
    else if (*first2 < *first1)
      return false;
    else {
      ++first1;
      ++first2;
    }
  }
  return first2 == last2;

}

template <class InputIterator1, class InputIterator2, class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2,
              Compare comp)
{
  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2))
      ++first1;
    else if (comp(*first2, *first1))
      return false;
    else {
      ++first1;
      ++first2;
    }
  }
  return first2 == last2;

}

/* ------------------------ max_element ------------------------ */
template <class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
{
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last) {
    if (*first > *result) result = first;
  }
  return result;
}

template <class ForwardIterator, class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last,
                            Compare comp)
{
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last) {
    if (comp(*result, *first)) result = first;
  }
  return result;
}


/* ------------------------ min_element ------------------------ */
template <class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last)
{
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last) {
    if (*first < *result)
      *result = *first;
  }
  return result;
}

template <class ForwardIterator,class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last,
                            Compare comp)
{
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last) {
    if (comp(*first, *result))
      *result = *first;
  }
  return result;
}


/* ------------------------ merge ------------------------ */
template <class Inputerator1, class InputIterator2, class OutputIterator>
OutputIterator merge(Inputerator1 first1, Inputerator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result)
{
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      *result = *first1;
      ++first1;
    }
    else {
      *result = *first2;
      ++first2;
    }
    ++result;
  }
  return result;
}

template <class Inputerator1, class InputIterator2,
          class OutputIterator, class Compare>
OutputIterator merge(Inputerator1 first1, Inputerator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result, Compare comp)
{
  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      *result = *first1;
      ++first1;
    }
    else {
      *result = *first2;
      ++first2;
    }
    ++result;
  }
  return result;
}


/* ------------------------ partition ------------------------ */
template <class BidirectionalIterator, class Predicate>
BidirectionalIterator partition(BidirectionalIterator first,
                                BidirectionalIterator last,
                                Predicate pred)
{
  while (true) {
    while (true) {
      if (first == last)
        return first;
      else if (pred(*first))
        ++first;
      else
        break;
    }
    --last;
    while (true) {
      if (first == last)
        return first;
      else if (pred(*last))
        --last;
      else
        break;
    }
    iter_swap(first, last);
    ++first;
  }
}

/* ------------------------ remove ------------------------ */
template <class ForwardIterator, class OutputIterator, class T>
OutputIterator remove_copy(ForwardIterator first, ForwardIterator last,
                           OutputIterator result, const T& value)
{
  while (first != last) {
    if (*first != value) {
      *result = *first;
      ++result;
    }
    ++first;
  }
  return result;
}

template <class ForwardIterator, class OutputIterator, class Predicate>
OutputIterator remove_copy_if(ForwardIterator first, ForwardIterator last,
                           OutputIterator result, Predicate pred)
{
  while (first != last) {
    if (!pred(*first)) {
      *result = *first;
      ++result;
    }
    ++first;
  }
  return result;
}



template <class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last,
                       const T& value)
{
  first = find(first, last, value);
  ForwardIterator next = first;
  return first == last ? first : remove_copy(++next, last, first, value);
}

template <class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
                          Predicate pred)
{
  first = find_if(first, last, pred);
  ForwardIterator next = first;
  return first == last ? first : remove_copy_if(++next, last, first, pred);
}


/* ------------------------ replace ------------------------ */
template <class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last,
             const T& old_value, const T& new_value)
{
  for ( ; first != last; ++first)
    if (*first == old_value)
      *first = new_value;
}

template <class ForwardIterator, class OutputIterator, class T>
OutputIterator replace_copy(ForwardIterator first, ForwardIterator last,
                            OutputIterator result, const T& old_value,
                            const T& new_value)
{
  for ( ; first != last; ++first, ++result)
    if (*first == old_value) {
      *result = new_value;
    }
    else
      *result = *first;

  return result;
}

template <class ForwardIterator, class Predicate, class T>
void replace_if(ForwardIterator first, ForwardIterator last,
             Predicate pred, const T& new_value)
{
  for ( ; first != last; ++first)
    if (pred(*first))
      *first = new_value;
}

template <class ForwardIterator, class OutputIterator,
          class Predicate, class T>
OutputIterator replace_copy_if(ForwardIterator first, ForwardIterator last,
                               OutputIterator result, Predicate pred,
                               const T& new_value)
{
  for ( ; first != last; ++first)
    *result = pred(*first) ? new_value : *first;

  return result;
}

/* ------------------------ reverse ------------------------ */
template <class BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last,
             bidirectional_iterator_tag)
{
  while (true) {
    if (first == last || first == --last)
      return ;
    else
      iter_swap(first++, last);
  }
}

template <class BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last,
               random_access_iterator_tag)
{
  while (first < last) {
    iter_swap(first++, --last);
  }
}

template <class BidirectionalIterator>
void reverse(BidirectionalIterator first, BidirectionalIterator last)
{
  __reverse(first, last, iterator_category(first));
}

template <class BidirectionalIterator, class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last,
                            OutputIterator result)
{
  while (first != last) {
    --last;
    *result = *last;
    ++result;
  }
  return result;
}

/* ------------------------ swap_ranges ------------------------ */
template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
                             ForwardIterator2 first2)
{
  for ( ; first1 != last1; ++first1, ++ first2)
    iter_swap(first1, first2);
  return first2;
}

/* ------------------------ transform ------------------------ */
template <class InputIterator, class OutputIterator, class UnaryOperation>
OutputIterator transform(InputIterator first, InputIterator last,
                         OutputIterator result, UnaryOperation op)
{
  for ( ; first != last; ++first, ++result)
    *result = op(*first);
  return result;
}

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class BinaryOperation>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, OutputIterator result,
                         BinaryOperation binary_op)
{
  for ( ; first1 != last1; ++first1, ++first2, ++ result)
    *result = binary_op(*first1, *first2);
}

/* ------------------------ unique ------------------------ */
template <class InputIterator, class ForwardIterator>
ForwardIterator __unique_copy(InputIterator first, InputIterator last,
                             ForwardIterator result, forward_iterator_tag)
{
  *result = *first;
  while (++first != last) {
    if (*first != *result)
      *++result = *first;
  }
  return ++result;
}

template <class InputIterator, class OutputIterator, class T>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
                             OutputIterator result, T*)
{
  T value = *first;
  *result = value;
  while (++first != last) {
    if (*first != value) {
      value = *first;
      *++result = value;
    }
  }
  return ++result;
}


template <class InputIterator, class OutputIterator>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
                             OutputIterator result, output_iterator_tag)
{
  return __unique_copy(first, last, result, value_type(first));
}

template <class InputIterator, class OutputIterator>
OutputIterator unique_copy(InputIterator first, InputIterator last,
                          OutputIterator result)
{
  return __unique_copy(first, last, result, iterator_category(result));
}

template <class ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last)
{
  first = adjacent_find(first, last);
  return unique_copy(first, last, first);
}

/* ------------------------ random_shuffle ------------------------ */
template <class RandomAccessIterator, class Distance>
void __random_shuffle(RandomAccessIterator first,
                      RandomAccessIterator last,
                      Distance*)
{
  if (first == last) return ;
  for (RandomAccessIterator i = first + 1; i != last; ++i)
    iter_swap(i, first + Distance(rand() % ( (i - first) + 1)));
}

template <class RandomAccessIterator>
inline void random_shuffle(RandomAccessIterator first,
                           RandomAccessIterator last)
{
  __random_shuffle(first, last, distance_type(first));
}


__GSTL_END_NAMESPACE

#endif
