/**
 * @file tree_test.hpp
 * @author jiskim
 * @brief
 * @date 2023-01-14
 *
 * @copyright Copyright (c) 2023
 */

#ifndef TREE_TEST_HPP
#define TREE_TEST_HPP

#include <iostream>
#include <string>

#include "_rb_tree.hpp"

#define PRINT_RED "\033[0;31m"
#define PRINT_GREEN "\033[0;32m"
#define PRINT_YELLOW "\033[0;33m"
#define PRINT_BLUE "\033[0;34m"
#define PRINT_MAGENTA "\033[0;35m"
#define PRINT_CYAN "\033[0;36m"
#define PRINT_WHITE "\033[0;37m"
#define PRINT_GRAY "\033[0;38m"
#define PRINT_RESET "\033[0m"

namespace ft {

// pair operator<< overloading
template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const pair<T1, T2>& pr) {
  os << "[" << pr.first << "]";
  return os;
}

template <typename T>
void print_rb_tree(const std::string& prefix, const _rb_tree_node_base* node,
                   bool isLeft) {
  if (node != NULL) {
    typedef const _rb_tree_node<T>* link_type;
    link_type x = static_cast<link_type>(node);
    std::cout << prefix;
    if (prefix.size() > 0)
      std::cout << (!isLeft ? "├── (R)" : "└── (L)");
    else {
      std::cout << "└── (root)";
      isLeft = true;
    }

    // print the value of the node
    std::string color = PRINT_RED;
    (x->color == RED) ? color = PRINT_RED : color = PRINT_WHITE;
    std::cout << color << x->value << PRINT_RESET << std::endl;

    // enter the next tree level - left and right branch
    print_rb_tree<T>(prefix + (!isLeft ? "│   " : "    "), node->right, false);
    print_rb_tree<T>(prefix + (!isLeft ? "│   " : "    "), node->left, true);
  } else {
    std::cout << prefix;
    std::cout << (!isLeft ? "├── (R) " : "└── (L) ");
    std::cout << PRINT_WHITE << "NIL" << PRINT_RESET << std::endl;
  }
}

template <typename T>
void print_rb_tree(_rb_tree_iterator<T> it) {
  _rb_tree_node_base* header = it._node;

  if (header->parent == NULL) return;
  // root 시작
  print_rb_tree<T>("", header->parent, false);
  std::cout << "\n\n";
}
}  // namespace ft

void tree_test(void);
void map_test(void);
void set_test(void);

#endif  // TREE_TEST_HPP
