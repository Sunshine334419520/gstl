/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-19T12:00:37+08:00
 * @Email:  guang334419520@126.com
 * @Filename: HashTable.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-20T17:53:03+08:00
 */

#ifndef GSTL_HASH_TABLE_H
#define GSTL_HASH_TABLE_H

#include <cstddef>
#include "config.h"
#include "alloc.h"
#include "construct.h"
#include "iterator.h"
#include "Pair.h"
#include "Algorithm.h"
#include "Vector.h"


__GSTL_BEGIN_NAMESPACE

template <class Value, class Key, class HashFun,
          class ExtractKey, class EqualKey, class Alloc = alloc>
class HashTable;

template <class Value, class Key, class HashFun,
          class ExtractKey, class EqualKey, class Alloc = alloc>
struct HashTableIterator;

template <class Value, class Key, class HashFun,
          class ExtractKey, class EqualKey, class Alloc = alloc>
struct HashTableConstIterator;

template <class Value, class Key, class HashFun,
          class ExtractKey, class EqualKey, class Alloc>
bool operator==(const HashTable<Value, Key, HashFun,
                ExtractKey, EqualKey, Alloc>& x,
                const HashTable<Value, Key, HashFun,
                ExtractKey, EqualKey, Alloc>& y);

//hashtable 节点
template <class Value>
struct HashTableNode {
  HashTableNode* next;
  Value          val;
};

//hashtable迭代器
template <class Value, class Key, class HashFun,
          class ExtractKey, class EqualKey, class Alloc>
struct HashTableIterator {
  typedef HashTableIterator<Value, Key, HashFun, ExtractKey,
                            EqualKey, Alloc>    iterator;
  typedef HashTableConstIterator<Value, Key, HashFun, ExtractKey,
                            EqualKey, Alloc>    const_iterator;

  typedef HashTable<Value, Key, HashFun, ExtractKey,
                                 EqualKey, Alloc>    hashtable;

  typedef HashTableNode<Value> hash_node;
  typedef forward_iterator_tag iterator_category;
  typedef Value                value_type;
  typedef Value*               pointer;
  typedef Value&               reference;
  typedef ptrdiff_t            difference_type;
  typedef size_t               size_type;

  hash_node*  cur;        //迭代器目前所指节点
  hashtable*   ht;        //保持对容器对连接关系

  //  构造函数
  HashTableIterator() : cur(nullptr), ht(nullptr) {}
  HashTableIterator(hash_node* n, hashtable* tab) : cur(n), ht(tab) { }

  //操作符重载
  reference operator*() const { return cur->val; }
  pointer operator->() const { return &(operator*()); }

  iterator& operator++();
  iterator operator++(int);

  bool operator==(const iterator& it) const { return cur == it.cur; }
  bool operator!=(const iterator& it) const { return cur != it.cur; }
};

//const 迭代器
template <class Value, class Key, class HashFun,
          class ExtractKey, class EqualKey, class Alloc>
struct HashTableConstIterator {
  typedef HashTableIterator<Value, Key, HashFun, ExtractKey,
                            EqualKey, Alloc>    iterator;
  typedef HashTableConstIterator<Value, Key, HashFun, ExtractKey,
                            EqualKey, Alloc>    const_iterator;

  typedef HashTable<Value, Key, HashFun, ExtractKey,
                                 EqualKey, Alloc>    hashtable;

  typedef HashTableNode<Value> hash_node;
  typedef forward_iterator_tag iterator_category;
  typedef Value                value_type;
  typedef Value*               pointer;
  typedef Value&               reference;
  typedef ptrdiff_t            difference_type;
  typedef size_t               size_type;

  const hash_node*  cur;        //迭代器目前所指节点
  const hashtable*   ht;        //保持对容器对连接关系

  //  构造函数
  HashTableConstIterator() : cur(nullptr), ht(nullptr) {}
  HashTableConstIterator(const hash_node* n, const hashtable* tab)
   : cur(n), ht(n) {}
  HashTableConstIterator(const iterator& it)
   : cur(it.cur), ht(it.ht) {}

  //操作符重载
  reference operator*() const { return cur->val; }
  pointer operator->() const { return &(operator*()); }

  const_iterator& operator++();
  const_iterator operator++(int);

