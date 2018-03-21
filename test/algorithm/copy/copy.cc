/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-02-27T16:51:42+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-03-17T17:50:01+08:00
 */

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>


using std::cout;
using std::endl;
using std::vector;
using std::list;
using std::deque;

class C {
public:
  C() : data_(3) {}
private:
  int data_;
};

int main(int argc, char const *argv[]) {
  // test 1
  const char ccs[5] = {'a', 'b', 'c', 'd', 'e'};    //数据来源
  char ccd[5];                                      //数据去处
  std::copy(ccs, ccs + 5, ccd);
  // 调用来源
  // copy(const char*)


  // test 2
  const wchar_t cwcs[5] = {'a', 'b', 'c', 'd', 'e'};  //数据来源
  wchar_t cwcd[5];                  //数据去处
  std::copy(cwcs, cwcs + 5, cwcd);
  //调用来源
  // copy(wchar_t*)

  int ia[5] = {0, 1, 2, 3, 4};
  std::copy(ia, ia + 5, ia);
  //调用来源
  //copy()
  //__copy_dispatch(T*, T*)
  //__copy_t(__true_type)

  list<int> ilists(ia, ia + 5);             //数据来源
  list<int> ilistd(5);                    //数据去处
  std::copy(ilists.begin(), ilists.end(), ilistd.begin());
  //调用来源
  //copy()
  //__copy_dispatch()
  //__copy_d(input_iterator_tag)

  //此处会调用copy函数
  vector<int> ivecs(ia, ia + 5);        //数据来源
  vector<int> ivecd(5);               //数据去处
  std::copy(ivecs.begin(), ivecs.end(), ivecd.begin());
  //调用来源
  //cipy()
  //__copy_dispatch(T*, T*)
  //__copy_t(__true_type)

  C c[5];
  vector<C> Cys(c, c+5);      //数据来源
  //上面对调用会调用到copy函数
  vector<C> Cyd(5);         //数据去处
  std::copy(Cys.begin(), Cys.end(), Cyd.begin());
  //调用来源
  //copy()
  //__copy_dispatch(T*, T*)
  //__copy_t(__false_type)

  deque<C> Cds(c, c+5);     //数据来源
  deque<C> Cdd(5);          //数据去处
  std::copy(Cds.begin(), Cds.end(), Cdd.begin());
  //copy()
  //__copy_dispatch()
  //__copy_d(random_access_iterator_tag)

  return 0;
}
