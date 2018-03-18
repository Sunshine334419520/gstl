/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-18T16:06:07+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-18T16:47:02+08:00
 */
#include "../../Map.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char const *argv[]) {
  gstl::Map<string, int> simap;
  simap[string("C")] = 1;
  simap[string("C++")] = 2;
  simap[string("Java")] = 3;
  simap[string("Python")] = 4;

  simap.insert(gstl::make_pair(string("Ruby"), 5));
  simap.insert(gstl::make_pair(string("Object-C"), 6));
  simap.insert(gstl::make_pair(string("JavaScript"), 7));

  gstl::Map<string, int>::iterator it1 = simap.begin();
  gstl::Map<string, int>::iterator it2 = simap.end();

  for( ; it1 != it2; ++it1)
    cout << it1->first << '\t' << it1->sencond << endl;

  for (auto rit = simap.rbegin(); rit != simap.rend(); ++ rit)
    cout << rit->first << '\t' << rit->sencond << endl;

  int number = simap[string("C++")];
  cout << number << endl;

  it1 = simap.find(string("Python"));
  if (it1 == it2)
    cout << "not found" << endl;
  else {
    cout << "found Python : " << it1->sencond << endl;

    simap.insert(it1, gstl::make_pair(string("Go"), 8));
    simap.erase(it1);
  }

  for (auto it : simap)
    cout << it.first << it.sencond << endl;


  return 0;
}