  bool operator==(const iterator& it) const { return cur == it.cur; }
  bool operator!=(const iterator& it) const { return cur != it.cur; }
};

static const int num_primes = 28;
static const unsigned long prime_list[num_primes] =
{
  53,          97,          197,           389,           769,
  1543,        3079,        6151,          12289,         24593,
  49157,       98317,       196613,        393241,        786433,
  1572869,     3145739,     6291469,       12582917,      25165843,
  50331653,    100663319,   201326611,     402653189,     805306457,
  1610612741,  3221225473ul,4294947291ul
};
/* 找出距离n最近并大于n的质数 */
inline unsigned long NextPrime(unsigned long n)
{
  const unsigned long* first = prime_list;
  const unsigned long* last = prime_list + num_primes;
  const unsigned long* pos = lower_bound(first, last, n);

  return pos == last ? *(last - 1) : *pos;
}

//hashtable
template <class Value, class Key, class HashFun,
          class ExtractKey, class EqualKey, class Alloc>
class HashTable {
  friend struct
  HashTableIterator<Value, Key, HashFun, ExtractKey, EqualKey, Alloc>;
  friend struct
  HashTableConstIterator<Value, Key, HashFun, ExtractKey, EqualKey, Alloc>;
  friend bool operator== <> (const HashTable&, const HashTable&);
public:
  typedef Value value_type;
  typedef Key   key_type;
  typedef HashFun hasher;
  typedef EqualKey  key_equal;

  typedef size_t size_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef const value_type* const_pointer;
  typedef const value_type& const_reference;
  typedef ptrdiff_t difference_type;

  typedef HashTableIterator<Value, Key, HashFun,
                            ExtractKey, EqualKey, Alloc>    iterator;
  typedef HashTableConstIterator<Value, Key, HashFun,
                            ExtractKey, EqualKey, Alloc>    const_iterator;
private:
  typedef HashTableNode<Value> hash_node;
  typedef SimpleAlloc<hash_node, alloc>  hash_node_allcoate;
public:
  HashTable(size_type n, const HashFun& hf,
            const EqualKey& eql)
    : hash_(hf), equals_(eql), num_elements_(0)
  {
    initialize_buckets(n);
  }

  HashTable(size_type n, const HashFun& hf,
            const EqualKey& eql, const ExtractKey& ext)
    : hash_(hf), equals_(eql), get_key(ext), num_elements_(0)
  {
    initialize_buckets(n);
  }

  HashTable(const HashTable& ht)
    : hash_(ht.hash_), equals_(ht.equals_), get_key(ht.get_key),num_elements_(0)
  {
    copy_from(ht);
  }

  HashTable& operator=(const HashTable& ht)
  {
    if (&ht != this) {
      clear();
      hash_ = ht.hash;
      equals_ = ht.equals_;
      get_key = ht.get_key;
      copy_from(ht);
    }
    return *this;
  }

  ~HashTable() { clear(); }
public:
  bool empty() const { return num_elements_ == 0; }
  size_type size() const { return num_elements_; }
  size_type max_size() const { return static_cast<size_type>(-1); }
  void swap(HashTable& ht)
  {
    __GSTL::swap(this->hash_, ht.hash_);
    __GSTL::swap(this->equals_, ht.equals_);
    __GSTL::swap(this->get_key, ht.get_key);
    __GSTL::swap(this->buckets_, ht.buckets_);
    __GSTL::swap(this->num_elements_, ht.num_elements_);
  }

  iterator begin()
  {
    for (size_type n = 0; n < buckets_.size(); n++) {
      if (buckets_[n])
        return iterator(buckets_[n], this);
    }
    return end();
  }
  iterator end() { return iterator(nullptr, this); }

  const_iterator begin() const
  {
    for (size_type n = 0; n < buckets_.size(); n++) {
      if (buckets_[n])
        return const_iterator(buckets_[n], this);
    }
    return end();
  }
  const_iterator end() const { return const_iterator(nullptr, this); }

