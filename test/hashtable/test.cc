/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-20T17:05:16+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-20T17:51:38+08:00
 */

#include "HashTable.h"
#include "HashFun.h"
#include "Functional.h"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]) {
  gstl::HashTable<int,
                  int,
                  gstl::Hash<int>,
                  gstl::Identity<int>,
                  gstl::EqualTo<int> >
  iht(50, gstl::Hash<int>(), gstl::EqualTo<int>());

  cout << "size = " << iht.size() << endl;
  cout << "bucket = " << iht.bucket_count() << endl;
  cout << "max_bucket_count = " << iht.max_bucket_count() << endl;

  iht.insert_unique(59);
  iht.insert_unique(63);
  iht.insert_unique(108);
  iht.insert_equal(63);
  iht.insert_unique(2);
  iht.insert_unique(53);
  iht.insert_unique(55);
  iht.insert_equal(10);
  cout << iht.size() << endl;

  auto ite = iht.begin();
  for (int i = 0; i < iht.size(); ++i, ++ite)
    cout << *ite << '\t';
  cout << endl;

  for (int i = 0; i < iht.bucket_count(); i++) {
    int n = iht.elems_in_bucket(i);
    if (n != 0)
      cout << "bucket[" << i << "] has " << n << " elems. " << endl;
  }

  for (int i = 0; i <= 45; i++)
    iht.insert_equal(i);
  cout << "size = " << iht.size() << endl;
  cout << "bucket = " << iht.bucket_count() << endl;

  for (int i = 0; i < iht.bucket_count(); i++) {
    int n = iht.elems_in_bucket(i);
    if (n != 0)
      cout << "bucket[" << i << "] has " << n << " elems. " << endl;
  }

  ite = iht.begin();
  for (int i = 0; i < iht.size(); ++i, ++ite)
    cout << *ite << '\t';
  cout << endl;

  ite = iht.find(2);
  cout << *ite << endl;
  iht.erase(ite);
  cout << iht.count(2) << endl;
  ite = iht.begin();
  for (int i = 0; i < iht.size(); ++i, ++ite)
    cout << *ite << '\t';
  cout << endl;
  return 0;
}
