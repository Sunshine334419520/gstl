/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-28T14:17:36+08:00
 * @Email:  guang334419520@126.com
 * @Filename: String.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-29T22:12:08+08:00
 */

#include "String/String.hpp"

__GSTL_BEGIN_NAMESPACE

const size_t String::npos;

String::String(const String& str)
{
  allocate_and_copy(str.begin(), str.end());
}

String::String(String&& str)
{
  MoveData(str);
}

String::String(const String& str, size_type pos, size_type len)
{
  allocate_and_copy(str.begin() + pos, str.begin() + pos + len);
}

String::String(const char* s)
{
  allocate_and_copy(s, s + strlen(s));
}

String::String(const char* s, size_type n)
{
  allocate_and_copy(s, s + n);
}

String::String(size_type n, char c)
{
  allocate_and_fill(n, c);
}

String& String::operator=(const String& str)
{
  if (&str != this) {
    if (this->size() < str.size())
      insert(this->end(), str.begin() + this->size(), str.end());
    else
      erase(this->begin() + str.size(), this->end());
  }
  return *this;
}

String& String::operator=(const char* s)
{
  clear();
  deallocate();

  allocate_and_copy(s, s + strlen(s));
  return *this;
}

String& String::operator=(String&& str)
{
  if (&str != this)
    MoveData(str);
  return *this;
}


String& String::insert(size_type pos, const String& str)
{
  insert(start_ + pos, str.begin(), str.end());
  return *this;
}

String& String::insert(size_type pos, const String& str,
                       size_type subpos, size_type n)
{
  n = check_len_equal_npos(n, str.size(), subpos);
  insert(start_ + pos, str.start_ + subpos, str.start_ + subpos + n);
  return *this;
}

String& String::insert(size_type pos, const char* str)
{
  insert(start_ + pos, str, str + strlen(str));
  return *this;
}

String& String::insert(size_type pos, const char* str, size_type n)
{
  insert(start_ + pos, str, str + n);
  return *this;
}

void String::insert(iterator it, size_type n, value_type value)
{
  size_type free_size = capacity() - size();
  if (n < free_size) {
    iterator new_finish = finish_ + n;
    try {
      copy_backward(it, finish_, new_finish);
      uninitialized_fill_n(it, n, value);
    }
    catch(...) {
      clear();
      deallocate();
    }
  }
  else {
    insert_aux_fill(it, n, value);
  }
}

typename String::iterator String::insert(iterator it, value_type value)
{
  insert(it, 1, value);
  return it;
}


String& String::append(const String& str)
{
  append(str.begin(), str.end());
  return *this;
}

String& String::append(const String& str, size_type subpos, size_type sublen)
{
  sublen = check_len_equal_npos(sublen, size(), subpos);
  return *this;
}

String& String::append(const char* s)
{
  append(s, s + strlen(s));
  return *this;
}

String& String::append(const char* s, size_type n)
{
  append(s, s + n);
  return *this;
}

String& String::append(size_type n, value_type value)
{
  insert(end(), n, value);
}


String& String::erase(size_type pos, size_type len)
{
  erase(start_ + pos, start_ + pos + len);
  return *this;
}

typename String::iterator String::erase(iterator pos)
{
  if (pos != (finish_ - 1)) {
    uninitialized_copy(pos + 1, finish_, pos);
  }
  --finish_;
  Destroy(finish_);
  return pos;
}


String& String::replace(size_type pos, size_type n, const String& str)
{
  replace(start_ + pos, start_ + pos + n, str.begin(), str.edn());
  return *this;
}

String& String::replace(size_type pos, size_type n, const String& str,
                  size_type subpos, size_type sublen)
{
    replace(start_ + pos, start_ + pos + n,
            str.begin() + subpos, str.begin() + subpos + sublen);
    return *this;
}

String& String::replace(size_type pos, size_type n, const char* s)
{
  replace(start_ + pos, start_ + pos + n, s, strlen(s));
  return *this;
}

String& String::replace(iterator first, iterator last, const String& str)
{
  replace(first, last, str.begin(), str.end());
  return *this;
}
String& String::replace(iterator first, iterator last, const char* s)
{
  replace(first, last, str, strlen(s));
}

typename String::iterator String::erase(iterator first, iterator last)
{
  if (first == last) return first;
  if (first == start_ && last == finish_) {
    clear();
    return first;
  }
  if (last != finish_) {
    iterator cur = uninitialized_copy(last, finish_, first);
    Destroy(cur, finish_);
  } else
    Destroy(first, finish_);
  finish_ -= (last - first);
  return first;
}


void String::MoveData(String& str)
{
  this->start_ = str.start_;
  this->finish_ = str.finish_;
  this->end_of_storage_ = str.end_of_storage_;

  str.start_ = str.finish_ = str.end_of_storage_ = nullptr;
}

String::size_type String::check_len_equal_npos(size_type n,
                                               size_type max_len,
                                               size_type start_len)
{ return n == npos ? max_len - start_len : n; }

void String::allocate_and_fill(size_type n, char c)
{
  start_ = data_allocator::Allocate(n);
  try {
    finish_ = uninitialized_fill_n(start_, n, c);
    end_of_storage_ = finish_;
  }
  catch(...) {
    data_allocator::Deallocate(start_, n);
    throw;
  }
}

void String::insert_aux_fill(iterator it, size_type n, value_type value)
{
  const size_type new_size = 2 * capacity() + 1;
  iterator new_finish;

  iterator new_start = data_allocator::Allocate(new_size);

  try {
    new_finish = uninitialized_copy(begin(), it, new_start);
    new_finish = uninitialized_fill_n(new_finish, n, value);
    new_finish = uninitialized_copy(it, end(), new_finish);

    Destroy(start_, finish_);
    deallocate();
  }
  catch(...) {
    data_allocator::Deallocate(new_start, new_size);
  }

  start_ = new_start;
  finish_ = new_finish;
  end_of_storage_ = start_ + new_size;
}


String& String::operator+=(const String& str)
{
  append(str.begin(), str.end());
  return *this;
}

String& String::operator+=(const char* s)
{
  append(s, s + strlen(s));
  return *this;
}

String& String::operator+=(char c)
{
  insert(end(), c);
  return *this;
}

__GSTL_END_NAMESPACE
