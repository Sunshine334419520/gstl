/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-11T16:39:40+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-11T17:00:45+08:00
 */

#include "heap.h"
#include "Vector.h"

#include <iostream>
using std::cout;
using std::endl;
using namespace gstl;

int main(int argc, char const *argv[]) {
  int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
  gstl::Vector<int> ivec(ia, ia + 9);

  make_heap(ivec.begin(), ivec.end());
  for (int i = 0; i < ivec.size(); i++)
    cout << ivec[i] << '\t';
  cout << endl;

  ivec.push_back(7);
  push_heap(ivec.begin(), ivec.end());
  for (int i = 0; i < ivec.size(); i++)
    cout << ivec[i] << '\t';
  cout << endl;

  pop_heap(ivec.begin(), ivec.end());
  ivec.pop_back();
  for (int i = 0; i < ivec.size(); i++)
    cout << ivec[i] << '\t';
  cout << endl;

  sort_heap(ivec.begin(), ivec.end());
  for (int i = 0; i < ivec.size(); i++)
    cout << ivec[i] << '\t';
  cout << endl;
  return 0;
}
