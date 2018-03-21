/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-13T14:16:03+08:00
 * @Email:  guang334419520@126.com
 * @Filename: RB_tree.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-21T17:42:11+08:00
 */

#ifndef __GSTL_RB_TREE_H
#define __GSTL_RB_TREE_H

#include "Allocate.h"
#include "Uninitialized.h"
#include "Iterator.h"
#include "Algorithm.h"
#include "ReverseIterator.h"
#include "Pair.h"
#ifndef __GSTL_RLPOS_H
  #include "Relops.h"
#endif

__GSTL_BEGIN_NAMESPACE

using rb_tree_color_type = bool;

const rb_tree_color_type kRbTreeRed = false;
const rb_tree_color_type kRbTreeBlack = true;

struct RB_tree_node_base {
  typedef rb_tree_color_type color_type;
  typedef RB_tree_node_base* base_ptr;

  color_type color;       // 节点颜色
  base_ptr parent;        // 父节点
  base_ptr left;          // 左节点
  base_ptr right;         // 右节点

  // 求最小节点
  static base_ptr Minimum(base_ptr x)
  {
    while (x->left != nullptr)
      x = x->left;

    return x;
  }

  //求最大节点
  static base_ptr Maximum(base_ptr x)
  {
    while (x->right != nullptr)
      x = x->right;
    return x;
  }
};

void rb_tree_rebalance(RB_tree_node_base* x, RB_tree_node_base*& root);
void rb_tree_rotate_left(RB_tree_node_base* x, RB_tree_node_base*& root);
void rb_tree_rotate_right(RB_tree_node_base* x, RB_tree_node_base*& root);
RB_tree_node_base* rb_tree_rebalance_for_erase(RB_tree_node_base* z,
                            RB_tree_node_base*& root,
                            RB_tree_node_base*& leftmost,
                            RB_tree_node_base*& rightmost);



template <class T>
struct RB_tree_node : public RB_tree_node_base {
  typedef RB_tree_node<T*> link_type;
  T value;
};

struct RB_tree_base_iterator {
  typedef RB_tree_node_base::base_ptr base_ptr;
  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

  base_ptr node;

  void Increment()
  {
    if (node->right != nullptr) {
      node = node->right;
      while (node->left != nullptr)
        node = node->left;
    }
    else {
      base_ptr cur_parent = node->parent;
      while (cur_parent->right == node) {
        node = cur_parent;
        cur_parent = cur_parent->parent;
      }
      if (node->right != cur_parent)
        node = cur_parent;
    }

  }

  void Decrement()
  {
    if (node->color == kRbTreeRed && node->parent->parent == node)
      node = node->right;
    else if (node->left != 0) {
      node = node->left;
      while (node->right != nullptr)
        node = node->right;
    } else {
      base_ptr cur_parent = node->parent;
      while (cur_parent->left == node) {
        node = cur_parent;
        cur_parent = cur_parent->parent;
      }
      node = cur_parent;
    }
  }

};


template <class T, class Ref, class Ptr>
struct RB_tree_iterator : public RB_tree_base_iterator {
  typedef T                            value_type;
  typedef Ref                          reference;
  typedef Ptr                          pointer;

  typedef RB_tree_iterator<T, T&, T*>                 iterator;
  typedef RB_tree_iterator<T, const T&, const T*>     const_iterator;
  typedef RB_tree_iterator<T, Ref, Ptr>               self;
  typedef RB_tree_node<T>*                            link_type;

  RB_tree_iterator() {}
  RB_tree_iterator(link_type x) { node = x; }
  RB_tree_iterator(const iterator& it) { node = it.node; }

  reference operator*() const { return static_cast<link_type>(node)->value; }
  pointer operator->() const { return &(operator*()); }

  self& operator++()
  {
    Increment();
    return *this;
  }

