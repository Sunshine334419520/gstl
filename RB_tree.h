/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-13T14:16:03+08:00
 * @Email:  guang334419520@126.com
 * @Filename: RB_tree.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-13T17:50:38+08:00
 */

#ifndef GSTL_RB_TREE_H
#define GSTL_RB_TREE_H

__GSTL_BEGIN_NAMESPACE

using rb_tree_color_type = bool;

const rb_tree_color_type KRbTreeRed = false;
const rb_tree_color_type KRbTreeBlack = true;

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

template <class T>
struct RB_tree_node : public RB_tree_node_base {
  typedef RB_trr_node<T*> link_type;
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
    if (node->color == KRbTreeRed && node->parent->parent == node)
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
struct RB_tree_iterator : publie RB_tree_base_iterator {
  typedef T                            value_type;
  typedef Ref                          reference;
  typedef Ptr                          pointer;

  typedef RB_tree_iterator<T, T&, T*>                 iterator;
  typedef RB_tree_iterator<T, const T&, const T*>     const_iterator;
  typedef RB_tree_iterator<T, Ref, Ptr>               self;
  typedef RB_tree_node<T>*                            link_type;

  RB_tree_iterator() {}
  RB_tree_iterator(link_type) { node = x; }
  RB_tree_iterator(const iterator& it) { it.node = node; }

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

template <class Key, class Value, class KeyOfValue, class Alloc = alloc>
class RB_tree {
public:
  typedef Key                   key_type;
  typedef Value                 value_type;
  typedef Value&                reference;
  typedef value*                pointer;
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

public:
  iterator begin() { return leftmost(); }
  iterator end() { return header_; }
  const_iterator begin() const { return leftmost(); }
  const_iterator end() const { return header_; }
  const_iterator cbegin() const { return leftmost(); }
  const_iterator cend() const [ return header_; ]
  reverse_iterator rbegin() { return end(); }
  reverse_iterator rend() { return begin(); }
  const_reverse_iterator rcbegin() const { return end(); }
  const_reverse_iterator rcend() const { return begin(); }


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

  link_type destroy_node(link_type x)
  {
    Destory(&x->value);
    put_node(x);
  }

protected:
  // 取得跟根节点
  link_type& root() const { return static_cast<link_type&>(header_->parent); }
  // 取得最小节点
  link_type& leftmost() const { return static_cast<link_type&>(header_->left); }
  // 取得最大节点
  link_type& rightmost() const { return static_cast<link_type&>(header_->right); }

  static link_type& left(link_type x)
  { return static_cast<link_type&>(x->left); }
  static link_type& right(link_type x)
  { return static_cast<link_type&>(x->right); }
  static link_type& parent(link_type x)
  { return static_cast<link_type&>(x->parent); }
  static reference value(link_type x)
  { return x->value; }
  static const Key& key(link_type x)
  { return KeyOfValue()(value(x)); }
  static color_type& color(link_type x)
  { return static_cast<color_type&>(x->color); }

  static link_type& left(base_ptr x)
  { return static_cast<link_type&>(x->left); }
  static link_type& right(base_ptr x)
  { return static_cast<link_type&>(x->right); }
  static link_type& parent(base_ptr x)
  { return static_cast<link_type&>(x->parent); }
  static reference value(base_ptr x)
  { return x->value; }
  static const Key& key(base_ptr x)
  { return KeyOfValue()(value(x)); }
  static color_type& color(base_ptr x)
  { return static_cast<color_type&>(x->color); }

  static link_type Minimum(link_type x)
  {
    return static_cast<link_type>(rb_tree_node_base::Minimum(x));
  }

  static link_type Maximum(link_type x)
  {
    return static_cast<link_type>(rb_tree_node_base::Maximum(x));
  }

protected:
  size_type node_count_;
  link_type header_;
};

__GSTL_END_NAMESPACE

#endif
