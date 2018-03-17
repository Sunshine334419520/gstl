/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-12T14:01:42+08:00
 * @Email:  guang334419520@126.com
 * @Filename: Slist.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-15T14:30:23+08:00
 */

#ifndef GSTL_SLIST_H
#define GSTL_SLIST_H

#include "allocate.h"
#include "Uninitialized.h"
#include "iterator.h"
#include "Algorithm.h"
//#include "reverseiterator.h"
 #ifndef GSTL_RLPOS_H
  #include "relops.h"
#endif
__GSTL_BEGIN_NAMESPACE

struct SlistNodeBase {
  SlistNodeBase* next;
};

template <class T>
struct SlistNode : public SlistNodeBase {
    T data;
};

inline SlistNodeBase* SlistMakeLink(SlistNodeBase* prev_node,
                                    SlistNodeBase* new_node)
{
  new_node->next = prev_node->next;
  prev_node->next = new_node;
  return new_node;
}

//获取指定节点的前一个节点
inline SlistNodeBase* SlistPrevious(SlistNodeBase* head,
                                    const SlistNodeBase* node)
{
  while (head && head->next != node) {
    head = head->next;
  }
  return head;
}

inline const SlistNodeBase* SlistPrevious(const SlistNodeBase* head,
                                          const SlistNodeBase* node)
{
  while (head && head->next != node) {
    head = head->next;
  }
  return head;
}


inline size_t SlistSize(SlistNodeBase* node)
{
  size_t result = 0;
  while (node != nullptr) {
    ++result;
    node = node->next;
  }
  return result;

}

struct SlistIteratorBase {

  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef forward_iterator_tag iterator_category;

  SlistNodeBase* node;

  SlistIteratorBase(SlistNodeBase* x) : node(x) {}

  void incr() { node = node->next;}

  bool operator==(const SlistIteratorBase& x) const {
    return x.node == this->node;
  }
  bool operator!=(const SlistIteratorBase& x) const {
    return x.node != this->node;
  }

};

template <class T, class Ref, class Ptr>
struct SlistIterator : public SlistIteratorBase {
    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef SlistNode<T> list_node;

    typedef SlistIterator<T, T&, T*>  iterator;
    typedef SlistIterator<T, const T&, const T*> const_iterator;
    typedef SlistIterator<T, Ref, Ptr>  self;

    SlistIterator(list_node* x) : SlistIteratorBase(x) {}
    SlistIterator() : SlistIteratorBase(nullptr) {}
    SlistIterator(const SlistIterator& x) : SlistIteratorBase(x.node) {}

    reference operator*() const { return static_cast<list_node*>(node)->data; }
    pointer operator->() const { return &(operator*()); }

    self& operator++() {
      incr();
      return *this;
    }

    self& operator++(int) {
      self tmp = *this;
      ++*this;
      return tmp;
    }
};

template <class T, class Alloc>
class Slist;

template <class T, class Alloc>
bool operator==(const Slist<T, Alloc>&, const Slist<T, Alloc>&);


template <class T, class Alloc = alloc>
class Slist {
  friend bool operator== <> (const Slist& x, const Slist& y);
public:
  typedef T                     value_type;
  typedef value_type&           reference;
  typedef value_type*           pointer;
  typedef const value_type&     const_reference;
  typedef const value_type*     const_pointer;
  typedef size_t                size_type;
  typedef ptrdiff_t             difference_type;

  typedef SlistIterator<T, T&, T*>                iterator;
  typedef SlistIterator<T, const T&, const T*>    const_iterator;

  /*
  typedef reverse_iterator<const_iterator>        const_reverse_iterator;
  typedef reverse_iterator<iterator>              reverse_iterator;
  */

private:
  typedef SlistNode<T>            list_node;
  typedef SlistNodeBase           list_node_base;
  typedef SlistIteratorBase       list_iterator_base;
  typedef SimpleAlloc<list_node, alloc> list_node_alllocate;

public:
  Slist() { head.next = nullptr; }
  Slist(size_type n, const T&x) { fill_initialize(n, x); }
  Slist(int n, const T&x) { fill_initialize(n, x); }
  Slist(long n, const T&x) { fill_initialize(n, x); }
  explicit Slist(size_type n) { fill_initialize(n, value_type()); }

