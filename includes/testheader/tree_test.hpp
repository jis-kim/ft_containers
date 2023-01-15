/**
 * @file tree_test.hpp
 * @author jiskim
 * @brief
 * @date 2023-01-14
 *
 * @copyright Copyright (c) 2023
 */

#ifndef TREE_TEST_HPP_
#define TREE_TEST_HPP_

#include <iostream>
#include <queue>
#include <string>

#include "_rb_tree.hpp"
#include "pair.hpp"

#define PRINT_RED "\033[0;31m"
#define PRINT_GREEN "\033[0;32m"
#define PRINT_YELLOW "\033[0;33m"
#define PRINT_BLUE "\033[0;34m"
#define PRINT_MAGENTA "\033[0;35m"
#define PRINT_CYAN "\033[0;36m"
#define PRINT_WHITE "\033[0;37m"
#define PRINT_GRAY "\033[0;38m"
#define PRINT_RESET "\033[0m"

using namespace ft;

// REVIEW : TEST

template <typename T>
void print_rb_tree(const std::string& prefix, const _rb_tree_node_base* node,
                   bool isLeft) {
  if (node != NIL) {
    typedef const _rb_tree_node<T>* link_type;
    link_type x = static_cast<link_type>(node);
    std::cout << prefix;

    std::cout << (isLeft ? "├──" : "└──");

    // print the value of the node
    std::string color = PRINT_RED;
    (x->color == RED) ? color = PRINT_RED : color = PRINT_WHITE;
    std::cout << color << x->value << PRINT_RESET << std::endl;

    // enter the next tree level - left and right branch
    ::print_rb_tree<T>(prefix + (isLeft ? "│   " : "    "), node->left, true);
    ::print_rb_tree<T>(prefix + (isLeft ? "│   " : "    "), node->right, false);
  }
  // else {
  //   std::cout << prefix;
  //   std::cout << (isLeft ? "├──" : "└──");
  //   std::cout << PRINT_WHITE << "NIL" << PRINT_RESET << std::endl;
  // }
}

template <typename T>
void print_rb_tree(_rb_tree_iterator<T> root) {
  ::print_rb_tree<T>("", root._node, false);
}

void tree_test(void);

#endif  // TREE_TEST_HPP
