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

#include "algorithm.hpp"
#include "pair.hpp"
#include "reverse_iterator.hpp"

namespace ft {

enum _rb_tree_color { RED = 0, BLACK };

/**
 * @brief base class of red-black tree node
 * _rb_tree_node 가 이를 상속받는다.
 * value 가 없기 때문에 template 이 필요하지 않다.
 */
struct _rb_tree_node_base {
  typedef _rb_tree_node_base* base_ptr;
  typedef const _rb_tree_node_base* const_base_ptr;

  _rb_tree_color color;
  base_ptr parent;
  base_ptr left;
  base_ptr right;

  void _node_init(base_ptr p) {
    color = RED;
    parent = p;
    left = NULL;
    right = NULL;
  }
};

template <typename Val>
struct _rb_tree_node : public _rb_tree_node_base {
  typedef _rb_tree_node<Val>* link_type;
  typedef Val value_type;

  value_type value;

  _rb_tree_node(const value_type& value = value_type()) : value(value) {}
};

template <typename Compare>
struct _rb_tree_key_compare {
  Compare _compare;

  _rb_tree_key_compare(void) : _compare() {}
  _rb_tree_key_compare(const Compare& comp) : _compare(comp) {}
};

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

  base_ptr _node;

  _rb_tree_iterator(void) : _node() {}

  explicit _rb_tree_iterator(base_ptr x) : _node(x) {}

  _rb_tree_iterator(const self& rhs) : _node(rhs._node) {}

  self& operator=(const self& rhs) {
    _node = rhs._node;
    return *this;
  }

  reference operator*(void) const {
    return static_cast<link_type>(_node)->value;
  }

  pointer operator->(void) const {
    return &(static_cast<link_type>(_node)->value);
  }

  self& operator++(void) {
    _node = _node_increment(_node);
    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    _node = _node_increment(_node);
    return tmp;
  }

  self& operator--(void) {
    _node = _node_decrement(_node);
    return *this;
  }

  self operator--(int) {
    self tmp = *this;
    _node = _node_decrement(_node);
    return tmp;
  }

  friend bool operator==(const self& lhs, const self& rhs) {
    return lhs._node == rhs._node;
  }

  friend bool operator!=(const self& lhs, const self& rhs) {
    return !(lhs == rhs);
  }

  // SECTION : assginment operator
};

template <typename T>
struct _rb_tree_const_iterator {
  typedef T value_type;
  typedef const T* pointer;
  typedef const T& reference;

  typedef _rb_tree_iterator<T> iterator;

  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

  typedef _rb_tree_const_iterator<T> self;
  typedef _rb_tree_node_base::const_base_ptr base_ptr;
  typedef const _rb_tree_node<T>* link_type;

  base_ptr _node;

  _rb_tree_const_iterator(void) : _node() {}

  explicit _rb_tree_const_iterator(base_ptr x) : _node(x) {}

  _rb_tree_const_iterator(const iterator& it) : _node(it._node) {}

  self& operator=(const self& rhs) {
    _node = rhs._node;
    return *this;
  }

  reference operator*(void) const {
    return static_cast<link_type>(_node)->value;
  }

  pointer operator->(void) const {
    return &(static_cast<link_type>(_node)->value);
  }

  self& operator++(void) {
    _node = _node_increment(_node);
    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    _node = _node_increment(_node);
    return tmp;
  }

  self& operator--(void) {
    _node = _node_decrement(_node);
    return *this;
  }

  self operator--(int) {
    self tmp = *this;
    _node = _node_decrement(_node);
    return tmp;
  }

  /**
   * @brief const iterator to iterator casting
   *
   * @return iterator
   */
  iterator _const_cast(void) const {
    return iterator(const_cast<typename iterator::base_ptr>(_node));
  }

  friend bool operator==(const self& lhs, const self& rhs) {
    return lhs._node == rhs._node;
  }

  friend bool operator!=(const self& lhs, const self& rhs) {
    return !(lhs == rhs);
  }
};

// !SECTION: red-black tree iterator

// 트리 정보를 저장하는 header cell
struct _rb_tree_header {
  _rb_tree_node_base _header;
  size_t _node_count;

  _rb_tree_header(void) {
    _header.color = RED;
    _reset();
  }

  /**
   * @brief tree 가 empty 이고 from 에서 데이터를 받아와야 할 경우 사용.
   *
   * @param from
   */
  void _move_data(_rb_tree_header& from) {
    _header.parent = from._header.parent;
    _header.left = from._header.left;
    _header.right = from._header.right;
    _header.parent->parent = &_header;  // update root data
    _node_count = from._node_count;

    from._reset();
  }

