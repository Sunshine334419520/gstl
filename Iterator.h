/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-02-03T16:00:41+08:00
 * @Email:  guang334419520@126.com
 * @Filename: iterator.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-21T17:45:57+08:00
 */

#ifndef GSTL_ITERATOR_H
#define GSTL_ITERATOR_H

#include <cstddef>


__GSTL_BEGIN_NAMESPACE

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};


template <class T, class Distance> struct input_iterator {
  typedef input_iterator_tag iterator_category;
  typedef T                  value_type;
  typedef Distance           difference_type;
  typedef T*                 pointer;
  typedef T&                 reference;
};

struct output_iterator {
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;
};

template <class T, class Distance> struct forward_iterator {
  typedef forward_iterator_tag iterator_category;
  typedef T                    value_type;
  typedef Distance             difference_type;
  typedef T*                   pointer;
  typedef T&                   reference;
};

template <class T, class Distance> struct bidirectional_iterator {
  typedef bidirectional_iterator_tag iterator_category;
  typedef T                          value_type;
  typedef Distance                   difference_type;
  typedef T*                         pointer;
  typedef T&                         reference;
};

template <class T, class Distance> struct random_access_iterator {
  typedef random_access_iterator_tag iterator_category;
  typedef T                          value_type;
  typedef Distance                   difference_type;
  typedef T*                         pointer;
  typedef T&                         reference;
};

template <class Category, class T, class Distance = ptrdiff_t,
class Pointer = T*, class Reference = T&>
struct iterator {
  typedef Category  iterator_category;
  typedef T         value_type;
  typedef Distance  difference_type;
  typedef Pointer   pointer;
  typedef Reference reference;
};


template <class Iterator>
struct iterator_traits {
  typedef typename Iterator::value_type        value_type;
  typedef typename Iterator::pointer           pointer;
  typedef typename Iterator::reference         reference;
  typedef typename Iterator::difference_type   difference_type;
  typedef typename Iterator::iterator_category iterator_category;
};

template <class Iterator>
struct iterator_traits<Iterator *> {
  typedef Iterator           value_type;
  typedef Iterator*          pointer;
  typedef Iterator&          reference;
  typedef ptrdiff_t               difference_type;
  typedef random_access_iterator_tag iterator_category;
};

template <class Iterator>
struct iterator_traits<const Iterator *> {
  typedef Iterator           value_type;
  typedef const Iterator*    pointer;
  typedef const Iterator&    reference;
  typedef ptrdiff_t               difference_type;
  typedef random_access_iterator_tag iterator_category;
};


template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
  typedef typename iterator_traits<Iterator>::iterator_category category;
  return category();
}

template <class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
  return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
  return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}


template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last,
           input_iterator_tag)
{
  typename iterator_traits<InputIterator>::difference_type n = 0;
  while (first != last) {
    ++first;
    ++n;
  }

  return n;
}

template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last,
           random_access_iterator_tag)
{
  return last - first;
}


//计算距离函数
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
  return __distance(first, last, iterator_category(first));
}

template <class InputIterator>
inline void
distance(InputIterator first, InputIterator last, size_t n)
{
   n = static_cast<size_t>(__distance(first, last, iterator_category(first)));
}

template <class InputIterator, class Distance>
inline void
__advance(InputIterator& first, Distance n,
          input_iterator_tag)
{
  while (n--)
    ++first;
}

template <class BidirectionalIterator, class Distance>
inline void
__advance(BidirectionalIterator& first, Distance n,
          bidirectional_iterator_tag)
{
  if (n >= 0)
    while (n--)
      ++first;
  else
    while (n++)
      --first;
}


template <class RandomAccessIterator, class Distance>
inline void
__advance(RandomAccessIterator& first, Distance n,
          random_access_iterator_tag)
{
  first += n;
}

//移动n个距离
template <class InputIterator, class Distance>
inline void
advance(InputIterator& first, Distance n)
{
  __advance(first, n, iterator_category(first));
}



__GSTL_END_NAMESPACE

#endif
