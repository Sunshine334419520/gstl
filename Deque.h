/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-07T15:03:05+08:00
 * @Email:  guang334419520@126.com
 * @Filename: Deque.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-07T16:52:42+08:00
 */

#ifndef GSTL_DEQUE_H
#define GSTL_DEQUE_H

__GSTL_BEGIN_NAMESPACE

template <class T, class Ref, class Ptr, size_t BufSize>
struct DequeIterator {
  typedef DequeIterator<T, T&, T*, BufSize>               iterator;
  typedef DequeIterator<T, const T&, const T*, BufSize>   const_iterator;
  typedef DequeIterator<T, Ref, Ptr, BufSize>              self;

  typedef T                             value_type;
  typedef value_type&                   reference;
  typedef value_type*                   pointer;
  typedef ptrdiff_t                     difference_type;
  typedef random_access_iterator_tag    iterator_category;
  typedef const value_type&             const_reference;
  typedef const value_type*             const_pointer;
  typedef size_t                        size_type;
  typedef T**                           map_pointer;

  T* first_;
  T* last_;
  T* current_;
  map_pointer node_;

  static size_t buffer_size() { return __deque_buf_size(BufSize, sizeof(T));}

  void set_node(map_pointer new_node) {
    node_ = new_node;
    first_ = *node_;
    last_ = *node_ + static_cast<difference_type>(buffer_size());
  }

  reference operator*() const { return *current_; }
  pointer operator const { return &(operator*()); }

  difference_type operator-(const self& x) {
    return static_cast<difference_type>(buffer_size()) * (node - x.node - 1) +
           (x.last_ - x.current_) + (current_ - first_);
  }

  self& operator++() {
    ++current_;
    if(current_ == last_) {
      set_node(node_ + 1);
      current_ = first_;
    }

    return *this;
  }

  self operator++() {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self& operator--() {

    if(current_ == first) {
      set_node(node_ - 1);
      current_ = last - 1;
    }
    --current_;
    return *this;
  }

  self operator--() {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self& operator+=(difference_type n) {
    difference_type offset = current_ - first_ + n;
    if (offset >= 0 && offset < static_cast<difference_type>(buffer_size())) {
      //在同一缓冲区内
      current += n;
    } else {    //不再同一缓冲区内}
      difference_type node_offset =
        offset > 0 ? offset / static_cast<difference_type>(buffer_size())
                   : -static_cast<difference_type>( (-offset - 1) / buffer_size()) - 1

      set_node(node_ + node_offset);
      current_ = first + offset - node_offset * static_cast<difference_type>(buffer_size());
    }
    return *this;
  }

  self& operator-=(difference_type n) {
    current_ += -n;
    return *this;
  }

  self operator+(difference_type n) const {
    self tmp = *this;
    return tmp += n;
  }

  self operator-(difference_type n) const {
    self tmp = *this;
    return tmp -= n;
  }

  reference operator[](difference_type n) {
    return *(*this + n);
  }

  bool operator==(const self& x) { return cur == x.current_; }
  bool operator!=(const self& x) { return !(*this == x); }
  bool operator<(const self& x) {
    return (node_ == x.node_) ? current_ < x.current_ : node_ < x.node_;
  }

};

inline size_t
__deque_buf_size(size_t n, size_t sz)
{
  return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}


template <class T, class Alloc = alloc, class BufSize = 0>
class Deque {
public:
  typedef Deque<T, Alloc, BufSize> self;
  typedef DequeIterator<T, T&, T*, BufSize> iterator;
  typedef DequeIterator<T, const T&, const T* BufSize> const_iterator;
  typedef reverse_iterator<const_iterator> const_reverse_iterator;
  typedef reverse_iterator<iterator> reverse_iterator;
public:
  typedef T                             value_type;
  typedef value_type&                   reference;
  typedef value_type*                   pointer;
  typedef ptrdiff_t                     difference_type;
  typedef const value_type&             const_reference;
  typedef const value_type*             const_pointer;
  typedef size_t                        size_type;

protected:
  typedef pointer*                          map_pointer;
  typedef SimpleAlloc<value_type, Alloc>    data_allocator;
  typedef SimpleAlloc<pointer, Alloc>       map_allocator;
public:
  iterator begin() { return start_; }
  iterator end() { return finish_; }
  const_iterator begin() const { return start_; }
  const_iterator end() const { return finish_; }
  const_iterator cbegin() const { return start_; }
  const_iterator cend() const { return finish_; }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rcbegin() const { return const_reverse_iterator(end()); }
  const_reverse_iterator rcend() const { return const_reverse_iterator(begin()); }

  reference operator[](size_type n) { return start_[static_cast<difference_type>(n)]; }
  const_reference operator[](size_type n) const {
    return start_[static_cast<difference_type>(n)];
  }

  reference front() { return *start_; }
  reference back() {
    iterator tmp = finish_;
    return *(--tmp);
  }
  const_reference front() const { return *start_; }
  const_reference back() const {
    const_iterator tmp = finish_;
    return *(--tmp);
  }

  size_type size() const { return finish_ - start_; }
  size_type max_size() const { return static_cast<size_type>(-1);}
  bool empty() const { return finish_ == start_; }

protected:
  static size_type buffer_size() {
    return __deque_buf_size(BufSize, sizeof(value_type));
  }
  static size_type initial_map_size() { return 8; }

protected:
  iterator start_;
  iterator finish_;
  map_pointer map_;
  size_type map_size;

};



__GSTL_END_NAMESPACE