  template <class InputIterator>
  Slist(InputIterator first, InputIterator last)
  {
    range_initialize(first, last);
  }

  Slist(const Slist& x) { range_initialize(x.begin(), x.end()); }
  Slist& operator=(const Slist& x);
  ~Slist() { clear(); }

public:
  iterator begin() { return iterator(static_cast<list_node*>(head.next)); }
  iterator end() { return iterator(nullptr); }
  const_iterator begin() const { return iterator(static_cast<list_node*>(head.next)); }
  const_iterator end() const { return iterator(nullptr); }
  const_iterator cbegin() const { return iterator(static_cast<list_node*>(head.next)); }
  const_iterator cend() const { return iterator(nullptr);  }
  //slist 是一个单向链表
  /*
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rcbegin() const { return const_reverse_iterator(end()); }
  const_reverse_iterator rcend() const { return const_reverse_iterator(begin()); }
  */

  size_type size() const { return SlistSize(head.next); }
  size_type max_size() const { return static_cast<size_type>(-1); }
  bool empty() const { return head.next == nullptr; }

public:
  reference front() { return static_cast<list_node>(head.next)->data; }
  const_reference front() const { return static_cast<list_node>(head.next)->data; }

  void push_front(const T& x)
  {
    SlistMakeLink(&head, create_node(x));
  }

  void pop_front()
  {
    list_node* node = static_cast<list_node*>(head.next);
    head.next = node->next;
    destroy_node(node);
  }


  void swap(Slist& x) {
    list_node_base* tmp = head.next;
    head.next = x.head.next;
    x.head.next = tmp;
  }

  //在pos节点后插入1个节点
  iterator insert_after(iterator pos, const T& x)
  {
    return iterator(_insert_after(pos.node, x));
  }
  iterator insert_after(iterator pos)
  {
    return insert_after(pos, value_type());
  }

  //  在 pos 后买呢插入n个节点
  void insert_after(iterator pos, size_type n, const T& x)
  {
     _insert_after_fill(pos.node, n, x);
  }
  void insert_after(iterator pos, int n, const T& x)
  {
      _insert_after_fill(pos.node, static_cast<size_type>(n), x);
  }
  void insert_after(iterator pos, long n, const T& x)
  {
      _insert_after_fill(pos.node, static_cast<size_type>(n), x);
  }

  //在pos节点后插入区间节点
  template <class InIter>
  void insert_after(iterator pos, InIter first, InIter last)
  {
    _insert_after_range(pos, first, last);
  }

  //在pos节点前面插入一个节点
  iterator insert(iterator pos, const T& x)
  {
    return iterator(_insert_after(SlistPrevious(&head,
           pos.node), x));
  }
  iterator insert(iterator pos)
  {
    return insert(pos, value_type());
  }

  //pos前插入n个节点
  void insert(iterator pos, size_type n, const T& x)
  {
    _insert_after_fill(SlistPrevious(&head, pos.node), n, x);
  }
  void insert(iterator pos, int n, const T& x)
  {
    _insert_after_fill(SlistPrevious(&head, pos.node),
                       static_cast<size_type>(n), x);
  }
  void insert(iterator pos, long n, const T& x)
  {
    _insert_after_fill(SlistPrevious(&head, pos.node),
                       static_cast<size_type>(n), x);
  }

  template <class InIter>
  void insert(iterator pos, InIter first, InIter last)
  {
    _insert_after_range(SlistPrevious(&head, pos.node), first, last);
  }

  iterator erase_after(iterator pos)
  {
    return iterator(static_cast<list_node*>(_erase_after(pos.node)));
  }
  iterator erase_after(iterator before_first, iterator last_node)
  {
    return iterator(static_cast<list_node*>(
                   _erase_after(before_first.node, last_node.node)));
  }

  iterator erase(iterator pos)
  {
    return iterator(static_cast<list_node*>(
                  _erase_after(SlistPrevious(&head, pos.node))));
  }
  iterator erase(iterator first, iterator last)
  {
    return iterator(static_cast<list_node*>(
                  _erase_after(SlistPrevious(&head, first.node), last.node)));
  }

