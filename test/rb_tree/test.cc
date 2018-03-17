/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-03-17T13:02:31+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-17T16:33:39+08:00
 */



#include <iostream>
#include "RB_tree.h"
#include "Functional.h"
using std::cout;
using std::endl;

int main(int argc, char const *argv[])
{
	gstl::RB_tree<int, int, gstl::Identity<int>, gstl::Less<int> > itree;
	cout << itree.size() << endl;
	itree.insert_unique(10);
	itree.insert_unique(10);
	itree.insert_unique(7);
	itree.insert_unique(8);
	itree.insert_unique(15);
	itree.insert_unique(5);
	itree.insert_unique(6);
	itree.insert_unique(11);
	itree.insert_unique(13);
	itree.insert_unique(12);

	gstl::RB_tree<int, int, gstl::Identity<int>, gstl::Less<int> >::iterator
	ite1 = itree.begin();
	gstl::RB_tree<int, int, gstl::Identity<int>, gstl::Less<int> >::iterator
	ite2 = itree.end();
	gstl::RB_tree_base_iterator rbtite;

	for ( ;ite1 != ite2; ++ite1) {
		rbtite = gstl::RB_tree_base_iterator(ite1);
		cout << *ite1 << "(" << rbtite.node->color << ")";
	}
	cout << endl;

	ite1 = itree.find(10);
	if (ite1 != itree.end()) {
		itree.insert_unique(ite1, 40);
		itree.erase(ite1);
	}

	for (ite1 = itree.begin();ite1 != ite2; ++ite1) {
		rbtite = gstl::RB_tree_base_iterator(ite1);
		cout << *ite1 << "(" << rbtite.node->color << ")";
	}
	cout << endl;

	gstl::RB_tree<int, int, gstl::Identity<int>, gstl::Less<int> > itree2;


	itree2.insert_equal(20);
	itree2.insert_equal(18);
	itree2.insert_equal(20);
	itree2.insert_equal(13);
	itree2.insert_equal(21);
	itree2.insert_equal(25);
	itree2.insert_equal(30);
	itree2.insert_equal(35);

	for (auto it : itree2)
		cout << it << '\t';

	cout << endl;

	return 0;
}
