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

/**
 * @brief subtree 를 왼쪽으로 회전
 *  할아버지가 오른쪽 자식, 부모가 루트, 자식이 왼쪽 자식이 된다.
 *
 * @param x 서브트리 root
 * @param root 트리의 root
 */
static void _rotate_left(_rb_tree_node_base* const x,
                         _rb_tree_node_base*& root) {
  _rb_tree_node_base* const y = x->right;
  x->right = y->left;
  if (y->left != NULL) {
    y->left->parent = x;
  }
  y->parent = x->parent;

  if (x == root) {
    root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

static void _rotate_right(_rb_tree_node_base* const x,
                          _rb_tree_node_base*& root) {
  _rb_tree_node_base* const y = x->left;

  x->left = y->right;
  if (y->right != NULL) {
    y->right->parent = x;
  }
  y->parent = x->parent;

  if (x == root) {
    root = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}

_rb_tree_node_base* _get_subtree_min(_rb_tree_node_base* x) {
  while (x->left != NULL) x = x->left;
  return x;
}

const _rb_tree_node_base* _get_subtree_min(const _rb_tree_node_base* x) {
  while (x->left != NULL) x = x->left;
  return x;
}

_rb_tree_node_base* _get_subtree_max(_rb_tree_node_base* x) {
  while (x->right != NULL) x = x->right;
  return x;
}

const _rb_tree_node_base* _get_subtree_max(const _rb_tree_node_base* x) {
  while (x->right != NULL) x = x->right;
  return x;
}

_rb_tree_node_base* _node_increment(_rb_tree_node_base* x) {
  if (x->right != NULL) {
    return _get_subtree_min(x->right);
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

const _rb_tree_node_base* _node_increment(const _rb_tree_node_base* x) {
  if (x->right != NULL) {
    return _get_subtree_min(x->right);
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

_rb_tree_node_base* _node_decrement(_rb_tree_node_base* x) {
  if (x->color == RED && x->parent->parent == x) {
    x = x->right;
  } else if (x->left != NULL) {
    return _get_subtree_max(x->left);
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

const _rb_tree_node_base* _node_decrement(const _rb_tree_node_base* x) {
  if (x->color == RED && x->parent->parent == x) {
    x = x->right;
  } else if (x->left != NULL) {
    return _get_subtree_max(x->left);
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

// NOTHROW
/**
 * @brief insert and rebalance tree
 *
 * @param left
 * @param x
 * @param p
 * @param header
 */
void _insert_rebalance(bool left, _rb_tree_node_base* x, _rb_tree_node_base* p,
                       _rb_tree_node_base& header) {
  _rb_tree_node_base*& root = header.parent;
  x->_node_init(p);

  // insert
  // root, leftmost, rightmost 를 갱신해준다.
  // empty 이면 left 이다.
  if (left) {
    p->left = x;         // also makes leftmost = x when p == header
    if (p == &header) {  // root 자리에 들어갈 경우
      header.parent = x;
      header.right = x;
      x->color = BLACK;
    } else if (p == header.left) {  // leftmost update
      header.left = x;
    }
  } else {
    p->right = x;
    if (p == header.right) {  // rightmost update
      header.right = x;
    }
  }
  // rotate
  while (x != root && x->parent->color == RED) {  // header -> no pp
    _rb_tree_node_base* const xpp = x->parent->parent;
    if (x->parent == xpp->left) {                 // 부모가 왼쪽 자식임
      _rb_tree_node_base* const xu = xpp->right;  // uncle
      if (xu && xu->color == RED) {
        // case 1
        // 삼촌과 부모가 빨간색, 나는 왼쪽 자식 -> color 교체만 해주면 됨
        // 이미 할아버지는 BLACK 인 게 확실함 (rb tree 의 성질)
        x->parent->color = BLACK;
        xu->color = BLACK;
        xpp->color = RED;
        x = xpp;  // 다시 확인할 노드를 할아버지 (xpp) 로 바꿔줌
      } else {  // 부모가 왼쪽, 삼촌이 없거나 (이것도 BLACK) 삼촌이 검은색
        if (x == x->parent->right) {  // 나 오른쪽 자식
                                      // case 2
          x = x->parent;
          // 부모 기준으로 왼쪽 회전 후 case 3으로 넘어감
          _rotate_left(x, root);
        }
        // case 3
        // 부모와 할아버지의 색을 바꾸고 할아버지 기준으로 오른쪽 회전.
        x->parent->color = BLACK;
        xpp->color = RED;
        _rotate_right(xpp, root);
      }
    } else {
      // 좌우만 바뀌고 위랑 똑같!!
      _rb_tree_node_base* const xu = xpp->left;
      if (xu && xu->color == RED) {
        x->parent->color = BLACK;
        xu->color = BLACK;
        xpp->color = RED;
        x = xpp;
      } else {
        if (x == x->parent->left) {
          x = x->parent;
          _rotate_right(x, root);
        }
        x->parent->color = BLACK;
        xpp->color = RED;
        _rotate_left(xpp, root);
      }
    }
  }
  root->color = BLACK;  // while 을 바로 빠져나온 경우를 위해 setting.
}

/**
 * @brief 노드를 진짜 삭제하기 전 맛있게 리밸런싱 해준다.
 *
 * @param z 삭제하고자 하는 node 값
 * @param header tree 정보를 담은 header node
 * @return _rb_tree_node_base* 삭제할 실제 노드
 */
_rb_tree_node_base* _rebalance_for_erase(_rb_tree_node_base* const z,
                                         _rb_tree_node_base& header) {
  _rb_tree_node_base*& root = header.parent;
  _rb_tree_node_base*& leftmost = header.left;
  _rb_tree_node_base*& rightmost = header.right;

  _rb_tree_node_base* y = z;
  _rb_tree_node_base* x = NULL;
  _rb_tree_node_base* xp = NULL;

  // 자식이 0 또는 1개인 경우 y 는 z 이고, 2개인 경우 y 는 z 의 successor
  if (y->left == NULL) {  // z 오른쪽 자식이 있을 수도.. 없을 수도...
    x = y->right;
  } else if (y->right == NULL) {  // z 왼쪽 자식 있음
    x = y->left;
  } else {  // z 의 자식이 둘 다 있음
    y = _get_subtree_min(z->right);
    x = y->right;
  }

  if (y != z) {
    // y가 z의 successor 인 경우 relink y in place of z.
    z->left->parent = y;
    y->left = z->left;  // y의 왼쪽 자식을 z의 왼쪽 자식으로 바꿔줌
    if (y != z->right) {  // y가 z의 오른 자식이 아닌 손자 이상의 successor
      xp = y->parent;  // y의 부모를 xp에 저장
      if (x != NULL) {
        x->parent = y->parent;
      }
      y->parent->left = x;  // y의 부모의 왼쪽 자식을 x로 바꿔줌 -> y의 형제노드
      y->right = z->right;
      z->right->parent = y;
    } else {
      xp = y;
    }

    if (root == z) {
      root = y;
    } else if (z->parent->left == z) {
      z->parent->left = y;
    } else {
      z->parent->right = y;
    }
    y->parent = z->parent;
    _swap(y->color, z->color);
    y = z;
  } else {  // y == z
    // z의 자식이 0 또는 1개인 경우
    xp = y->parent;
    if (x != NULL) {
      x->parent = y->parent;
    }
    if (root == z) {
      root = x;
    } else if (z->parent->left == z) {
      z->parent->left = x;
    } else {
      z->parent->right = x;
    }
    if (leftmost == z) {
      if (z->right == NULL) {
        leftmost = z->parent;
      } else {
        leftmost = _get_subtree_min(x);
      }
    }
    if (rightmost == z) {
      if (z->left == NULL) {
        rightmost = z->parent;
      } else {
        rightmost = _get_subtree_max(x);
      }
    }
  }
  // 재정렬
  // y가 삭제되어야 하는 노드인데 RED 면 이상이 없으므로 BLACK 인 경우만 rotate
  if (y->color != RED) {
    while (x != root && (x == NULL || x->color == BLACK)) {
      if (x == xp->left) {
        _rb_tree_node_base* w = xp->right;
        if (w->color == RED) {
          w->color = BLACK;
          xp->color = RED;
          _rotate_left(xp, root);
          w = xp->right;
        }
        if ((w->left == NULL || w->left->color == BLACK) &&
            (w->right == NULL || w->right->color == BLACK)) {
          w->color = RED;
          x = xp;
          xp = xp->parent;
        } else {
          if (w->right == NULL || w->right->color == BLACK) {
            w->left->color = BLACK;
            w->color = RED;
            _rotate_right(w, root);
            w = xp->right;
          }
          w->color = xp->color;
          xp->color = BLACK;
          if (w->right != NULL) {
            w->right->color = BLACK;
          }
          _rotate_left(xp, root);
          break;
        }
      } else {
        _rb_tree_node_base* w = xp->left;
        if (w->color == RED) {
          w->color = BLACK;
          xp->color = RED;
          _rotate_right(xp, root);
          w = xp->left;
        }
        if ((w->right == NULL || w->right->color == BLACK) &&
            (w->left == NULL || w->left->color == BLACK)) {
          w->color = RED;
          x = xp;
          xp = xp->parent;
        } else {
          if (w->left == NULL || w->left->color == BLACK) {
            w->right->color = BLACK;
            w->color = RED;
            _rotate_left(w, root);
            w = xp->left;
          }
          w->color = xp->color;
          xp->color = BLACK;
          if (w->left != NULL) {
            w->left->color = BLACK;
          }
          _rotate_right(xp, root);
          break;
        }
      }
    }
    if (x != NULL) {
      x->color = BLACK;
    }
  }
  return y;
}

}  // namespace ft