  self operator++(int)
  {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self& operator--()
  {
    Decrement();
    return *this;
  }

  self operator--(int)
  {
    self tmp = *this;
    --*this;
    return tmp;
  }

};

inline bool operator==(const RB_tree_base_iterator& x, const RB_tree_base_iterator& y)
{
  return x.node == y.node;
}

inline bool operator!=(const RB_tree_base_iterator& x, const RB_tree_base_iterator& y)
{
  return x.node != y.node;
}

template <class Key, class Value, class KeyOfValue, class Compare,
          class Alloc = alloc>
class RB_tree {
public:
  typedef Key                   key_type;
  typedef Value                 value_type;
  typedef Value&                reference;
  typedef Value*                pointer;
  typedef const value_type&     const_reference;
  typedef const value_type*     const_pointer;
  typedef RB_tree_node<Value>*  link_type;
  typedef size_t                size_type;
  typedef ptrdiff_t             difference_type;
protected:
  typedef void*                 void_pointer;
  typedef RB_tree_node_base*    base_ptr;
  typedef RB_tree_node<Value>   rb_tree_node;
  typedef RB_tree_node_base     rb_tree_node_base;
  typedef rb_tree_color_type    color_type;
  typedef SimpleAlloc<rb_tree_node, Alloc> rb_tree_node_allocate;

public:
  typedef RB_tree_iterator<Value, reference, pointer> iterator;
  typedef RB_tree_iterator<Value, const_reference, const_pointer> const_iterator;
  typedef reverse_iterator<const_iterator>  const_reverse_iterator;
  typedef reverse_iterator<iterator>        reverse_iterator;

public:
  RB_tree(const Compare& comp = Compare())
    : node_count_(0), key_compare_(comp) { init(); }

