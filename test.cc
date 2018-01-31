/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-01-31T20:19:44+08:00
 * @Email:  guang334419520@126.com
 * @Filename: test.cc
 * @Last modified by:   sunshine
 * @Last modified time: 2018-01-31T20:41:16+08:00
 */
#include "allocate.h"
//using namespace simple_stl;
int main(int argc, char const *argv[]) {

  char * a = simple_stl::SimpleAlloc<char, simple_stl::alloc>::Allocate(sizeof(char));

  return 0;
}
