/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-04T15:59:40+08:00
 * @Email:  guang334419520@126.com
 * @Filename: List.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-13T14:35:59+08:00
 */

#ifndef GSTL_LIST_H
#define GSTL_LIST_H

#include "allocate.h"
#include "Algorithm.h"
#include "Uninitialized.h"
#include "iterator.h"
#include "Traits.h"
#include "reverseiterator.h"

#ifndef GSTL_RLPOS_H
    #include "relops.h"

#endif

__GSTL_BEGIN_NAMESPACE

/* 链表节点数据结构 */
template <class T>
struct ListNode {
  ListNode<T> *prev;
  ListNode<T> *next;
  T           data;
};

/* List 的迭代器  */
template <class T, class Ref, class Ptr>
class ListIterator {
public:
  typedef ListIterator<T, T&, T*>               iterator;
  typedef ListIterator<T, const T&, const T*>   const_iterator;
  typedef ListIterator<T, Ref, Ptr>             self;

  typedef T                   value_type;
  typedef Ref                 reference;
  typedef Ptr                 pointer;
  typedef ptrdiff_t           difference_type;
  typedef size_t              size_type;
  typedef ListNode<T>*        link_type;
  typedef bidirectional_iterator_tag iterator_category;

public:
  ListIterator() : node_(nullptr) {}
  ListIterator(const iterator& x) : node_(x.node_) {}
  ListIterator(link_type x) : node_(x) {}

  link_type data() { return node_; }

  bool operator==(const self& x) const { return this->node_ == x.node_; }
  bool operator!=(const self& x) const { return this->node_ != x.node_; }

  reference operator*() const{ return node_->data; }
  pointer  operator->() const { return &(operator*()); }

  self& operator++() {
    node_ = node_->next;
    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self& operator--() {
    node_ = node_->prev;
    return *this;
  }

  self operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
  }

private:
  link_type node_;

};

//List 是一个环状链表
template <class T, class Alloc = alloc>
class List {
public:
  template <class T2, class Alloc2>
  friend bool operator==(const List<T2, Alloc2>& x, const List<T2, Alloc2>& y);
protected:
  typedef ListNode<T>           list_node;
  typedef SimpleAlloc<list_node, Alloc> list_node_alllocate;

public:
  typedef T                     value_type;
  typedef value_type&                reference;
  typedef value_type*                pointer;
  typedef const value_type&          const_reference;
  typedef const value_type*          const_pointer;
  typedef size_t                size_type;
  typedef ptrdiff_t             difference_type;
  typedef list_node*            link_type;

public:
  typedef ListIterator<T, T&, T*>               iterator;
  typedef ListIterator<T, const T&, const T*>   const_iterator;

  typedef reverse_iterator<const_iterator>      const_reverse_iterator;
  typedef reverse_iterator<iterator>            reverse_iterator;

public:
  List() { empty_initialize(); }
  List(size_type n, const T& x) { fill_initialize(n, x); }
  List(int n, const T& x) { fill_initialize(n, x); }
  List(long n, const T& x) { fill_initialize(n, x); };
  explicit List(size_type n) { fill_initialize(n, T()); }

  template <class InputIterator>
  List(InputIterator first, InputIterator last) { range_initialize(first, last);}

  List(const List<T, Alloc>& x) {
    range_initialize(x.begin(), x.end());
  }


  List<T, Alloc>& operator=(const List<T, Alloc>& x);
  List(List<T, Alloc>&& x) : node_(nullptr) {

    node_ = x.data();

    x.data() = nullptr;
  }

  List<T, Alloc>& operator=(List<T, Alloc>&& x) {
    if (this != &x) {
      clear();
      put_node(node_);

      node_ = x.data();
      x.data() = nullptr;
    }
    return *this;
  }

  ~List() {
    clear();
    put_node(node_);
  }


public:
  iterator begin() { return node_->next; }
  iterator end()   { return node_; }
  const_iterator begin() const { return node_->next; }
  const_iterator end() const { return node_; }
  const_iterator cbegin() const { return node_->next; }
  const_iterator cend() const { return node_; }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rcbegin() const { return const_reverse_iterator(end()); }
  const_reverse_iterator rcend() const { return const_reverse_iterator(begin()); }

  size_type size() {
    size_type result = distance(begin(), end());
    return result;
  }
  size_type max_size() const { return size_type(-1); }
  link_type data() { return node_; }

