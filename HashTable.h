/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-19T12:00:37+08:00
 * @Email:  guang334419520@126.com
 * @Filename: HashTable.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-19T17:27:51+08:00
 */

#ifndef GSTL_HASH_TABLE_H
#define GSTL_HASH_TABLE_H

#include <cstddef>
#include "config.h"
#include "alloc.h"
#include "construct.h"
#include "iterator.h"
#include "Pair.h"
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
  HashTableIterator() {}
  HashTableIterator(hash_node* n, hashtable* tab) : cur(n), ht(n) { }

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
   : cur(it.cur), ht(ht.ht) {}

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
  typedef SimpleAlloc<hash_node*, alloc>  hash_node_allcoate;
public:
  HashTable(size_type n, const HashFun& hf,
            const EqualKey& eql)
    : hash_(hf), equals_(eql)
  {
    initialize_buckets(n);
  }
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

  Pair<iterator, bool> insert_unique(const value_type& v)
  {
    resize(num_elements_ + 1);
    return insert_unique_noresize(v);
  }

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
    return hash(key) % n;
  }

  void resize(size_type);
  Pair<iterator, bool> insert_unique_noresize(const value_type& v);
private:
  hash_node* new_node(const value_type& v)
  {
    hash_node* node = hash_node_allcoate::Allocate();
    node->next = nullptr;
    try {
      Construct(&node->val, v);
    }
    catch(...) {
      hash_node_allcoate::Deallocate(node);
      throw;
    }
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

  for (hash_node* cur = first; cur; ++cur)
    if (equals_(get_key(cur->val), get_key(v)))
      return make_pair(iterator(cur, this), false);

  hash_node* tmp = new_node(v);
  new_node->next = first->next;
  buckets_[n] = tmp;
  ++num_elements_;
  return make_pair(iterator(tmp, this), true);
}





__GSTL_END_NAMESPACE
#endif
