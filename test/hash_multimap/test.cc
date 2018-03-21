/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-21T17:11:11+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-21T17:28:16+08:00
 */

 #include <iostream>
 #include <string>
 #include "HashMultiMap.h"

 using namespace std;

 int main(int argc, char const *argv[]) {
   gstl::HashMultiMap<string, int, gstl::Hash<string>, gstl::EqualTo<string> > days;
   /*
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
   */
   days.insert(gstl::make_pair(string("january"), 31));
   days.insert(gstl::make_pair(string("february"), 28));
   days.insert(gstl::make_pair(string("march"), 31));
   days.insert(gstl::make_pair(string("april"), 30));
   days.insert(gstl::make_pair(string("may"), 31));
   days.insert(gstl::make_pair(string("june"), 30));
   days.insert(gstl::make_pair(string("july"), 31));
   days.insert(gstl::make_pair(string("august"), 31));
   days.insert(gstl::make_pair(string("september"), 30));
   days.insert(gstl::make_pair(string("december"), 31));


/*
   cout << "september -> " << days["september"] << endl;
   cout << "june -> " << days["june"] << endl;
   cout << "august -> " << days["august"] << endl;
   cout << "december -> " << days["december"] << endl;

*/
   for (auto i  : days)
     cout << i.first << i.sencond << endl;

   return 0;
 }