  bool empty() { return begin() == end(); }
  reference front() { return *begin(); }
  reference back() { return *(--end()); }
  const_reference front() const { return *begin(); }
  const_reference back() const { return *(--end()); }

  void swap(List<T, Alloc>& x) { __GSTL::swap(x.node_, this->node_);}

  void push_back(const T& x) { insert(end(), x); }
  void push_front(const T& x) { insert(begin(), x); }

  void pop_front() { erase(begin()); }
  void pop_back() { iterator tmp = end(); erase(--tmp); }

  iterator insert(iterator position, const T& x) {
    link_type tmp = create_node(x);

    tmp->next = position.data();
    tmp->prev = position.data()->prev;
    position.data()->prev->next = tmp;
    position.data()->prev = tmp;

    return tmp;
  }

  iterator insert(iterator position) { return insert(position, T()); }
  void insert(iterator position, size_type n, const T& x);
  void insert(iterator position, int n, const T& x) {
    insert(position, static_cast<size_type>(n), x);
  }
  void insert(iterator position, long n, const T& x) {
    insert(position, static_cast<size_type>(n), x);
  }

  template <class Inputerator>
  void insert(iterator position,Inputerator first, Inputerator last);

  iterator erase(iterator position) {
    link_type next_node = position.data()->next;
    link_type prev_node = position.data()->prev;

    prev_node->next = next_node;
    next_node->prev = prev_node;
    return static_cast<iterator>(next_node);
  }

  iterator erase(iterator first, iterator last);
  void resize(size_type new_size, const T& x);
  void resize(size_type new_size) { resize(new_size, T()); }

  void clear();
public:
  // 将链表x移动到position之前
  void splice(iterator position, List& x) {
    if (!x.empty())
      transfer(position, x.begin(), x.end());
  }
  // 将链表中i指向的内容移动到position之前
  void splice(iterator position, List&, iterator i) {
    iterator j = i;
    ++j;
    if(position == i || position == j) return ;
    transfer(position, i, j);
  }
 // 将[first, last}元素移动到position之前
  void splice(iterator position, List&, iterator first, iterator last) {
    if (first != last)
      transfer(position, first, last);
  }

  void remove(const T& value);
  void unique();
  void merge(List<T, Alloc>& x);
  void reverse();
  void sort();






protected:
  //构造一个新节点，不进行构造
  link_type get_node() { return list_node_alllocate::Allocate(); }
  //释放一个节点, 并不调用析构
  void put_node(link_type p) { list_node_alllocate::Deallocate(p); }
  //创建一个节点
  link_type create_node(const T& x) {
    link_type p = get_node();
    try {
      Construct(&p->data, x);
    } catch (...) {
      put_node(p);
      throw;
    }

    return p;
  }
  // 释放一个节点
  void destroy_node(link_type p) {
    Destroy(&(p->data));
    put_node(p);
  }
protected:
  //构造一个空链表
  void empty_initialize() {
    node_ = get_node();
    node_->next = node_;
    node_->prev = node_;
  }
  //构造一个n个链表初始值为x的链表
  void fill_initialize(size_type n, const T& x) {
    empty_initialize();
    try {
      insert(begin(), n, x);
    } catch(...) {
      clear();
      put_node(node_);
      throw;
    }
  }
  //构造一个区域链表
  template <class InputIterator>
  void range_initialize(InputIterator first, InputIterator last) {
    empty_initialize();
    try {
      insert(this->begin(), first, last);
    } catch(...) {
      clear();
      put_node(node_);
      throw;
    }
  }
protected:
  void transfer(iterator position, iterator first, iterator last) {
    if (position != last) {
      last.data()->prev->next = position.data();
      first.data()->prev->next = last.data();
      position.data()->prev->next = first.data();

      link_type tmp = first.data()->prev;
      first.data()->prev = position.data()->prev;
      position.data()->prev = last.data()->prev;
      last.data()->prev = tmp;
    }
  }
protected:
  link_type node_;

};

template <class T2, class Alloc2>
inline bool operator==(const List<T2, Alloc2>& x, const List<T2, Alloc2>& y)
{
  typedef typename List<T2, Alloc2>::link_type link_type;
  link_type e1 = x.node_;
  link_type e2 = y.node_;
  link_type n1 = e1->next;
  link_type n2 = e2->next;

  for ( ; e1 != n1 && e2 != n2; n1->next, n2->next)
    if (n1->data != n2->data)
      return false;

  return n1 == e1 && n2 == e2;
}

