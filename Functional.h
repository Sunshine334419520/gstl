/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-15T11:50:25+08:00
 * @Email:  guang334419520@126.com
 * @Filename: Functional.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-27T17:25:13+08:00
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
  typedef Arg1    first_argument_type;
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

/* ------------------ Adapters ----------------- */
template <class Predicate>
class UnaryNegate
  : public UnaryFunction<typename Predicate::argument_type, bool> {
public:
  explicit UnaryNegate(const Predicate& x) : pred(x) {}
  bool operator()(const typename Predicate::argument_type& x)
  {
    return !pred(x);
  }
protected:
  Predicate pred;
};

template <class Predicate>
inline UnaryNegate<Predicate> Not1(const Predicate& pred)
{
  return UnaryNegate<Predicate>(pred);
}

template <class Predicate>
class BinaryNegate
  : public BinaryFunction<typename Predicate::first_argument_type,
                          typename Predicate::sencond_argument_type,
                          bool> {
public:
  explicit BinaryNegate(const Predicate& x) : pred(x) {}
  bool operator()(const typename Predicate::first_argument_type& x,
                  const typename Predicate::first_argument_type& y)
  {
    return !pred(x, y);
  }
protected:
  Predicate pred;
};

template <class Predicate>
inline BinaryNegate<Predicate> Not2(const Predicate& pred)
{
  return BinaryNegate<Predicate>(pred);
}


template <class Operation>
class Binder1st
  : public UnaryFunction<typename Operation::sencond_argument_type,
                         typename Operation::result_type> {
public:
  Binder1st(const Operation& x,
            const typename Operation::first_argument_type& v)
    : op(x), value(v) {}

  typename Operation::result_type
  operator()(const typename Operation::first_argument_type& v) const
  {
    return op(value, v);
  }
protected:
  Operation op;
  typename Operation::first_argument_type value;
};

template <class Operation, class T>
inline Binder1st<Operation> Bind1st(const Operation& op, const T& x)
{
  typedef typename Operation::first_argument_type arg1_type;
  return Binder1st<Operation>(op, arg1_type(x));
}

template <class Operation>
class Binder2nd
  : public UnaryFunction<typename Operation::first_argument_type,
                         typename Operation::result_type> {
public:
  Binder2nd(const Operation& x,
            const typename Operation::sencond_argument_type& v)
    : op(x), value(v) {}

  typename Operation::result_type
  operator()(const typename Operation::sencond_argument_type& v) const
  {
    return op(value, v);
  }
protected:
  Operation op;
  typename Operation::sencond_argument_type value;
};

template <class Operation, class T>
inline Binder2nd<Operation> Bind2nd(const Operation& op, const T& x)
{
  typedef typename Operation::sencond_argument_type arg2_type;
  return Binder1st<Operation>(op, arg2_type(x));
}

/* -------------------------- ptr_fun --------------------------- */
template <class Arg, class Result>
class PointerUnaryFunction : public UnaryFunction<Arg, Result> {
public:
  PointerUnaryFunction() {}
  explicit PointerUnaryFunction(Result (*x)(Arg)) : ptr(x) {}

  Result operator()(Arg x) const { return ptr(x); }

protected:
  Result (*ptr)(Arg);
};

template <class Arg1, class Arg2, class Result>
class PointerBinaryFunction : public BinaryFunction<Arg1, Arg2, Result> {
public:
  PointerBinaryFunction() {}
  PointerBinaryFunction(Result (*x)(Arg1, Arg2)) : ptr(x) {}

  Result operator()(Arg1 x, Arg2 y) const { return ptr(x, y); }
protected:
  Result (*ptr)(Arg1, Arg2);
};

template <class Arg, class Result>
inline PointerUnaryFunction<Arg, Result>
PtrFun(Result (*x)(Arg))
{
  return PointerUnaryFunction<Arg, Result>(x);
}