  /**
   * @brief
   * 초기 상태 : root 는 NULL, leftmost, rightmost 는 header 자신 (end)
   * 삽입 : left-most, right-most 를 갱신
   *
   */
  void _reset(void) {
    _header.parent = NULL;
    _header.left = &_header;
    _header.right = &_header;
    _node_count = 0;
  }
};

/**
 * @brief 실질적으로 데이터를 가지고 있다. _compare, _header 보유
 *
 * @tparam Compare
 */
template <typename Compare>
struct _rb_tree_impl : public _rb_tree_header, _rb_tree_key_compare<Compare> {
  typedef _rb_tree_key_compare<Compare> _base_key_compare;

  _rb_tree_impl(void) : _rb_tree_header(), _base_key_compare() {}
  _rb_tree_impl(const _rb_tree_impl& src)
      : _rb_tree_header(), _base_key_compare(src._compare) {}
  explicit _rb_tree_impl(const Compare& comp)
      : _rb_tree_header(), _base_key_compare(comp) {}
};

// SECTION: red-black tree
// Compare 로 노드 간에 비교해야 함
/**
 * @brief
 *
 * @tparam Key T key type
 * @tparam Val pair<Key, Val>
 * @tparam KeyOfValue map: first of value, set: value itself
 * @tparam Compare
 * @tparam Alloc
 */
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc = std::allocator<Val> >
class _rb_tree {
 protected:
  typedef _rb_tree_node_base* base_ptr;
  typedef const _rb_tree_node_base* const_base_ptr;
  typedef _rb_tree_node<Val>* link_type;
  typedef const _rb_tree_node<Val>* const_link_type;

 public:
  typedef Key key_type;
  typedef Val value_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef const value_type* const_pointer;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef _rb_tree_iterator<value_type> iterator;
  typedef _rb_tree_const_iterator<value_type> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

  // rebind for nodes
  typedef typename Alloc::template rebind<_rb_tree_node<value_type> >::other
      node_allocator;

  // SECTION : member
 private:
  _rb_tree_impl<Compare> _impl;
  node_allocator _alloc;

 public:
  _rb_tree(void) {}

  _rb_tree(const Compare& comp, const node_allocator& alloc = node_allocator())
      : _impl(comp), _alloc(alloc) {}

  _rb_tree(const _rb_tree& x) : _impl(x._impl), _alloc(x._alloc) {
    if (x._root() != NULL) {
      _copy_tree(x);
    }
  }

  ~_rb_tree(void) { _erase_all(_root()); }

  _rb_tree& operator=(const _rb_tree& x) {
    if (this != &x) {
      clear();
      _impl._compare = x._impl._compare;
      if (x._root() != NULL) {
        _copy_tree(x);
      }
    }
    return *this;
  }

  // SECTION : red-black tree operation
  iterator begin(void) { return iterator(_impl._header.left); }
  const_iterator begin(void) const {
    return const_iterator(_impl._header.left);
  }

  iterator end(void) { return iterator(&_impl._header); }
  const_iterator end(void) const { return const_iterator(&_impl._header); }

