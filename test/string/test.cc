/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-31T16:36:06+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-04-01T10:36:37+08:00
 */

#include "String.hpp"
#include <iostream>
using namespace std;
using namespace gstl;

int main(int argc, char const *argv[]) {
  String s1 = "abcdefg";
  String s2 = "cdberre";
  String s3 = s1 + "abc";
  s1.insert(4, "fjslf");
  s2.erase(2);
  cout << s1 << '\t' << s2 << '\t' << s3 << '\t' << endl;
  cout << s1.size() << '\t' << s2.size() << '\t' << s3.size() << endl;

  cout << s1.compare("abcdefg") << endl;
  cout << s1.compare(s2) << endl;

  auto n = s1.find('c');
  if (n == String::npos)
    cout << "no c" << endl;
  else
    cout << "yes c " << n << endl;

  n = s1.find('f');
  if (n == String::npos)
    cout << "no c" << endl;
  else
    cout << "yes c " << n << endl;

  n = s1.rfind('f');
  if (n == String::npos)
    cout << "no f" << endl;
  else
    cout << "yes f" << n << endl;

     n = s2.find('r');
    if (n == String::npos)
      cout << "no c" << endl;
    else
      cout << "yes c " << n << endl;

  s3.replace(2, 3, s2);
  s1.replace(3, 5, "woainiaa");
  cout << s3 << '\t' << s1 << endl;
  cout << s2.c_str() << endl;
  cout << s1[5] << endl;
  //cout << s1.at(15) << endl;
  cout << (s1 == s2) << (s2 < s3) << endl;
  String s4;
  cin >> s4;
  cout << s4 << endl;
  cout << s1.substr(2, 6) << endl;
  char a[20];
  memset(a, 0, 20);
  s1.copy(a, 5);
  for (auto i : a)
    cout << i << '\t';
  cout << endl;
  return 0;
}