template <class Arg1, class Arg2, class Result>
inline PointerBinaryFunction<Arg1, Arg2, Result>
PtrFun(Result (*x)(Arg1, Arg2))
{
  return PointerBinaryFunction<Arg1, Arg2, Result>(x);
}

/* -------------------------- mem_fun and men_fun_ref --------------------------- */
template <class S, class T>
class MemFunT : public UnaryFunction<T*, S> {
public:
  explicit MemFunT(S (T::*pf)()) : f(pf) {}
  S operator()(T* p) const { return (p->*f)(); }
private:
  S (T::*f)();
};

template <class S, class T>
class ConstMemFunT : public UnaryFunction<const T*, S> {
public:
  explicit ConstMemFunT(S (T::*pf)() const) : f(pf) {}
  S operator()(const T* p) const { return (p->*f)(); }
private:
  S (T::*f)() const;
};

template <class S, class T>
class RefMemFunT : public UnaryFunction<T, S> {
public:
  explicit RefMemFunT(S (T::*pf)()) : f(pf) {}
  S operator()(T& r) const { return (r.*f)(); }
private:
  S (T::*f)();
};

template <class S, class T>
class ConstRefMemFunT : public UnaryFunction<T, S> {
public:
  explicit ConstRefMemFunT(S (T::*pf)() const) : f(pf) {}
  S operator()(const T& r) const { return (r.*f)(); }
private:
  S (T::*f)() const;
};

template <class S, class T, class A>
class MemFun1T : public BinaryFunction<T*, A, S> {
public:
  explicit MemFun1T(S (T::*pf)(A)) : f(pf) {}
  S operator()(T* p, A x) const { return (p->*f)(x); }
private:
  S (T::*f)(A);
};

template <class S, class T, class A>
class ConstMemFun1T : public BinaryFunction<const T*, A, S> {
public:
  explicit ConstMemFun1T(S (T::*pf)(A) const) : f(pf) {}
  S operator()(const T* p, A x) const { return (p->*f)(x); }
private:
  S (T::*f)(A) const;
};

template <class S, class T, class A>
class RefMemFun1T : public BinaryFunction<T, A, S> {
public:
  explicit RefMemFun1T(S (T::*pf)(A)) : f(pf) {}
  S operator()(T& p, A x) const { return (p.*f)(x); }
private:
  S (T::*f)(A);
};

template <class S, class T, class A>
class ConstRefMemFun1T : public BinaryFunction<T, A, S> {
public:
  explicit ConstRefMemFun1T(S (T::*pf)(A) const) : f(pf) {}
  S operator()(T& p, A x) const { return (p.*f)(x); }
private:
  S (T::*f)(A) const;
};


template <class S, class T>
inline MemFunT<S, T> MemFun(S (T::*f)()) {
  return MemFunT<S, T>(f);
}

template <class S, class T>
inline ConstMemFunT<S, T> MemFun(S (T::*f)() const) {
  return ConstMemFunT<S, T>(f);
}

template <class S, class T>
inline RefMemFunT<S, T> MemFunRef(S (T::*f)()) {
  return RefMemFunT<S, T>(f);
}

template <class S, class T>
inline ConstRefMemFunT<S, T> MemFunRef(S (T::*f)() const) {
  return ConstRefMemFunT<S, T>(f);
}

template <class S, class A, class T>
inline MemFun1T<S, T, A> MemFun1(S (T::*f)(A)) {
  return MemFun1T<S, T, A>(f);
}

template <class S, class A, class T>
inline ConstMemFun1T<S, T, A> MemFun1(S (T::*f)(A) const) {
  return ConstMemFun1T<S, T, A>(f);
}

template <class S, class A, class T>
inline RefMemFun1T<S, T, A> MemFun1(S (T::*f)(A)) {
  return RefMemFun1T<S, T, A>(f);
}

template <class S, class A, class T>
inline ConstRefMemFun1T<S, T, A> MemFun1(S (T::*f)(A) const) {
  return ConstRefMemFun1T<S, T, A>(f);
}

__GSTL_END_NAMESPACE


#endif
