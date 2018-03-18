/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-18T15:25:37+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-18T15:51:57+08:00
 */

#include "Set.h"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]) {
  int ia[5] = {0, 1, 2, 3, 4};
  gstl::Set<int> iset(ia, ia + 5);

  cout << "size = " << iset.size() << endl;
  cout << "3 count = " << iset.count(3) << endl;
  iset.insert(3);
  cout << "size = " << iset.size() << endl;
  cout << "3 count = " << iset.count(3) << endl;
  iset.insert(5);
  cout << "size = " << iset.size() << endl;
  cout << "3 count = " << iset.count(3) << endl;
  iset.erase(1);
  cout << "size = " << iset.size() << endl;
  cout << "3 count = " << iset.count(3) << endl;

  gstl::Set<int>::iterator it1 = iset.begin();
  gstl::Set<int>::iterator it2 = iset.end();

  for ( ; it1 != it2; ++it1)
    cout << *it1 << '\t';
  cout << endl;

  it1 = iset.find(3);
  if (it1 != iset.end()) {
    cout << "3 found" << endl;
    iset.insert(it1, 10);
    iset.erase(it1);
  } else {
    cout << "3 not found" << endl;
  }

  for (it1 = iset.begin(); it1 != it2; ++it1)
    cout << *it1 << '\t';
  cout << endl;


  gstl::Set<int> iset2(iset);
  cout << "iset == iset2 : " <<(iset == iset2) << endl;
  cout << "iset < iset2 : " <<(iset < iset2) << endl;
  cout << "iset >= iset2 : " <<(iset >= iset2) << endl;


  return 0;
}