  reverse_iterator rbegin(void) { return reverse_iterator(end()); }
  const_reverse_iterator rbegin(void) const {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend(void) { return reverse_iterator(begin()); }
  const_reverse_iterator rend(void) const {
    return const_reverse_iterator(begin());
  }

  bool empty(void) const { return _impl._node_count == 0; }
  size_type size(void) const { return _impl._node_count; }
  size_type max_size(void) const { return _alloc.max_size(); }

  // SECTION : insert
  pair<iterator, bool> insert(const value_type& val) {
    pair<base_ptr, base_ptr> res = _find_insert_pos(KeyOfValue()(val));
    if (res.second) {
      return pair<iterator, bool>(_insert(res.first, res.second, val), true);
    }
    return pair<iterator, bool>(iterator(res.first), false);
  }

  /**
   * @brief insert with hint
   *
   * @param position
   * @param val
   * @return iterator 성공 -> 새 element, 실패 -> 기존 element
   */
  iterator insert(iterator position, const value_type& val) {
    pair<base_ptr, base_ptr> res =
        _find_insert_hint_pos(position, KeyOfValue()(val));
    if (res.second) {
      return _insert(res.first, res.second, val);
    }
    return iterator(res.first);
  }

  template <typename InputIterator>
  void insert(InputIterator first, InputIterator last) {
    for (; first != last; ++first) {
      insert(*first);
    }
  }

  // SECTION : erase
  void erase(iterator position) { _erase(position._node); }

  size_type erase(const key_type& key) {
    pair<iterator, iterator> range = equal_range(key);
    size_type count = 0;
    while (range.first != range.second) {
      erase(range.first++);
      ++count;
    }
    return count;
  }

  void swap(_rb_tree& x) {
    if (_get_root() == NULL) {
      if (x._get_root() != NULL) {
        _impl._move_data(x._impl);
      }
    } else if (x._get_root() == NULL) {
      x._impl._move_data(_impl);
    } else {
      _swap(_impl._header.parent, x._impl._header.parent);
      _swap(_impl._header.left, x._impl._header.left);
      _swap(_impl._header.right, x._impl._header.right);

      _get_root()->parent = &_impl._header;
      x._get_root()->parent = &x._impl._header;
      _swap(_impl._node_count, x._impl._node_count);
      _swap(_impl._compare, x._impl._compare);
    }
  }

  // NOTHROW
  void clear(void) {
    _erase_all(_root());
    _impl._reset();
  }

  /**
   * @brief key 에 해당하는 노드를 찾는다.
   *
   * @param k key
   * @return iterator 찾으면 찾은 iterator, 못찾으면 end.
   */
  iterator find(const key_type& key) {
    iterator gte = lower_bound(key);
    // 1. gte 가 end 이면 크거나 같은 키가 없음.
    // 2. 1 에서 || 이므로 key <= gte.
    // 거기서 key < gte 가 true 이면 같은 것이 아니라 더 큰 것이므로 end return.
    return (gte == end() || _impl._compare(key, KeyOfValue()(*gte))) ? end()
                                                                     : gte;
  }

  const_iterator find(const key_type& key) const {
    const_iterator gte = lower_bound(key);
    return (gte == end() || _impl._compare(key, KeyOfValue()(*gte))) ? end()
                                                                     : gte;
  }

  // distance of equal_range
  size_type count(const key_type& key) const {
    pair<const_iterator, const_iterator> p = equal_range(key);
    return static_cast<size_type>(std::distance(p.first, p.second));
  }

  /**
   * @brief key 보다 크거나 같은 노드 중 가장 작은 노드 반환.
   * Compare : comp(a,b) 는 a 가 b 보다 앞에 있다고 판단되면 true 를 리턴한다.
   * 기준은 함수가 정의한 strict weak ordering 에 따른다.
   * ... 이기 때문에 compare true -> small, false-> big.
   *
   * @param key
   * @return iterator
   */
  iterator lower_bound(const key_type& key) {
    link_type x = _root();
    base_ptr y = _get_end();
    while (x != NULL) {
      if (!_impl._compare(_get_key(x), key)) {
        y = x;
        x = _left(x);
      } else {
        x = _right(x);
      }
    }
    return iterator(y);
  }

  const_iterator lower_bound(const key_type& key) const {
    link_type x = _root();
    base_ptr y = _get_end();
    while (x != NULL) {
      if (!_impl._compare(_get_key(x), key)) {
        y = x;
        x = _left(x);
      } else {
        x = _right(x);
      }
    }
    return const_iterator(y);
  }

  /**
   * @brief key 보다 큰 노드 중 가장 작은 노드 반환.
   *
   * @param key
   * @return iterator
   */
  iterator upper_bound(const key_type& key) {
    link_type x = _root();
    base_ptr y = _get_end();
    while (x != NULL) {
      if (_impl._compare(key, _get_key(x))) {
        y = x;
        x = _left(x);
      } else {
        x = _right(x);
      }
    }
    return iterator(y);
  }

  const_iterator upper_bound(const key_type& key) const {
    link_type x = _root();
    base_ptr y = _get_end();
    while (x != NULL) {
      if (_impl._compare(key, _get_key(x))) {
        y = x;
        x = _left(x);
      } else {
        x = _right(x);
      }
    }
    return const_iterator(y);
  }

  pair<iterator, iterator> equal_range(const key_type& key) {
    return ft::make_pair(lower_bound(key), upper_bound(key));
  }

  pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
    return ft::make_pair(lower_bound(key), upper_bound(key));
  }

  node_allocator get_node_allocator(void) { return _alloc; }