  const_iterator cbegin() const
  {
    for (size_type n = 0; n < buckets_.size(); n++) {
      if (buckets_[n])
        return const_iterator(buckets_[n], this);
    }
    return cend();
  }
  const_iterator cend() const { return const_iterator(nullptr, this); }



public:
  size_type bucket_count() const { return buckets_.size(); }
  size_type max_bucket_count() const { return prime_list[num_primes - 1]; }
  //计算key映射了多少value
  size_type elems_in_bucket(size_type bucket)
  {
    size_type result = 0;
    for (hash_node* cur = buckets_[bucket]; cur; cur = cur->next)
      ++result;
    return result;
  }

  Pair<iterator, bool> insert_unique(const value_type& v)
  {
    resize(num_elements_ + 1);
    return insert_unique_noresize(v);
  }
  iterator insert_equal(const value_type& v)
  {
    resize(num_elements_ + 1);
    return insert_equal_noresize(v);
  }

  template <class InputIterator>
  void insert_unique(InputIterator first, InputIterator last)
  {
    insert_unique(first, last, iterator_category(first));
  }

  template <class InputIterator>
  void insert_equal(InputIterator first, InputIterator last)
  {
    insert_equal(first, last, iterator_category(first));
  }

  template <class InputIterator>
  void insert_unique(InputIterator first, InputIterator last,
                     input_iterator_tag)
  {
    for ( ;first != last; ++first)
      insert_unique(*first);
  }

  template <class InputIterator>
  void insert_equal(InputIterator first, InputIterator last,
                     input_iterator_tag)
  {
    for ( ;first != last; ++first)
      insert_equal(*first);
  }

  template <class InputIterator>
  void insert_unique(InputIterator first, InputIterator last,
                     forward_iterator_tag)
  {
    size_type n = distance(first, last);
    resize(num_elements_ + n);
    for ( ; n > 0; n--,++first)
      insert_unique_noresize(*first);
  }

  template <class InputIterator>
  void insert_equal(InputIterator first, InputIterator last,
                     forward_iterator_tag)
  {
    size_type n = distance(first, last);
    resize(num_elements_ + n);
    for ( ; n > 0; n--,++first)
      insert_equal_noresize(*first);
  }

  reference find_of_insert(const value_type& v);

  size_type erase(const key_type& key);
  void erase(const iterator& it);
  void erase(const const_iterator& it);
  void erase(iterator first, iterator last);
  void erase(const_iterator first, const_iterator last);

  void resize(size_type);
  void clear();
public:

  iterator find(const key_type&);
  const_iterator find(const key_type&) const;
  size_type count(const Key&) const;
  Pair<iterator, iterator> equal_range(const key_type& key);
  Pair<const_iterator, const_iterator> equal_range(const key_type& key) const;
private:
  size_type next_size(size_type n) const { return NextPrime(n); }
  void initialize_buckets(size_type n)
  {
      const size_type n_buckets = next_size(n);
      buckets_.reserve(n_buckets);
      buckets_.insert(buckets_.end(), n_buckets, nullptr);
      num_elements_ = 0;
  }

  size_type bkt_num(const value_type& v, size_type n) const
  {
    return bkt_num_key(get_key(v), n);
  }

  size_type bkt_num(const value_type& v) const
  {
    return bkt_num_key(get_key(v));
  }

  size_type bkt_num_key(const key_type& key) const
  {
    return bkt_num_key(key, buckets_.size());
  }

  size_type bkt_num_key(const key_type& key, size_type n) const
  {
    return hash_(key) % n;
  }

  void copy_from(const HashTable& ht);
  Pair<iterator, bool> insert_unique_noresize(const value_type& v);
  iterator insert_equal_noresize(const value_type& v);
  void erase_bucket(const size_type n, hash_node* first, hash_node* last);
  void erase_bucket(const size_type n, hash_node* first);
private:
  hash_node* new_node(const value_type& v)
  {
    hash_node* node = hash_node_allcoate::Allocate();
    node->next = nullptr;
    try {
      Construct(&node->val, v);
      return node;
    }
    catch(...) {
      hash_node_allcoate::Deallocate(node);
      throw;
    }
    return nullptr;
  }

  void delete_node(hash_node* node)
  {
    Destroy(&node->val);
    hash_node_allcoate::Deallocate(node);
  }

private:
  hasher hash_;
  key_equal equals_;
  ExtractKey  get_key;

