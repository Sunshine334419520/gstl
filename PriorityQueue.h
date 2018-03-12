/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-12T11:10:42+08:00
 * @Email:  guang334419520@126.com
 * @Filename: PriorityQueue.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-12T11:35:19+08:00
 */

#ifndef GSTL_PRIORITYQUEUE_H
#define GSTL_PRIORITYQUEUE_H

#include "Vector.h"
#include "heap.h"

__GSTL_BEGIN_NAMESPACE

template <class T, class Sequence = Vector<T> >
class PriorityQueue {
public:
  typedef typename Sequence::value_type       value_type;
  typedef typename Sequence::reference        reference;
  typedef typename Sequence::pointer          pointer;
  typedef typename Sequence::const_pointer    const_pointer;
  typedef typename Sequence::const_reference  const_reference;
  typedef typename Sequence::size_type        size_type;
public:
  PriorityQueue() : containne_() {}

  template <class Inputerator>
  PriorityQueue(Inputerator first, Inputerator last)
    : containne_(first, last)
  {
    make_heap(containne_.begin(), containne_.end());
  }

  ~PriorityQueue() { containne_.clear(); }
public:
  bool empty() const { return containne_.empty(); }
  size_type size() const { return containne_.size(); }
  size_type max_size() const { return containne_.max_size(); }
  reference top() { return containne_.front(); }
  const_reference top() const { return containne_.front(); }
  void push(const value_type& x) {
    try {
      containne_.push_back(x);
      push_heap(containne_.begin(), containne_.end());
    }
    catch (...) {
      containne_.clear();
    }
  }
  void pop() {
    try {
      pop_heap(containne_.begin(), containne_.end());
      containne_.pop_back();
    }
    catch (...) {
      containne_.clear();
    }
  }

private:
  Sequence containne_;

};

__GSTL_END_NAMESPACE

#endif      // End
