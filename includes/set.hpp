/**
 * @file set.hpp
 * @author jiskim
 * @brief set container
 * @date 2023-01-23
 *
 * @copyright Copyright (c) 2023
 */

#ifndef SET_HPP
#define SET_HPP

#include "_rb_tree.hpp"
#include "function.hpp"

namespace ft {

// SECTION: set
template <typename T, typename Compare = std::less<T>,
          typename Alloc = std::allocator<T> >
class set {
 public:
  typedef T key_type;
  typedef T value_type;
  typedef Compare key_compare;
  typedef Compare value_compare;
  typedef Alloc allocator_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;

  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

 private:
  typedef _rb_tree<key_type, value_type, _Identity<value_type>, key_compare,
                   Alloc>
      _rep_type;
  _rep_type _tree;

 public:
  typedef typename _rep_type::const_iterator iterator;
  typedef typename _rep_type::const_iterator const_iterator;
  typedef typename _rep_type::const_reverse_iterator reverse_iterator;
  typedef typename _rep_type::const_reverse_iterator const_reverse_iterator;

  // SECTION: constructor and destructor
  // STRONG
  set(void) : _tree() {}

  explicit set(const Compare& comp, const Alloc& alloc = Alloc())
      : _tree(comp, alloc) {}

  template <typename InputIterator>
  set(InputIterator first, InputIterator last, const Compare& comp = Compare(),
      const Alloc& alloc = Alloc())
      : _tree(comp, alloc) {
    _tree.insert(first, last);
  }

  set(const set& x) : _tree(x._tree) {}

  // NOTHROW
  ~set(void) {}
  // !SECTION: constructor and destructor

  // BASIC
  set& operator=(const set& x) {
    _tree = x._tree;
    return *this;
  }

  // SECTION: iterators
  iterator begin(void) { return _tree.begin(); }
  const_iterator begin(void) const { return _tree.begin(); }

  iterator end(void) { return _tree.end(); }
  const_iterator end(void) const { return _tree.end(); }

  reverse_iterator rbegin(void) { return _tree.rbegin(); }
  const_reverse_iterator rbegin(void) const { return _tree.rbegin(); }

  reverse_iterator rend(void) { return _tree.rend(); }
  const_reverse_iterator rend(void) const { return _tree.rend(); }
  // !SECTION: iterators

  // SECTION: capacity
  bool empty(void) const { return _tree.empty(); }

  size_type size(void) const { return _tree.size(); }

  size_type max_size(void) const { return _tree.max_size(); }
  // !SECTION: capacity

  // SECTION: modifiers
  pair<iterator, bool> insert(const value_type& val) {
    return _tree.insert(val);
  }

  iterator insert(iterator position, const value_type& val) {
    return _tree.insert(position._const_cast(), val);
  }

  template <typename InputIterator>
  void insert(InputIterator first, InputIterator last) {
    _tree.insert(first, last);
  }

  void erase(iterator position) { _tree.erase(position._const_cast()); }

  size_type erase(const key_type& key) { return _tree.erase(key); }

  void erase(iterator first, iterator last) {
    while (first != last) {
      _tree.erase((first++)._const_cast());
    }
  }

  void swap(set& x) { _tree.swap(x._tree); }

  void clear(void) { _tree.clear(); }
  // !SECTION: modifiers

  // SECTION: observers
  key_compare key_comp(void) const { return key_compare(); }

  /**
   * @brief 컨테이너의 comparison object 의 복사본을 리턴한다.
   * set 에서 value_compare 는 key_compare 와 동일하다.
   *
   * @return value_compare
   */
  value_compare value_comp(void) const { return key_compare(); }
  // !SECTION: observers

  // SECTION: operations
  iterator find(const value_type& val) const { return _tree.find(val); }

  // STRONG
  size_type count(const value_type& val) const { return _tree.count(val); }

  iterator lower_bound(const value_type& val) const {
    return _tree.lower_bound(val);
  }

  iterator upper_bound(const value_type& val) const {
    return _tree.upper_bound(val);
  }

  pair<iterator, iterator> equal_range(const value_type& val) const {
    return _tree.equal_range(val);
  }
  // !SECTION: operations

  // SECTION: allocator
  allocator_type get_allocator(void) const { return _tree.get_allocator(); }
  // !SECTION: allocator

  template <typename T1, typename Compare1, typename Alloc1>
  friend bool operator==(const set<T1, Compare1, Alloc1>& lhs,
                         const set<T1, Compare1, Alloc1>& rhs);

  template <typename T1, typename Compare1, typename Alloc1>
  friend bool operator<(const set<T1, Compare1, Alloc1>& lhs,
                        const set<T1, Compare1, Alloc1>& rhs);
};

// SECTION: non-member function
template <typename T, typename Compare, typename Alloc>
bool operator==(const set<T, Compare, Alloc>& lhs,
                const set<T, Compare, Alloc>& rhs) {
  return lhs._tree == rhs._tree;
}

template <typename T, typename Compare, typename Alloc>
bool operator!=(const set<T, Compare, Alloc>& lhs,
                const set<T, Compare, Alloc>& rhs) {
  return !(lhs == rhs);
}

template <typename T, typename Compare, typename Alloc>
bool operator<(const set<T, Compare, Alloc>& lhs,
               const set<T, Compare, Alloc>& rhs) {
  return lhs._tree < rhs._tree;
}

template <typename T, typename Compare, typename Alloc>
bool operator>(const set<T, Compare, Alloc>& lhs,
               const set<T, Compare, Alloc>& rhs) {
  return rhs < lhs;
}

template <typename T, typename Compare, typename Alloc>
bool operator<=(const set<T, Compare, Alloc>& lhs,
                const set<T, Compare, Alloc>& rhs) {
  return !(rhs < lhs);
}

template <typename T, typename Compare, typename Alloc>
bool operator>=(const set<T, Compare, Alloc>& lhs,
                const set<T, Compare, Alloc>& rhs) {
  return !(lhs < rhs);
}

template <typename T, typename Compare, typename Alloc>
void swap(set<T, Compare, Alloc>& lhs, set<T, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}
// !SECTION: non-member function
// !SECTION: set

}  // namespace ft

#endif  // SET_HPP
