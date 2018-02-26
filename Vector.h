/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-02-04T16:08:59+08:00
 * @Email:  guang334419520@126.com
 * @Filename: gvector.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-02-26T19:00:24+08:00
 */

#ifndef GSTL_VECTOR_H
#define GSTL_VECTOR_H

#include "allocate.h"
#include "Uninitialized.h"

__GSTL_BEGIN_NAMESPACE


template <class T, class Alloc = alloc>
class Vector {
public:
  typedef T                   value_type;
  typedef value_type&         reference;
  typedef value_type*         pointer;
  typedef value_type*         iterator;
  typedef size_t              size_type;
  typedef ptrdiff_t           difference_type;
  typedef const value_type&   const_reference;
  typedef const value_type*   const_pointer;
  typedef const value_type*   const_iterator;

protected:
  typedef SimpleAlloc<value_type, Alloc>  data_allocator;

public:
  Vector() : start_(nullptr), finish_(nullptr), end_of_storage_(nullptr) {}
  Vector(size_type n, const T& value) { fill_initialize(n, value); }
  Vector(int n, const T& value) { fill_initialize(n, value); }
  Vector(long n, const T& value) { fill_initialize(n, value); }
  explicit Vector(size_type n) { fill_initialize(n, value_type()); }

  Vector(const Vector& vec) {
    start_ = allocate_and_copy(vec.end() - vec.begin(), vec.begin(), vec.end());
    finish_ = start_ + (vec.end() - vec.begin());
    end_of_storage_ = finish_;
  }

  Vector(const Vector<T, Alloc>&& vec) :
    start_(nullptr), finish_(nullptr), end_of_storage_(nullptr) {
    this->start_ = vec.start_;
    this->finish = vec.finish;
    this->end_of_storage_ = vec.end_of_storage_;

    vec.start_ = nullptr;
    vec.finish = nullptr;
    vec.end_of_storage_ = nullptr;
  }

  Vector<T, Alloc>& operator=(const Vector<T, Alloc>& vec) {
    if (&vec != this) {
      Destroy(start_, finish_);
      deallocate();

      start_ = allocate_and_copy(vec.end() - vec.begin(), vec.begin(), vec.end());
      finish_ = start_ + (vec.end() - vec.begin());
      end_of_storage_ = finish_;
    }
    return *this;
  }

  Vector<T, Alloc>& operator=(const Vector<T, Alloc>&& vec) {
    if(&vec != this) {
      Destroy(start_, finish_);
      deallocate();

      this->start_ = vec.start_;
      this->finish_ = vec.finish_;
      this->end_of_storage_ = vec.end_of_storage_;

      vec.start_ = nullptr;
      vec.finish_ = nullptr;
      vec.end_of_storage_ = nullptr;
    }
  }

  ~Vector() {
    Destroy(start_, finish_);
    deallocate();
  }

public:
  //迭代器种类
  iterator begin() { return start_; }
  iterator end() { return finish_; }
  const_iterator begin() const { return start_; }
  const_iterator end() const {return finish_; }
  const_iterator cbegin() const { return start_; }
  const_iterator cend() const { return finish_; }



  size_type size() const { return static_cast<size_type>(finish_ - start_); }
  //size_type size() const { return size_type(start_ - finish_); }
  size_type max_size() const { return size_type(-1) / sizeof(T); }
  bool empty() const { return begin() == end(); }
  size_type capacity() const
  { return static_cast<size_type>(end_of_storage_ - start_); }

  reference operator[](size_type n) { return *(begin() + n); }
  const_reference operator[](size_type n) const { return *(begin() + n); }

  reference front() { return *begin(); }
  reference back() { return *(end() - 1); }
  const_reference front() const {  return *cbegin(); }
  const_reference back() const { return *(cend() - 1); }

  void push_back(const T& x) {
    if (finish_ != end_of_storage_) {
      Construct(finish_, x);
      ++finish_;
    } else {
      insert_aux(end(), x);
    }
  }

  void pop_back() {
    Destroy(finish_);
    --finish_;
  }

  void swap(Vector<T, Alloc>& v) {
    __GSTL::swap(this->start_, v.start_);
    __GSTL::swap(this->finish_, v.finish_);
    __GSTL::swap(this->end_of_storage_, v.end_of_storage_);
  }

  iterator erase(iterator first, iterator last) {
    iterator i = copy(last, finish_, first);

    Destroy(i, finish_);
    finish_ -= (last - first);
    return first;
  }

  iterator erase(iterator position) {
    //是否为最后一个元素
    if(position + 1 != end())
      copy(position + 1, finish_, position);

    --finish_;
    Destroy(finish_);
    return position;
  }

  void resize(size_type new_size, const T& x) {
    if (new_size < size())
      erase(begin() + new_size, end());
    else
      insert(end(), new_size - size(), x);

  }

