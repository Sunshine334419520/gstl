/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-07T15:03:05+08:00
 * @Email:  guang334419520@126.com
 * @Filename: Deque.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-15T14:33:33+08:00
 */

#ifndef GSTL_DEQUE_H
#define GSTL_DEQUE_H

#include "allocate.h"
#include "Uninitialized.h"
#include "iterator.h"
#include "Algorithm.h"
#include "reverseiterator.h"
#ifndef GSTL_RLPOS_H
  #include "relops.h"
#endif

__GSTL_BEGIN_NAMESPACE

inline size_t
__deque_buf_size(size_t n, size_t sz)
{
  return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

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

  T* current_;
  T* first_;
  T* last_;
  map_pointer node_;

  DequeIterator(T* x, map_pointer y)
   : current_(x), first_(*y), last_(*y + buffer_size()), node_(y) {}
  DequeIterator() : current_(nullptr), first_(nullptr), last_(nullptr), node_(nullptr) {}
  DequeIterator(const iterator& x)
   : current_(x.current_), first_(x.first_), last_(x.last_), node_(x.node_) {}

  static size_t buffer_size() { return __deque_buf_size(BufSize, sizeof(T));}

  void set_node(map_pointer new_node) {
    node_ = new_node;
    first_ = *node_;
    last_ = *node_ + static_cast<difference_type>(buffer_size());
  }

  reference operator*() const { return *current_; }
  pointer operator->() const { return &(operator*()); }

  difference_type operator-(const self& x) const {
    return static_cast<difference_type>(buffer_size()) * (node_ - x.node_ - 1) +
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

  self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self& operator--() {

    if(current_ == first_) {
      set_node(node_ - 1);
      current_ = last_;
    }
    --current_;
    return *this;
  }

  self operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
  }

  self& operator+=(difference_type n) {
    difference_type offset = current_ - first_ + n;
    if (offset >= 0 && offset < static_cast<difference_type>(buffer_size())) {
      //在同一缓冲区内
      current_ += n;
    } else {    //不再同一缓冲区内}
      difference_type node_offset =
        offset > 0 ? offset / static_cast<difference_type>(buffer_size())
                   : -static_cast<difference_type>( (-offset - 1) / buffer_size()) - 1;

      set_node(node_ + node_offset);
      current_ = first_ + offset - node_offset * static_cast<difference_type>(buffer_size());
    }
    return *this;
  }

  self& operator-=(difference_type n) {
    return *this += -n;
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

  bool operator==(const self& x) { return current_ == x.current_; }
  bool operator!=(const self& x) { return !(*this == x); }
  bool operator<(const self& x) {
    return (node_ == x.node_) ? current_ < x.current_ : node_ < x.node_;
  }

};




template <class T, class Alloc = alloc, size_t BufSize = 0>
class Deque {
public:
  typedef Deque<T, Alloc, BufSize> self;
  typedef DequeIterator<T, T&, T*, BufSize> iterator;
  typedef DequeIterator<T, const T&, const T*, BufSize> const_iterator;
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

public:
  Deque()
    : start_(), finish_(), map_(nullptr),map_size_(0)
  {
    create_map_and_nodes(0);
  }
  Deque(const Deque& x)
    : start_(), finish_(), map_(nullptr),map_size_(0)
  {
    create_map_and_nodes(x.size());

    try {
      uninitialized_copy(x.begin(), x.end(), this->begin());
    } catch(...) {
      destroy_map_and_nodes();
      throw;
    }
  }
  Deque(size_type n, const T& x)
    : start_(), finish_(), map_(nullptr),map_size_(0)
  {
      fill_initialize(n, x);
  }
  Deque(int n, const T& x)
    : start_(), finish_(), map_(nullptr),map_size_(0)
  {
     fill_initialize(n, x);
  }
  Deque(long n, const T& x)
    : start_(), finish_(), map_(nullptr),map_size_(0)
  {
    fill_initialize(n, x);
  }
  explicit Deque(size_type n)
    : start_(), finish_(), map_(nullptr),map_size_(0)
  {
     fill_initialize(n, T());
  }

  template <class Inputerator>
  Deque(Inputerator first, Inputerator last)
    : start_(), finish_(), map_(nullptr),map_size_(0)
  {
    range_initialize(first, last, iterator_category(first));
  }

  Deque& operator=(Deque& x)
  {
    if (this != &x) {
      if (this->size() >= x.size()) {
        const_iterator tmp = copy(x.begin(), x.end(), begin());
        erase(tmp,  end());
      }
      else {
        const_iterator mid = x.begin() + static_cast<difference_type>(size());
        copy(x.begin(), mid, begin());
        insert(end(), mid, x.end());
      }
    }
    return *this;
  }

  ~Deque()
  {
    Destroy(start_, finish_);
    destroy_map_and_nodes();
  }

public:
  void swap(Deque& x)
  {
    __GSTL::swap(this->start_, x.start_);
    __GSTL::swap(this->finish_, x. finish_);
    __GSTL::swap(this->map_, x.map_);
    __GSTL::swap(this->map_size_, x.map_size_);
  }

  void push_back(const T& x)
  {
    if (finish_.current_ != finish_.last_ - 1) {
      Construct(finish_.current_, x);
      ++finish_.current_;
    } else
      push_back_aux(x);
  }
  void push_front(const T& x)
  {
    if (start_.current_ != start_.first_) {
      --start_.current_;
      Construct(start_.current_, x);
    } else {
      push_front_aux(x);
    }
  }

  void pop_back() {
    if (finish_.current_ != finish_.first_)
    {
      //最后缓冲区还有元素
      --finish_.current_;
      Destroy(finish_.current_);
    } else
      //最后的缓冲区没有任何位置，需要做释放工作
      pop_back_aux();
  }

  void pop_front()
  {
    if (start_.current_ != start_.last_ - 1) {
      Destroy(start_.current_);
      ++start_.current_;
    } else
      //第一个缓冲区还有一个元素
      pop_front_aux();
  }

  void clear();

  void resize(size_type new_size, const value_type& x)
  {
    const size_type len = size();
    if (new_size < len)
    erase(start_ + new_size, finish_);
    else
    insert(finish_, new_size - len, x);
  }

  void resize(size_type new_size) { resize(new_size, value_type()); }

  iterator erase(iterator pos)
  {
    iterator pos_next = pos;
    ++pos_next;
    difference_type index = pos - start_;

    if (index < (size() >> 1)) {  //删除点后面元素较多，移动前面的元素
      copy_backward(start_, pos, pos_next);
      pop_front();
    } else { //删除点钱面元素较多，移动后面的元素
      copy(pos_next, finish_, pos);
      pop_back();
    }

    return start_ + index;
  }

  iterator erase(iterator first, iterator last);

  iterator insert(iterator position, const T& x)
  {
    if (position.current_ == start_.current_) {   //在deque最前端插入
      push_front(x);
      return start_;
    }
    else if (position.current_ == finish_.current_) {
      push_back(x);
      iterator tmp = finish_;
      return --tmp;
    }
    else {
      return insert_aux(position, x);
    }
  }

  iterator insert(iterator position) { return insert(position, value_type()); }
  void insert(iterator position, size_type n, const T& x);
  void insert(iterator position, int n, const T& x)
  {
    insert(position, static_cast<size_type>(n), x);
  }
  void insert(iterator position, long n, const T& x)
  {
    insert(position, static_cast<size_type>(n), x);
  }

  template <class Inputerator>
  void insert(iterator position, Inputerator first, Inputerator last)
  {
    insert(position, first, last, iterator_category(first));
  }

public:
  //操作符重载，优先调用成员函数重载
  bool operator==(const Deque<T, Alloc, BufSize>& x)
  {
    return size() == x.size() && equal(begin(), end(), x.begin());
  }

  bool operator!=(const Deque<T, Alloc, BufSize>& x)
  {
    return size() != x.size() && !equal(begin(), end(), x.begin());
  }

  bool operator<(const Deque<T, Alloc, BufSize>& x)
  {
    return lexicographical_compare(begin(), end(), x.begin(), x.end());
  }

  /*
  reference operator[](size_type n)
  {
     return start_[static_cast<difference_type>(n)];
  }
  const_reference operator[](size_type n) const
  {
    return start_[static_cast<difference_type>(n)];
  }
  */

protected:
  static size_type buffer_size()
  {
    return __deque_buf_size(BufSize, sizeof(value_type));
  }
  static size_type initial_map_size() { return 8; }

protected:
  void create_map_and_nodes(size_type n);
  void destroy_map_and_nodes();
  void fill_initialize(size_type n, const T& x);

  template <class InputIterator>
  void range_initialize(InputIterator first,
                        InputIterator last, input_iterator_tag);
  template <class ForwardIterator>
  void range_initialize(ForwardIterator first,
                        ForwardIterator last, forward_iterator_tag);

  void deallocate_node(pointer p) { data_allocator::Deallocate(p, buffer_size()); }
  pointer allocate_node() { return data_allocator::Allocate(buffer_size()); }


protected:

  void push_front_aux(const T& x);
  void push_back_aux(const T& x);
  void pop_back_aux();
  void pop_front_aux();

  iterator insert_aux(iterator position, const T& x);
  void insert_aux(iterator position, size_type n, const T& x);
  template <class ForwardIterator>
  void insert_aux(iterator position, ForwardIterator first,
                  ForwardIterator last, size_type n);

  void reserve_map_at_back(size_type nodes_to_add = 1);
  void reserve_map_at_front(size_type nodes_to_add = 1);
  void reallocate_map(size_type nodes_to_add, bool add_at_front);

  iterator reserve_elements_at_front(size_type n)
  {
    size_type vacancies = start_.current_ - start_.first_;
    if (n > vacancies)
      new_elements_at_front(n - vacancies);
    return start_ - static_cast<difference_type>(n);
  }

  iterator reserve_elements_at_back(size_type n)
  {
    size_type vacancies = finish_.last_ - finish_.current_;
    if (n > vacancies)
      new_elements_at_back(n - vacancies);
    return finish_ + static_cast<difference_type>(n);
  }

  void new_elements_at_back(size_type n);
  void new_elements_at_front(size_type n);

  void destroy_nodes_at_front(iterator before_start);
  void destroy_nodes_at_back(iterator after_finish);

  template <class Inputerator>
  void insert(iterator position, Inputerator first, Inputerator last,
              input_iterator_tag);
  template <class ForwardIterator>
  void insert(iterator position, ForwardIterator first, ForwardIterator last,
              forward_iterator_tag);

protected:
  iterator start_;
  iterator finish_;
  map_pointer map_;
  size_type map_size_;

};

// 操作符重载，如果不符合成员函数重载，就调用外部的重载
template <class T, class Alloc, size_t BufSize>
bool operator==(const Deque<T, Alloc, BufSize>& x,
                const Deque<T, Alloc, BufSize>& y)
{
  return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}

template <class T, class Alloc, size_t BufSize>
bool operator!=(const Deque<T, Alloc, BufSize>& x,
                const Deque<T, Alloc, BufSize>& y)
{
  return x.size() != y.size() && !equal(x.begin(), x.end(), y.begin());
}

template <class T, class Alloc, size_t BufSize>
bool operator<(const Deque<T, Alloc, BufSize>& x,
                const Deque<T, Alloc, BufSize>& y)
{
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elemes)
{
  //计算所需的节点数
  size_type num_nodes =  num_elemes / buffer_size() + 1;

  //map大小最小为8
  map_size_ = max(initial_map_size(), num_nodes);
  //申请内存
  map_ = map_allocator::Allocate(map_size_);

  // 把节点放到map中间位置
  map_pointer nstart = map_ + (map_size_ - num_nodes) / 2;
  map_pointer nfinish = nstart + num_nodes - 1;

  map_pointer cur;

  try {
    //给每一个节点分配缓冲区
    for (cur = nstart; cur <= nfinish; ++cur)
      *cur = allocate_node();
  } catch(...) {
    for (map_pointer n = nstart; n < cur; ++n)
      deallocate_node(*cur);
    map_allocator::Deallocate(map_, map_size_);
    throw;
  }

  //调整 start_ and finish_
  start_.set_node(nstart);
  finish_.set_node(nfinish);

  start_.current_ = start_.first_;
  finish_.current_ = finish_.first_ + num_elemes % buffer_size();

}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::destroy_map_and_nodes()
{
  for (map_pointer cur = start_.node_; cur <= finish_.node_; ++cur)
    deallocate_node(*cur);

  map_allocator::Deallocate(map_, map_size_);
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::fill_initialize(size_type n, const T& x)
{
  //分配n个内存
  create_map_and_nodes(n);
  map_pointer cur;
  try {
    //付初值
    for (cur = start_.node_; cur < finish_.node_; ++cur)
      uninitialized_fill(*cur, *cur + buffer_size(), x);
    uninitialized_fill(finish_.first_, finish_.current_, x);
  } catch(...) {
    //出现异常 释放内存
    for (map_pointer n = start_.node_; n < cur; ++n)
      Destroy(*n, *n + buffer_size());
    destroy_map_and_nodes();
    throw;
  }

}

template <class T, class Alloc, size_t BufSize>
template <class Inputerator>
void Deque<T, Alloc, BufSize>::range_initialize(Inputerator first,
                                                Inputerator last,
                                                input_iterator_tag)
{
  create_map_and_nodes(0);

  for ( ; first != last; ++first)
    push_back(*first);
}

template <class T, class Alloc, size_t BufSize>
template <class ForwardIterator>
void Deque<T, Alloc, BufSize>::range_initialize(ForwardIterator first,
                                                ForwardIterator last,
                                                forward_iterator_tag)
{
  difference_type n = distance(first, last);
  create_map_and_nodes(n);

  try {
    uninitialized_copy(first, last, start_);
  }
  catch(...) {
    destroy_map_and_nodes();
    throw;
  }
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::new_elements_at_front(size_type new_elements)
{
  size_type new_nodes = (new_elements + buffer_size() - 1) / buffer_size();
  reserve_map_at_front(new_nodes);
  size_type i;
  try {
    for (i = 1; i <= new_nodes; i++)
      *(start_.node_ - i) = allocate_node();
  }
  catch (...) {
     for (size_type j = 1; j < i; ++j)
      deallocate_node(*(start_.node_ - j));
      throw;
  }
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::new_elements_at_back(size_type new_elements)
{
  size_type new_nodes = (new_elements + buffer_size() - 1) / buffer_size();
  reserve_map_at_back(new_nodes);
  size_type i;
  try {
    for (i = 1; i <= new_nodes; i++)
      *(finish_.node_ + i) = allocate_node();
  }
  catch (...) {
     for (size_type j = 1; j < i; ++j)
      deallocate_node(*(finish_.node_ + j));
      throw;
  }
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::destroy_nodes_at_front(iterator before_start)
{
  for (map_pointer n = before_start.node_; n < start_.node_; ++n)
    deallocate_node(*n);
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::destroy_nodes_at_back(iterator after_finish)
{
  for (map_pointer n = after_finish.node_; n > finish_.node_; --n)
    deallocate_node(*n);
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add,
                                              bool add_at_front)
{
  size_type old_num_nodes = finish_.node_ - start_.node_ + 1;
  size_type new_num_nodes = old_num_nodes + 1;

  map_pointer new_nstart;
  if (map_size_ > 2 * new_num_nodes) {
    //不需要重新分配内存，只需要移动节点到中心位置
    new_nstart = map_ + (map_size_ - new_num_nodes) / 2 +
                 (add_at_front ? nodes_to_add : 0);
    if (new_nstart < start_.node_)
      copy(start_.node_, finish_.node_ + 1, new_nstart);
    else
      copy_backward(start_.node_, finish_.node_ + 1, new_nstart + old_num_nodes);
  } else {
    //需要重新申请map内存， 计算新内存大小，and 分配内存
    size_type new_map_size = map_size_ + max(map_size_, nodes_to_add) + 2;
    map_pointer new_map = map_allocator::Allocate(new_map_size);
    //设置节点起点位置
    new_nstart = new_map + (new_map_size - new_num_nodes) / 2 +
                 (add_at_front ? nodes_to_add : 0);

    //拷贝以前map到内存到new内存
    copy(start_.node_, finish_.node_ + 1, new_nstart);
    //释放原内存
    map_allocator::Deallocate(map_, map_size_);

    map_ = new_map;
    map_size_ = new_map_size;
  }

  //重新设置迭代器，start， finish
  start_.set_node(new_nstart);
  finish_.set_node(new_nstart + new_num_nodes - 1);
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::reserve_map_at_back(size_type nodes_to_add)
{
  if (map_size_ - (finish_.node_ - map_ + 1) < 1)
    //没有节点可用，需要重新分配map
    reallocate_map(nodes_to_add, false);
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::reserve_map_at_front(size_type nodes_to_add)
{
  if (start_.node_ - map_ < 1)
    //前部没有节点可用，需要重新分配map空间
    reallocate_map(nodes_to_add, true);
}


template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::push_back_aux(const T &x)
{
  value_type x_copy = x;
  reserve_map_at_back();

  *(finish_.node_ + 1) = allocate_node();
  try {
    Construct(finish_.current_, x_copy);
    finish_.set_node(finish_.node_ + 1);
    finish_.current_ = finish_.first_;
  }
  catch(...) {
    deallocate_node(*(finish_.node_ + 1));
    throw;
  }
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::push_front_aux(const T& x)
{
  value_type x_copy = x;
  reserve_map_at_front();

  *(start_.node_ - 1) = allocate_node();
  try {
    start_.set_node(start_.node_ - 1);
    start_.current_ = start_.last_ - 1;
    Construct(start_.current_, x_copy);
  }
  catch(...) {
    deallocate_node(*(start_.node_ - 1));
    throw;
  }
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::pop_back_aux()
{
  deallocate_node(finish_.first_);         //释放最后一个缓冲区
  finish_.set_node(finish_.node_ - 1);     //调整finish_到上一个节点
  finish_.current_ = finish_.last_ - 1;    //把current 设置为上一个缓冲区到最后一个元素
  Destroy(finish_.current_);              //析构最后一个元素
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::pop_front_aux()
{
  Destroy(start_.current_);           //析构第一个缓冲区的最后一个元素
  deallocate_node(start_.first_);      //释放第一个缓冲区
  start_.set_node(start_.node_ + 1);    //调整start
  start_.current_ = start_.first_;      //设置current

}



template <class T, class Alloc, size_t BufSize>
typename Deque<T, Alloc, BufSize>::iterator
Deque<T, Alloc, BufSize>::insert_aux(iterator position, const T &x)
{
  difference_type index = position - start_;    //插入点前面的个数

  if (index < size() / 2) {
    push_front(front());
    iterator front1 = start_;
    ++front1;
    iterator front2 = front1;
    ++front2;
    position = start_ + index;
    iterator pos1 = position;
    ++pos1;
    copy(front2, pos1, front1);
  } else {
    push_back(back());
    iterator back1 = finish_;
    --back1;
    iterator back2 = back1;
    --back2;
    position = start_ + index;
    copy_backward(position, back2, back1);
  }
  *position = x;
  return position;
}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::insert_aux(iterator pos, size_type n,
                                          const T& x)
{
  const difference_type elems_before = pos - start_;
  size_type length = size();
  value_type x_copy = x;

  if (elems_before < length / 2) {
   iterator new_start = reserve_elements_at_front(n);
   iterator old_start = start_;
   pos = start_ + elems_before;
   try {
     if (elems_before >= difference_type(n)) {
       iterator start_n = start_ + difference_type(n);
       uninitialized_copy(start_, start_n, new_start);
       start_ = new_start;
       copy(start_n, pos, old_start);
       fill(pos - difference_type(n), pos, x_copy);
     }
     else {
       __uninitialized_copy_fill(start_, pos, new_start, start_, x_copy);
       start_ = new_start;
       fill(old_start, pos, x_copy);
     }
   }
   catch (...) {
     destroy_nodes_at_front(new_start);
     throw;
   }
 }
 else {
   iterator new_finish = reserve_elements_at_back(n);
   iterator old_finish = finish_;
   const difference_type elems_after = difference_type(length) - elems_before;
   pos = finish_ - elems_after;
   try {
     if (elems_after > difference_type(n)) {
       iterator finish_n = finish_ - difference_type(n);
       uninitialized_copy(finish_n, finish_, finish_);
       finish_ = new_finish;
       copy_backward(pos, finish_n, old_finish);
       fill(pos, pos + difference_type(n), x_copy);
     }
     else {
       __uninitialized_fill_copy(finish_, pos + difference_type(n),
                                 x_copy,
                                 pos, finish_);
       finish_ = new_finish;
       fill(pos, old_finish, x_copy);
     }
   }
   catch (...) {
     destroy_nodes_at_back(new_finish);
     throw;
   }
 }
}

template <class T, class Alloc, size_t BufSize>
template <class ForwardIterator>
void Deque<T, Alloc, BufSize>::insert_aux(iterator pos,
                                          ForwardIterator first,
                                          ForwardIterator last,
                                          size_type n)
{
  const difference_type elems_before = pos - start_;
  size_type length = size();
  if (elems_before < length / 2) {
    iterator new_start = reserve_elements_at_front(n);
    iterator old_start = start_;
    pos = start_ + elems_before;
    try {
      if (elems_before >= difference_type(n)) {
        iterator start_n = start_ + difference_type(n);
        uninitialized_copy(start_, start_n, new_start);
        start_ = new_start;
        copy(start_n, pos, old_start);
        copy(first, last, pos - difference_type(n));
      }
      else {
        ForwardIterator mid = first;
        advance(mid, difference_type(n) - elems_before);
        __uninitialized_copy_copy(start_, pos, first, mid, new_start);
        start_ = new_start;
        copy(mid, last, old_start);
      }
    }
    catch (...) {
      destroy_nodes_at_front(new_start);
      throw;
    }
  }
  else {
    iterator new_finish = reserve_elements_at_back(n);
    iterator old_finish = finish_;
    const difference_type elems_after = difference_type(length) - elems_before;
    pos = finish_ - elems_after;
    try {
      if (elems_after > difference_type(n)) {
        iterator finish_n = finish_ - difference_type(n);
        uninitialized_copy(finish_n, finish_, finish_);
        finish_ = new_finish;
        copy_backward(pos, finish_n, old_finish);
        copy(first, last, pos);
      }
      else {
        ForwardIterator mid = first;
        advance(mid, elems_after);
        __uninitialized_copy_copy(mid, last, pos, finish_, finish_);
        finish_ = new_finish;
        copy(first, mid, pos);
      }
    }
    catch (...) {
      destroy_nodes_at_back(new_finish);
      throw;
    }
  }

}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::clear()
{
  //保留一个缓冲区
  for (map_pointer node = start_.node_ + 1; node < finish_.node_; ++node) {
    //析构缓冲区所有元素
    Destroy(*node, *node + buffer_size());
    //释放缓冲区
    data_allocator::Deallocate(*node, buffer_size());
  }

  if (start_.node_ != finish_.node_) {   //还剩下两个缓冲区，头和尾
    //析构头尾缓冲区
    Destroy(start_.current_, start_.last_);
    Destroy(finish_.first_, finish_.current_);

    //释放尾缓冲区，保留头缓冲区
    data_allocator::Deallocate(*finish_.node_, buffer_size());

  } else {  //只有一个缓冲区，保留它
    Destroy(start_.current_, finish_.current_);
  }

  finish_ = start_;

}

template <class T, class Alloc, size_t BufSize>
typename Deque<T, Alloc, BufSize>::iterator
Deque<T, Alloc, BufSize>::erase(iterator first, iterator last)
{
  if (first == start_ && last == finish_) {   //如果删除的是整个区域，直接调用clear
    clear();
    return start_;
  } else {
    difference_type clear_len = last - first; //需要清除区间的长度
    difference_type elems_befroe = first - start_;  //清除区间前面的长度

    if (elems_befroe < (size() - clear_len) / 2) {
      //前面的长度少，移动前面的长度
      copy_backward(start_, first, last);
      iterator new_start = start_ + clear_len;
      Destroy(start_, new_start);

      for (map_pointer cur = start_.node_; cur < new_start.node_; ++cur){
        deallocate_node(*cur);
      }
      start_ = new_start;
    } else {
      //后面的长度少，移动后面的元素
      copy(last, finish_, first);
      iterator new_finish = finish_ - clear_len;
      Destroy(new_finish, finish_);

      for (map_pointer cur = new_finish.node_ - 1; cur <= finish_.node; ++cur)
        deallocate_node(*cur);

      finish_ = new_finish;
    }
    return start_ + elems_befroe;
  }


}

template <class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::insert(iterator position, size_type n,
                                      const T& x)
{
  if (position.current_ == start_.current_) {
    iterator new_start = reserve_elements_at_front(n);
    uninitialized_fill(new_start, start_, x);
    start_ = new_start;
  }
  else if(position.current_ == finish_.current_){
    iterator new_finish = reserve_elements_at_back(n);
    uninitialized_fill(finish_, new_finish, x);
    finish_ = new_finish;
  }
  else {
    insert_aux(position, n, x);
  }
}

template <class T, class Alloc, size_t BufSize>
template <class Inputerator>
void Deque<T, Alloc, BufSize>::insert(iterator position,
                                      Inputerator first,
                                      Inputerator last,
                                      input_iterator_tag)
{
  copy(first, last, inserter(*this, position));
}

template <class T, class Alloc, size_t BufSize>
template <class ForwardIterator>
void Deque<T, Alloc, BufSize>::insert(iterator position,
                                      ForwardIterator first,
                                      ForwardIterator last,
                                      forward_iterator_tag)
{
  size_type n = 0;
  distance(first, last, n);
  if (position.current_ == start_.current_) {
    iterator new_start = reserve_elements_at_front(n);
    try {
      uninitialized_copy(first, last, new_start);
      start_ = new_start;
    }
    catch (...) {
      destroy_nodes_at_front(new_start);
      throw;
    }

  }
  else if (position.current_ == finish_.current_) {
    iterator new_finish = reserve_elements_at_back(n);
    try {
      uninitialized_copy(first, last, finish_);
      finish_ = new_finish;
    }
    catch (...) {
      destroy_nodes_at_back(new_finish);
      throw
    }
  }
  else {
    insert_aux(position, first, last, n);
  }
}



__GSTL_END_NAMESPACE

#endif
