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
  // tree 가 empty 라면 left 이다.
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
  if (y->left == NULL) {  // z 왼쪽 자식 없음. 오른쪽 자식은 모름
    x = y->right;         // NULL 일 수도 있다.
  } else if (y->right == NULL) {  // z 왼쪽 자식 있음, 오른쪽 없음.
    x = y->left;
  } else {  // z 의 자식이 둘 다 있음
    y = _get_subtree_min(z->right);
    x = y->right;
  }

  // z 가 leaf 이거나 오른쪽만 있으면 x 가 null 이다.
  // 이 경우 y 는 z 와 같다.

  // z 없앨 준비 하는 과정
  if (y != z) {
    // y가 z의 successor 인 경우 relink y in place of z.
    z->left->parent = y;
    y->left = z->left;  // y의 왼쪽 자식을 z의 왼쪽 자식으로 바꿔줌
    if (y != z->right) {
      // x 가 y 자리에 갈 것이므로 xp를 y 의 부모로 세팅해야 한다.
      xp = y->parent;  // y의 부모를 xp에 저장 -> 어차피 y 자리에 x를 넣을 것
      if (x != NULL) {
        x->parent = y->parent;
      }
      y->parent->left = x;
      y->right = z->right;
      z->right->parent = y;
    } else {
      // z 가 y 의 부모인 경우 x.p 가 z 이면 유감이므로 y 를 x.p 로 세팅.
      // 어차피 결과적으로 y는 x의 부모가 된다.
      xp = y;
    }
    // 부모와 연결
    if (root == z) {
      root = y;
    } else if (z->parent->left == z) {
      z->parent->left = y;
    } else {
      z->parent->right = y;
    }
    y->parent = z->parent;

    swap(y->color, z->color);  // y 가 z 색 상속
    y = z;                     // 실제 없어질 노드 -> z
  } else {                     // y == z
    // z의 자식이 0 또는 1개인 경우 -> x 가 z 자리 바로 대체함.
    xp = z->parent;
    if (x != NULL) {
      x->parent = z->parent;
    }
    if (root == z) {
      root = x;
    } else if (z->parent->left == z) {
      z->parent->left = x;
    } else {
      z->parent->right = x;
    }
    // 자식이 있으면 이럴 일이 없다.
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

  // doubly black 해결 과정
  if (y->color != RED) {  // 삭제될 node 의 color 판별
    while (x != root && (x == NULL || x->color == BLACK)) {
      // red -> black 제거해서 black 설정하면 됨.
      // root -> extra black 제거하면 됨.
      // x is always doubly black node which is non-root
      if (x == xp->left) {
        _rb_tree_node_base* w = xp->right;  // sibling of x
        if (w->color == RED) {              // case 1
          w->color = BLACK;
          xp->color = RED;
          _rotate_left(xp, root);
          w = xp->right;  // update sibling to new sibling after rotate
        }
        if ((w->left == NULL || w->left->color == BLACK) &&
            (w->right == NULL || w->right->color == BLACK)) {
          // case 2 (both children of w are black)
          w->color = RED;
          x = xp;  // 대상 노드 (extra black 이 추가된 black node)를 부모로 갱신
          xp = xp->parent;
        } else {
          // case 3, 4
          if (w->right == NULL || w->right->color == BLACK) {
            // case 3
            // w->left 와 w 의 색을 바꾼다.
            w->left->color = BLACK;
            w->color = RED;
            _rotate_right(w, root);
            w = xp->right;  // w 새로 설정. (rotate 했기 때문)
            // case 4로 이어짐
          }
          // case 4
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
      x->color = BLACK;  // red-and-black or root
    }
  }
  return y;
}

}  // namespace ft
