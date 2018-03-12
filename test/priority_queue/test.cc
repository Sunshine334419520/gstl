/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-12T11:26:04+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-12T11:35:42+08:00
 */

#include "PriorityQueue.h"
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char const *argv[]) {
  int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
  gstl::PriorityQueue<int> ipq(ia, ia + 9);
  cout << "size = " << ipq.size() << endl;

  for (int i = 0; i < ipq.size(); ++i)
    cout << ipq.top() << '\t';
  cout << endl;

  while (!ipq.empty()) {
    cout << ipq.top() << '\t';
    ipq.pop();
  }
  cout << endl;

  return 0;
}