  Vector<hash_node*, Alloc> buckets_;
  size_type num_elements_;
};

/*-------------------- HashTableIterator ++ --------------------*/
template <class V, class K, class HF, class Ex, class Eq, class A>
HashTableIterator<V, K, HF, Ex, Eq, A>&
HashTableIterator<V, K, HF, Ex, Eq, A>::operator++()
{
  const hash_node* old = cur;
  cur = cur->next;

  if (!cur) {
    size_type bucket = ht->bkt_num(old->val);
    while (!cur && ++bucket < ht->buckets_.size())
      cur = ht->buckets_[bucket];
  }
  return *this;
}

template <class V, class K, class HF, class Ex, class Eq, class A>
HashTableIterator<V, K, HF, Ex, Eq, A>
HashTableIterator<V, K, HF, Ex, Eq, A>::operator++(int)
{
  iterator tmp = *this;
  ++*this;
  return tmp;
}


/*-------------------- HashTableConstIterator ++ --------------------*/
template <class V, class K, class HF, class Ex, class Eq, class A>
HashTableConstIterator<V, K, HF, Ex, Eq, A>&
HashTableConstIterator<V, K, HF, Ex, Eq, A>::operator++()
{
  const hash_node* old = cur;
  cur = cur->next;

  if (!cur) {
    size_type bucket = ht->bkt_num(old->val);
    while (!cur && ++bucket < ht->buckets_.size())
      cur = ht->buckets_[bucket];
  }
  return *this;
}

template <class V, class K, class HF, class Ex, class Eq, class A>
HashTableConstIterator<V, K, HF, Ex, Eq, A>
HashTableConstIterator<V, K, HF, Ex, Eq, A>::operator++(int)
{
  const_iterator tmp = *this;
  ++*this;
  return tmp;
}

/* ----------------------- 操作符重载 ------------------------ */
template <class Value, class Key, class HashFun,
          class ExtractKey, class EqualKey, class Alloc>
bool operator==(const HashTable<Value, Key, HashFun,
                ExtractKey, EqualKey, Alloc>& x,
                const HashTable<Value, Key, HashFun,
                ExtractKey, EqualKey, Alloc>& y)
{
  typedef typename HashTable<Value, Key, HashFun, ExtractKey,
                             EqualKey, Alloc>::hash_node hash_node;
  if (x.buckets_.size() != y.buckets_.size())
    return false;
  for (int i = 0; i < x.buckets_.size(); i++) {
    hash_node* cur_x = x.buckets_[i];
    hash_node* cur_y = y.buckets_[i];
    while (cur_x && cur_x && cur_x->val == cur_y->val)
      cur_x = cur_x->next, cur_y = cur_y->next;
    if (cur_x || cur_y)
      return false;
  }
  return true;
}

/* ----------------------- 全局swap ------------------------ */
//优先调用这个swap，其次才会调用Algorithm里面的swap
template <class Value, class Key, class HashFun,
          class ExtractKey, class EqualKey, class Alloc>
void swap(HashTable<Value, Key, HashFun,
          ExtractKey, EqualKey, Alloc>& x,
          HashTable<Value, Key, HashFun,
          ExtractKey, EqualKey, Alloc>& y)
{
  x.swap(y);
}


/* ----------------------- erase ------------------------ */
/**
 * 删除与这个key相等的元素
 * @param key key
 * return 返回删除的个数
 */
template <class V, class K, class HF, class Ex, class Eq, class A>
typename HashTable<V, K, HF, Ex, Eq, A>::size_type
HashTable<V, K, HF, Ex, Eq, A>::erase(const key_type &key)
{
  const size_type n = bkt_num_key(key); //首先定位处key所属 bucket
  hash_node* first = buckets_[n];
  size_type erase_count = 0;          //删除的个数
  //进行删除
  if (first) {
    hash_node* cur = first;
    hash_node* next = first->next;
    while (next) {
      if (equals_(get_key(next->val), key)) {
        cur->next = next->next;
        delete_node(next);
        next = cur->next;
        --num_elements_;
        ++erase_count;
      }
      else {
        cur = next;
        next = next->next;
      }
    }

    // 处理开头处就和kay相等的情况，写的很玄妙。。。
    if (equals_(get_key(get_key(first->val), key))) {
      buckets_[n] = first->next;
      delete_node(first);
      --num_elements_;
      ++erase_count;
    }
  }
  return erase_count;

}

