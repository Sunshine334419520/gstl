/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-28T14:08:15+08:00
 * @Email:  guang334419520@126.com
 * @Filename: String.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-31T12:39:42+08:00
 */

#ifndef __GSTL_STRING_H
#define __GSTL_STRING_H

#include "config.h"
#include "Alloc.h"
#include "Construct.h"
#include "ReverseIterator.h"
#include "Algorithm.h"
#include "Uninitialized.h"
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <stdexcept>


__GSTL_BEGIN_NAMESPACE

class String {
public:
  typedef char                      value_type;
  typedef value_type*               pointer;
  typedef value_type*               iterator;
  typedef value_type&               reference;
  typedef const value_type*         const_pointer;
  typedef const value_type*         const_iterator;
  typedef const value_type&         const_reference;
  typedef size_t                    size_type;
  typedef ptrdiff_t                 difference_type;

  typedef reverse_iterator<const_iterator> const_reverse_iterator;
  typedef reverse_iterator<iterator> reverse_iterator;

private:
  typedef SimpleAlloc<value_type, alloc> data_allocator;

public:
  String() : start_(nullptr), finish_(nullptr), end_of_storage_(nullptr) {}
  String(const String& str);
  String(String&& str);
  String(const String& str, size_type pos, size_type len = npos);
  String(const char* s);
  String(const char* s, size_type n);
  String(size_t n, char c);
  template <class InputIterator>
  String(InputIterator first, InputIterator last);

  String& operator=(const String& str);
  String& operator=(const char* s);
  String& operator=(String&&);

  ~String()
  {
    clear();
    deallocate();
  }
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

  bool empty() const { return start_ == finish_; }
  size_type size() const { return finish_ - start_; }
  size_type length() const { return size(); }
  size_type max_size() const { (size_type)(-1); }
  size_type capacity() const { return end_of_storage_ - start_; }
  void resize(size_type n, char c);
  void resize(size_type n) { resize(n, value_type()); }

  reference operator[](size_type n) { return *(start_ + n); }
  const_reference operator[](size_type n) const { return *(start_ + n); }
  reference at(size_type n)
  {
    if (n < size()) {
      return *(start_ + n);
    }
    else {
        throw std::out_of_range("the memory of crossing the line");
    }
  }

  const_reference at(size_type n) const
  {
    if (n < size()) {
      return *(start_ + n);
    }
    else {
        throw std::out_of_range("the memory of crossing the line");
    }
  }

  const char* c_str() const { return start_; }
  reference back() { return *(finish_ - 1); }
  const_reference back() const { return *(finish_ - 1); }
  reference front() { return *(start_); }
  const_reference front() const { return *(start_); }

  size_type copy(char* s, size_type n, size_type pos = 0) const;

  void push_back(char c) { insert(end(), c); }

  String& insert(size_type pos, const String& str);
  String& insert(size_type pos, const String& str, size_type subpos, size_type n = npos);
  String& insert(size_type pos, const char* str);
  String& insert(size_type pos, const char* str, size_type n);

  iterator insert(iterator it, value_type value);
  void insert(iterator it, size_type n, value_type value);
  template <class InputIterator>
  void insert(iterator it, InputIterator first, InputIterator last);

  String& append(const String& str);
  String& append(const String& str, size_type subpos, size_type sublen);
  String& append(const char* s);
  String& append(const char* s, size_type n);
  String& append(size_type n, value_type value);
  template <class InputIterator>
  void append(InputIterator first, InputIterator last);

  String& operator+=(const String& str);
  String& operator+=(const char* s);
  String& operator+=(char c);

  void pop_back() { erase(end() - 1); }
  String& erase(size_type pos = 0, size_type len = npos);
  iterator erase(iterator pos);
  iterator erase(iterator first, iterator last);

  String& replace(size_type pos, size_type n, const String& str);
  String& replace(size_type pos, size_type n, const String& str,
                    size_type subpos, size_type sublen);
  String& replace(size_type pos, size_type n, const char* s);
  String& replace(iterator first, iterator last, const String& str);
  String& replace(iterator first, iterator last, const char* s);
  template <class InputIterator>
  String& replace(iterator first1, iterator last1,
                  InputIterator first2, InputIterator last2);

  int compare(const String& str) const;
  int compare(size_type pos, size_type n, const String& str) const;
  int compare(const char* str) const;
  int compare(size_type pos, size_type n, const char* str) const;

  void swap(String& str)
  {
    __GSTL::swap(start_, str.start_);
    __GSTL::swap(finish_, str.finish_);
    __GSTL::swap(end_of_storage_, str.end_of_storage_);
  }


  size_type find(char c, size_type pos = 0) const;
  size_type find(const char* s, size_type pos = 0) const;
  size_type find(const char *s, size_type pos, size_type n) const;
  size_type find(const String& s, size_type pos = 0) const;

  size_type rfind(char c, size_type pos = npos) const;
  size_type rfind(const char* s, size_type pos = npos) const;
  size_type rfind(const char* s, size_type pos, size_type n) const;
  size_type rfind(const String& s, size_type pos = npos) const;

  size_type find_first_of(char c, size_type pos = 0) const;
  size_type find_first_of(const char* s, size_type pos = 0) const;
  size_type find_first_of(const char* s, size_type pos, size_type n) const;
  size_type find_first_of(const String& str, size_type pos = 0) const;

  size_type find_first_not_of(char c, size_type pos = 0) const;
  size_type find_first_not_of(const char* s, size_type pos = 0) const;
  size_type find_first_not_of(const char* s, size_type pos, size_type n) const;
  size_type find_first_not_of(const String& str, size_type pos = 0) const;

