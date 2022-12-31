/**
 * @file _tree.hpp
 * @author jiskim
 * @brief
 * @date 2022-12-31
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _TREE_HPP_
#define _TREE_HPP_

#include <iterator>  // std::bidirectional_iterator_tag

namespace ft {
enum color { RED = 0, BLACK };

template <typename T>
struct _rb_tree_iterator {
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;

  typedef std::bidirectional_iterator_tag iterator_category;
  typedef _rb_tree_iterator<T> self;
};

template <typename T>
class _rb_tree {
 private:
 public:
  _tree(void) {}
  ~_tree(void) {}

  void insert(const& T element) {}
};

}  // namespace ft

#endif  // _TREE_HPP_
