/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-14T17:29:02+08:00
 * @Email:  guang334419520@126.com
 * @Filename: Pair.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-21T17:41:13+08:00
 */

#ifndef __GSTL_PAIR_H
#define __GSTL_PAIR_H

#include "Relops.h"

__GSTL_BEGIN_NAMESPACE

template <class T1, class T2>
struct Pair {
  typedef T1 first_type;
  typedef T2 sencond_type;

  T1 first;
  T2 sencond;

  Pair() : first(), sencond() {}
  Pair(const first_type& x, const sencond_type& y) : first(x), sencond(y) {}

  template <class U1, class U2>
  Pair(const Pair<U1, U2>& x) : first(x.first),sencond(x.sencond) {}

  Pair<T1, T2>& operator=(const Pair<T1, T2>& x) {
    if (x != &this) {
      first = x.first;
      sencond = x.sencond;
    }
    return *this;
  }

};

template <class T1, class T2>
bool operator==(const Pair<T1, T2>& x, const Pair<T1, T2>& y)
{
  return x.first == y.first && x.sencond == y.sencond;
}

template <class T1, class T2>
bool operator<(const Pair<T1, T2>& x, const Pair<T1, T2>& y)
{
  return x.first < y.first || (!(y.first < x.first) && x.sencond < y.sencond);
}

template <class T1, class T2>
Pair<T1, T2> make_pair(const T1& x, const T2& y)
{
  return Pair<T1, T2>(x, y);
}


__GSTL_END_NAMESPACE


#endif