  void resize(size_type new_size) {  resize(new_size, T());  }
  void clear() { erase(begin(), end()); }


  void insert(iterator position, size_type n, const T& x);
  void insert(iterator position, const T& x);
  iterator insert(iterator position) { return insert(position, T()); }






protected:
  void deallocate() {
    if (start_)
      data_allocator::Deallocate(start_, end_of_storage_ - start_);
  }

  void fill_initialize(size_type n, const T& value) {
    start_ = allocate_and_fill(n, value);
    finish_ = start_ + n;
    end_of_storage_ = finish_;
  }

  iterator allocate_and_fill(size_type n, const T& value) {
    iterator result = data_allocator::Allocate(n);
    try {
      uninitialized_fill_n(result, n, value);
      return result;

    } catch(...) {
      data_allocator::Deallocate(result, n);
    }

  }

  iterator allocate_and_copy(size_type n, iterator first, iterator last) {
    iterator result = data_allocator::Allocate(n);
    try {
      uninitialized_copy(first, last, result);
      return result;

    } catch (...) {
      data_allocator::Deallocate(result, n);
    }

  }

  void insert_aux(iterator position, const T& x);


protected:
  iterator start_;
  iterator finish_;
  iterator end_of_storage_;

};

template <class T, class Alloc>
void Vector<T, Alloc>::insert_aux(iterator position, const T& x)
{
  if (finish_ != end_of_storage_) { //有备用空间
    //有备用空格键可用，直接构造一个元素，以vector最后一个值
    Construct(finish_, *(finish_ - 1));
    ++finish_;

    T x_copy = x;
    copy_backward(position, finish_ - 2, finish_ -1);
    *position = x_copy;
  } else {    //无备用空间
    //计算新空间大小
    const size_type old_size = size();
    const size_type new_size = old_size != 0 ? old_size * 2 : 1;
    //分配空间
    iterator new_start = data_allocator::Allocate(new_size);
    iterator new_finish = new_start;

    try {
      //将原来的内容拷贝到新空间
      new_finish = uninitialized_copy(start_, position, new_start);

      //创建新元素
      Construct(new_finish, x);
      ++new_finish;
      //将剩余到部分拷贝过来
      new_finish = uninitialized_copy(position, finish_, new_finish);
    } catch (...) {
      Destroy(new_start, new_finish);
      data_allocator::Deallocate(new_start, new_size);
      throw;
    }

    //清理原来到空间
    Destroy(begin(), end());
    deallocate();

    //调整水位
    start_ = new_start;
    finish_ = new_finish;
    end_of_storage_ = start_ + new_size;

  }
}

template <class T, class Alloc>
void Vector<T, Alloc>::insert(iterator position, size_type n, const T& x)
{
    if (n != 0) { //只有n不等于0才能进行插入操作
      if ( size_type(end_of_storage_ - finish_) > n) {   //备用空间足够
        T x_copy = x;

        //插入点之后到元素
        size_type elems_after = finish_ - position;
        iterator old_finish = finish_;

        if (n < elems_after) {    //插入点之后到个数大于插入到个数
          finish_ = uninitialized_copy(finish_ - n, finish_, finish_);
          copy_backward(position, old_finish - n, old_finish);
          fill(position, position + n, x_copy);
        } else {    // 小于
          uninitialized_fill_n(finish_, n - elems_after, x_copy);
          finish_ += (n - elems_after);
          finish_ = uninitialized_copy(position, old_finish, finish_);
          fill(position, position + elems_after, x_copy);
        }

      } else {    //空间不够插入n 个元素
        //新增空间大小为原空间大小2倍
        size_type old_size = size();
        size_type new_size = old_size + max(old_size + n);

        //配置新空间
        iterator new_start = data_allocator::Allocate(new_size);
        iterator new_finish = new_start;

        try {   //拷贝旧元素到新空间
          //拷贝开始到position 到元素到新空间
          new_finish = uninitialized_copy(start_, position, new_start);

          //插入n个元素为x到值到新空间
          new_finish = uninitialized_fill_n(new_finish, n, x);
          //copy剩余到元素到新空间
          new_finish = uninitialized_copy(position, finish_, new_finish);
        } catch (...) {
          Destroy(new_start, new_finish);
          data_allocator::Deallocate(new_start, new_size);
          throw;
        }

        //清理原来到空间
        Destroy(begin(), end());
        deallocate();

        //调整水位
        start_ = new_start;
        finish_ = new_finish;
        end_of_storage_ = start_ + new_size;
      }
    }
}

template <class T, class Alloc>
void Vector<T, Alloc>::insert(iterator position, const T &x)
{
  size_type n = position - begin();
  if (finish != end_of_storage && position == end()) {
    construct(finish, x);
    ++finish;
  }
  else
    insert_aux(position, x);

  return begin() + n;
}

__GSTL_END_NAMESPACE



#endif  //END stl vector