template <class T, class Alloc>
bool operator<(const List<T, Alloc>& x, const List<T, Alloc>& y)
{
  return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}


template <class T, class Alloc>
List<T, Alloc>& List<T, Alloc>::operator=(const List<T, Alloc> &x)
{
  if (this != &x) {
    iterator first1 = this->begin();
    iterator last1 = this->end();
    iterator first2 = x.begin();
    iterator last2 = x.end();

    //把first2的值复制给first1
    while (first1 != last1 && first2!= last2) *first1++ = *first2++;
    //当前元素大与x就清除多余的元素，小于插入缺少的元素
    if (first1 == last1)
      insert(last1, first2, last2);
    else
      erase(first1, last1);

  }
}

template <class T, class Alloc>
inline void swap(List<T, Alloc>& x, List<T, Alloc>& y)
{
  x.swap(y);
}

template <class T, class Alloc>
void List<T, Alloc>::clear()
{
  link_type cur = node_->next;
  while (cur != node_) {
    link_type tmp = cur;
    cur = cur->next;
    destroy_node(tmp);
  }

  node_->next = node_;
  node_->prev = node_;
}

template <class T, class Alloc>
template <class InputIterator>
void List<T, Alloc>::insert(iterator position,
                            InputIterator first, InputIterator last)
{
  while (first != last) {
    insert(position, *first);
    ++first;
  }
}

template <class T, class Alloc>
void List<T, Alloc>::insert(iterator position, size_type n, const T& x)
{
  for ( ; n > 0; n--)
    insert(position, x);
}

template <class T, class Alloc>
typename List<T, Alloc>::iterator List<T, Alloc>::erase(iterator first, iterator last)
{
  while (first != last) erase(first++);
  return last;
}

template <class T, class Alloc>
void List<T, Alloc>::resize(size_type new_size, const T& x)
{
  size_type len = 0;
  iterator i = begin();
  for ( ; i != end() && len < new_size; len++, ++i)
    ;
  if (len == new_size)
    erase(i, end());
  else
    insert(end(), new_size - len, x);
}

template <class T, class Alloc>
void List<T, Alloc>::remove(const T &value)
{
  iterator first = begin();
  iterator last = end();

  while (first != last)
    if(*first == value)
      first = erase(first);
    else
      ++first;
}

template <class T, class Alloc>
void List<T, Alloc>::unique()
{
  iterator first = begin();
  iterator last = end();

  //如果为空链表，直接返回
  if (first == last) return ;
  iterator next = first;
  while (++next != last) {
    if (*first == *next)
      erase(next);
    else
      first = next;
    next = first;
  }

}

template <class T, class Alloc>
void List<T, Alloc>::merge(List<T, Alloc> &x)
{
  iterator first1 = begin();
  iterator last1 = end();
  iterator first2 = x.begin();
  iterator last2 = x.end();

  while (first1 != last1 && first2 != last2)
    if (*first2 < *first1) {
      iterator next = first2;
      transfer(first1, first2, ++next);
      first2 = next;
    } else {
      ++first1;
    }
    if (first2 != last2) transfer(last1, first2, last2);
}

template <class T, class Alloc>
void List<T, Alloc>::reverse()
{
  //空链表或者仅有一个元素，直接返回
  if (node_->next == node_ || node_->next->next == node_)
    return ;
  iterator first = begin();
  ++first;
  while (first != end()) {
    iterator old_first = first;
    ++first;
    transfer(begin(), old_first, first);
  }
}


// 按照升序排序
template <class T, class Alloc>
void List<T, Alloc>::sort()
{
   if (node_->next == node_ || link_type(node_->next)->next == node_) return;
   List<T, Alloc> carry;
   List<T, Alloc> counter[64];
   int fill = 0;
   while (!empty()) {
     carry.splice(carry.begin(), *this, begin());
     int i = 0;
     while(i < fill && !counter[i].empty()) {
       counter[i].merge(carry);
       carry.swap(counter[i++]);
     }
     carry.swap(counter[i]);
     if (i == fill) ++fill;
   }

   for (int i = 1; i < fill; ++i) counter[i].merge(counter[i-1]);
   swap(counter[fill-1]);
 }

__GSTL_END_NAMESPACE

#endif
