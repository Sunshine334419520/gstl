/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-02-04T14:28:23+08:00
 * @Email:  guang334419520@126.com
 * @Filename: Traits.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-02-26T16:47:45+08:00
 */




 #ifndef __TYPE_TRAITS_H
 #define __TYPE_TRAITS_H

 #include "config.h"



 struct __true_type
 {
 };

 struct __false_type
  {
  };

  template <class type>
  struct __type_traits
  {
      // 不要移除这个成员
      // 它通知能自动特化__type_traits的编译器, 现在这个__type_traits template是特化的
      // 这是为了确保万一编译器使用了__type_traits而与此处无任何关联的模板时
      // 一切也能顺利运作
     typedef __true_type     this_dummy_member_must_be_first;

     // 以下条款应当被遵守, 因为编译器有可能自动生成类型的特化版本
     //   - 你可以重新安排的成员次序
     //   - 你可以移除你想移除的成员
     //   - 一定不可以修改下列成员名称, 却没有修改编译器中的相应名称
     //   - 新加入的成员被当作一般成员, 除非编译器提供特殊支持

     typedef __false_type    has_trivial_default_constructor;
     typedef __false_type    has_trivial_copy_constructor;
     typedef __false_type    has_trivial_assignment_operator;
     typedef __false_type    has_trivial_destructor;
     typedef __false_type    is_POD_type;
  };



 __STL_TEMPLATE_NULL struct __type_traits<char>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL struct __type_traits<signed char>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL struct __type_traits<unsigned char>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL struct __type_traits<short>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL struct __type_traits<unsigned short>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL struct __type_traits<int>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL struct __type_traits<unsigned int>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL struct __type_traits<long>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL struct __type_traits<unsigned long>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL struct __type_traits<float>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL struct __type_traits<double>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL struct __type_traits<long double>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };


 // 针对指针提供特化
 template <class T>
 struct __type_traits<T*>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };



 // 针对char *, signed char *, unsigned char *提供特化
 __STL_TEMPLATE_NULL
 struct __type_traits<char*>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL
 struct __type_traits<signed char*>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };

 __STL_TEMPLATE_NULL
 struct __type_traits<unsigned char*>
 {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
 };




 #endif /* __TYPE_TRAITS_H */

 // Local Variables:
 // mode:C++
 // End:
