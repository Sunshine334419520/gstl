/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-15T11:50:25+08:00
 * @Email:  guang334419520@126.com
 * @Filename: Functional.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-22T12:20:37+08:00
 */

#ifndef __GSTL_FUNCTIONAL_H
#define __GSTL_FUNCTIONAL_H

#include "config.h"
#include <cstring>

__GSTL_BEGIN_NAMESPACE

template <class Arg, class Result>
struct UnaryFunction {
  typedef Arg argument_type;
  typedef Result result_type;
};

template <class Arg1, class Arg2, class Result>
struct BinaryFunction {
  typedef Arg1    frist_argument_type;
  typedef Arg2    sencond_argument_type;
  typedef Result  result_type;
};



/* ----------------------  算术类仿函数  ----------------------*/

template <class T>
struct Plus : public BinaryFunction<T, T, T> {
  T operator()(const T& x, const T& y) const { return x + y; }
};

template <class T>
struct Minus : public BinaryFunction<T, T, T> {
  T operator()(const T& x, const T& y) const { return x - y; }
};

template <class T>
struct Multiplies : public BinaryFunction<T, T, T> {
  T operator()(const T& x, const T& y) const { return x * y; }
};

template <class T>
struct Divides : public BinaryFunction<T, T, T> {
  T operator()(const T& x, const T& y) const { return x / y; }
};


template <class T>
struct Modulus : public BinaryFunction<T, T, T> {
  T operator()(const T& x, const T& y) const { return x % y; }
};

template <class T>
struct Negate : public UnaryFunction<T, T> {
  T operator()(const T& x) const { return -x; }
};


/* ----------------------  逻辑类仿函数  ----------------------*/
template <class T>
struct EqualTo : public BinaryFunction<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x == y; }
};

__GSTL_TEMPLATE_NULL struct EqualTo<char *> {
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) == 0;
  }
};

__GSTL_TEMPLATE_NULL struct EqualTo<const char *> {
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) == 0;
  }
};

template <class T>
struct NotEqualTo : public BinaryFunction<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x != y; }
};

template <class T>
struct Greater : public BinaryFunction<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x > y; }
};
template <class T>
struct Less : public BinaryFunction<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class T>
struct GreaterOrEqual : public BinaryFunction<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x >= y; }
};

template <class T>
struct LessOrEqual : public BinaryFunction<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x <= y; }
};


/* ----------------------  逻辑类仿函数  ----------------------*/
template <class T>
struct AndLogical : public BinaryFunction<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x && y; }
};

template <class T>
struct OrLogical : public BinaryFunction<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x || y; }
};

template <class T>
struct NotLogical : public UnaryFunction<T, bool> {
  bool operator()(const T& x) const { return !x; }
};


/* ---------------     ------------- */
template <class T>
struct Identity : public UnaryFunction<T, T> {
  const T& operator()(const T& x) const { return x; }
};

template <class PAIR>
struct Select1st : public UnaryFunction<PAIR, typename PAIR::first_type> {
  const typename PAIR::first_type& operator()(const PAIR& x) const {
    return x.first;
  }
};

template <class PAIR>
struct Select2st : public UnaryFunction<PAIR, typename PAIR::first_type> {
  const typename PAIR::first_type& operator()(const PAIR& x) const {
    return x.sencond;
  }
};

template <class Arg1, class Arg2>
struct Project1st : public BinaryFunction<Arg1, Arg2, Arg1> {
  Arg1 operator()(const Arg1& x, const Arg2& y) { return x; }
};

template <class T>
inline
T IdentityElement(Plus<T>)
{
  return T(0);
}

template <class T>
inline
T IdentityElement(Multiplies<T>)
{
  return T(1);
}


__GSTL_END_NAMESPACE


#endif
