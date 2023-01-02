/**
 * @file _rb_tree.hpp
 * @author jiskim
 * @brief
 * @date 2022-12-31
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _RB_TREE_HPP_
#define _RB_TREE_HPP_

#include <iterator>  // std::bidirectional_iterator_tag

namespace ft {
enum _rb_tree_color { RED = 0, BLACK };

/**
 * header cell 은 root 와 leftmost, rightmost 를 가리킨다. leftmost 는 begin
 * 상수 시간에 찾을 수 있도록 한다. rightmost 는 generic set algorithm을 사용할
 * 때 선형 시간복잡도를 갖도록 한다. (set_union 등)
 *
 */

/**
 * @brief base class of red-black tree node
 * _rb_tree_node 가 이를 상속받는다.
 *
 */
struct _rb_tree_node_base {
  typedef _rb_tree_node_base* base_ptr;
  typedef const _rb_tree_node_base* const_base_ptr;

  _rb_tree_color color;
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

  _rb_tree_header(void) {
    node_count = 0;
    header.color = RED;
    header.parent = 0;
    header.left = &header;
    header.right = &header;
  }
};

template <typename T>
struct _rb_tree_node : public _rb_tree_node_base {
  typedef T value_type;
  typedef _rb_tree_node<T>* link_type;

  value_type value;

  _rb_tree_node(const value_type& value = value_type(), link_type parent = 0,
                link_type left = 0, link_type right = 0,
                _rb_tree_color color = RED)
      : value(value), parent(parent), left(left), right(right), color(color) {}
};

// SECTION : red-black tree iterator
template <typename T>
struct _rb_tree_iterator {
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;

  typedef std::bidirectional_iterator_tag iterator_category;
  typedef std::ptrdiff_t difference_type;

  typedef _rb_tree_iterator<T> self;
  typedef _rb_tree_node_base::base_ptr base_ptr;
  typedef _rb_tree_node<T>* link_type;  // child of base_ptr

 private:
  base_ptr _node;

 public:
  _rb_tree_iterator(void) : _node() {}

  explicit _rb_tree_iterator(base_ptr x) : _node(x) {}

  reference operator*(void) const {
    return *static_cast<link_type>(_node)->value;
  }

  pointer operator->(void) const { static_cast<link_type>(_node)->value; }

  self& operator++(void) {
    _node = _rb_tree_increament(_node);
    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    _node = _rb_tree_increment(_node);
    return tmp;
  }

  self& operator--(void) {
    _node = _rb_tree_decrement(_node);
    return *this;
  }

  self operator--(int) {
    self tmp = *this;
    _node = _rb_tree_decrement(_node);
    return tmp;
  }

  // NOTE: STL uses friend keyword. 왜지
  bool operator==(const self& lhs, const self& rhs) const {
    return lhs._node == rhs._node;
  }

  bool operator!=(const self& lhs, const self& rhs) const {
    return !(lhs == rhs);
  }

  // SECTION : assginment operator
};

// SECTION: red-black tree
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc = std::allocator<Val>>
class _rb_tree {
 protected:
  typedef _rb_tree_node_base* base_ptr;
  typedef const _rb_tree_node_base* const_base_ptr;
  typedef _rb_tree_node<Val>* link_type;
  typedef const _rb_tree_node<Val>* const_link_type;

 public:
  _rb_tree(void) {}

  _rb_tree(const _rb_tree& src) {}

  ~_rb_tree(void) {}

  _rb_tree& operator=(const _rb_tree& src) {}

  // SECTION : iterator
};
// !SECTION: red-black tree

// SECTION : red-black tree operation

// SECTION : search operation
_rb_tree_node_base* _rb_tree_increment(_rb_tree_node_base* x) {
  if (x->right != 0) {
    return _rb_tree_subtree_min(x->right);
  } else {
    _rb_tree_node_base* xp = x->parent;
    while (x == xp->right) {
      x = xp;
      xp = xp->parent;
    }
    if (x->right != xp) x = xp;
  }
  return x;
}

_rb_tree_node_base* _rb_tree_decrement(_rb_tree_node_base* x) {
  if (x->color == RED && x->parent->parent == x) {  // header node (end node)
    x = x->right;                                   // 바로 right most return
  } else if (x->left != 0) {
    return _rb_tree_subtree_max(x->left);  // max of left subtree
  } else {
    _rb_tree_node_base* xp = x->parent;
    while (x == xp->left) {
      x = xp;
      xp = xp->parent;
    }
    x = xp;
  }
  return x;
}

/**
 * @brief subtree 를 왼쪽으로 회전
 *
 * @param x 이건 뭐지? 기준 노드의 부모
 * @param root 회전할 기준 노드
 */
void _rb_tree_rotate_left(_rb_tree_node_base* const x,
                          _rb_tree_node_base*& root) {}

void _rb_tree_rotate_right(_rb_tree_node_base* const x,
                           _rb_tree_node_base*& root) {}

void _rb_tree_recolorize(_rb_tree_node_base* x, _rb_tree_node_base* xp) {}

// SECTION : for simple BST search
_rb_tree_node_base* _rb_tree_subtree_min(_rb_tree_node_base* x) {
  while (x->left != 0) x = x->left;
  return x;
}

_rb_tree_node_base* _rb_tree_subtree_max(_rb_tree_node_base* x) {
  while (x->right != 0) x = x->right;
  return x;
}

}  // namespace ft

#endif  // _RB_TREE_HPP_
