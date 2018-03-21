/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-19T11:49:37+08:00
 * @Email:  guang334419520@126.com
 * @Filename: MultiMap.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-21T17:41:00+08:00
 */
#ifndef __GSTL_MULTI_MAP_H
#define __GSTL_MULTI_MAP_H

#include "RB_tree.h"
#include "Functional.h"
#include "Relops.h"

 __GSTL_BEGIN_NAMESPACE

 template <class Key, class T,
           class Compare = Less<Key>,
           class Alloc = alloc>
 class MultiMap;

 template <class Key, class T, class Compare, class Alloc>
 bool operator==(const MultiMap<Key, T, Compare, Alloc>& x,
                 const MultiMap<Key, T, Compare, Alloc>& y);

 template <class Key, class T, class Compare, class Alloc>
 bool operator<(const MultiMap<Key, T, Compare, Alloc>& x,
                const MultiMap<Key, T, Compare, Alloc>& y);

 template <class Key, class T, class Compare, class Alloc>
 class MultiMap {
   friend bool operator == <> (const MultiMap&, const MultiMap&);
   friend bool operator < <> (const MultiMap&, const MultiMap&);
 public:
   typedef Key                     key_type;
   typedef T                       data_type;
   typedef T                       mapped_type;
   typedef Pair<const Key, T>      value_type;
   typedef Compare                 key_compare;
 private:
   typedef RB_tree<key_type, value_type, Select1st<value_type>,
                   key_compare, Alloc>       rep_type;
 public:
   class ValueCompare
     : public BinaryFunction<value_type, value_type, bool> {
     friend class MultiMap<Key, T, Compare, Alloc>;
   public:
     bool operator()(const value_type& x, const value_type& y)
     {
       return comp_(x.first, y.first);
     }

   private:
     Compare comp_;
     ValueCompare(Compare c) : comp_(c) { }
   };
 public:
   typedef typename rep_type::pointer                    pointer;
   typedef typename rep_type::const_pointer              const_pointer;
   typedef typename rep_type::reference                  reference;
   typedef typename rep_type::const_reference            const_reference;

   typedef typename rep_type::iterator             iterator;
   typedef typename rep_type::const_iterator             const_iterator;
   typedef typename rep_type::reverse_iterator     reverse_iterator;
   typedef typename rep_type::const_reverse_iterator     const_reverse_iterator;
   typedef typename rep_type::size_type                  size_type;
   typedef typename rep_type::difference_type            difference_type;
 public:
   MultiMap() : t_(Compare()) { }

   MultiMap(Compare c) : t_(c) { }

   template <class InputIterator>
   MultiMap(InputIterator first, InputIterator last)
     : t_(Compare()) { t_.insert_equal(first, last); }
   template <class InputIterator>
   MultiMap(InputIterator first, InputIterator last, Compare c)
     : t_(c) { t_.insert_equal(first, last); }

   MultiMap<Key, T, Compare, Alloc>& operator=(const MultiMap& x)
   {
     if (this != &x) {
       this->t_ = x.t_;
     }
     return *this;
   }
 public:
   key_compare key_comp() const { return t_.key_comp(); }
   ValueCompare value_comp() const { return ValueCompare(t_.key_comp()); }
   bool empty() const { return t_.empty(); }
   size_type size() const { return t_.size(); }
   size_type max_size() const { return t_.max_size(); }

   iterator begin() { return t_.begin(); }
   iterator end() { return t_.end(); }
   const_iterator begin() const { return t_.begin(); }
   const_iterator end() const { return t_.end(); }
   const_iterator cbegin() const { return t_.cbegin(); }
   const_iterator cend() const { return t_.cend(); }
   reverse_iterator rbegin() { return t_.rbegin(); }
   reverse_iterator rend() { return t_.rend(); }
   const_reverse_iterator rcbegin() const { return t_.rcbegin(); }
   const_reverse_iterator rcend() const { return t_.rcend(); }
   /*
   T& operator[] (const key_type& k) {
     return (* (insert(value_type(k, T()))).first).sencond;
   }
   */
   void swap(MultiMap& x) { t_.swap(x.t_); }

   iterator insert(const value_type& x)
   {
     return t_.insert_equal(x);
   }
   iterator insert(iterator position, const value_type& x)
   {
     return t_.insert_equal(position, x);
   }
   template <class InputIterator>
   void insert(InputIterator first, InputIterator last)
   {
     return t_.insert_equal(first, last);
   }

   void erase(iterator position)
   {
     t_.erase(position);
   }
   size_type erase(const key_type& x)
   {
     return t_.erase(x);
   }
   void erase(iterator first, iterator last)
   {
     return t_.erase(first, last);
   }
   void clear() { t_.clear(); }

 public:
   iterator find(const key_type& x) { return t_.find(x); }
   const_iterator find(const key_type& x) const { return t_.find(x); }
   size_type count(const key_type& x) const { return t_.count(x); }
   iterator lower_bound(const key_type& x) { return t_.lower_bound(x); }
   const_iterator lower_bound(const key_type& x) const { return t_.lower_bound(x); }
   iterator upper_bound(const key_type& x) { return t_.upper_bound(x); }
   const_iterator upper_bound(const key_type& x) const { return t_.upper_bound(x); }

   Pair<iterator, iterator> equal_range(const key_type& x)
   {
     return t_.equal_range(x);
   }
   Pair<const_iterator, const_iterator> equal_range(const key_type& x) const
   {
     return t_.equal_range(x);
   }

 private:
   rep_type t_;
 };


 template <class Key, class T, class Compare, class Alloc>
 bool operator==(const MultiMap<Key, T, Compare, Alloc>& x,
                 const MultiMap<Key, T, Compare, Alloc>& y)
 {
   return x.t_ == y.t_;
 }

 template <class Key, class T, class Compare, class Alloc>
 bool operator<(const MultiMap<Key, T, Compare, Alloc>& x,
                const MultiMap<Key, T, Compare, Alloc>& y)
 {
   return x.t_ < y.t_;
 }

 template <class Key, class T, class Compare, class Alloc>
 void swap(const MultiMap<Key, T, Compare, Alloc>& x,
                const MultiMap<Key, T, Compare, Alloc>& y)
 {
   x.swap(y);
 }


 __GSTL_END_NAMESPACE    // End namespace

 #endif     // ENd map
