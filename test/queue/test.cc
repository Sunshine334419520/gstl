/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-11T12:55:49+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-11T13:23:57+08:00
 */

#include "Queue.h"
#include "Algorithm.h"
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char const *argv[]) {
  gstl::Queue<int>  iqueue;
  iqueue.push(1);
  iqueue.push(3);
  iqueue.push(5);
  iqueue.push(7);
  iqueue.push(9);

  cout << iqueue.size() << endl;
  cout << iqueue.front() << endl;
  cout << iqueue.back() << endl;

  iqueue.pop(); cout << iqueue.front() << endl;
  iqueue.pop(); cout << iqueue.front() << endl;
  iqueue.pop(); cout << iqueue.front() << endl;
  iqueue.pop(); cout << iqueue.front() << endl;

  cout << iqueue.size() << endl;

  gstl::Queue<int> iqueue2;
  iqueue.push(5);

  if (iqueue == iqueue2) cout << "true" << endl;
  else cout << "false" << endl;

  return 0;
}
