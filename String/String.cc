/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-28T14:17:36+08:00
 * @Email:  guang334419520@126.com
 * @Filename: String.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-04-01T10:29:59+08:00
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

void String::resize(size_type n, char c)
{
  if (n < size()) {
    erase(start_ + n, finish_);
  }
  else {
    insert(end(), n - size(), c);
  }
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
      finish_ = new_finish;
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
  return *this;
}


String& String::erase(size_type pos, size_type len)
{
  len = check_len_equal_npos(len, size(), pos);
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
  *finish_ = '\0';
  return first;
}

String& String::replace(size_type pos, size_type n, const String& str)
{
  replace(start_ + pos, start_ + pos + n, str.begin(), str.end());
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
  replace(start_ + pos, start_ + pos + n, s, s + strlen(s));
  return *this;
}

String& String::replace(iterator first, iterator last, const String& str)
{
  replace(first, last, str.begin(), str.end());
  return *this;
}
String& String::replace(iterator first, iterator last, const char* s)
{
  replace(first, last, s, s + strlen(s));
  return *this;
}



int String::compare(const String& str) const
{
  return strcmp(this->start_, str.begin());
}
int String::compare(size_type pos, size_type n, const String& str) const
{
  return compare(pos, n, str.c_str());
}

int String::compare(const char* str) const
{
  return strcmp(this->start_, str);
}
int String::compare(size_type pos, size_type n, const char* str) const
{
  for (size_type i = 0; i < n; ++i) {
    if (start_[i + pos] < str[i])
      return -1;
    else if (start_[i + pos] > str[i])
      return 1;
  }
  return str[n - 1] != 0 ? 1 : 0;
}

typename String::size_type String::copy(char* s, size_type n, size_type pos) const
{
  /*
  s = new char[n];
  const size_type len = n;
  for (size_type i = pos; i < size() && n; ++i, --n, ++s)
    *s = start_[i];
  return n ? len - n : len;
  */
 if (n > size() - pos)
  n = size() - pos;

 strncpy(s, start_ + pos, n);
 return n;
}

typename String::size_type String::find(char c, size_type pos) const
{
  iterator cur;
  for (cur = start_ + pos; cur != finish_ && *cur != c; ++cur)
    ;
  return cur == finish_ ? npos : (cur - start_);
}

typename String::size_type String::find(const char* s, size_type pos) const
{
  return find(s, pos, strlen(s));
}

typename String::size_type String::find(const char *s, size_type pos, size_type n) const
{
  for (size_type i = pos; i < size() - n; ++i) {
    for (size_type j = 0; j < n; ++j) {
      if (s[j] == start_[i]) {
        ++i;
        ++j;
        if (j == n)
          return i - n;
      }
      else
        break;
    }
  }
  return npos;
}

typename String::size_type String::find(const String& s, size_type pos) const
{
  return find(s.c_str(), pos, s.size());
}

typename String::size_type String::rfind(char c, size_type pos) const
{
  pos = check_len_equal_npos(pos, size(), 1);
  iterator cur;
  for (cur = start_ + pos; cur != finish_ && *cur != c; --cur)
    ;
  return cur == finish_ ? npos : (cur - start_);
}

typename String::size_type String::rfind(const char* s, size_type pos) const
{
  return rfind(s, pos, strlen(s));
}

typename String::size_type String::rfind(const char* s, size_type pos, size_type n) const
{
  pos = check_len_equal_npos(pos, size(), 1);
  for (size_type i = pos; i >= n - 1; --i) {
    for (size_type j = 0; j < n; ++j) {
      if (start_[i] == s[j]) {
        --i;
        ++j;
        if (j == n)
          return i + 1;
      }
      else
        break;
    }
  }
  return npos;
}

typename String::size_type String::rfind(const String& s, size_type pos) const
{
  return rfind(s.c_str(), pos, s.size());
}


typename String::size_type String::find_first_of(char c, size_type pos) const
{
  return find(c, pos);
}

typename String::size_type String::find_first_of(const char* s, size_type pos) const
{
  return find_first_of(s, pos, strlen(s));
}

typename String::size_type String::find_first_of(const char* s, size_type pos, size_type n) const
{
  const_iterator first = s;
  const_iterator last = s + n;
  for (size_type i = pos; i < size(); ++i) {
    if (is_contained(start_[i], first, last))
      return i;
  }
  return npos;
}

typename String::size_type String::find_first_of(const String& str, size_type pos) const
{
  return find_first_of(str.c_str(), pos, str.size());
}

typename String::size_type String::find_first_not_of(char c, size_type pos) const
{
  for (iterator cur = start_ + pos; cur != finish_; ++cur)
    if (*cur != c)
      return cur - start_;

  return npos;
}

typename String::size_type String::find_first_not_of(const char* s, size_type pos) const
{
  return find_first_not_of(s, pos, strlen(s));
}

typename String::size_type String::find_first_not_of(const char* s, size_type pos, size_type n) const
{
  pos = check_len_equal_npos(pos, size(), 1);
  const_iterator last = s + n;
  for (size_type i = pos; i < size(); ++i)
    if (!is_contained(start_[i], s, last))
      return i;

  return npos;
}
typename String::size_type String::find_first_not_of(const String& str, size_type pos) const
{
  return find_first_not_of(str.c_str(), pos, str.size());
}

