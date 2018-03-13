/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-13T14:17:46+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-13T15:01:45+08:00
 */

#include "Slist.h"
#include "Algorithm.h"
#include <iostream>
using namespace std;


int main(int argc, char const *argv[]) {
  gstl::Slist<int> islist;
  cout << "size = " << islist.size();

  islist.push_front(9);
  islist.push_front(1);
  islist.push_front(2);
  islist.push_front(3);
  islist.push_front(4);
  islist.push_front(5);
  cout << "size = " << islist.size();

  gstl::Slist<int>::iterator ite = islist.begin();
  gstl::Slist<int>::iterator ite2 = islist.end();
  for ( ; ite != ite2; ++ite)
    cout << *ite << '\t';
  cout << endl;

  ite = gstl::find(islist.begin(), islist.end(), 1);
  if (ite != 0)
    islist.insert(ite, 99);
  islist.insert_after(ite, 100);
  cout << "size = " << islist.size() << endl;
  cout << *ite << endl;

  for (ite = islist.begin() ; ite != ite2; ++ite)
    cout << *ite << '\t';
  cout << endl;

  ite = gstl::find(islist.begin(), islist.end(), 3);
  if (ite != 0)
    cout << *(islist.erase(ite)) << endl;

    for (ite = islist.begin() ; ite != ite2; ++ite)
      cout << *ite << '\t';
    cout << endl;

    islist.clear();
    cout << "size = " << islist.size() << endl;




  return 0;
}
