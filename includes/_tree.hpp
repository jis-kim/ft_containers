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
enum _color { RED = 0, BLACK };

/**
 * header cell 은 root 와 leftmost, rightmost 를 가리킨다. leftmost 는 begin
 * 상수 시간에 찾을 수 있도록 한다. rightmost 는 generic set algorithm을 사용할
 * 때 선형 시간복잡도를 갖도록 한다. (set_union 등)
 *
 */

struct _rb_tree_node_base {
  typedef _rb_tree_node_base* base_ptr;
  typedef const _rb_tree_node_base* const_base_ptr;

  _color color;
  base_ptr parent;
  base_ptr left;
  base_ptr right;

  static base_ptr _left_most(base_ptr x) {
    return x->left == 0 ? x : _left_most(x->left);
  }

  static const_base_ptr _left_most(const_base_ptr x) {
    return x->left == 0 ? x : _left_most(x->left);
  }

  static base_ptr _right_most(base_ptr x) {
    return x->right == 0 ? x : _right_most(x->right);
  }

  static const_base_ptr _right_most(const_base_ptr x) {
    return x->right == 0 ? x : _right_most(x->right);
  }
};

struct _rb_tree_header {
  _rb_tree_node_base header;
  size_t node_count;

  _rb_tree_header(void) {}
};

template <typename T>
struct _rb_tree_node : public _rb_tree_node_base {
  typedef T value_type;

  typedef _rb_tree_node<T>* link_type;

  value_type value;

  _rb_tree_node(const value_type& value = value_type(), link_type parent = 0,
                link_type left = 0, link_type right = 0, color color = RED)
      : value(value), parent(parent), left(left), right(right), color(color) {}
};

template <typename T>
struct _rb_tree_iterator {
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;

  typedef std::bidirectional_iterator_tag iterator_category;
  typedef std::ptrdiff_t difference_type;

  typedef _rb_tree_iterator<T> self;
  typedef _rb_tree_node_base::base_ptr base_ptr;
  typedef _rb_tree_node<T>* link_type;

 private:
  base_ptr _node;

  _rb_tree_iterator(void) {}
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