/**
 * 删除指定处的元素
 * @param num_elements_hint [description]
 */
 template <class V, class K, class HF, class Ex, class Eq, class A>
 void HashTable<V, K, HF, Ex, Eq, A>::erase(const iterator &it)
 {
   if (hash_node* const p = it.cur) {
     const size_type n = bkt_num(p->val);
     hash_node* cur = buckets_[n];
     //处理开头处就是it所在位置
     if (cur == p) {
       buckets_[n] = cur->next;
       delete_node(cur);
       --num_elements_;
     }
     else {
       hash_node* next = cur->next;
       while (next) {
         if (equals_(get_key(next->val), get_key(p->val))) {
           cur->next = next->next;
           delete_node(next);
           --num_elements_;
           break;
         }
         else {
           cur = cur->next;
           next = next->next;
         }
       }
     }
   }
 }

 // 擦除指定区间的元素
 template <class V, class K, class HF, class Ex, class Eq, class A>
 void HashTable<V, K, HF, Ex, Eq, A>::erase(iterator first, iterator last)
 {
   size_type f_bucket = first.cur ? bkt_num(first.cur->val) : buckets_.size();
   size_type l_bucket = last.cur ? bkt_num(last.cur->val) : buckets_.size();

   if (first.cur == last.cur)
     return;
   else if (f_bucket == l_bucket)
     erase_bucket(f_bucket, first.cur, last.cur);
   else {
     erase_bucket(f_bucket, first.cur, 0);
     for (size_type n = f_bucket + 1; n < l_bucket; ++n)
       erase_bucket(n, 0);
     if (l_bucket != buckets_.size())
       erase_bucket(l_bucket, last.cur);
   }
 }

template <class V, class K, class HF, class Ex, class Eq, class A>
inline void
HashTable<V, K, HF, Ex, Eq, A>::erase(const_iterator first,
                                      const_iterator last)
{
  erase(iterator(const_cast<hash_node*>(first.cur),
                 const_cast<HashTable*>(first.ht)),
        iterator(const_cast<hash_node*>(last.cur),
                 const_cast<HashTable*>(last.ht)));
}

/* ----------------------- find_of_insert ------------------------ */
// 先查找是否存在这个元素，存在直接放回，不存在插入这个元素
template <class V, class K, class HF, class Ex, class Eq, class A>
typename HashTable<V, K, HF, Ex, Eq, A>::reference
HashTable<V, K, HF, Ex, Eq, A>::find_of_insert(const value_type &v)
{
  resize(num_elements_ + 1);

  const size_type n = bkt_num(v);
  hash_node* first = buckets_[n];

  for (hash_node* cur = first ; cur; cur = cur->next)
    if (equals_(get_key(cur->val), get_key(v)))
      return cur->val;
  //没有找到
  hash_node* node = new_node(v);
  node->next = first;
  buckets_[n] = node;
  ++num_elements_;
  return node->val;
}

/*--------------------  resize --------------------
                  判断是否需要重建表格                */
template <class V, class K, class HF, class Ex, class Eq, class A>
void HashTable<V, K, HF, Ex, Eq, A>::resize(size_type num_elements_hint)
{
  /* 是否需要重新配置空间的判断规则是 : 元素个数大于筒的大小，就进行
     重新配置空间，也就是说元素个数不不能大于筒的大小 */
  const size_type old_size = buckets_.size();

  if (num_elements_hint > old_size) { //需要重新配置空间
    const size_type n = next_size(num_elements_hint);     // n 是新的质数，代表新大小
    if (n > old_size) {
      Vector<hash_node*, A> tmp(n, static_cast<hash_node*>(nullptr));
      try {
        for (size_type bucket = 0; bucket < old_size; bucket++) { //遍历的旧buckets
          hash_node* first = buckets_[bucket];
          while (first) { //遍历bucket里面的链表,并且拷贝到新空间
            size_type new_bucket = bkt_num(first->val, n);    // 新的bucket存储位置
            // 保存下一个需要处理的节点
            buckets_[bucket] = first->next;
            // 把当前节点拷贝到新空间对应的位置
            first->next = tmp[new_bucket];
            tmp[new_bucket] = first;
            // 设置下一个需要处理的节点
            first = buckets_[bucket];
          }
        }
        // 进行交换，交换之后旧内容放到了tmp，而且并不用释放所有到hash_node*(也就是链表)，
        // 因为它只是改变了一下链表到指向，并没有新申请空间，tmp是一个临时变量，
        // 离开作用域会自动释放（调用Vector析构函数)
        // ps（这个函数真的写到好牛B，不能不佩服)
        buckets_.swap(tmp);
      }
      catch (...) {
        for (size_type bucket = 0; bucket < tmp.size(); ++bucket) {
          while (tmp[bucket]) {
            hash_node* next = tmp[bucket]->next;
            delete_node(tmp[bucket]);
            tmp[bucket] = next;
        }
      }
      throw;
    }
  }

  }
}