  RB_tree(const RB_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
    : node_count_(0), key_compare_(x.key_compare_)
  {
    header_ = get_node();
    header_->color = kRbTreeRed;

    if (x.root() == nullptr) {
      leftmost() = header_;
      rightmost() = header_;
      root() = nullptr;
    }
    else {
      try {
        root() = _copy(x.root(), header_);
      }
      catch (...) {
        put_node(header_);
        throw;
      }
      leftmost() = Minimum(root());
      rightmost() = Maximum(root());
    } // else End
    node_count_ = x.node_count_;
  }

  RB_tree& operator=(const RB_tree& x);

  ~RB_tree()
  {
    clear();
    put_node(header_);
  }
public:
  Compare key_comp() const { return key_compare_; }
  iterator begin() { return leftmost(); }
  iterator end() { return header_; }
  const_iterator begin() const { return leftmost(); }
  const_iterator end() const { return header_; }
  const_iterator cbegin() const { return leftmost(); }
  const_iterator cend() const { return header_; }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rcbegin() const { return const_reverse_iterator(end()); }
  const_reverse_iterator rcend() const { return const_reverse_iterator(begin()); }

  bool empty() const { return node_count_ == 0; }
  size_type size() const { return node_count_; }
  size_type max_size() const { return size_type(-1); }

  void swap( RB_tree& x, RB_tree& y)
  {
    __GSTL::swap(x.header_, y.header_);
    __GSTL::swap(x.node_count_, y.node_count_);
    __GSTL::swap(x.key_compare_, y.key_compare_);

  }

public:
  Pair<iterator, bool> insert_unique(const value_type& v);
  iterator insert_unique(iterator position, const value_type& );
  iterator insert_unique(iterator position) { insert_equal(position, value_type()); }
  template <class II>
  void insert_unique(II first, II last);


  iterator insert_equal(const value_type& v);
  iterator insert_equal(iterator position, const value_type& );
  iterator insert_equal(iterator position) { insert_equal(position, value_type()); }
  template <class II>
  void insert_equal(II first, II last);

  void erase(iterator position);
  size_type erase(const Key&);
  void erase(iterator first, iterator last);
  void erase(const Key* first, const Key* last);

  void clear() {
    if (node_count_ != 0) {
      _erase(root());
      leftmost() = header_;
      root() = nullptr;
      rightmost() = header_;
      node_count_ = 0;
    }
  }

public:

  iterator find(const Key&);
  const_iterator find(const Key&) const;
  size_type count(const Key&) const;
  iterator lower_bound(const Key&);
  const_iterator lower_bound(const Key&) const;
  iterator upper_bound(const Key&);
  const_iterator upper_bound(const Key&) const;
  Pair<iterator, iterator> equal_range(const Key&);
  Pair<const_iterator, const_iterator> equal_range(const Key&) const;


protected:
  link_type get_node() { return rb_tree_node_allocate::Allocate(); }
  void put_node(link_type p) { return rb_tree_node_allocate::Deallocate(p);}

  link_type create_node(const value_type& x)
  {
    link_type node = get_node();
    try {
      Construct(&node->value, x);
    }
    catch (...) {
      put_node(node);
      throw;
    }
    return node;
  }

  link_type clone_node(link_type x)
  {
    link_type node = create_node(x->value);
    node->color = x->color;
    node->left = nullptr;
    node->right = nullptr;
    return node;
  }

  void destroy_node(link_type x)
  {
    Destroy(&x->value);
    put_node(x);
  }

protected:
  // 取得跟根节点
  link_type& root() const { return reinterpret_cast<link_type&>(header_->parent); }
  // 取得最小节点
  link_type& leftmost() const { return reinterpret_cast<link_type&>(header_->left); }
  // 取得最大节点
  link_type& rightmost() const { return reinterpret_cast<link_type&>(header_->right); }

  static link_type& left(link_type x)
  { return reinterpret_cast<link_type&>(x->left); }
  static link_type& right(link_type x)
  { return reinterpret_cast<link_type&>(x->right); }
  static link_type& parent(link_type x)
  { return reinterpret_cast<link_type&>(x->parent); }
  static reference value(link_type x)
  { return x->value; }
  static const Key& key(link_type x)
  { return KeyOfValue()(value(x)); }
  static color_type& color(link_type x)
  { return reinterpret_cast<color_type&>(x->color); }

  static link_type& left(base_ptr x)
  { return reinterpret_cast<link_type&>(x->left); }
  static link_type& right(base_ptr x)
  { return reinterpret_cast<link_type&>(x->right); }
  static link_type& parent(base_ptr x)
  { return reinterpret_cast<link_type>(x->parent); }
  static reference value(base_ptr x)
  { return reinterpret_cast<link_type&>(x)->value; }
  static const Key& key(base_ptr x)
  { return KeyOfValue()(value(static_cast<link_type>(x))); }
  static color_type& color(base_ptr x)
  { return reinterpret_cast<color_type&>(x->color); }

  static link_type Minimum(link_type x)
  {
    return static_cast<link_type>(rb_tree_node_base::Minimum(x));
  }

  static link_type Maximum(link_type x)
  {
    return static_cast<link_type>(rb_tree_node_base::Maximum(x));
  }

private:
  iterator _insert(base_ptr x, base_ptr y, const value_type& v);
  link_type _copy(link_type x, link_type p);
  void _erase(link_type x);


  void init()
  {
    header_ = get_node();
    color(header_) = kRbTreeRed;    //令header颜色为红色，区分 header 和 root
    root() = nullptr;
    leftmost() = header_;
    rightmost() = header_;
  }

protected:
  size_type node_count_;
  link_type header_;
  Compare key_compare_;
};


/* 操作符重载 */

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
 inline bool operator==(const RB_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                        const RB_tree<Key, Value, KeyOfValue, Compare, Alloc>& y) {
   return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
 }

