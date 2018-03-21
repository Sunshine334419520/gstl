/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-11T12:05:41+08:00
 * @Email:  guang334419520@126.com
 * @Filename: Queue.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-21T17:41:39+08:00
 */

#ifndef __GSTL_QUEUE_H
#define __GSTL_QUEUE_H

#include "Deque.h"

#ifndef GSTL_RLPOS_H
  #include "Relops.h"
#endif
__GSTL_BEGIN_NAMESPACE

template <class T, class Sequence>
class Queue;

template <class T, class Sequence>
bool operator==(const Queue<T, Sequence>&,
                const Queue<T, Sequence>&);

template <class T, class Sequence>
bool operator<(const Queue<T, Sequence>&,
                const Queue<T, Sequence>&);


template <class T, class Sequence = Deque<T> >
class Queue {
  friend bool operator== <> (const Queue&, const Queue&);
  friend bool operator< <> (const Queue&, const Queue&);
public:
  typedef typename Sequence::value_type       value_type;
  typedef typename Sequence::reference        reference;
  typedef typename Sequence::pointer          pointer;
  typedef typename Sequence::const_pointer    const_pointer;
  typedef typename Sequence::const_reference  const_reference;
  typedef typename Sequence::size_type        size_type;
public:
  bool empty() const { return containne_.empyt(); }
  size_type size() const { return containne_.size(); }
  size_type max_size() const { return containne_.max_size(); }
  reference front() { return containne_.front(); }
  const_reference front() const { return containne_.front(); }
  reference back() { return containne_.back(); }
  const_reference back() const { return containne_.back(); }

  void push(const T& x) { containne_.push_back(x); }
  void pop() { containne_.pop_front(); }


private:
  Sequence containne_;
};

template <class T, class Sequence>
bool operator==(const Queue<T, Sequence>& x, const Queue<T, Sequence>& y)
{
  return x.containne_ == y.containne_;
}

template <class T, class Sequence>
bool operator<(const Queue<T, Sequence>& x, const Queue<T, Sequence>& y)
{
  return x.containne_ < y.containne_;
}

__GSTL_END_NAMESPACE
#endif    //End
