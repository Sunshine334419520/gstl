/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-10T16:46:11+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-10T21:39:12+08:00
 */

#include "Deque.h"
#include "Algorithm.h"
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char const *argv[]) {
  gstl::Deque<int> ideq(20, 9);
  cout << "size = " << ideq.size() << endl;

  for (int i = 0; i < ideq.size(); i++)
    ideq[i] = i;

  for (int i = 0; i < ideq.size(); i++)
    cout << ideq[i] << '\t';

  cout << endl;

  cout << "iterator : ";
  for (gstl::Deque<int>::iterator it = ideq.begin(); it != ideq.end(); ++it)
    cout << *it << '\t';
  cout << endl;

  cout << "const_iterator : ";
  for (gstl::Deque<int>::const_iterator it = ideq.cbegin(); it != ideq.cend(); ++it)
    cout <<*it <<'\t';
  cout << endl;

  cout << "reverse_iterator : ";
  for (auto it = ideq.rcbegin(); it != ideq.rcend(); ++it)
    cout << *it << '\t';
  cout << endl;



  for (int i = 0; i < 3; i++)
  ideq.push_back(i);

  for (int i = 0; i < ideq.size(); i++)
  cout << ideq[i] << '\t';

  cout << endl;
  cout << "size = " << ideq.size() << endl;

  ideq.push_back(3);
  for (int i = 0; i < ideq.size(); i++)
  cout << ideq[i] << '\t';
  cout << endl;
  cout << "size = " << ideq.size() << endl;

  ideq.push_front(99);
  for (int i = 0; i < ideq.size(); i++)
  cout << ideq[i] << '\t';
  cout << endl;
  cout << "size = " << ideq.size() << endl;

  ideq.push_front(98);
  ideq.push_front(97);
  for (int i = 0; i < ideq.size(); i++)
    cout << ideq[i] << '\t';
  cout << endl;
  cout << "size = " << ideq.size() << endl;

  gstl::Deque<int>::iterator itr;
  itr = find(ideq.begin(), ideq.end(), 99);
  cout << *itr << endl;
  cout << *(itr.current_) << endl;


  gstl::Deque<int> ideq2(ideq);

  itr = find(ideq2.begin(), ideq2.end(), 99);
  ideq2.insert(itr, 100);
  ideq2.insert(itr, 3, 100);
  ideq2.insert(itr, ideq.begin(), ideq.end() - 5);
  cout << "ideq2 : ";
  for (auto &i : ideq2)
    cout << i << '\t';

  cout << endl;

  ideq.clear();
  cout << "ideq : ";
  for (auto &i : ideq)
    cout << i << '\t';






  return 0;
}
