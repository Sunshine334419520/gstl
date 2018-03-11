/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-11T11:53:52+08:00
 * @Email:  guang334419520@126.com
 * @Filename: relops.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-11T12:13:20+08:00
 */

#ifndef GSTL_RLPOS_H
#define GSTL_RLPOS_H

__GSTL_BEGIN_NAMESPACE

template <class T>
bool operator!=(const T& x, const T& y)
{
  return !(x == y);
}

template <class T>
bool operator>(const T& x, const T& y)
{
  return y < x;
}

template <class T>
bool operator>=(const T& x, const T& y)
{
  return !(x < y);
}

template <class T>
bool operator<=(const T& x, const T& y)
{
  return !(y < x);
}

__GSTL_END_NAMESPACE
#endif
