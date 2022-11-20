/**
 * @file vector.hpp
 * @author jiskim
 * @brief vector container
 * @date 2022-11-12
 *
 * @copyright Copyright (c) 2022
 */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>

#include "iterator.hpp"

namespace ft {

// SECTION : vector iterator
template <class T>
struct vector_iterator {
  typedef typename iterator_traits<T>::difference_type difference_type;
  typedef typename iterator_traits<T>::value_type value_type;
  typedef typename iterator_traits<T>::pointer pointer;
  typedef typename iterator_traits<T>::reference reference;
  typedef typename iterator_traits<T>::iterator_category iterator_category;
};

// SECTION : vector base
template <typename T, class Allocator = std::allocator<T> >
struct vector_base {
 protected:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef allocator_type::pointer pointer;
  typedef allocator_type::const_pointer const_pointer;

  allocator_type alloc;
  pointer begin;
  pointer end;
  pointer space;

  vector_base(const& alocator_type allocator,
              typename allocator_type::size_type n)
      : alloc(allocator),
        begin(alloc.allocate(n)),
        end(begin),
        space(begin + n) {}
  ~vector_base(void) {
    alloc.destroy(begin);
    alloc.deallocate(begin);
  }
};

// SECTION : real vector
template <typename T, typename Allocator = std::allocator<T> >
class vector : private vector_base<T, Allocator> {
 public:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef allocator_type::pointer pointer;
  typedef allocator_type::const_pointer const_pointer;

  typedef vector_iterator<pointer> iterator;
  typedef vector_iterator<const_pointer> const_iterator;
  // typedef std::reverse_iterator<iterator> reverse_iterator;
  // typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  typedef ptrdiff_t difference_type;
  typedef size_t size_type;

  // SECTION : public member functions
  // STRONG
  explicit vector(const allocator_type& alloc = allocator_type())
      : vector_base(alloc, size_type()) {}

  explicit vector(size_type n, const value_type& val = value_type(),
                  const allocator_type& alloc = allocator_type())
      : vector_base(alloc, n) {
    for (size_type i = 0; i < n; ++i) {
      alloc.construct(begin + i, val);
    }
  }

  template <class InputIterator>
  vector(InputIterator first, InputIterator last,
         const allocator_type& alloc = allocator_type())
      : vector_base(alloc, last - first) {
    for (InputIterator it = first; it < last; ++it) {
    }
  }

  vector(const vector& x) : vector_base(alloc, x.size()) {
    for (size_type i = 0; i < x.size(); i++) {
      alloc.construct(begin + i, val);
    }
  }

  // SECTION: destructor
  // NOTHROW
  ~vector(void) {}

  // BASIC
  vector& operator=(const vector& x) {}

  // SECTION : iterator

  // NOTHROW
  iterator begin(void) {}
  const_iterator begin(void) const {}

  // NOTHROW
  iterator end(void) {}
  const_iterator end(void) const {}

  // NOTHROW
  iterator rbegin(void) {}
  const_iterator rbegin(void) const {}

  // NOTHROW
  iterator rend(void) {}
  const_iterator rend(void) const {}

  // SECTION : capacity

  // NOTHROW
  size_type size(void) const {}

  // NOTHROW
  size_type max_size(void) const {}

  // NOTHROW n <= size
  // STRONG n > size and reallocation required, type of elements is copyable
  // BASIC otherwise
  void resize(size_type n, value_type val = value_type()) {}

  // NOTHROW
  size_type capacity(void) const {}

  // NOTHROW
  bool empty(void) const {}

  // STRONG n > size and reallocation required, type of elements is copyable
  // BASIC otherwise
  void reserve(size_type n) {}

  // SECTION : element access

  // NOTHROW size > n
  // otherwise UB
  reference operator[](size_type n) {}
  const_reference operator[](size_type n) const {}

  // STRONG
  reference at(size_type n) {}
  const_reference at(size_type n) const {}

  // NOTHROW container is not empty
  // otherwise UB
  reference front(void) {}
  const_reference front(void) const {}

  // NOTHROW container is not empty
  // otherwise UB
  reference back(void) {}
  const_reference back(void) const {}

  // NOTHROW
  value_type* data() {}
  const value_type* data() const {}

  // SECTION : modifiers
  // BASIC
  // if [first, last) is not valid UB
  template <class InputIterator>
  void assign(InputIterator first, InputIterator last) {}
  void assign(size_type n, const value_type& val) {}

  // STRONG 1. no reallocation 2. reallocation happens & elements copyable
  // BASIC otherwise
  void push_back(const value_type& val) {}

  // NOTHROW container is not empty
  // otherwise UB
  void pop_back(void) {}

  // STRONG 1. insert single element at the end, no reallocations happen
  // 2. reallocation happens & elements copyable
  // BASIC otherwise
  iterator insert(iterator position, const value_type& val) {}
  void insert(iterator position, size_type n, const value_type& val) {}
  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last) {}

  // NOTHROW removed elements include the last element
  // BASIC otherwise
  // invalid position or range UB
  iterator erase(iterator position) {}
  iterator erase(iterator first, iterator last) {}

  // NOTHROW allocator in both vectors compare equal
  // otherwise UB
  void swap(vector& x) {}

  // NOTHROW
  void clear(void) {}

  // NOTHROW
  allocator_type get_allocator(void) const {}
};

// SECTION : non-member function
// SECTION : relational operators
// NOTHROW if the type of elements supports operations
template <class T, class Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <class T, class Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <class T, class Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <class T, class Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <class T, class Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <class T, class Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

// NOTHROW allocator in both vectors compare equal
// otherwise UB
template <class T, class Alloc>
void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {}

}  // namespace ft

#endif  // VECTOR_HPP