  void clear() { _erase_after(&head, nullptr); }

private:
  //在指定节点后插入一个节点
  list_node* _insert_after(list_node_base* pos, const value_type& x)
  {
    return static_cast<list_node*>(SlistMakeLink(pos, create_node(x)));
  }
  //在指定节点后插入n个节点
  void _insert_after_fill(list_node_base* pos, size_type n, value_type& x)
  {
    for ( ; n > 0; n--)
      pos = SlistMakeLink(pos, create_node(x));
  }
  //在指定节点后插入[first, last)区间的节点
  template <class InIter>
  void _insert_after_range(list_node_base* pos, InIter first, InIter last)
  {
    for ( ; first != last; ++first)
      pos = SlistMakeLink(pos, create_node(*first));
  }

  //擦除pos后面的节点
  list_node_base* _erase_after(list_node_base* pos)
  {
    list_node* tmp = static_cast<list_node*>(pos->next);
    pos->next = pos->next->next;
    destroy_node(tmp);
    return pos->next;
  }
  //擦除(before_first, last_node)的节点
  list_node_base* _erase_after(list_node_base* before_first, list_node_base* last_node)
  {
    list_node* cur = static_cast<list_node*>(before_first->next);
    while (cur != last_node) {
      list_node* tmp = cur;
      cur = static_cast<list_node*>(cur->next);
      destroy_node(tmp);
    }
    before_first->next = last_node;
    return last_node;
  }


private:
  static list_node* create_node(const value_type& x)
  {
    list_node* node = list_node_alllocate::Allocate();
    try {
      Construct(&node->data, x);
      node->next = nullptr;
    }
    catch (...) {
      list_node_alllocate::Deallocate(node);
      throw;
    }
    return node;
  }
  static void destroy_node(list_node* node)
  {
    Destroy(&node->data);
    list_node_alllocate::Deallocate(node);
  }

  void fill_initialize(size_type n, const value_type& x)
  {
    head.next = nullptr;
    try {
      _insert_after_fill(&head, n, x);
    }
    catch (...) {
      clear();
      throw;
    }
  }

  template <class InputIterator>
  void range_initialize(InputIterator first, InputIterator last)
  {
    head.next = nullptr;
    try {
      _insert_after_range(&head, first, last);
    }
    catch (...) {
      clear();
      throw;
    }
  }

private:
  list_node_base head;

};

template <class T, class Alloc>
Slist<T, Alloc>& Slist<T,Alloc>::operator=(const Slist<T, Alloc> &x)
{
  if (this != &x) {
    list_node_base* p1 = &head;   //用来保存最后遍历当前对象结束的位置
    list_node* n1 = head.next;
    list_node n2 = x.head.next;

    while (n1 != nullptr && n2 != nullptr) {
      n1->data = n2->data;
      p1 = n1;
      n1 = static_cast<list_node*>(n1->next);
      n2 = static_cast<list_node*>(n2->next);
    }
    if (n1 == nullptr)
      _insert_after_range(p1, const_iterator(static_cast<list_node*>(n2)),
                          const_iterator(nullptr));
    else
      erase(p1, nullptr);
  }
  return *this;
}


template <class T, class Alloc>
bool operator==(const Slist<T, Alloc>& x, const Slist<T, Alloc>& y)
{
  typedef typename Slist<T, Alloc>::list_node list_node;
  list_node* n1 = static_cast<list_node*>(x.head.next);
  list_node* n2 = static_cast<list_node*>(y.head.next);

  while (n1 && n2) {
    if (n1->data != n2->data)
      return false;
  }

  return n1 == nullptr && n2 == nullptr;
}

template <class T, class Alloc>
bool operator<(const Slist<T, Alloc>& x, const Slist<T, Alloc>& y)
{
  return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class T, class Alloc>
void swap(Slist<T, Alloc>& x, Slist<T,Alloc>& y)
{
  x.swap(y);
}

__GSTL_END_NAMESPACE


#endif
