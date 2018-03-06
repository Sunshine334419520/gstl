/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-06T17:06:20+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-06T19:58:28+08:00
 */

#include "List.h"
#include "Algorithm.h"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

int main(int argc, char const *argv[]) {
  gstl::List<int> ilist;
  cout << "size = " << ilist.size() << endl;

  ilist.push_back(0);
  ilist.push_back(1);
  ilist.push_back(2);
  ilist.push_back(3);
  ilist.push_back(4);
  cout << "size = " << ilist.size() << endl;

  gstl::List<int>::iterator ite;
  gstl::List<int>::const_iterator cite;

  for (ite = ilist.begin(); ite != ilist.end(); ++ite)
    cout <<  "iterator : " << *ite << '\t';
  for (cite = ilist.cbegin(); cite != ilist.cend(); ++cite)
    cout << "const_iterator : " << *cite << '\t';

  for (auto crite = ilist.rcbegin(); crite != ilist.rcend(); ++crite)
    cout << "cosnt_reverse_iterator : " << *crite << '\t';

  cout << endl;

  ite = find(ilist.begin(), ilist.end(), 3);
  if (ite != 0)
    ilist.insert(ite, 99);
  cout << "size = " << ilist.size() << endl;
  cout << *ite << endl;

  for (ite = ilist.begin(); ite != ilist.end(); ++ite)
    cout <<  "iterator : " << *ite << '\t';

  ite = find(ilist.begin(), ilist.end(), 1);
  if (ite != 0)
    cout << *(ilist.erase(ite)) << endl;

  for (ite = ilist.begin(); ite != ilist.end(); ++ite)
  cout <<  "iterator : " << *ite << '\t';

  int iv[5] = {5, 6, 7, 8, 9};
  gstl::List<int> ilist2(iv, iv + 5);
  gstl::List<int> ilist3(ilist);



  ilist.clear();
  for (ite = ilist.begin(); ite != ilist.end(); ++ite)
    cout <<  "ilist1 iterator : " << *ite << '\t';

  cout << endl;

  auto ite3 = find(ilist3.begin(), ilist3.end(), 99);
  ilist3.splice(ite3, ilist2);
  for (auto i : ilist3)
    cout << i << '\t';
  cout << endl;
  ilist3.reverse();
  for (auto i : ilist3)
    cout << i << '\t';
  cout << endl;
  ilist3.sort();
  for (auto i : ilist3)
    cout << i << '\t';
  cout << endl;

  return 0;
}