  size_type find_last_of(char c, size_type pos = npos) const;
  size_type find_last_of(const char* s, size_type pos = npos) const;
  size_type find_last_of(const char* s, size_type pos, size_type n) const;
  size_type find_last_of(const String& str, size_type pos = npos) const;

  size_type find_last_not_of(char c, size_type pos = npos) const;
  size_type find_last_not_of(const char* s, size_type pos = npos) const;
  size_type find_last_not_of(const char* s, size_type pos, size_type n) const;
  size_type find_last_not_of(const String& str, size_type pos = npos) const;

  String substr(size_type pos, size_type n = npos) const;

  void clear()
  {
    Destroy(start_, finish_);
    start_ = finish_ = end_of_storage_ = nullptr;
  }

private:
  void deallocate()
  {
    if (start_)
      data_allocator::Deallocate(start_, end_of_storage_ - start_);
  }

  void allocate_and_fill(size_type n, value_type c);
	template<class InputIterator>
	void allocate_and_copy(InputIterator first, InputIterator last);

  template <class InputIterator>
  void insert_aux_copy(iterator it, InputIterator first, InputIterator last);
  void insert_aux_fill(iterator it, size_type n, value_type value);
  void string_aux(size_t n, char c, std::true_type);
	//template<class InputIterator>
	//void string_aux(InputIterator first, InputIterator last, std::false_type);
  template <class InputIterator>
  void append_aux(InputIterator first, InputIterator last);
  void MoveData(String& s);
  size_type check_len_equal_npos(size_type n, size_type max_len, size_type start_len) const;
  bool is_contained(value_type value, const_iterator first, const_iterator last) const;
public:
  static const size_t npos = -1;
private:
  char* start_;
  char* finish_;
  char* end_of_storage_;
};

template <class InputIterator>
String::String(InputIterator first, InputIterator last)
{
  allocate_and_copy(first, last);
}

template <class InputIterator>
void String::insert(iterator it, InputIterator first, InputIterator last)
{
  const size_type free_size = (capacity() - size());
  if (free_size >= (last - first)) {
    iterator new_finish = finish_ + (last - first);
    try {
      copy_backward(it, finish_, new_finish);
      uninitialized_copy(first, last, it);
    }
    catch(...) {
      Destroy(start_, finish_);
      data_allocator::Deallocate(start_, capacity());
      throw ;
    }

    if (it == begin())
      start_ = start_ - (last - first);
    finish_ = new_finish;
  }
  else {
    insert_aux_copy(it, first, last);
  }
}


template <class InputIterator>
void String::allocate_and_copy(InputIterator first, InputIterator last)
{
  start_ = data_allocator::Allocate(last - first);
  try {
    finish_ = uninitialized_copy(first, last, start_);
    end_of_storage_ = finish_;
  }
  catch(...) {
    data_allocator::Deallocate(start_, last - first);
    throw;
  }
}

template <class InputIterator>
void String::insert_aux_copy(iterator it, InputIterator first, InputIterator last)
{
  size_type new_size = max(2 * capacity() + 1, size() + (last - first));

  iterator new_start = data_allocator::Allocate(new_size);
  iterator new_finish;
  try {
    new_finish = uninitialized_copy(start_, it, new_start);
    new_finish = uninitialized_copy(first, last, new_finish);
    new_finish = uninitialized_copy(it, finish_, new_finish);

    Destroy(start_, finish_);
    data_allocator::Deallocate(start_, capacity());
  }
  catch(...) {
    data_allocator::Deallocate(new_start, new_size);
    throw;
  }

  start_ = new_start;
  finish_ = new_finish;
  end_of_storage_ = start_ + new_size;
}

template <class InputIterator>
void String::append(InputIterator first, InputIterator last)
{
  const size_type free_size = capacity() - size();
  if (free_size >= (last - first)) {
    try {
      uninitialized_copy(first, last, finish_);
    }
    catch(...) {
      clear();
      deallocate();
      throw;
    }

  }
  else {
    append_aux(first, last);
  }
}

template <class InputIterator>
void String::append_aux(InputIterator first, InputIterator last)
{
  const size_type new_size = max(2 * capacity() + 1, size() + (last - first));

  iterator new_start = data_allocator::Allocate(new_size);
  iterator new_finish;

  try {
    new_finish = uninitialized_copy(start_, finish_, new_start);
    new_finish = uninitialized_copy(first, last, new_finish);

    clear();
    deallocate();
  }
  catch(...) {
    Destroy(new_start, new_finish);
    data_allocator::Deallocate(new_start, new_size);
    throw;
  }

  start_ = new_start;
  finish_ = new_finish;
  end_of_storage_ = start_ + new_size;

}

template <class InputIterator>
String& String::replace(iterator first1, iterator last1,
                        InputIterator first2, InputIterator last2)
{
  const size_type n1 = last1 - first1;
  const size_type n2 = last2 - first2;

  if (n1 > n2) {
    try {
      iterator cur = uninitialized_copy(first2, last2, first1);
      cur = uninitialized_copy(last1, finish_, cur);
      Destroy(cur, finish_);
      finish_ = cur;
    }
    catch(...) {
      clear();
      deallocate();
      throw;
    }
  }
  else if (n2 > n1){
    //const difference_type n = n2 - n1;
    try {
      uninitialized_copy(first2, first2 + n1, first1);
      insert(last1, first2 + n1, last2);
    }
    catch(...) {
      clear();
      deallocate();
      throw;
    }

  }
  else
    uninitialized_copy(first2, last2, first1);

  return *this;
}

/*
template<class InputIterator>
void string_aux(InputIterator first, InputIterator last, std::false_type)
{

}
*/
__GSTL_END_NAMESPACE

#endif
