/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-11T12:45:42+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-11T12:55:05+08:00
 */
#include "Stack.h"
#include "Algorithm.h"
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char const *argv[]) {
  gstl::Stack<int> istack;
  istack.push(1);
  istack.push(3);
  istack.push(5);
  istack.push(7);

  cout << istack.size() << endl;
  cout << istack.top() << endl;

  istack.pop(); cout << istack.top() << endl;
  istack.pop(); cout << istack.top() << endl;
  istack.pop(); cout << istack.top() << endl;

  cout << istack.size() << endl;
  return 0;
}
