/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-20T11:57:10+08:00
 * @Email:  guang334419520@126.com
 * @Filename: HashFun.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-20T17:27:33+08:00
 */


#ifndef GSTL_HASH_FUN_H
#define GSTL_HASH_FUN_H

#include <cstdlib>
#include <string>
#include "config.h"

__GSTL_BEGIN_NAMESPACE

template <class key> struct Hash { };

inline size_t HashString(const char* s)
{
  unsigned long h = 0;
  for ( ; *s; ++s)
    h = 5 * h + *s;
  return static_cast<size_t>(h);
}

inline size_t HashString(const std::string& s)
{
  unsigned long h = 0;
  size_t length = s.length();
  for (size_t i = 0; i < length; i++)
    h = 5 * h + s.at(i);

  return static_cast<size_t>(h);
}

template <> struct Hash<std::string>
{
  size_t operator()(const std::string& s) const { return HashString(s); }
};


template <> struct Hash<char*>
{
  size_t operator()(const char* s) const { return HashString(s); }
};
template <> struct Hash<const char*>
{
  size_t operator()(const char* s) const { return HashString(s); }
};

template <> struct Hash<char>
{
  size_t operator()(char x) const { return x; }
};
template <> struct Hash<unsigned char>
{
  size_t operator()(unsigned char x) const { return x; }
};
template <> struct Hash<signed char>
{
  size_t operator()(unsigned char x) const { return x; }
};

template <> struct Hash<short>
{
  size_t operator()(short x) const { return x; }
};
template <> struct Hash<unsigned short>
{
  size_t operator()(short x) const { return x;}
};
template <> struct Hash<int>
{
  size_t operator()(int x) const { return x; }
};
template <> struct Hash<unsigned int>
{
  size_t operator()(unsigned int x) const { return x; }
};
template <> struct Hash<long>
{
  size_t operator()(long x) const { return x; }
};
template <> struct Hash<unsigned long>
{
  size_t operator()(unsigned long x) const { return x; }
};

__GSTL_END_NAMESPACE

#endif