/*--------------------  insert_unique_noresize -------------------- */
// 不需要重建表格，直接进行插入
template <class V, class K, class HF, class Ex, class Eq, class A>
Pair<typename HashTable<V, K, HF, Ex, Eq, A>::iterator, bool>
HashTable<V, K, HF, Ex, Eq, A>::insert_unique_noresize(const value_type& v)
{
  const size_type n = bkt_num(v);
  hash_node* first = buckets_[n];

  for (hash_node* cur = first; cur; cur = cur->next)
    if (equals_(get_key(cur->val), get_key(v)))
      return make_pair(iterator(cur, this), false);

  hash_node* tmp = new_node(v);
  tmp->next = first;
  buckets_[n] = tmp;
  ++num_elements_;
  return make_pair(iterator(tmp, this), true);
}

template <class V, class K, class HF, class Ex, class Eq, class A>
typename HashTable<V, K, HF, Ex, Eq, A>::iterator
HashTable<V, K, HF, Ex, Eq, A>::insert_equal_noresize(const value_type &v)
{
  const size_type n = bkt_num(v);
  hash_node* first = buckets_[n];

  for (hash_node* cur = first; cur; cur = cur->next)
    if (equals_(get_key(cur->val), get_key(v))) {
      hash_node* node = new_node(v);
      node->next = cur->next;
      cur->next = node;
      ++num_elements_;
      return iterator(node, this);
    }

  hash_node* tmp = new_node(v);
  tmp->next = first;
  buckets_[n] = tmp;
  ++num_elements_;
  return iterator(tmp, this);
}

/*--------------------  erase_bucket -------------------- */
// 擦除指定映射位置的所有元素
template <class V, class K, class HF, class Ex, class Eq, class A>
void HashTable<V, K, HF, Ex, Eq, A>::erase_bucket(const size_type n,
                                                  hash_node* first, hash_node* last)
{
  hash_node* cur = buckets_[n];
  if (cur == first)
    erase_bucket(n, last);
  else {
    hash_node* next;
    for (next = cur->next; next != first; cur = next, next = cur->next)
      ;
    while (next) {
      cur->next = next->next;
      delete_node(next);
      next = cur->next;
      --num_elements_;
    }
  }
}

template <class V, class K, class HF, class Ex, class Eq, class A>
void
HashTable<V, K, HF, Ex, Eq, A>::erase_bucket(const size_type n, hash_node* last)
{
  hash_node* cur = buckets_[n];
  while (cur != last) {
    hash_node* next = cur->next;
    delete_node(cur);
    cur = next;
    buckets_[n] = cur;
    --num_elements_;
  }
}

/* ----------------------- clear ------------------------ */
template <class V, class K, class HF, class Ex, class Eq, class A>
void HashTable<V, K, HF, Ex, Eq, A>::clear()
{
  for (size_type i = 0; i < buckets_.size(); i++) {
    hash_node* cur = buckets_[i];
    while (cur) {
      hash_node* next = cur->next;
      delete_node(cur);
      cur = next;
    }
    buckets_[i] = nullptr;
  }
  num_elements_ = 0;
}