 private:
  // SECTION : about elements
  base_ptr _get_root(void) const { return _impl._header.parent; }
  base_ptr _get_left_most(void) const { return _impl._header.left; }
  base_ptr _get_right_most(void) const { return _impl._header.right; }
  base_ptr _get_end(void) const {
    return const_cast<base_ptr>(&(_impl._header));
  }

  key_type _get_key(const_link_type x) const { return KeyOfValue()(x->value); }

  key_type _get_key(const_base_ptr x) const {
    return _get_key(static_cast<const_link_type>(x));
  }

  link_type _root(void) const {
    return static_cast<link_type>(_impl._header.parent);
  }

  link_type _left(base_ptr x) const { return static_cast<link_type>(x->left); }

  link_type _right(base_ptr x) const {
    return static_cast<link_type>(x->right);
  }

  // copy constructor, assignment operator 에서 호출
  void _copy_tree(const _rb_tree& x) {
    _impl._header.parent = _copy_nodes(x._root(), _root());
    _impl._header.left = _get_subtree_min(_impl._header.parent);
    _impl._header.right = _get_subtree_max(_impl._header.parent);
    _impl._header.parent->parent = _get_end();
    _impl._node_count = x._impl._node_count;
  }

  // SECTION: tree modification
  /**
   * @brief find parent's position to insert new node
   *
   * @param key
   * @return pair<base_ptr, base_ptr> success - first : null, second : parent
   * fail - first : equal node, second : null
   */
  pair<base_ptr, base_ptr> _find_insert_pos(const key_type& key) {
    typedef pair<base_ptr, base_ptr> pair_type;

    base_ptr x = _get_root();
    base_ptr y = &_impl._header;
    bool comp = true;

    while (x != NULL) {
      y = x;
      comp = _impl._compare(key, _get_key(x));
      x = comp ? x->left : x->right;
    }
    iterator tmp(y);
    if (comp) {              // key < y.key x가 왼쪽에 있음
      if (tmp == begin()) {  // 여기 넣어야 함!
        return pair_type(x, y);
      }
      --tmp;
    }
    // tmp < key -> y보다 크고, tmp 보다 크다.
    if (_impl._compare(KeyOfValue()(*tmp), key)) {
      return pair_type(x, y);
    }
    // tmp == key
    return pair_type(tmp._node, NULL);
  }

  /**
   * @brief position 과 최대한 가까운 위치에 있는 node 를 찾는다.
   *
   * @param position
   * @param key
   * @return pair<base_ptr, base_ptr>
   */
  pair<base_ptr, base_ptr> _find_insert_hint_pos(const_iterator position,
                                                 const key_type& key) {
    typedef pair<base_ptr, base_ptr> pair_type;
    iterator pos = position._const_cast();

    if (pos._node == &_impl._header) {  // end
      if (size() > 0 && _impl._compare(_get_key(_get_right_most()), key)) {
        // key가  rightmost 보다 크다면 rightmost 뒤에 삽입
        return pair_type(NULL, _get_right_most());
      } else {
        // 아니면 허위사실이므로 새로 찾아야 함
        return _find_insert_pos(key);
      }
    } else if (_impl._compare(key, _get_key(pos._node))) {
      // key < pos && key != end
      iterator before = pos;
      if (pos._node == _get_left_most()) {  // begin()
        // 근데 pos 가 left most 이면 left most 앞에 삽입.
        return pair_type(_get_left_most(), _get_left_most());
      } else if (_impl._compare(_get_key((--before)._node), key)) {
        // before < key < pos
        if (_right(before._node) == NULL) {
          // before 의 right 가 없으면 before 자식으로 삽입 (오른쪽)
          return pair_type(NULL, before._node);
        } else {
          // pos._node != NULL 이므로 pos 의 왼편에 들어갈 것이다.
          // 근데 Pos 왼쪽자식 없는 건 어케 알아?...
          return pair_type(pos._node, pos._node);
        }
      } else {
        return _find_insert_pos(key);
      }
    } else if (_impl._compare(_get_key(pos._node), key)) {
      // pos < key
      iterator after = pos;
      if (pos._node == _get_right_most()) {
        // pos 가 right most 이면 right most 뒤에 삽입
        return pair_type(NULL, _get_right_most());
      } else if (_impl._compare(key, _get_key((++after)._node))) {
        // pos < key < after
        if (_right(pos._node) == NULL) {
          // pos 의 right 가 없으면 right 자식으로 삽입
          return pair_type(NULL, pos._node);
        } else {
          return pair_type(after._node, after._node);
        }
      } else {
        return _find_insert_pos(key);
      }
    } else {
      // pos == key
      return pair_type(pos._node, NULL);
    }
  }

