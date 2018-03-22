/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-22T12:15:33+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-22T12:56:20+08:00
 */

#include "Numeric.h"
#include "Vector.h"
#include "Functional.h"
#include <iostream>
#include <iterator>
using std::cout;
using std::endl;

int main(int argc, char const *argv[]) {
  int ia[5] = {1, 2, 3, 4, 5};
  gstl::Vector<int> iv(ia, ia + 5);
  cout << gstl::accmulate(iv.begin(), iv.end(), 0) << endl;
  cout << gstl::accmulate(iv.begin(), iv.end(),0, gstl::Minus<int>()) << endl;

  cout << gstl::inner_product(iv.begin(), iv.end(), iv.begin(), 10) << endl;
  cout << gstl::inner_product(iv.begin(), iv.end(), iv.begin(), 10, gstl::Minus<int>(), gstl::Plus<int>()) << endl;
  std::ostream_iterator<int> oite(cout, " ");
  gstl::partial_sum(iv.begin(), iv.end(), oite);
  cout << endl;
  gstl::partial_sum(iv.begin(), iv.end(), oite, gstl::Minus<int>());
  cout << endl;
  gstl::adjacent_difference(iv.begin(), iv.end(), oite);
  cout << endl;
  gstl::adjacent_difference(iv.begin(), iv.end(), oite, gstl::Plus<int>());
  cout << endl;

  cout << gstl::power(10, 3) << endl;
  cout << gstl::power(10, 3, gstl::Plus<int>()) << endl;
  int n = 3;
  gstl::iota(iv.begin(), iv.end(), n);
  for (auto i : iv)
    cout << i << '\t';
  cout << endl;
  return 0;
}
