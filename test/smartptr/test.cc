/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-04-01T14:55:00+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-04-01T15:09:59+08:00
 */

#include "../../Memory.h"
#include <iostream>
using namespace std;

class Point {
public:
  Point(int x = 0, int y = 0) : x_(x), y_(y) {}
public:
  int x_;
  int y_;
};

int main(int argc, char const *argv[]) {
  auto sptr = gstl::make_smart<Point>(10, 20);
  cout << sptr->x_ << sptr->y_ << endl;
  //cout << *sptr << endl;
  cout << (*sptr).x_ << (*sptr).y_ << endl;

  auto sptr2 = sptr;
  {
    auto sptr3 = sptr;
  }

  return 0;
}
