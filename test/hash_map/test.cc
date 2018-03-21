/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-21T16:45:41+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-21T16:51:31+08:00
 */

#include <iostream>
#include "../../HashMap.h"

using namespace std;

int main(int argc, char const *argv[]) {
  gstl::HashMap<const char*, int, gstl::Hash<const char*>, gstl::EqualTo<const char*> > days;
  days["january"] = 31;
  days["february"] = 28;
  days["march"] = 31;
  days["april"] = 30;
  days["may"] = 31;
  days["june"] = 30;
  days["july"] = 31;
  days["august"] = 31;
  days["september"] = 30;
  days["december"] = 31;

  cout << "september -> " << days["september"] << endl;
  cout << "june -> " << days["june"] << endl;
  cout << "august -> " << days["august"] << endl;
  cout << "december -> " << days["december"] << endl;

  for (auto i  : days)
    cout << i.first << i.sencond << endl;

  return 0;
}
