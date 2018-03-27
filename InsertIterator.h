/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-27T14:44:03+08:00
 * @Email:  guang334419520@126.com
 * @Filename: InsertIterator.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-27T17:25:18+08:00
 */

#ifndef __GSTL_INSERT_ITERATOR_H
#define __GSTL_INSERT_ITERATOR_H

#include "config.h"
#include "Iterator.h"

__GSTL_BEGIN_NAMESPACE

template <class Containne>
class BackInsertIterator {
public:
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  explicit BackInsertIterator(Containne& x) : containne_(&x) {}
  BackInsertIterator& operator=(const typename Containne::value_type& x)
  {
    containne_->push_back(x);
    return *this;
  }

  BackInsertIterator& operator*() { return *this; }
  BackInsertIterator& operator++() { return *this; }
  BackInsertIterator& operator++(int) { return *this; }
private:
    Containne* containne_;
};



template <class Containne>
class FrontInsertIterator {
public:
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  explicit FrontInsertIterator(FrontInsertIterator& x) : containne_(&x) {}
  FrontInsertIterator& operator=(const typename Containne::value_type& x)
  {
    containne_->push_front(x);
    return *this;
  }

  FrontInsertIterator& operator*() { return *this; }
  FrontInsertIterator& operator++() { return *this; }
  FrontInsertIterator& operator++(int) { return *this; }
private:
  Containne* containne_;
};



template <class Containne>
class InsertIterator {
public:
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  explicit InsertIterator(InsertIterator& x,
                          typename Containne::iterator i)
    : containne_(&x), iter(i) {}
  InsertIterator& operator=(const typename Containne::value_type& value)
  {
    iter = containne_->insert(iter, value);
    ++iter;
    return *this;
  }

  InsertIterator& operator*() { return *this; }
  InsertIterator& operator++() { return *this; }
  InsertIterator& operator++(int) { return *this; }
private:
  Containne* containne_;
  typename Containne::iterator iter;
};

template <class Containne>
inline BackInsertIterator<Containne>
back_inserter(Containne& x)
{
  return BackInsertIterator<Containne>(x);
}

template <class Containne>
inline FrontInsertIterator<Containne>
front_inserter(Containne& x)
{
  return FrontInsertIterator<Containne>(x);
}

template <class Containne>
inline InsertIterator<Containne>
inserter(Containne& x, typename Containne::iterator iter)
{
  return InsertIterator<Containne>(x, iter);
}

__GSTL_END_NAMESPACE


#endif
