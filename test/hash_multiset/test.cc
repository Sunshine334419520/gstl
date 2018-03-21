/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-21T17:03:00+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-21T17:04:13+08:00
 */
 #include <iostream>
 #include "HashMultiSet.h"
 using namespace std;

 void
 lookup(const gstl::HashMultiSet<const char*, gstl::Hash<const char*>, gstl::EqualTo<const char*>>& Set,
        const char* word)
 {
   gstl::HashMultiSet<const char*, gstl::Hash<const char*>,
   gstl::EqualTo<const char*> >::iterator it = Set.find(word);
   /*if (it != Set.end()) {
     cout << "present";
   }*/
   cout << '\t' << word << " : " << (it != Set.end() ? "present" : "not present") << endl;
   //cout << "\t" << word < " : "
     //   << (it != Set.end() ? "present" : "not present") << endl;

 }

 int main(int argc, char const *argv[]) {
   gstl::HashMultiSet<const char*, gstl::Hash<const char*>, gstl::EqualTo<const char*> > Set;
   Set.insert("ASM");
   Set.insert("C");
   Set.insert("C++");
   Set.insert("Python");
   Set.insert("Java");
   Set.insert("Ruby");
   Set.insert("Swift");
   Set.insert("Object-C");
   Set.insert("Go");
   Set.insert("C#");

   lookup(Set, "C++");
   lookup(Set, "C");
   lookup(Set, "JavaScript");



   for (auto it = Set.begin(); it != Set.end(); ++it)
     cout << *it << endl;

   gstl::HashMultiSet<int> Set2;
   Set2.insert(59);
   Set2.insert(63);
   Set2.insert(108);
   Set2.insert(32);
   Set2.insert(99);
   Set2.insert(22);
   Set2.insert(10);

   for (auto i : Set2)
     cout << i << '\t';
   cout << endl;

   gstl::HashMultiSet<int> Set3;
   Set3.insert(3);
   Set3.insert(196);
   Set3.insert(1);
   Set3.insert(389);
   Set3.insert(194);
   Set3.insert(387);
   Set3.insert(10);

   for (auto i : Set3)
     cout << i << '\t';
   cout << endl;

   return 0;
 }
