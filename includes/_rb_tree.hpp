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
enum _color { RED = 0, BLACK };

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

  _rb_tree_header(void) {
    node_count = 0;
    header.color = BLACK;
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
  typedef _rb_tree_node<T>* link_type;  // child of base_ptr

 private:
  base_ptr _node;

 public:
  _rb_tree_iterator(void) : _node() {}

  explicit _rb_tree_iterator(base_ptr x) : _node(x) {}

  //??
  reference operator*(void) const {
    return *static_cast<link_type>(_node)->value;
  }

  pointer operator->(void) const { static_cast<link_type>(_node)->value; }

  self& operator++(void) {
    _node = _rb_tree_increament(_node);
    return *this;)
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

  // SECTION : assginment operator
};

// SECTION: red-black tree
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc = std::allocator<Val>>
class _rb_tree {
 private:
 public:
  _tree(void) {}
  ~_tree(void) {}

  void insert(const& T element) {}
};
// !SECTION: red-black tree

// SECTION : red-black tree operation

// SECTION : search operation
_rb_tree_node_base* _rb_tree_increment(_rb_tree_node_base* x) {
  // rb tree + 1 -> 오른쪽 서브트리에서 가장 작은 값
  // 오른쪽 없으면
  // 내가 부모의 왼쪽 자식 -> 부모
  // 내가 부모의 오른쪽 자식 -> 왼쪽 자식이 될 때 까지 부모 찾기
  if (x->right != 0) {
    return _rb_tree_subtree_min(x->right);
  } else {
    _rb_tree_node_base* xp = x->parent;
    while (x == xp->right) {  // x가 xp의 right child
      x = xp;
      xp = xp->parent;
    }
    // 왼쪽 자식이 될 때 까지 트리 등반. y가 부모, x가 왼쪽 자식
    // 이거나 xp 가 끝나고 x가 root 일 경우 끝.
    // 종료 조건이 x != xp->right 이므로 두 가지가 있다.
    // 1. x 가 xp의 왼쪽 자식
    // 2. x 가 root (더 이상 자식이 없음)
    // 1의 경우여야 우리가 원하는 그 부분인...
    // 그럼 2는 뭐란 말임? 몰라~

    if (x->right != xp) x = xp;
  }
  return x;
}

_rb_tree_node_base* _rb_tree_decrement(_rb_tree_node_base* x) {}

/**
 * @brief subtree 를 왼쪽으로 회전
 *
 * @param x 이건 뭐지?
 * @param root 회전할 노드
 */
void _rb_tree_rotate_left(_rb_tree_node_base* const x,
                          _rb_tree_node_base*& root) {}

void _rb_tree_rotate_right(_rb_tree_node_base* const x,
                           _rb_tree_node_base*& root) {}

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