/* ----------------------- copy_from ------------------------ */
template <class V, class K, class HF, class Ex, class Eq, class A>
void HashTable<V, K, HF, Ex, Eq, A>::copy_from(const HashTable& ht)
{
  buckets_.clear();
  buckets_.reserve(ht.buckets_.size());
  try {
    for (size_type i = 0; i < ht.buckets_.size(); ++i) {
      if (const hash_node* cur = ht.buckets_[i]) {
        hash_node* copy = new_node(cur->val);
        buckets_[i] = copy;


        for (hash_node* next = cur->next; next; cur = next, next = cur->next) {
          copy->next = new_node(next->val);
          copy = copy->next;
        }
      }
    }
    num_elements_ = ht.num_elements_;
  }
  catch(...) {
    clear();
    throw;
  }
}

/* ----------------------- find ------------------------ */
template <class V, class K, class HF, class Ex, class Eq, class A>
typename HashTable<V, K, HF, Ex, Eq, A>::iterator
HashTable<V, K, HF, Ex, Eq, A>::find(const key_type& key)
{
  const size_type n = bkt_num_key(key); //找到key所属哪一个bucket
  hash_node* first = buckets_[n];
  while (first && !equals_(get_key(first->val), key))
    first = first->next;
  return iterator(first, this);
}

template <class V, class K, class HF, class Ex, class Eq, class A>
typename HashTable<V, K, HF, Ex, Eq, A>::const_iterator
HashTable<V, K, HF, Ex, Eq, A>::find(const key_type& key) const
{
  const size_type n = bkt_num_key(key); //找到key所属哪一个bucket
  hash_node* first = buckets_[n];
  while (first && !equals_(get_key(first->val), key))
    first = first->next;
  return const_iterator(first, this);
}

/* ----------------------- count ------------------------ */
template <class V, class K, class HF, class Ex, class Eq, class A>
typename HashTable<V, K, HF, Ex, Eq, A>::size_type
HashTable<V, K, HF, Ex, Eq, A>::count(const key_type& key) const
{
  const size_type n = bkt_num_key(key);
  size_type result = 0;
  hash_node* first = buckets_[n];
  while (first) {
    if (equals_(get_key(first->val), key))
      result++;
    first = first->next;
  }
  return result;
}

/* ----------------------- equal_range ------------------------ */
template <class V, class K, class HF, class Ex, class Eq, class A>
Pair<typename HashTable<V, K, HF, Ex, Eq, A>::iterator,
     typename HashTable<V, K, HF, Ex, Eq, A>::iterator>
HashTable<V, K, HF, Ex, Eq, A>::equal_range(const key_type& key)
{
  typedef Pair<iterator, iterator> pii;
  const size_type n = bkt_num_key(key);

  for (const hash_node* first = buckets_[n] ; first; first = first->next) {
    if (equals_(get_key(first->val), key)) {
      for (const hash_node* cur = first->next; cur; cur = cur->next)
      if (!equals_(get_key(cur->val), key))
      return pii(iterator(first, this),
      const_iterator(cur, this));
      for (size_type m = n + 1; m < buckets_.size(); ++m)
      if (buckets_[m])
      return pii(iterator(first, this),
      const_iterator(buckets_[m], this));
      return pii(iterator(first, this), end());
    }
  }
  return pii(end(), end());
}

template <class V, class K, class HF, class Ex, class Eq, class A>
Pair<typename HashTable<V, K, HF, Ex, Eq, A>::const_iterator,
     typename HashTable<V, K, HF, Ex, Eq, A>::const_iterator>
HashTable<V, K, HF, Ex, Eq, A>::equal_range(const key_type& key) const
{
  typedef Pair<const_iterator, const_iterator> pii;
  const size_type n = bkt_num_key(key);

  for (const hash_node* first = buckets_[n] ; first; first = first->next) {
    if (equals_(get_key(first->val), key)) {
      for (const hash_node* cur = first->next; cur; cur = cur->next)
      if (!equals_(get_key(cur->val), key))
      return pii(const_iterator(first, this),
      const_iterator(cur, this));
      for (size_type m = n + 1; m < buckets_.size(); ++m)
      if (buckets_[m])
      return pii(const_iterator(first, this),
      const_iterator(buckets_[m], this));
      return pii(const_iterator(first, this), end());
    }
  }
  return pii(end(), end());
}
__GSTL_END_NAMESPACE    // End namespace
#endif        // end hashtable
