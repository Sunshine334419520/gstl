/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-02-03T17:21:41+08:00
 * @Email:  guang334419520@126.com
 * @Filename: memory.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-04-01T15:03:33+08:00
 */

#ifndef __GSTL_MEMORY_H
#define __GSTL_MEMORY_H

#include <cstdlib>
#include <utility>
#include <cstddef>
#include "Algorithm.h"
#include "config.h"

__GSTL_BEGIN_NAMESPACE

template <class T>
struct defaule_delete {
  void operator()(T* ptr) { if(ptr) delete ptr; }
};

template <class T>
struct defaule_delete<T []> {
  void operator()(T* ptr) { if(ptr) delete[] ptr;}
};

template <class T, class Delete = defaule_delete<T> >
class unique_ptr {
public:
  typedef T element_type;
  typedef Delete delete_type;
  typedef element_type* pointer;
  typedef element_type& reference;
public:
  unique_ptr(pointer data = nullptr) : data_(data) {}
  unique_ptr(pointer data, delete_type del) : data_(data), delete_(del) {}
  unique_ptr(const unique_ptr&) = delete;
  unique_ptr& operator=(const unique_ptr&) = delete;

  unique_ptr(unique_ptr&& up) : data_(nullptr)
  {   __GSTL::swap(up.data_, this->data_); }

  unique_ptr& operator=(unique_ptr&& up)
  {
    if(&up != this) {
      clean();
      __GSTL::swap(up, *this);
    }
    return *this;
  }

  ~unique_ptr() { clean(); }

  const pointer get() const { return data_; }
  //pointer get() { return data_; }
  const delete_type& get_delete() const { return delete_; }
  //deleter_type& get_delete(){ return delete_; }
  operator bool() const { return get() != nullptr; }
  reference operator*() const { return (*data_); }
  pointer   operator->() const {return data_; }

  pointer release()
  {
			T *p = nullptr;
			__GSTL::swap(p, data_);
			return p;
	}

  void reset(pointer p = pointer())
  {
			clean();
			data_ = p;
	}
  void swap(unique_ptr& up){ __GSTL::swap(data_, up.data_); }



private:
  inline void clean()
  {
    delete_(data_);
    data_ = nullptr;
  }
private:
  pointer       data_;
  delete_type   delete_;

};

template <class T, class D>
void swap(unique_ptr<T, D>& x, unique_ptr<T, D>& y)
{
  x.swap(y);
}

template <class T1, class D1, class T2, class D2>
bool operator == (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
{
  return lhs.get() == rhs.get();
}

template <class T, class D>
bool operator == (const unique_ptr<T, D>& up, std::nullptr_t p)
{
  return up.get() == p;
}

template <class T, class D>
bool operator == (std::nullptr_t p, const unique_ptr<T, D>& up)
{
  return up.get() == p;
}

template <class T1, class D1, class T2, class D2>
bool operator != (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
{
  return !(lhs == rhs);
}

template <class T, class D>
bool operator != (const unique_ptr<T, D>& up, std::nullptr_t p)
{
  return up.get() != p;
}

template <class T, class D>
bool operator != (std::nullptr_t p, const unique_ptr<T, D>& up)
{
  return up.get() != p;
}

template <class T, class... Args>
unique_ptr<T> make_unique(Args&&... args)
{
  return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class SmartPointer, class T>
class RefCount {
  friend SmartPointer;
  RefCount(T* p) : pointer_(p), count_(1) {}
  ~RefCount() { delete pointer_; }

  T* pointer_;
  size_t count_;
};

template <class T>
class SmartPtr {
public:
  using value_type = T;
  using pointer = T*;
  using reference = T&;
public:
  SmartPtr(value_type* ptr) : ptr_(new RefCount<SmartPtr, T>(ptr)) {}
  SmartPtr(const SmartPtr& orig) : ptr_(orig.ptr_) {
    ++ptr_->count_;
  }
  SmartPtr& operator=(const SmartPtr& rhs) {
    ++rhs.ptr_->count_;
    if (--ptr_->count_ == 0)
      delete ptr_;
    ptr_ = rhs.ptr_;
    return *this;
  }

  ~SmartPtr() {
    if (--ptr_->count_ == 0)
      delete ptr_;
  }

public:
  reference operator*() const { return *ptr_->pointer_;}
  pointer operator->() const { return &(operator*()); }
private:
  RefCount<SmartPtr, T>* ptr_;
};

template <class T, class ...Args>
SmartPtr<T> make_smart(Args&&... args)
{
  return SmartPtr<T>(new T(std::forward<Args>(args)...));
}



__GSTL_END_NAMESPACE

#endif
