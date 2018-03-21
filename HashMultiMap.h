/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-21T15:23:21+08:00
 * @Email:  guang334419520@126.com
 * @Filename: HashMultiMap.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-21T17:17:30+08:00
 */

 #ifndef __GSTL_HASH_MULTI_MAP_H
 #define __GSTL_HASH_MULTI_MAP_H

 #include "config.h"
 #include "Functional.h"
 #include "HashFun.h"
 #include "HashTable.h"

 __GSTL_BEGIN_NAMESPACE

 template <class Key, class T, class HashFun = Hash<Key>,
           class EqualKey = EqualTo<Key>,
           class Alloc = alloc>
 class HashMultiMap;

 template <class Key, class T, class HashFun, class EqualKey,
           class Alloc>
 bool operator==(const HashMultiMap<Key, T, HashFun, EqualKey, Alloc>& x,
                 const HashMultiMap<Key, T, HashFun, EqualKey, Alloc>& y);

 template <class Key, class T, class HashFun, class EqualKey,
           class Alloc>
 class HashMultiMap {
   friend bool operator== <>(const HashMultiMap&, const HashMultiMap&);
 private:
   typedef HashTable<Pair<Key, T>, Key, HashFun, Select1st<Pair<Key, T> >,
                     EqualKey, Alloc> ht;
 public:
   typedef typename ht::key_type                 key_type;
   typedef T                                     data_type;
   typedef T                                     mapped_type;
   typedef typename ht::value_type               value_type;
   typedef typename ht::hasher                   hasher;
   typedef typename ht::key_equal                key_equal;

   typedef typename ht::size_type                size_type;
   typedef typename ht::pointer                  pointer;
   typedef typename ht::const_pointer            const_pointer;
   typedef typename ht::reference                reference;
   typedef typename ht::const_reference          const_reference;

   typedef typename ht::iterator                 iterator;
   typedef typename ht::const_iterator           const_iterator;

   hasher hash_funct() const { return rep_.hash_funct();}
   key_equal key_eq() const { return rep_.key_eq(); }

 public:
   HashMultiMap() : rep_(100, hasher(), key_equal()) {}
   explicit HashMultiMap(size_type n) : rep_(n, hasher(), key_equal()) {}
   HashMultiMap(size_type n, const hasher& hf)
     : rep_(n, hf, key_equal()) {}
   HashMultiMap(size_type n, const hasher& hf, const key_equal& eql)
     : rep_(n, hf, eql) {}

   template <class InputIterator>
   HashMultiMap(InputIterator first, InputIterator last)
     : rep_(100, hasher(), key_equal())
   {
     rep_.insert_equal(first, last);
   }

   template <class InputIterator>
   HashMultiMap(InputIterator first, InputIterator last, size_type n)
     : rep_(n, hasher(), key_equal())
   {
     rep_.insert_equal(first, last);
   }

   template <class InputIterator>
   HashMultiMap(InputIterator first, InputIterator last, size_type n,
           const hasher& hf)
     : rep_(n, hf, key_equal())
   {
     rep_.insert_equal(first, last);
   }

   template <class InputIterator>
   HashMultiMap(InputIterator first, InputIterator last, size_type n,
           const hasher& hf, const key_equal& eql)
     : rep_(n, hf, eql)
   {
     rep_.insert_equal(first, last);
   }

 public:
   bool empyt() const { return rep_.empyt(); }
   size_type size() const { return rep_.size();}
   size_type max_size() const { return rep_.max_size(); }
   void swap(HashMultiMap& hs) { rep_.swap(hs.rep_); }

   iterator begin() { return rep_.begin(); }
   iterator end() { return rep_.end(); }

   const_iterator begin() const { return rep_.begin(); }
   const_iterator end() const { return rep_.end(); }

   const_iterator cbegin() const { return rep_.cbegin(); }
   const_iterator cend() const { return rep_.cend(); }
 public:
   iterator insert(const value_type& obj)
   {
     return rep_.insert_equal(obj);
   }

   template <class InputIterator>
   void insert(InputIterator first, InputIterator last)
   {
     rep_.insert_equal(first, last);
   }

/*
   T& operator[](const key_type& key)
   {
     return rep_.find_of_insert(value_type(key, T())).sencond;
   }
   */

   iterator find(const key_type& key) { return rep_.find(key); }
   const_iterator find(const key_type& key) const { return rep_.find(key); }

   iterator count(const key_type& key) { return rep_.find(key); }
   const_iterator count(const key_type& key) const { return rep_.find(key); }

   Pair<iterator, iterator> equal_range(const key_type& key)
   {
     return rep_.equal_range(key);
   }

   Pair<const_iterator, const_iterator> equal_range(const key_type& key) const
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

 template <class Key, class T, class HashFun, class EqualKey,
           class Alloc>
 bool operator==(const HashMultiMap<Key, T, HashFun, EqualKey, Alloc>& x,
                 const HashMultiMap<Key, T, HashFun, EqualKey, Alloc>& y)
 {
   return x.rep_ == y.rep_;
 }

 template <class Key, class T, class HashFun, class EqualKey,
           class Alloc>
 void swap(HashMultiMap<Key, T, HashFun, EqualKey, Alloc>& x,
           HashMultiMap<Key, T, HashFun, EqualKey, Alloc>& y)
 {
   x.swap(y);
 }

 __GSTL_END_NAMESPACE
 #endif
