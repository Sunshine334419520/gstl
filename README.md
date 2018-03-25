gstl
=======
利用C++11实现的一个简易版的stl

目的：掌握stl，练习数据结构与算法和C++ Template编程

编译环境：GCC4.0 以上，目前只支持gcc）

### 开发计划：
  * STL的几大基本组件，如string、vector、list、deque、set、map、unordered_\*等
  * STL算法库的大部分算法

### 完成进度：
* STL的几大基本组件
    * type traits :           100%  
    * allocate :              100%
    * iterator :              100%
    * reverse_iterator :      100%
    * vector :                100%
    * string :                0%
    * PriorityQueue :        100%
    * stack :                 100%
    * deque :                 100%
    * queue :                 100%
    * pair :                  100%
    * list：                   100%
    * slist:                  100%
    * rb_tree:                100%
    * set:                    100%
    * multimset:              100%
    * map:                    100%
    * multimmap:              100%
    * hashtable:              100%
    * hashset :               100%
    * hashmap :               100%
* STL Algorithms:  
    * STL Numeric :
        * accmulate :     100%
        * adjacent_difference :       100%
        * inner_product :       100%
        * partial_sum :         100%
        * power :       100%
        * iota :    100%
    * fill :                  100%
    * fill_n :                100%
    * find :                  100%
    * copy :                  100%
    * swap :                  100%
    * iter_swap :             100%
    * swap_ranges :           100%
    * min、max :               100%
    * equal :                 100%
    * make_heap :             100%
    * pop_heap :              100%
    * push_heap :             100%
    * sort_heap :             100%
    * all_of :                0%
    * any_of :                0%
    * none_of :               0%
    * find_if :               100%
    * find_end :              100%
    * find_first_of           100%
    * adjacent_find :         100%
    * count :                 100%
    * count_if :              100%
    * mismatch :              100%
	  * is_permutation :        0%
	  * search :                100%
    * search_n :              100%
    * binary_search :         0%
	  * advance :               100%
    * distance :              100%
    * lower_bound             100%
    * upper_bound             100%
	  * sort :                  0%
	  * generate :              100%
	  * generate_n :            100%
    * includes :              100%
    * for_each :              100%
    * generate :              100%
    * generate_n :            100%
    * max_element :           100%
    * min_element :           100%
    * merge :                 100%
    * partition :             100%
    * remove :                100%
    * remove_copy :           100%
    * remove_if :             100%
    * remove_copy_if :        100%
    * replace :               100%
    * replace_copy :               100%
    * replace_if :            100%
    * replace_copy_if :       100%
    * reverse :               100%
    * reverse_copy :          100%
    * transform :             100%
    * unique :                100%
    * unique_copy :           100%
    * next_permutation :      100%
    * prev_permutation :      100%
    * random_shuffle :        100%




* STL Functor :
    * 算术类 :        100%
    * 关系运算类 :     100%
    * 逻辑运算类 :     100%
    * Identity :     100%
    * Select :       100%
    * Project :      100%

* STL Adapters :
    * reverse_iterator :      100%
    * istream_iterator :       0%
    * ostream_iterator :      0%
    * insert_iterator :       0%
    * back_insert_iterator :  0%
    * front_insert_iterator :   0%
    * not1 :      100%
    * not2 :      100%
    * bind1st :   100%
    * bind2nd :   100%
    * compose1  :   0%
    * compose2 :  0%
    * ptr_fun     0%

* 其他组件：
    * hash_fun :                  100%
    * circular_buffer :           0%   
    * bitmap :                    0%
    * binary_search_tree :        0%
    * avl_tree :                  0%
	* suffix_array :                0%
	* directed_graph :              0%
	* trie tree :                   0%
	* Disjoint-set data structure : 0%

##TinySTL单元测试(原单元测试代码逐步)：

  * vector：                       100%
  * string：                       0%
  * pair：                         100%
  * algorithm：                    20%
  * priority_queue：               100%
  * suffix_array：                 0%
  * queue：                        100%
  * stack：                        100%
  * bitmap：                       0%
  * circular_buffer：              0%
  * deque：                        100%
  * list：                         100%
  * slist:                          100%
  * rb_tree:                100%
  * set:                    100%
  * multimset:              100%
  * map:                    100%
  * multimmap:              100%
  * hashtable:              100%
  * hashset :               100%
  * hashmap :               100%
  * avl_tree：                     0%
  * unordered_set：                0%
  * directed_graph：               0%
  * trie tree：                    0%
  * unique_ptr：                   0%
  * shared_ptr：                   0%
  * Disjoint-set data structure：100%
