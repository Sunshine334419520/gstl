/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-11T12:05:29+08:00
 * @Email:  guang334419520@126.com
 * @Filename: Stack.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-13T17:52:43+08:00
 */
#ifndef GSTL_STACK_H
#define GSTL_STACK_H

#include "Deque.h"

#ifndef GSTL_RLPOS_H
  #include "relops.h"
#endif

__GSTL_BEGIN_NAMESPACE

template <class T, class Sequence>
class Stack;

template <class T, class Sequence>
bool operator==(const Stack<T, Sequence>& x,
                const Stack<T, Sequence>& y);

template <class T, class Sequence>
bool operator<(const Stack<T, Sequence>& x,
                const Stack<T, Sequence>& y);

template <class T, class Sequence = Deque<T> >
class Stack {
  //template <class T2, class Sequence2>
  friend bool operator== <> (const Stack&, const Stack&);
  //template <class T2, class Sequence2>
  friend bool operator<(const Stack&, const Stack&);
public:
  typedef typename Sequence::value_type       value_type;
  typedef typename Sequence::reference        reference;
  typedef typename Sequence::pointer          pointer;
  typedef typename Sequence::const_pointer    const_pointer;
  typedef typename Sequence::const_reference  const_reference;
  typedef typename Sequence::size_type        size_type;

public:
  bool empty() const { return containne_.empty(); }
  size_type size() const {return containne_.size(); }
  size_type max_size() const { return containne_.max_size(); }
  reference top() { return containne_.back(); }
  const_reference top() const {return containne_.back(); }
  void push(const value_type& x) { containne_.push_back(x); }
  void pop() { containne_.pop_back(); }

private:
  Sequence containne_;
};

template <class T, class Sequence>
bool operator==(const Stack<T, Sequence>& x, const Stack<T, Sequence>& y)
{
  return x.containne_ == y.containne_;
}

template <class T, class Sequence>
bool operator<(const Stack<T, Sequence>& x, const Stack<T, Sequence>& y)
{
  return x.containne_ < y.containne_;
}


__GSTL_END_NAMESPACE
#endif      //End
