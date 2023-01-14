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

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define GRAY "\033[0;38m"
#define RESET "\033[0m"

using namespace ft;

// REVIEW : TEST

template <typename T>
void print_rb_tree(const std::string& prefix, const _rb_tree_node_base* node,
                   bool isLeft) {
  if (node != NIL) {
    typedef _rb_tree_node<T>* link_type;
    link_type x = static_cast<link_type>(node);
    std::cout << prefix;

    std::cout << (isLeft ? "├──" : "└──");

    // print the value of the node
    std::string color = RED;
    x->color == RED ? color = RED : color = WHITE;
    std::cout << color << x->value << RESET << '\n';

    // enter the next tree level - left and right branch
    print_rb_tree(prefix + (isLeft ? "│   " : "    "), node->left, true);
    print_rb_tree(prefix + (isLeft ? "│   " : "    "), node->right, false);
  }
}

template <typename T>
void print_rb_tree(const _rb_tree_node_base& header) {
  typedef _rb_tree_node<T>* link_type;
  link_type x = static_cast<link_type>(header.parent);  // root
  print_rb_tree("", header, false);
}

#endif  // TREE_TEST_HPP_
