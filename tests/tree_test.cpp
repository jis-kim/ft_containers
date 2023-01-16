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
  insert_print_result(tree, value_type(3, "three"));
  insert_print_result(tree, value_type(5, "five"));
  insert_print_result(tree, value_type(4, "four"));
  insert_print_result(tree, value_type(2, "two"));
  insert_print_result(tree, value_type(1, "one"));

  std::cout << "\n\n================================tree "
               "info====================================\n\n";
  std::cout << "size : " << tree.size() << "\n\n";
  ::print_rb_tree<value_type>(tree.end());
}
