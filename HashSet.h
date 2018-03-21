/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-21T14:05:09+08:00
 * @Email:  guang334419520@126.com
 * @Filename: HashSet.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-21T17:45:37+08:00
 */

#ifndef __GSTL_HASH_SET_H
#define __GSTL_HASH_SET_H

#include "config.h"
#include "Functional.h"
#include "HashFun.h"
#include "HashTable.h"

__GSTL_BEGIN_NAMESPACE

template <class Value, class HashFun = Hash<Value>,
          class EqualKey = EqualTo<Value>,
          class Alloc = alloc>
class HashSet;

template <class Value, class HashFun, class EqualKey,
          class Alloc>
bool operator==(const HashSet<Value, HashFun, EqualKey, Alloc>& x,
                const HashSet<Value, HashFun, EqualKey, Alloc>& y);

template <class Value, class HashFun,
          class EqualKey,
          class Alloc>
class HashSet {
  friend bool operator== <> (const HashSet&, const HashSet&);
private:
  typedef HashTable<Value, Value, HashFun, Identity<Value>,
                    EqualKey>               ht;
public:
  typedef typename ht::key_type               key_type;
  typedef typename ht::value_type             value_type;
  typedef typename ht::hasher                 hasher;
  typedef typename ht::key_equal              key_equal;

  typedef typename ht::size_type              size_type;
  typedef typename ht::difference_type        difference_type;
  typedef typename ht::const_pointer          pointer;
  typedef typename ht::const_pointer          const_pointer;
  typedef typename ht::const_reference        reference;
  typedef typename ht::const_reference        const_reference;

  typedef typename ht::const_iterator         iterator;
  typedef typename ht::const_iterator         const_iterator;

  hasher hash_funct() const { return rep_.hash_funct(); }
  key_equal key_eq() const { return rep_.key_eq(); }

public:
  HashSet() : rep_(100, hasher(), key_equal()) {}
  explicit HashSet(size_type n) : rep_(n, hasher(), key_equal()) {}
  HashSet(size_type n, const hasher& hf) : rep_(n, hf, key_equal()) {}
  HashSet(size_type n, const hasher& hf, const key_equal& kq)
    : rep_(n, hf, kq) {}

  template <class InputIterator>
  HashSet(InputIterator first, InputIterator last)
    : rep_(100, hasher(), key_equal())
  {
    rep_.insert_unique(first, last);
  }

  template <class InputIterator>
  HashSet(InputIterator first, InputIterator last, size_type n)
    : rep_(n, hasher(), key_equal())
  {
    rep_.insert_unique(first, last);
  }

  template <class InputIterator>
  HashSet(InputIterator first, InputIterator last, size_type n,
          const hasher& hf)
    : rep_(n, hf, key_equal())
  {
    rep_.insert_unique(first, last);
  }

  template <class InputIterator>
  HashSet(InputIterator first, InputIterator last, size_type n,
          const hasher& hf, const key_equal& eql)
    : rep_(n, hf, eql)
  {
    rep_.insert_unique(first, last);
  }

public:
  bool empyt() const { return rep_.empyt(); }
  size_type size() const { return rep_.size();}
  size_type max_size() const { return rep_.max_size(); }
  void swap(HashSet& hs) { rep_.swap(hs.rep_); }

  iterator begin() const { return rep_.begin(); }
  iterator end() const { return rep_.end(); }

public:
  Pair<iterator, bool> insert(const value_type& obj)
  {
    Pair<typename ht::iterator, bool> p = rep_.insert_unique(obj);
    return Pair<iterator, bool>(p.first, p.sencond);
  }

  template <class InputIterator>
  void insert(InputIterator first, InputIterator last)
  {
    rep_.insert_unique(first, last);
  }

  iterator find(const key_type& key) const { return rep_.find(key); }

  size_type count(const key_type& key) const { return rep_.count(key); }

  Pair<iterator, iterator> equal_range(const key_type& key) const
  {
    return rep_.equal_range(key);
  }

  size_type erase(const key_type& key) { return rep_.erase(key); }
  void erase(iterator it) { rep_.erase(it); }
  void erase(iterator f, iterator l) { rep_.erase(f, l); }
  void clear() { rep_.clear(); }

public:
  void resize(size_type hint) { rep_.resize(hint); }
  size_type bucket_count() const { return rep_.bucket_count(); }
  size_type max_bucket_count() const { return rep_.max_bucket_count(); }
  size_type elems_in_bucket(size_type n) const { return rep_.elems_in_bucket(n); }
private:
  ht rep_;
};

template <class Value, class HashFun, class EqualKey,
          class Alloc>
bool operator==(const HashSet<Value, HashFun, EqualKey, Alloc>& x,
                const HashSet<Value, HashFun, EqualKey, Alloc>& y)
{
  return x.rep_ == y.rep_;
}

/*
template <class Value, class HashFun, class EqualKey,
          class Alloc>
bool operator!=(const HashSet<Value, HashFun, EqualKey, Alloc>& x,
                const HashSet<Value, HashFun, EqualKey, Alloc>& y)
{
  return !(x == y);
}
*/



template <class Value, class HashFun, class EqualKey,
          class Alloc>
void swap(HashSet<Value, HashFun, EqualKey, Alloc>& x,
          HashSet<Value, HashFun, EqualKey, Alloc>& y)
{
  x.swap(y);
}


__GSTL_END_NAMESPACE

#endif
