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

#include "pair.hpp"
#include "reverse_iterator.hpp"
#include "testheader/print_vector.hpp"
#define NIL NULL

namespace ft {
enum _rb_tree_color { RED = 0, BLACK };

/**
 * @brief base class of red-black tree node
 * _rb_tree_node 가 이를 상속받는다.
 * 걍 타입이 필요없음 Value 가 없기 때문
 */
struct _rb_tree_node_base {
  typedef _rb_tree_node_base* base_ptr;
  typedef const _rb_tree_node_base* const_base_ptr;

  _rb_tree_color color;
  base_ptr parent;
  base_ptr left;
  base_ptr right;

  void _rb_tree_init(base_ptr p) {
    color = RED;
    parent = p;
    left = NIL;
    right = NIL;
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

 public:
  _rb_tree_iterator(void) : _node() {}

  explicit _rb_tree_iterator(base_ptr x) : _node(x) {}

  reference operator*(void) const {
    return static_cast<link_type>(_node)->value;
  }

  pointer operator->(void) const {
    return &(static_cast<link_type>(_node)->value);
  }

  self& operator++(void) {
    _node = _rb_tree_increment(_node);
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
  friend bool operator==(const self& lhs, const self& rhs) {
    return lhs._node == rhs._node;  // private
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

 private:
  base_ptr _node;

 public:
  _rb_tree_const_iterator(void) : _node() {}

  explicit _rb_tree_const_iterator(base_ptr x) : _node(x) {}

  _rb_tree_const_iterator(const iterator& it) : _node(it._node) {}

  /**
   * @brief const iterator to iterator casting
   *
   * @return iterator
   */
  iterator _const_cast(void) const {
    return iterator(const_cast<typename iterator::base_ptr>(_node));
  }

  reference operator*(void) const {
    return static_cast<link_type>(_node)->value;
  }

  pointer operator->(void) const {
    return &(static_cast<link_type>(_node)->value);
  }

  self& operator++(void) {
    _node = _rb_tree_increment(_node);
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
  friend bool operator==(const self& lhs, const self& rhs) {
    return lhs._node == rhs._node;  // private
  }

  friend bool operator!=(const self& lhs, const self& rhs) {
    return !(lhs == rhs);
  }
};

// !SECTION: red-black tree iterator

// 트리 정보를  저장하는  header cell
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
   * 초기 상태 : root 는 NIL, leftmost, rightmost 는 header 자신 (end)
   * 삽입 : left-most, right-most 를 갱신
   *
   */
  void _reset(void) {
    _header.parent = NIL;
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

  _rb_tree_impl(void) {}
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
  _rb_tree(const _rb_tree& src) : _impl(src._impl) {}
  ~_rb_tree(void) { _erase(_root()); }

  _rb_tree& operator=(const _rb_tree& src) {
    if (this != &src) {
      _erase(_root());
      _impl._move_data(src._impl);
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
    return _insert_unique(val);
  }

  /**
   * @brief
   *
   * @param position
   * @param val
   * @return iterator 성공 -> 새 element, 실패 -> 기존 element
   */
  // iterator insert(iterator position, const value_type& val) {
  //  position 이 begin
  //  position 이 end
  //  else
  //}

  template <typename InputIterator>
  void insert(InputIterator first, InputIterator last) {
    for (; first != last; ++first) {
      insert(*first);
    }
  }

  void swap(_rb_tree& x) {
    /**
     * 1. 나는 비었는데 얘는 있음
     * 2. 나는 있는데 얘는 비었음
     * 3. 나도 얘도 있음
     * 4. 둘 다 없어
     *
     *
     * 비었을 경우 -> root 가 null. header (null, header, header)싱테
     * 아닌 애 거를 빈 애에 넣어주고.. 뭐쩌구~
     *
     * 둘 다 찼을 경우 -> 걍 헤더 세 개 스왑하면 안되나? 흠~ 헤더노드 스왑하면
     * 될 것 같은데 머임
     */
    if (_get_root() == NIL) {
      if (x._get_root() != NIL) {
        _impl_move_data(x);
      }
    } else if (x._get_root() == NIL) {
      x._impl._move_data(_impl);
    } else {
      swap(_impl._header.parent, x._impl._header.parent);
      swap(_impl._header.left, x._impl._header.left);
      swap(_impl._header.right, x._impl._header.right);

      _get_root()->parent = &_impl._header;  // header reset
      x._get_root()->parent = &x._impl._header;
      swap(_impl._node_count, x._impl._node_count);
      swap(_impl._compare, x._impl._compare);
    }
  }

  // NOTHROW
  void clear(void) {
    _erase(_get_root());
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
    // 1 에서 || 이므로 key <= gte 인 것은 확실함!
    // 거기서 key < gte 가 true 이면 같은 것이 아니라 더 큰 것이므로 return end
    return (gte == end() || _impl._compare(key, _get_key(*gte._node))) ? end()
                                                                       : gte;
  }

  const_iterator find(const key_type& key) const {
    const_iterator gte = lower_bound(key);
    return (gte == end() || _impl._compare(key, _get_key(*gte._node))) ? end()
                                                                       : gte;
  }

  // distance of equal_range
  size_type count(const key_type& key) const {
    pair<const_iterator, const_iterator> p = equal_range(key);
    return static_cast<size_type>(std::distance(p.first, p.second));
  }

  /**
   * @brief key 보다 같거나 큰 노드 중 가장 작은 노드 반환.
   * Compare : comp(a,b) 는 a 가 b 보다 앞에 있다고 판단되면 true 를 리턴한다.
   * 기준은 함수가 정의한 strict weak ordering 에 따른다.
   * ... 이기 때문에 compare true -> small, false-> big 임.
   *
   * @param key
   * @return iterator
   */
  iterator lower_bound(const key_type& key) {
    link_type x = _get_root();
    link_type y = end();
    while (x != NIL) {
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
    // casting 하는 이유 - 부모 -> 자식은 형변환이 안돼서..
    link_type x = static_cast<link_type>(_get_root());
    link_type y = end();
    while (x != NIL) {
      // x.key >= key -> go left (left most 찾아야 함)
      if (!_impl._compare(_get_key(x), key)) {
        y = x;  // update
        x = _left(x);
      } else {
        // x.key < key => go right (큰 것 찾아야 함)
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
    link_type x = _get_root();
    link_type y = end();
    while (x != NIL) {
      // key < x.key
      if (_impl._compare(key, _get_key(x))) {
        y = x;
        x = _left(x);
      } else {
        // key >= x.key
        x = _right(x);
      }
    }
    return iterator(y);
  }

  const_iterator upper_bound(const key_type& key) const {
    link_type x = _get_root();
    link_type y = end();
    while (x != NIL) {
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
    return make_pair(lower_bound(key), upper_bound(key));
  }

  pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
    return make_pair(lower_bound(key), upper_bound(key));
  }

  node_allocator get_node_allocator(void) { return _alloc; }

 private:
  // SECTION : about elements
  base_ptr _get_root(void) { return _impl._header.parent; }
  base_ptr _get_left_most(void) { return _impl._header.left; }
  base_ptr _get_end(void) { return _impl._header.right; }

  key_type _get_key(const_link_type x) const { return KeyOfValue()(x->value); }

  key_type _get_key(const_base_ptr x) const {
    return _get_key(static_cast<const_link_type>(x));
  }

  link_type _root(void) const {
    return static_cast<link_type>(_impl._header.parent);
  }

  static link_type _left(base_ptr x) { return static_cast<link_type>(x->left); }
  static const_link_type _left(const_base_ptr x) {
    return static_cast<const_link_type>(x->left);
  }

  static link_type _right(base_ptr x) {
    return static_cast<link_type>(x->right);
  }
  static const_link_type _right(const_base_ptr x) {
    return static_cast<const_link_type>(x->right);
  }

  static link_type _parent(base_ptr x) {
    return static_cast<link_type>(x->parent);
  }

  static const_link_type _parent(const_base_ptr x) {
    return static_cast<const_link_type>(x->parent);
  }

  link_type _create_node(const value_type& value) {
    link_type tmp = _alloc.allocate(1);
    _construct_node(tmp, value);
    return tmp;
  }

  // value-> pair (map)
  void _construct_node(link_type node, const value_type& value) {
    try {
      _alloc.construct(node, value);
    } catch (...) {
      _deallocate_node(node);
      throw;
    }
  }

  void _destroy_node(link_type node) {
    _alloc.destroy(node);
    _deallocate_node(node);
  }

  void _deallocate_node(link_type node) { _alloc.deallocate(node, 1); }

  /**
   * @brief node 를 기준으로 하는 subtree 를 모두 삭제한다.
   *
   * @param node
   */
  void _erase(link_type node) {
    // 재귀
    // if (node == NIL) return;
    //_erase(node->left);  // const 어캄이거 아니? 그럴 일 없다.
    //_erase(node->right);
    //_destroy_node(node);
    // node = NIL;

    // 재귀 + while  왜지?
    while (node != NIL) {
      _erase(_right(node));  // 왜 반만 재귀하지
      link_type tmp = _left(node);
      _destroy_node(node);
      node = tmp;
    }
  }

  /**
   * @brief find parent's position to insert new node
   *
   * @param key
   * @return pair<base_ptr, base_ptr>
   */
  pair<base_ptr, base_ptr> _find_insert_pos(const key_type& key) {
    typedef pair<base_ptr, base_ptr> pair_type;

    base_ptr x = _get_root();
    base_ptr y = _get_end();  // header
    bool comp = true;
    while (x != NIL) {
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
    if (_impl._compare(_get_key(tmp._node), key)) {
      return pair_type(x, y);
    }
    // tmp == key
    return pair_type(tmp._node, NIL);
  }

  // compexity O(log N)
  pair<iterator, bool> _insert_unique(const value_type& value) {
    key_type key = KeyOfValue()(value);
    pair<base_ptr, base_ptr> res = _find_insert_pos(key);
    if (res.second) {
      return pair<iterator, bool>(_insert(res.first, res.second, value), true);
    }
    return pair<iterator, bool>(iterator(res.first), false);
  }

  /**
   * @brief 무조건 삽입하는 경우만 들어오는 데 왜지?
   *
   * @param x
   * @param p
   * @param value
   * @return iterator insert 한 node 의 iterator
   */
  iterator _insert(base_ptr x, base_ptr p, const value_type& value) {
    // bool _insert_left = (x != NIL || p == end() ||
    //                      _impl._compare(_get_key(value), _get_key(*p)));
    bool _insert_left = (x != NIL || p == end()._node ||
                         _impl._compare(KeyOfValue()(value), _get_key(p)));
    link_type node = _create_node(value);
    _insert_rebalance(_insert_left, node, p, this->_impl._header);
    ++_impl._node_count;

    return iterator(node);
  }
};
// !SECTION: red-black tree

_rb_tree_node_base* _left_most(_rb_tree_node_base* x);

const _rb_tree_node_base* _left_most(const _rb_tree_node_base* x);

_rb_tree_node_base* _right_most(_rb_tree_node_base* x);
const _rb_tree_node_base* _right_most(const _rb_tree_node_base* x);

_rb_tree_node_base* _rb_tree_increment(_rb_tree_node_base* x);

const _rb_tree_node_base* _rb_tree_increment(const _rb_tree_node_base* x);

_rb_tree_node_base* _rb_tree_decrement(_rb_tree_node_base* x);

const _rb_tree_node_base* _rb_tree_decrement(const _rb_tree_node_base* x);

/**
 * @brief subtree 를 왼쪽으로 회전
 *
 * @param x
 * @param root
 */
void _rb_tree_rotate_left(_rb_tree_node_base* const x,
                          _rb_tree_node_base*& root);

void _rb_tree_rotate_right(_rb_tree_node_base* const x,
                           _rb_tree_node_base*& root);

void _rb_tree_recolorize(_rb_tree_node_base* x, _rb_tree_node_base* xp);

_rb_tree_node_base* _rb_tree_subtree_min(_rb_tree_node_base* x);

_rb_tree_node_base* _rb_tree_subtree_max(_rb_tree_node_base* x);

void _insert_rebalance(bool left, _rb_tree_node_base* x, _rb_tree_node_base* p,
                       _rb_tree_node_base& header);
_rb_tree_node_base* _find_successor(_rb_tree_node_base* x);

}  // namespace ft

#endif  // _RB_TREE_HPP_
