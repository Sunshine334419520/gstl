/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-02-28T15:08:49+08:00
 * @Email:  guang334419520@126.com
 * @Filename: reverseiterator.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-02-28T17:14:43+08:00
 */

#ifndef GSTL_REVERSE_ITERATOR_H
#define GSTL_REVERSE_ITERATOR_H


#include "iterator.h"

__GSTL_BEGIN_NAMESPACE

template <class Iterator>
class reverse_iterator {
public:
  typedef typename iterator_traits<Iterator>::value_type        value_type;
  typedef typename iterator_traits<Iterator>::pointer           pointer;
  typedef typename iterator_traits<Iterator>::reference         reference;
  typedef typename iterator_traits<Iterator>::difference_type   difference_type;
  typedef typename iterator_traits<Iterator>::iterator_category iterator_category;

  typedef Iterator iterator_type;
  typedef reverse_iterator<Iterator>  self;

public:
  //构造，复制，析构
  reverse_iterator() : current_(nullptr) {}
  explicit reverse_iterator(iterator_type x) : current_(x) {}

  reverse_iterator(const self& x) : current_(x.current_) {}
  template <class Iter>
  reverse_iterator(const reverse_iterator<Iter>& x) :
  current_(x.current_) {}

public:
  iterator_type base() const { return current_; }

  reference operator*() const {
    iterator_type tmp = current_;
    return *--tmp;
  }
  pointer operator->() const {
    return &(operator*());
  }

  self& operator++() {
    --current_;
    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    --current_;
    return tmp;
  }

  self& operator--() {
    ++current_;
    return *this;
  }

  self operator--(int) {
    self tmp = *this;
    ++current_;
    return tmp;
  }

  self operator+(difference_type n) const{
    return static_cast<self>(current_ - n);
  }
  self& operator+=(difference_type n) {
    current_ -= n;
    return *this;
  }

  self operator-(difference_type n) const {
    return static_cast<self>(current_ + n);
  }
  self& operator-=(difference_type n) {
    current_ -= n;
    return *this;
  }

  reference operator[](difference_type n) { return *(*this + n); }

protected:
  Iterator current_;
};

template <class Iterator>
inline bool operator==(const reverse_iterator<Iterator> &x,
                      const reverse_iterator<Iterator> &y)
{
  return x.base() == y.base();
}

template <class Iterator>
inline bool operator!=(const reverse_iterator<Iterator> &x,
                      const reverse_iterator<Iterator> &y)
{
  return x.base() != y.base();
}

template <class Iterator>
inline bool operator<(const reverse_iterator<Iterator> &x,
                      const reverse_iterator<Iterator> &y)
{
  return x.base() < y.base();
}

template <class Iterator>
inline reverse_iterator<Iterator>
operator+(typename reverse_iterator<Iterator>::difference_type n,
          reverse_iterator<Iterator>& x)
{
  return static_cast<reverse_iterator<Iterator> >(x.base() - n);
}

template <class Iterator>
inline typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator> &x,
          const reverse_iterator<Iterator> &y)
{
  return x.base() - y.base();
}




__GSTL_END_NAMESPACE

#endif