typename String::size_type String::check_len_equal_npos(size_type n,
                                                        size_type max_len,
                                                        size_type start_len) const
{
  return n == npos ? max_len - start_len : n;
}

typename String::size_type String::find_last_of(char c, size_type pos) const
{
  pos = check_len_equal_npos(pos, size(), 1);
  for (size_type i = pos; i > 0; --i) {
    if (start_[i] == c)
      return i;
  }
  return npos;
}

typename String::size_type String::find_last_of(const char* s, size_type pos) const
{
  return find_last_of(s, pos, strlen(s));
}

typename String::size_type String::find_last_of(const char* s, size_type pos, size_type n) const
{
  pos = check_len_equal_npos(pos, size(), 1);
  const_iterator last = s + n;
  for (size_type i = pos; i > 0; --i) {
    if (is_contained(start_[i], s, last))
      return i;
  }
  return npos;
}

typename String::size_type String::find_last_of(const String& str, size_type pos) const
{
  return find_last_of(str.c_str(), pos, str.size());
}

typename String::size_type String::find_last_not_of(char c, size_type pos) const
{
  pos = check_len_equal_npos(pos, size(), 1);
  for (size_type i = pos; i > 0; --i)
    if (start_[i] != c)
      return i;
  return npos;
}

typename String::size_type String::find_last_not_of(const char* s, size_type pos) const
{
  return find_last_not_of(s, pos, strlen(s));
}

typename String::size_type String::find_last_not_of(const char* s, size_type pos, size_type n) const
{
  pos = check_len_equal_npos(pos, size(), 1);
  const_iterator last = s + n;
  for (size_type i = pos; i > 0; --i)
    if (!is_contained(start_[i], s, last))
      return i;
  return npos;
}

typename String::size_type String::find_last_not_of(const String& str, size_type pos) const
{
  return find_last_not_of(str.c_str(), pos, str.size());
}

String String::substr(size_type pos, size_type n) const
{
  n = check_len_equal_npos(n, size(), 0);
  String result;
  for (size_type i = pos, j = 0; i < n; ++i, ++j)
    result += start_[i];
  return result;
}

bool String::is_contained(char value, const_iterator first, const_iterator last) const
{
  for (auto cur = first ; cur != last; ++cur)
    if (*cur == value)
      return true;

  return false;
}

void String::MoveData(String& str)
{
  this->start_ = str.start_;
  this->finish_ = str.finish_;
  this->end_of_storage_ = str.end_of_storage_;

  str.start_ = str.finish_ = str.end_of_storage_ = nullptr;
}

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

String operator+(const String& str1, const String& str2)
{
  String result(str1);
  result += str2;
  return result;
}

String operator+(const String& str, const char* s)
{
  String result(str);
  result += s;
  return result;
}

String operator+(const char* s, const String& str)
{
  return str + s;
}

std::ostream& operator<<(std::ostream& out, const String& str)
{
  for (auto c : str)
    out << c;
  return out;
}

std::istream& operator>>(std::istream& is, String& str)
{
    str.clear();
    char ch;
    while (is.get(ch)) {
      if (ch != EOF && !isblank(ch) && ch != '\n')
        str.push_back(ch);
      else
        break;
    }

    return is;
}

std::istream& getline(std::istream& is, String& str, char separator)
{
  str.clear();
  char ch;
  while (is.get(ch) && ch != separator)
    str.push_back(ch);
  return is;
}


bool operator==(const String& str1, const String& str2)
{
  return str1.size() == str2.size() && strcmp(str1.c_str(), str2.c_str()) == 0;
}

bool operator<(const String& str1, const String& str2)
{
  return str1.size() < str2.size() && strcmp(str1.c_str(), str2.c_str()) == -1;
}

bool operator==(const String& str, const char* s)
{
  //这里并没有先比较字符串大小，因为strlen(s)时间复杂度有O(n)，这样就得不到优化，不如直接比较字符串
  return strcmp(str.c_str(), s) == 0;
}

bool operator==(const char* s, const String& str)
{
  //这里并没有先比较字符串大小，因为strlen(s)时间复杂度有O(n)，这样就得不到优化，不如直接比较字符串
  return str == s;
}

bool operator<(const String& str, const char* s)
{
  return strcmp(str.c_str(), s) == -1;
}

bool operator<(const char* s, const String& str)
{
  return strcmp(s, str.c_str()) == -1;
}

bool operator!=(const String& str, const char* s)
{
  return !(str == s);
}

bool operator!=(const char* s, const String& str)
{
  return str != s;
}

bool operator>(const String& str, const char* s)
{
  return s < str;
}

bool operator>(const char* s, const String& str)
{
  return str < s;
}

bool operator>=(const String& str, const char* s)
{
  return !(str < s);
}

bool operator>=(const char* s, const String& str)
{
  return !(s < str);
}

bool operator<=(const String& str, const char* s)
{
  return !(s < str);
}

bool operator<=(const char* s, const String& str)
{
  return !(str < s);
}

void swap(String& str1, String& str2)
{
  str1.swap(str2);
}


__GSTL_END_NAMESPACE
