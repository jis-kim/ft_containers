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

#include "reverse_iterator.hpp"
#define NIL NULL

namespace ft {
enum _rb_tree_color { RED = 0, BLACK };

// SECTION : red-black tree iterator
template <typename T>
struct _rb_tree_iterator {
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;

  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

  typedef _rb_tree_iterator<T> self;
  typedef _rb_tree_node_base::base_ptr base_ptr;
  typedef _rb_tree_node<T>* link_type;

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

  // NOTE: STL uses friend keyword.
  bool operator==(const self& lhs, const self& rhs) const {
    return lhs._node == rhs._node;  // private
  }

  bool operator!=(const self& lhs, const self& rhs) const {
    return !(lhs == rhs);
  }

  // SECTION : assginment operator
};  // !SECTION: red-black tree iterator

/**
 * @brief base class of red-black tree node
 * _rb_tree_node 가 이를 상속받는다.
 *
 */
struct _rb_tree_node_base {
  // SECTION : typedef
  typedef _rb_tree_node_base* base_ptr;
  typedef const _rb_tree_node_base* const_base_ptr;

  // SECTION : member
  _rb_tree_color color;
  base_ptr parent;
  base_ptr left;
  base_ptr right;

  // SECTION : search - subtree 에서의 left, right most 를 찾는다.
  static base_ptr _left_most(base_ptr x) {
    while (x->left != NIL) x = x->left;
    return x;
  }

  static const_base_ptr _left_most(const_base_ptr x) {
    while (x->left != NIL) x = x->left;
    return x;
  }

  static base_ptr _right_most(base_ptr x) {
    while (x->right != NIL) x = x->right;
    return x;
  }

  static const_base_ptr _right_most(const_base_ptr x) {
    while (x->right != NIL) x = x->right;
    return x;
  }
};

template <typename Compare>
struct _rb_tree_key_compare {
  Compare _compare;

  _rb_tree_key_compare(void) : compare() {}
  _rb_tree_key_compare(const Compare& comp) : compare(comp) {}
};

// 트리 정보를  저장하는  header cell
struct _rb_tree_header {
  _rb_tree_node_base _header;
  size_t _node_count;

  /**
   * @brief Construct a new rb tree header object
   * 초기 상태 : root 는 NIL, leftmost, rightmost 는 header 자신 (end)
   * 삽입 : left-most, right-most 를 갱신
   *
   */
  _rb_tree_header(void) {
    _node_count = 0;
    _header.color = RED;
    _header.parent = NIL;
    _header.left = &_header;  // use _header as end
    _header.right = &_header;
  }
};

template <typename Val>
struct _rb_tree_node : public _rb_tree_node_base {
  typedef _rb_tree_node<Val>* link_type;
  typedef Val value_type;

  value_type value;

  _rb_tree_node(const value_type& value = value_type(), link_type parent = 0,
                link_type left = 0, link_type right = 0,
                _rb_tree_color color = RED)
      : value(value), parent(parent), left(left), right(right), color(color) {}

  void change_color(void) { color = (color == RED) ? BLACK : RED; }
};

/**
 * @brief 실질적으로 데이터를 가지고 있다. _compare, _header 보유
 *
 * @tparam Compare
 */
template <typename Compare>
struct _rb_tree_impl : public _rb_tree_header, _rb_tree_key_compare<Compare> {
  typedef _rb_tree_key_compare<Compare> _base_key_compare;
  _rb_tree_impl(void) {}
  _rb_tree_impl(const _rb_tree_impl& src)
      : _rb_tree_header(), _base_key_compare(src._compare) {}
  explicit _rb_tree_impl(const Compare& comp) : _base_key_compare(comp) {}
};

// SECTION: red-black tree
// Compare 로 노드 간에 비교해야 함
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc = std::allocator<Val>>
class _rb_tree {
 public:
  typedef Key key_type;
  typedef Val value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef _rb_tree_iterator<value_type> iterator;
  typedef _rb_tree_iterator<const value_type> const_iterator;
  typedef reverse_iterator<iterator> reverse_iterator;
  typedef reverse_iterator<const_iterator> const_reverse_iterator;

  // rebind for nodes
  typedef typename Alloc::template rebind<value_type>::other node_allocator;

  // SECTION : member
 private:
  _rb_tree_impl _impl;
  node_allocator _alloc;

 public:
  _rb_tree(void) {}
  _rb_tree(const _rb_tree& src) : _rb_tree_impl(src._impl) {}
  ~_rb_tree(void) {}

  _rb_tree& operator=(const _rb_tree& src) {}

  // SECTION : red-black tree operation
  iterator begin(void) { return iterator(_impl._header.left); }
  iterator end(void) { return iterator(_impl._header.right); }
  reverse_iterator rbegin(void) { return reverse_iterator(end()); }
  reverse_iterator rend(void) { return reverse_iterator(begin()); }

  bool empty(void) const { return _impl._node_count == 0; }
  size_type size(void) const { return _impl._node_count; }
  size_type max_size(void) const { return _alloc.max_size(); }

  void clear(void) {
    // for (iterator it = begin(); it != end(); ++it) {
    //   _alloc.destroy();  // 헉 이거 생각 좀 하고 해야할듯..
    // }
  }

  node_allocator get_node_allocator(void) { return _alloc; }

 private:
  typedef _rb_tree_node_base* base_ptr;
  typedef const _rb_tree_node_base* const_base_ptr;
  typedef _rb_tree_node<Val>* link_type;
  typedef const _rb_tree_node<Val>* const_link_type;

  // SECTION : about elements
  base_ptr _get_root(void) { return _header.parent; }
  base_ptr _get_left_most(void) { return _header.left; }
  base_ptr _get_right_most(void) { return _header.right; }

  link_type _create_node(const KeyOfValue& key_value) {
    return _alloc.allocate();
  }

  link_type _
};
// !SECTION: red-black tree

// SECTION : search operation
_rb_tree_node_base* _rb_tree_increment(_rb_tree_node_base* x) {
  if (x->right != NIL) {
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
  } else if (x->left != NIL) {
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
 * @param x
 * @param root
 */
void _rb_tree_rotate_left(_rb_tree_node_base* const x,
                          _rb_tree_node_base*& root) {}

void _rb_tree_rotate_right(_rb_tree_node_base* const x,
                           _rb_tree_node_base*& root) {}

void _rb_tree_recolorize(_rb_tree_node_base* x, _rb_tree_node_base* xp) {}

// SECTION : for simple BST search
_rb_tree_node_base* _rb_tree_subtree_min(_rb_tree_node_base* x) {
  while (x->left != NIL) x = x->left;
  return x;
}

_rb_tree_node_base* _rb_tree_subtree_max(_rb_tree_node_base* x) {
  while (x->right != NIL) x = x->right;
  return x;
}

}  // namespace ft

#endif  // _RB_TREE_HPP_
