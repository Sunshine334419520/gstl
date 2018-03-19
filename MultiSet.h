/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-19T11:49:23+08:00
 * @Email:  guang334419520@126.com
 * @Filename: MulteSet.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-19T14:14:07+08:00
 */

#ifndef GSTL_MULTESET_H
#define GSTL_MULTESET_H

#include "config.h"
#include "RB_tree.h"
#include "Functional.h"
#include "relops.h"


__GSTL_BEGIN_NAMESPACE

template <class Key, class Compare = Less<Key>,
          class Alloc = alloc > class MultiSet;

template <class Key, class Compare, class Alloc>
bool operator==(const MultiSet<Key, Compare, Alloc>&,
                const MultiSet<Key, Compare, Alloc>&);

template <class Key, class Compare, class Alloc>
bool operator<(const MultiSet<Key, Compare, Alloc>&,
                const MultiSet<Key, Compare, Alloc>&);

template <class Key, class Compare,
          class Alloc>
class MultiSet {
  friend bool operator == <> (const MultiSet&, const MultiSet&);
  friend bool operator < <> (const MultiSet&, const MultiSet&);
public:
  typedef Key               key_type;
  typedef Key               value_type;
  typedef Compare           key_compare;
  typedef Compare           value_compare;
private:
  typedef RB_tree<key_type, value_type, Identity<Key>,
                  key_compare, Alloc>   rep_type;
public:
  typedef typename rep_type::const_pointer            pointer;
  typedef typename rep_type::const_pointer            const_pointer;
  typedef typename rep_type::const_reference          reference;
  typedef typename rep_type::const_reference          const_reference;

  typedef typename rep_type::const_iterator           iterator;
  typedef typename rep_type::const_iterator           const_iterator;
  typedef typename rep_type::const_reverse_iterator   reverse_iterator;
  typedef typename rep_type::const_reverse_iterator   const_reverse_iterator;
  typedef typename rep_type::size_type                size_type;
  typedef typename rep_type::difference_type          difference_type;

public:
  MultiSet() : t_(Compare()) { }

  template <class InputIterator>
  MultiSet(InputIterator first, InputIterator last)
    : t_(Compare())
  {
    t_.insert_equal(first, last);
  }

  template <class InputIterator>
  MultiSet(InputIterator first, InputIterator last, const Compare& comp)
    : t_(comp)
  {
    t_.insert_equal(first, last);
  }

  MultiSet(const MultiSet<Key, Compare, Alloc>& x)
    : t_(x.t_) { }

  MultiSet<Key, Compare, Alloc>& operator=(const MultiSet<Key, Compare, Alloc>& x) {
    if (&x != this) {
      t_ = x.t_;
    }
    return *this;
  }

public:
  key_compare key_comp() const { return t_.key_comp(); }
  value_compare value_comp() const { return t_.key_comp(); }

  const_iterator begin() const { return t_.begin(); }
  const_iterator end() const { return t_.end(); }
  const_iterator cbegin() const { return t_.cbegin(); }
  const_iterator cend() const { return t_.cend(); }
  const_reverse_iterator rbegin() const { return t_.rcbegin(); }
  const_reverse_iterator rend() const { return t_.rcend(); }
  const_reverse_iterator rcbegin() const { return t_.rcbegin(); }
  const_reverse_iterator rcend() const { return t_.rcend(); }

  bool empty() const { return t_.empty(); }
  size_type size() const { return t_.size(); }
  size_type max_size() const { return t_.max_size(); }
  void swap(MultiSet<Key, Compare, Alloc>& x) { t_.swap(x.t_); }

public:
  iterator insert(const value_type& x)
  {
    return t_.insert_equal(x);
  }
  iterator insert(iterator position, const value_type& x)
  {
    typedef typename rep_type::iterator rep_iterator;
    return t_.insert_equal((rep_iterator&)position, x);
  }

  template <class InputIterator>
  void insert(InputIterator first, InputIterator last)
  {
    t_.insert_equal(first, last);
  }

  void erase(iterator position)
  {
    typedef typename rep_type::iterator rep_iterator;
    t_.erase(reinterpret_cast<rep_iterator&>(position));
  }

  size_type erase(const key_type& x)
  {
    return t_.erase(x);
  }

  void erase(iterator first, iterator last)
  {
    typedef typename rep_type::iterator rep_iterator;
    t_.erase(first, last);
  }

  void clear() { t_.clear(); }

  iterator find(const key_type& x) const { return t_.find(x); }
  size_type count(const key_type& x) const { return t_.count(x); }

  iterator lower_bound(const key_type& x) const
  {
    return t_.lower_bound(x);
  }

  iterator upper_bound(const key_type& x) const
  {
    return t_.upper_bound(x);
  }

  Pair<iterator, iterator> equal_range(const key_type& x) const
  {
    return t_.equal_range(x);
  }

private:
  rep_type t_;

};

template <class Key, class Compare, class Alloc>
bool operator==(const MultiSet<Key, Compare, Alloc>& x,
                const MultiSet<Key, Compare, Alloc>& y)
{
  return x.t_ == y.t_;
}

template <class Key, class Compare, class Alloc>
bool operator<(const MultiSet<Key, Compare, Alloc>& x,
                const MultiSet<Key, Compare, Alloc>& y)
{
  return x.t_ < y.t_;
}

template <class Key, class Compare, class Alloc>
void swap(const MultiSet<Key, Compare, Alloc>& x,
          const MultiSet<Key, Compare, Alloc>& y)
{
  x.swap(y);
}

__GSTL_END_NAMESPACE



#endif
