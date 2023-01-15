/**
 * @file _rb_tree.cpp
 * @author jiskim
 * @brief implement for rb_tree.hpp
 * @date 2023-01-15
 *
 * @copyright Copyright (c) 2023
 */

#include "_rb_tree.hpp"

namespace ft {
_rb_tree_node_base* _left_most(_rb_tree_node_base* x) {
  while (x->left != NIL) x = x->left;
  return x;
}

const _rb_tree_node_base* _left_most(const _rb_tree_node_base* x) {
  while (x->left != NIL) x = x->left;
  return x;
}

_rb_tree_node_base* _right_most(_rb_tree_node_base* x) {
  while (x->right != NIL) x = x->right;
  return x;
}

const _rb_tree_node_base* _right_most(const _rb_tree_node_base* x) {
  while (x->right != NIL) x = x->right;
  return x;
}

_rb_tree_node_base* _rb_tree_increment(_rb_tree_node_base* x) {
  if (x->right != NIL) {
    return _left_most(x->right);
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

const _rb_tree_node_base* _rb_tree_increment(const _rb_tree_node_base* x) {
  if (x->right != NIL) {
    return _left_most(x->right);
  } else {
    const _rb_tree_node_base* xp = x->parent;
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
    return _right_most(x->left);  // max of left subtree
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

const _rb_tree_node_base* _rb_tree_decrement(const _rb_tree_node_base* x) {
  if (x->color == RED && x->parent->parent == x) {
    x = x->right;
  } else if (x->left != NIL) {
    return _right_most(x->left);
  } else {
    const _rb_tree_node_base* xp = x->parent;
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

/**
 * @brief
 *
 * @param left
 * @param x
 * @param p
 * @param header
 */
void _insert_rebalance(bool left, _rb_tree_node_base* x, _rb_tree_node_base* p,
                       _rb_tree_node_base& header) {
  // insert and rebalance tree
  _rb_tree_node_base*& root = header.parent;

  if (p != &header) {
    std::cout << "p != header\n";
  }
  // Initialize new node
  x->_rb_tree_init(p);

  // insert
  // root, leftmost, rightmost 를 갱신해준다.
  // empty 이면 left 이다.
  if (left) {
    std::cout << "left insert\n";
    p->left = x;         // also makes leftmost = x when p == header
    if (p == &header) {  // root 자리에 들어갈 경우
      header.parent = x;
      header.right = x;
      x->color = BLACK;
    } else if (p == header.left) {  // leftmost update
      header.left = x;
      std::cout << "left most update\n";
    }
  } else {
    std::cout << "right insert\n";
    p->right = x;
    if (p == header.right) {  // rightmost update
      header.right = x;
      std::cout << "right most update\n";
    }
  }
  // rotate
  // while (x != root && x->parent->color == RED) {  // header -> no pp
  //  _rb_tree_node_base* const xpp = x->parent->parent;
  //  if (x->parent == xpp->left) {                 // 부모가 왼쪽 자식임
  //    _rb_tree_node_base* const xu = xpp->right;  // uncle
  //    if (xu && xu->color == RED) {
  //      // case 1
  //      // 삼촌과 부모가 빨간색, 나는 왼쪽 자식 -> color 교체만 해주면 됨
  //      // 이미 할아버지는 BLACK 인 게 확실함
  //      x->parent->color = BLACK;
  //      xu->color = BLACK;
  //      xpp->color = RED;
  //      x = xpp;  // 다시 확인할 노드를 할아버지 (xpp) 로 바꿔줌
  //    } else {  // 부모가 왼쪽, 삼촌이 없거나 (이것도 BLACK) 삼촌이 검은색
  //      if (x == x->parent->right) {  // 나 오른쪽 자식
  //                                    // case 2
  //        x = x->parent;
  //        // 부모 기준으로 왼쪽 회전 후 case 3으로 넘어감
  //        _rb_tree_rotate_left(x, root);
  //      }
  //      // case 3
  //      // 부모와 할아버지의 색을 바꾸고 할아버지 기준으로 오른쪽 회전.
  //      x->parent->color = BLACK;
  //      xpp->color = RED;
  //      _rb_tree_rotate_right(xpp, root);
  //    }
  //  } else {
  //    // 좌우만 바뀌고 위랑 똑같!!
  //    _rb_tree_node_base* const xu = xpp->left;
  //    if (xu && xu->color == RED) {
  //      x->parent->color = BLACK;
  //      xu->color = BLACK;
  //      xpp->color = RED;
  //      x = xpp;
  //    } else {
  //      if (x == x->parent->left) {
  //        x = x->parent;
  //        _rb_tree_rotate_right(x, root);
  //      }
  //      x->parent->color = BLACK;
  //      xpp->color = RED;
  //      _rb_tree_rotate_left(xpp, root);
  //    }
  //  }
  //}
  root->color = BLACK;  // while 을 바로 빠져나온 경우를 위해 setting.
}

/**
 * @brief x 를 기준으로 subtree 에서 successor 를 찾는다.
 * successor 는 x 보다 큰 노드 중 가장 작은 노드이다.
 *
 * @param x
 * @return _rb_tree_node_base*
 */
_rb_tree_node_base* _find_successor(_rb_tree_node_base* x) {
  if (x->right != NIL) {  // x has right child
    return _left_most(x->right);
  }
  _rb_tree_node_base* y = x->parent;
  while (x == y->right) {
    x = y;
    y = y->parent;
  }
  if (x->right != y) {
    x = y;
  }
  return x;
}
}  // namespace ft