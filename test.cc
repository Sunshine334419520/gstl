/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-01-31T20:19:44+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-02-26T18:09:30+08:00
 */
#include "Vector.h"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
//using namespace simple_stl;
int main(int argc, char const *argv[]) {

  int i;
  gstl::Vector<int> iv(2, 9);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  iv.push_back(1);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  iv.push_back(2);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  iv.push_back(3);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  iv.push_back(4);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  for(i = 0; i < iv.size(); i++)
    cout << iv[i] << '  ';

  cout << endl;

  iv.push_back(5);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  for(i = 0; i < iv.size(); i++)
    cout << iv[i] << '  ';
  cout << endl;

  iv.pop_back();
  iv.pop_back();
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;
  for(i = 0; i < iv.size(); i++)
    cout << iv[i] << '  ';
  cout << endl;

  gstl::Vector<int>::iterator it = iv.begin();
  iv.erase(it + 3);

  for(it = iv.begin(); it != iv.end(); ++it)
    cout << *it << endl;

  for(auto &i : iv)
    cout << i << endl;

  it = iv.begin();
  iv.insert(it + 2, 3, 7);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  for(i = 0; i < iv.size(); i++)
    cout << iv[i] << '  ';

  cout << endl;

  iv.clear();
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;


  return 0;
}
