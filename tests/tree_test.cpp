/**
 * @file tree_test.cpp
 * @author jiskim
 * @brief
 * @date 2023-01-15
 *
 * @copyright Copyright (c) 2023
 */

#include "testheader/tree_test.hpp"

#include <algorithm>
#include <iostream>

#include "_rb_tree.hpp"
#include "function.hpp"

using namespace ft;

typedef _rb_tree<int, pair<int, std::string>,
                 _SelectKey<pair<int, std::string> >, std::less<int> >
    tree_type;

typedef tree_type::value_type value_type;
typedef pair<tree_type::iterator, bool> result_type;

tree_type::iterator insert_print_result(tree_type& tree,
                                        const value_type& value) {
  result_type result = tree.insert(value);
  std::cout << std::boolalpha;
  std::cout << "insert " << *(result.first) << " : " << result.second << '\n';
  return result.first;
}

void tree_test(void) {
  result_type result;

  tree_type tree;
  tree_type::iterator it;
  std::cout << "size : " << tree.size() << '\n';
  insert_print_result(tree, value_type(1, "one"));
  insert_print_result(tree, value_type(2, "two"));
  insert_print_result(tree, value_type(3, "three"));
  insert_print_result(tree, value_type(4, "four"));
  insert_print_result(tree, value_type(5, "five"));

  std::cout << "\n\n================================ < tree "
               "info > ================================\n\n";
  std::cout << "size : " << tree.size() << "\n\n";
  ::print_rb_tree(tree.end());
  std::cout
      << "\n\n============================================================="
         "==================\n\n";

  tree_type tree2;
  insert_print_result(tree2, value_type(20, "a"));
  ::print_rb_tree(tree2.end());
  insert_print_result(tree2, value_type(10, "a"));
  ::print_rb_tree(tree2.end());
  insert_print_result(tree2, value_type(50, "a"));
  ::print_rb_tree(tree2.end());
  insert_print_result(tree2, value_type(30, "a"));
  ::print_rb_tree(tree2.end());
  insert_print_result(tree2, value_type(80, "a"));
  ::print_rb_tree(tree2.end());
  insert_print_result(tree2, value_type(40, "a"));
  ::print_rb_tree(tree2.end());
  insert_print_result(tree2, value_type(35, "a"));
  ::print_rb_tree(tree2.end());
  insert_print_result(tree2, value_type(25, "a"));
  ::print_rb_tree(tree2.end());

  tree.clear();

  tree2.clear();

  std::cout << "\n\n ======= tree delete test ======== \n\n";
  insert_print_result(tree2, value_type(35, "a"));
  insert_print_result(tree2, value_type(20, "a"));
  insert_print_result(tree2, value_type(50, "a"));
  insert_print_result(tree2, value_type(10, "a"));
  insert_print_result(tree2, value_type(30, "a"));
  insert_print_result(tree2, value_type(80, "a"));
  insert_print_result(tree2, value_type(40, "a"));
  insert_print_result(tree2, value_type(5, "a"));
  insert_print_result(tree2, value_type(15, "a"));
  insert_print_result(tree2, value_type(25, "a"));
  insert_print_result(tree2, value_type(35, "a"));
  insert_print_result(tree2, value_type(45, "a"));
  insert_print_result(tree2, value_type(37, "a"));
  insert_print_result(tree2, value_type(2, "a"));
  insert_print_result(tree2, value_type(27, "a"));

  std::cout << "\n\n================================ < tree "
               "info > ================================\n\n";
  ::print_rb_tree(tree2.end());

  tree2.erase(20);
  ::print_rb_tree(tree2.end());

  tree2.erase(++tree2.begin());
  ::print_rb_tree(tree2.end());
}