 template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
 inline bool operator<(const RB_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                       const RB_tree<Key, Value, KeyOfValue, Compare, Alloc>& y) {
   return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>&
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::
operator=(const RB_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
{
  if (&x != *this) {
    clear();
    this->node_count_ = 0;
    if (x.root() == nullptr) {
      root() = nullptr;
      leftmost() = header_;
      rightmost() = header_;
    }
    else {
      root() = _copy(x.root(), header_);
      leftmost() = x.leftmost();
      rightmost() = x.rightmost();
      node_count_ = x.node_count_;
    }
  }
  return *this;
}

//使用自己的查找函数

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k)
{
  link_type y = header_;
  link_type x = root();

  while (x != nullptr) {
    if (!key_compare_(key(x), k))
      y = x, x = left(x);
    else
      x = right(x);
  }

  iterator j = iterator(y);
  return (j == end() || key_compare_(k, key(j.node))) ? end() : j;
}
// const find 版本
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) const
{
  link_type y = header_;
  link_type x = root();

  while (x != nullptr) {
    if (!key_compare_(key(x), k))
      y = x, x = left(x);
    else
      x = right(x);
  }

  const_iterator j = const_iterator(y);
  return (j == end() || key_compare_(k, key(j.node))) ? end() : j;
}

//计数函数
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const Key& k) const {
  Pair<const_iterator, const_iterator> p = equal_range(k);
  size_type n = 0;
  distance(p.first, p.sencond, n);
  return n;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline Pair<typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
            typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const Key& k) {
  return Pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}

template <class Key, class Value, class KoV, class Compare, class Alloc>
inline Pair<typename RB_tree<Key, Value, KoV, Compare, Alloc>::const_iterator,
            typename RB_tree<Key, Value, KoV, Compare, Alloc>::const_iterator>
RB_tree<Key, Value, KoV, Compare, Alloc>::equal_range(const Key& k) const {
  return Pair<const_iterator,const_iterator>(lower_bound(k), upper_bound(k));
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k) {
  link_type y = header_;
  link_type x = root();

  while (x != 0)
    if (!key_compare_(key(x), k))
      y = x, x = left(x);
    else
      x = right(x);

  return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k) const {
  link_type y = header_; /* Last node which is not less than k. */
  link_type x = root(); /* Current node. */

  while (x != 0)
    if (!key_compare_(key(x), k))
      y = x, x = left(x);
    else
      x = right(x);

  return const_iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k) {
  link_type y = header_; /* Last node which is greater than k. */
  link_type x = root(); /* Current node. */

   while (x != 0)
     if (key_compare_(k, key(x)))
       y = x, x = left(x);
     else
       x = right(x);

   return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k) const {
  link_type y = header_; /* Last node which is greater than k. */
  link_type x = root(); /* Current node. */

   while (x != 0)
     if (key_compare_(k, key(x)))
       y = x, x = left(x);
     else
       x = right(x);

   return const_iterator(y);
}


//插入函数

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v)
{
  link_type y = header_;
  link_type cur = root();
  while (cur != nullptr) {
    y = cur;
    cur = key_compare_(KeyOfValue()(v), key(cur)) ? left(cur) : right(cur);
  }
  return _insert(cur, y, v);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
Pair<typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
     bool>
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v)
{
  link_type y = header_;
  link_type x = root();

  bool comp = true;
  //从根节点一直往下寻找到合适到插入位置
  while (x != nullptr) {
    y = x;
    comp = key_compare_(KeyOfValue()(v), key(x));      // 判断条件，key_compare 是一个仿函数
    x = comp ? left(x) : right(x);      //大于就往左， 小于等于右
  }

  //创建一个迭代器j指向父节点
  iterator j = iterator(y);
  if (comp)     //  comp如果真，就表示插入点最后是在左边
    if (j == begin())     // 如果为最左节点
      return Pair<iterator, bool>(_insert(x, y, v), true);
    else
      --j;
  if (key_compare_(key(j.node), KeyOfValue()(v)))    // 新到key不重复
    return Pair<iterator, bool>(_insert(x, y, v), true);

  //key重复
  return Pair<iterator, bool>(j, false);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator position,
                                                             const Value& v) {
  if (position.node == header_->left) // begin()
    if (size() > 0 && key_compare_(KeyOfValue()(v), key(position.node)))
      return _insert(position.node, position.node, v);
  // first argument just needs to be non-null
    else
      return insert_unique(v).first;
  else if (position.node == header_) // end()
    if (key_compare_(key(rightmost()), KeyOfValue()(v)))
      return _insert(0, rightmost(), v);
    else
      return insert_unique(v).first;
  else {
    iterator before = position;
    --before;
    if (key_compare_(key(before.node), KeyOfValue()(v))
        && key_compare_(KeyOfValue()(v), key(position.node)))
      if (right(before.node) == 0)
        return _insert(0, before.node, v);
      else
        return _insert(position.node, position.node, v);
    // first argument just needs to be non-null
    else
      return insert_unique(v).first;
  }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(iterator position,
                                                            const Value& v) {
  if (position.node == header_->left) // begin()
    if (size() > 0 && key_compare_(KeyOfValue()(v), key(position.node)))
      return _insert(position.node, position.node, v);
  // first argument just needs to be non-null
    else
      return insert_equal(v);
  else if (position.node == header_) // end()
    if (!key_compare_(KeyOfValue()(v), key(rightmost())))
      return _insert(0, rightmost(), v);
    else
      return insert_equal(v);
  else {
    iterator before = position;
    --before;
    if (!key_compare_(KeyOfValue()(v), key(before.node))
        && !key_compare_(key(position.node), KeyOfValue()(v)))
      if (right(before.node) == 0)
        return _insert(0, before.node, v);
      else
        return _insert(position.node, position.node, v);
    // first argument just needs to be non-null
    else
      return insert_equal(v);
  }
}

template <class K, class V, class KV, class C, class A>
template <class II>
void RB_tree<K, V, KV, C, A>::insert_equal(II first, II last)
{
  for ( ; first != last; ++first)
    insert_equal(*first);
}

template <class K, class V, class KV, class C, class A>
template <class II>
void RB_tree<K, V, KV, C, A>::insert_unique(II first, II last)
{
  for ( ; first != last; ++first)
    insert_unique(*first);
}

// 删除函数

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position)
{
  link_type y = static_cast<link_type>(rb_tree_rebalance_for_erase(position.node,
                                                                   header_->parent,
                                                                   header_->left,
                                                                   header_->right));
  destroy_node(y);
  --node_count_;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key& x) {
  Pair<iterator,iterator> p = equal_range(x);
  size_type n = 0;
  distance(p.first, p.sencond, n);
  erase(p.first, p.sencond);
  return n;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first,
                                                            iterator last) {
  if (first == begin() && last == end())
    clear();
  else
    while (first != last) erase(first++);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key* first,
                                                            const Key* last) {
  while (first != last) erase(*first++);
}

