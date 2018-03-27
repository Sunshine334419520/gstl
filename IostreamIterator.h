/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-27T14:44:30+08:00
 * @Email:  guang334419520@126.com
 * @Filename: IostreamIterator.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-27T17:25:26+08:00
 */

#ifndef __GSTL_IOSTREAM_ITERATOR_H
#define __GSTL_IOSTREAM_ITERATOR_H
#include "config.h"
#include "Iterator.h"
#include <iostream>

__GSTL_BEGIN_NAMESPACE

template <class T, class Distance = ptrdiff_t>
class IstreamIterator;

template <class T, class Distance>
bool operator==(const IstreamIterator<T, Distance>& x,
                const IstreamIterator<T, Distance>& y);

template <class T, class Distance>
class IstreamIterator {
  friend bool operator== __GSTL_NULL_TMPL_ARGS (const IstreamIterator&,
                                           const IstreamIterator&);
private:
  void read()
  {
    end_marker = (*stream) ? true : false;
    if (end_marker) *stream >> value;
    end_marker = (*stream) ? true : false;
  }
public:
  typedef input_iterator_tag iterator_category;
  typedef T                  value_type;
  typedef const value_type*  pointer;
  typedef const value_type&  reference;
  typedef Distance           difference_type;

  IstreamIterator() : stream(&std::cin), end_marker(false) {}
  IstreamIterator(std::istream& s) : stream(&s) { read(); }

  reference operator*() const { return value; }
  pointer operator->() const { return &(operator*()); }

  IstreamIterator& operator++()
  {
    read();
    return *this;
  }
  IstreamIterator operator++(int)
  {
    IstreamIterator tmp = *this;
    read();
    return tmp;
  }
private:
  std::istream* stream;
  T value;
  bool end_marker;

};

template <class T, class Distance>
bool operator==(const IstreamIterator<T, Distance>& x,
                const IstreamIterator<T, Distance>& y)
{
  return x.end_marker == y.end_marker && x.stream == y.stream
        || x.end_marker == false && y.end_marker == false;
}

template <class T>
class OstreamIterator {
public:
  typedef output_iterator_tag iterator_category;
  typedef T                   value_type;
  typedef const value_type*   pointer;
  typedef const value_type&   reference;
  typedef ptrdiff_t           difference_type;

  OstreamIterator() : stream(&std::cout), string(nullptr) {}
  OstreamIterator(std::ostream& c) : stream(&c), string(nullptr) {}
  OstreamIterator(std::ostream& c, const char* s) : stream(&c), string(s) {}

  OstreamIterator& operator=(const value_type& value)
  {
    *stream << value;
    if (string) *stream << string;
    return *this;
  }

  OstreamIterator& operator*() { return *this; }
  OstreamIterator& operator++() { return *this; }
  OstreamIterator& operator++(int) { return *this; }
private:
  std::ostream* stream;
  const char* string;
};

__GSTL_END_NAMESPACE

#endif
