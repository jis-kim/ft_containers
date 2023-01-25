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
  if (left) {
    p->left = x;
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
  while (x != root && x->parent->color == RED) {
    _rb_tree_node_base* const xpp = x->parent->parent;
    if (x->parent == xpp->left) {
      _rb_tree_node_base* const xu = xpp->right;  // uncle
      if (xu && xu->color == RED) {
        // case 1
        x->parent->color = BLACK;
        xu->color = BLACK;
        xpp->color = RED;
        x = xpp;
      } else {
        if (x == x->parent->right) {
          // case 2
          x = x->parent;
          _rotate_left(x, root);
        }
        // case 3
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
  root->color = BLACK;
}

/**
 * @brief 노드를 진짜 삭제하기 전 리밸런싱
 *
 * @param z 삭제하고자 하는 node
 * @param header tree 정보를 담은 header node
 * @return _rb_tree_node_base* 파괴할 실제 노드
 */
_rb_tree_node_base* _rebalance_for_erase(_rb_tree_node_base* const z,
                                         _rb_tree_node_base& header) {
  _rb_tree_node_base*& root = header.parent;
  _rb_tree_node_base*& leftmost = header.left;
  _rb_tree_node_base*& rightmost = header.right;

  _rb_tree_node_base* y = z;
  _rb_tree_node_base* x = NULL;
  _rb_tree_node_base* xp = NULL;

  if (y->left == NULL) {
    x = y->right;
  } else if (y->right == NULL) {
    x = y->left;
  } else {
    y = _get_subtree_min(z->right);
    x = y->right;
  }

  if (y != z) {
    z->left->parent = y;
    y->left = z->left;
    if (y != z->right) {
      xp = y->parent;
      if (x != NULL) {
        x->parent = y->parent;
      }
      y->parent->left = x;
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

    swap(y->color, z->color);
    y = z;
  } else {
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
  if (y->color != RED) {
    while (x != root && (x == NULL || x->color == BLACK)) {
      // x is always doubly black node which is non-root
      if (x == xp->left) {
        _rb_tree_node_base* w = xp->right;  // sibling of x
        if (w->color == RED) {              // case 1
          w->color = BLACK;
          xp->color = RED;
          _rotate_left(xp, root);
          w = xp->right;
        }
        if ((w->left == NULL || w->left->color == BLACK) &&
            (w->right == NULL || w->right->color == BLACK)) {
          // case 2 (both children of w are black)
          w->color = RED;
          x = xp;
          xp = xp->parent;
        } else {
          // case 3, 4
          if (w->right == NULL || w->right->color == BLACK) {
            // case 3
            w->left->color = BLACK;
            w->color = RED;
            _rotate_right(w, root);
            w = xp->right;
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
      x->color = BLACK;
    }
  }
  return y;
}

}  // namespace ft