// erase End



template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::
_insert(base_ptr _x, base_ptr _y, const value_type& v)
{
  link_type x = static_cast<link_type>(_x);
  link_type y = static_cast<link_type>(_y);
  link_type z;

  if (y == header_ || x != nullptr || key_compare_(KeyOfValue()(v), key(y))) {
    z = create_node(v);
    left(y) = z;
    if (y == header_) {
      root() = z;
      rightmost() = z;
    }
    else if (y == leftmost())
      leftmost() = z;

  }
  else {
    z = create_node(v);
    right(y) = z;
    if (y == rightmost())
      rightmost() = z;
  }

  parent(z) = y;
  left(z) = nullptr;
  right(z) = nullptr;

  rb_tree_rebalance(z, header_->parent);
  ++node_count_;
  return iterator(z);
}

template <class K, class V, class KeyOfValue, class Compare, class Alloc>
typename RB_tree<K, V, KeyOfValue, Compare, Alloc>::link_type
RB_tree<K, V, KeyOfValue, Compare, Alloc>::_copy(link_type x, link_type p) {
                                // structural copy.  x and p must be non-null.
  link_type top = clone_node(x);
  top->parent = p;

  try {
    if (x->right)
      top->right = _copy(right(x), top);
    p = top;
    x = left(x);

    while (x != 0) {
      link_type y = clone_node(x);
      p->left = y;
      y->parent = p;
      if (x->right)
        y->right = _copy(right(x), y);
      p = y;
      x = left(x);
    }
  }
  catch (...) { _erase(top); throw; }

  return top;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
 void RB_tree<Key, Value, KeyOfValue, Compare, Alloc>::_erase(link_type x) {
                                 // erase without rebalancing
   while (x != 0) {
     _erase(right(x));
     link_type y = left(x);
     destroy_node(x);
     x = y;
   }
 }

/**
 * 全局函数用来保证树到平衡性
 * @param x     旋转点
 * @param root  根节点，是一个*&类型，因为我们可能会修改root
 */
inline void
rb_tree_rebalance(RB_tree_node_base* x, RB_tree_node_base*& root)
{
  x->color = kRbTreeRed;      //新节点必须为红色
  while (x != root && x->parent->color == kRbTreeRed) { // x 的父节点为红，有问题，进入循环
    if (x->parent == x->parent->parent->left) {     //如果父节点是祖父节点的左节点
      RB_tree_node_base* uncle_x = x->parent->parent->right;  //x 伯父节点
      if (uncle_x != nullptr && uncle_x->color == kRbTreeRed) { //伯父节点存在且颜色为红
        //这样x的父节点和祖父节点都为红,
        //把father and uncle change blank, grandfather change red.
        x->parent->color = kRbTreeBlack;
        uncle_x->color = kRbTreeBlack;
        uncle_x->parent->color = kRbTreeRed;

        //重新设置x 进行下一轮检查
        x = x->parent->parent;
      }
      else {  //伯父节点不存在或则为黑
        //这种情况需要判断是内侧插入，还是外侧插入
        if (x == x->parent->right) {    //左右方向, 先左旋转在右旋转
          x = x->parent;    //把x移动到旋转点
          rb_tree_rotate_left(x, root);
        }
        // 改变颜色，然后进行右旋转
        x->parent->color = kRbTreeBlack;
        x->parent->parent->color = kRbTreeRed;

        rb_tree_rotate_right(x->parent->parent, root);
      }
    }
    else {  //父节点是祖父节点到右子节点
      RB_tree_node_base* uncle_x = x->parent->parent->left;
      if (uncle_x != nullptr && uncle_x->color == kRbTreeRed) {//伯父节点存在且颜色为红
        //这样x的父节点和祖父节点都为红,
        //把father and uncle change blank, grandfather change red.
        x->parent->color = kRbTreeBlack;
        uncle_x->color = kRbTreeBlack;
        uncle_x->parent->color = kRbTreeRed;

        //重新设置x 进行下一轮检查
        x = x->parent->parent;
      }
      else {//伯父节点不存在或则为黑
        if (x == x->parent->left) { //右左
          x = x->parent;
          rb_tree_rotate_right(x, root);
        }
          // 改变颜色，然后进行右旋转
        x->parent->color = kRbTreeBlack;
        x->parent->parent->color = kRbTreeRed;

        rb_tree_rotate_left(x->parent->parent, root);

      }
    }

  }   // while End
  root->color = kRbTreeBlack;       //  root color always black .
}


/*  ---------     删除节点维护平衡性       ---------- */


inline RB_tree_node_base*
rb_tree_rebalance_for_erase(RB_tree_node_base* z,
                            RB_tree_node_base*& root,
                            RB_tree_node_base*& leftmost,
                            RB_tree_node_base*& rightmost)
{
  RB_tree_node_base* y = z;
  RB_tree_node_base* x = nullptr;
  RB_tree_node_base* x_parent = nullptr;

  if (y->left == nullptr)     //左节点为空
    x = y->right;
  else {
    if (y->right == nullptr) //右节点为空
      x = y->left;
    else {    //左右都不为空
      //寻找右边最小节点
      y = y->right;
      while (y->left != nullptr)
        y = y->left;
      x = y->right;
    }
  }

  if (y != z) {     //左右都为空
    z->left->parent = y;
    y->left = z->left;
    if (y != z->right) {
    x_parent = y->parent;
    if (x) x->parent = y->parent;
    y->parent->left = x;      // y must be a left child
    y->right = z->right;
    z->right->parent = y;
  }
  else
    x_parent = y;
  if (root == z)
    root = y;
  else if (z->parent->left == z)
    z->parent->left = y;
  else
    z->parent->right = y;
  y->parent = z->parent;
  __GSTL::swap(y->color, z->color);
  y = z;
  // y now points to node to be actually deleted
}
else {                        // y == z
  x_parent = y->parent;
  if (x) x->parent = y->parent;
  if (root == z)
    root = x;
  else
    if (z->parent->left == z)
      z->parent->left = x;
    else
      z->parent->right = x;
  if (leftmost == z)
    if (z->right == 0)        // z->left must be null also
      leftmost = z->parent;
  // makes leftmost == header if z == root
    else
      leftmost = RB_tree_node_base::Minimum(x);
  if (rightmost == z)
    if (z->left == 0)         // z->right must be null also
      rightmost = z->parent;
  // makes rightmost == header if z == root
    else                      // x == z->left
      rightmost = RB_tree_node_base::Maximum(x);
}
if (y->color != kRbTreeRed) {
  while (x != root && (x == 0 || x->color == kRbTreeBlack))
    if (x == x_parent->left) {
      RB_tree_node_base* w = x_parent->right;
      if (w->color == kRbTreeRed) {
        w->color = kRbTreeBlack;
        x_parent->color = kRbTreeRed;
        rb_tree_rotate_left(x_parent, root);
        w = x_parent->right;
      }
      if ((w->left == 0 || w->left->color == kRbTreeBlack) &&
          (w->right == 0 || w->right->color == kRbTreeBlack)) {
        w->color = kRbTreeRed;
        x = x_parent;
        x_parent = x_parent->parent;
      } else {
        if (w->right == 0 || w->right->color == kRbTreeBlack) {
          if (w->left) w->left->color = kRbTreeBlack;
          w->color = kRbTreeRed;
          rb_tree_rotate_right(w, root);
          w = x_parent->right;
        }
        w->color = x_parent->color;
    x_parent->color = kRbTreeBlack;
    if (w->right) w->right->color = kRbTreeBlack;
    rb_tree_rotate_left(x_parent, root);
    break;
  }
} else {                  // same as above, with right <-> left.
  RB_tree_node_base* w = x_parent->left;
  if (w->color == kRbTreeRed) {
    w->color = kRbTreeBlack;
    x_parent->color = kRbTreeRed;
    rb_tree_rotate_right(x_parent, root);
    w = x_parent->left;
  }
  if ((w->right == 0 || w->right->color == kRbTreeBlack) &&
      (w->left == 0 || w->left->color == kRbTreeBlack)) {
    w->color = kRbTreeRed;
    x = x_parent;
    x_parent = x_parent->parent;
  } else {
    if (w->left == 0 || w->left->color == kRbTreeBlack) {
      if (w->right) w->right->color = kRbTreeBlack;
      w->color = kRbTreeRed;
      rb_tree_rotate_left(w, root);
      w = x_parent->left;
    }
    w->color = x_parent->color;
    x_parent->color = kRbTreeBlack;
    if (w->left) w->left->color = kRbTreeBlack;
           rb_tree_rotate_right(x_parent, root);
           break;
         }
       }
     if (x) x->color = kRbTreeBlack;
   }
   return y;


}

/*  ---------     左旋转函数       ---------- */
inline void
rb_tree_rotate_left(RB_tree_node_base* x, RB_tree_node_base*& root)
{
  RB_tree_node_base* y = x->right;  // save 需要替换x位置到节点

  x->right = y->left;
  if (y->left != nullptr)
    y->left->parent = x;      //设置y->left 的父节点

  y->parent = x->parent;

  if (x == root)    // x为根节点
    root = y;
  else if (x == x->parent->left)      // x在父节点的左子节点上
    x->parent->left = y;
  else                      // 在右节点
    x->parent->right = y;

  y->left = x;
  x->parent = y;
}

/*  ---------     右转函数       ---------- */
inline void
rb_tree_rotate_right(RB_tree_node_base* x, RB_tree_node_base*& root)
{
  RB_tree_node_base* y = x->left;

  x->left = y->right;
  if (y->right != nullptr)
    y->right->parent = x;

  y->parent = x->parent;

  if (x == root)
    root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else if (x == x->parent->right)
    x->parent->right = y;

  y->right = x;
  x->parent = y;
}


__GSTL_END_NAMESPACE

#endif