  /**
   * @brief insert node 내부 동작 함수
   *
   * @param x
   * @param p
   * @param value
   * @return iterator insert 한 node 의 iterator
   */
  iterator _insert(base_ptr x, base_ptr p, const value_type& value) {
    bool _insert_left = (x != NULL || p == end()._node ||
                         _impl._compare(KeyOfValue()(value), _get_key(p)));
    link_type node = _create_node(value);
    _insert_rebalance(_insert_left, node, p, this->_impl._header);
    ++_impl._node_count;

    return iterator(node);
  }

  void _erase(base_ptr x) {
    link_type y =
        static_cast<link_type>(_rebalance_for_erase(x, _impl._header));
    _destroy_node(y);
    --_impl._node_count;
  }

  /**
   * @brief node 를 기준으로 하는 subtree 를 모두 삭제한다.
   *
   * @param node
   */
  void _erase_all(link_type node) {
    while (node != NULL) {
      _erase_all(_right(node));
      link_type tmp = _left(node);
      _destroy_node(node);
      node = tmp;
    }
  }

  /**
   * @brief 완성된 tree 를 그대로 복사한다.
   * tree 가 정렬된 것이 확실하면 _insert_rebalance 를 사용하지 않고 더 빠르게
   * 복사할 수 있다.
   *
   * @param x 복사할 node
   * @param p x를 달 parent
   * @return link_type 복사된 node 의 포인터
   */
  link_type _copy_nodes(link_type x, base_ptr p) {
    // x 는 NULL 이 아닌 것이 보장되어 있다.
    link_type top = _clone_node(x);
    top->parent = p;

    try {
      if (x->right) {
        // right 있을 경우 -> top의 right 도 달아준다.
        top->right = _copy_nodes(_right(x), top);  // right top 에 copy
      }
      p = top;
      x = _left(x);  // 이제 left 할 준비

      while (x != NULL) {
        link_type y = _clone_node(x);            // 왼쪽거 카피할 거임..
        p->left = y;                             // 달아줌
        y->parent = p;                           // 연결해줌
        if (x->right) {                          // 오른쪽 있어??
          y->right = _copy_nodes(_right(x), y);  // 오른쪽 달아줘
        }
        p = y;
        x = _left(x);  // 다시 left 준비해.
      }
    } catch (...) {
      _erase_all(top);  // rollback
      throw;
    }
    return top;
  }

  // !SECTION : tree modification

  // SECTION : node memory management
  void _construct_node(link_type node, const value_type& value) {
    try {
      _alloc.construct(node, value);
    } catch (...) {
      _deallocate_node(node);
      throw;
    }
  }

  link_type _create_node(const value_type& value) {
    link_type tmp = _alloc.allocate(1);
    _construct_node(tmp, value);
    return tmp;
  }

  void _deallocate_node(link_type node) { _alloc.deallocate(node, 1); }

  void _destroy_node(link_type node) {
    _alloc.destroy(node);
    _deallocate_node(node);
  }

  link_type _clone_node(link_type x) {
    link_type tmp = _create_node(x->value);
    tmp->color = x->color;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
  }
  // !SECTION : node memory management

  friend bool operator==(const _rb_tree& x, const _rb_tree& y) {
    return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
  }

  friend bool operator<(const _rb_tree& x, const _rb_tree& y) {
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
  }
};
// !SECTION: red-black tree

_rb_tree_node_base* _get_subtree_min(_rb_tree_node_base* x);
const _rb_tree_node_base* _get_subtree_min(const _rb_tree_node_base* x);

_rb_tree_node_base* _get_subtree_max(_rb_tree_node_base* x);
const _rb_tree_node_base* _get_subtree_max(const _rb_tree_node_base* x);

_rb_tree_node_base* _node_increment(_rb_tree_node_base* x);
const _rb_tree_node_base* _node_increment(const _rb_tree_node_base* x);

_rb_tree_node_base* _node_decrement(_rb_tree_node_base* x);
const _rb_tree_node_base* _node_decrement(const _rb_tree_node_base* x);

void _insert_rebalance(bool left, _rb_tree_node_base* x, _rb_tree_node_base* p,
                       _rb_tree_node_base& header);

_rb_tree_node_base* _rebalance_for_erase(_rb_tree_node_base* const z,
                                         _rb_tree_node_base& header);

}  // namespace ft

#endif  // _RB_TREE_HPP_
