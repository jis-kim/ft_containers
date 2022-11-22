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
// Random access iterator
// T is pointer of elements
template <typename Iter>
class vector_iterator {
 private:
  Iter current_;  // pointer of elements

 public:
  typedef iterator_traits<Iter> traits_type;

  typedef typename traits_type::difference_type difference_type;
  typedef typename traits_type::value_type value_type;
  typedef typename traits_type::pointer pointer;
  typedef typename traits_type::reference reference;
  typedef typename traits_type::iterator_category iterator_category;

  typedef vector_iterator self;

  vector_iterator(void) {}
  vector_iterator(const Iter& current) : current_(current) {}
  //  이게 왜 없지
  //  builtin 써도 상관 없을 수도? deep copy 가 더 이상한 듯
  // vector_iterator(const vector_iterator<T> rhs) : current_(rhs.current_) {}
  //  vector_iterator& operator=(const vector_iterator<T> rhs) {}
  ~vector_iterator(void) {}

  reference operator*(void) const { return *current_; }
  pointer operator->(void) const { return current_; }

  self& operator++(void) {
    ++current_;
    return *this;
  }
  self operator++(int) { return vector_iterator(current_++); }

  self& operator--(void) {
    --current_;
    return *this;
  }
  self operator--(int) { return vector_iterator(current_--); }

  // NOTE : 이 둘은 iterator + n 의 경우고 n + iterator 도 가능해야 한다.
  self operator+(difference_type n) const {
    return vector_iterator(current_ + n);
  }
  self operator-(difference_type n) const {
    return vector_iterator(current_ - n);
  }

  self& operator+=(difference_type n) {
    current_ += n;
    return *this;
  }
  self& operator-=(difference_type n) {
    current_ -= n;
    return *this;
  }
  reference operator[](difference_type n) const { return current_[n]; }

  // access to value
  const Iter& base(void) const { return current_; }
};

/*
// template <typename T>
// typename vector_iterator<T>::self operator+(
//     const vector_iterator<T>::diffrence_type n,
//     const vector::iterator<T>& iter) {}

// SECTION : comparison operators

template <typename T, typename U>
bool operator==(const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() == rhs.base();
}

template <typename T, typename U>
bool operator!=(const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() != rhs.base();
}

template <typename T, typename U>
bool operator<(const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() < rhs.base();
}

template <typename T, typename U>
bool operator>(const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() > rhs.base();
}

template <typename T, typename U>
bool operator<=(const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() <= rhs.base();
}

template <typename T, typename U>
bool operator>=(const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() >= rhs.base();
}

// SECTION : vector base
template <typename T, typename Allocator = std::allocator<T> >
class vector_base {
 protected:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename allocator_type::reference reference;              // T&
  typedef typename allocator_type::const_reference const_reference;  // const T&
  typedef typename allocator_type::pointer pointer;                  // T*
  typedef typename allocator_type::const_pointer const_pointer;      // const T*

  allocator_type alloc;
  pointer begin;
  pointer end;
  pointer space;

  vector_base(const allocator_type& allocator,
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
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;

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

  template <typename InputIterator>
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
  template <typename InputIterator>
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
  template <typename InputIterator>
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
template <typename T, typename Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <typename T, typename Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <typename T, typename Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <typename T, typename Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <typename T, typename Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <typename T, typename Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

// NOTHROW allocator in both vectors compare equal
// otherwise UB
template <typename T, typename Alloc>
void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {}
*/

}  // namespace ft

#endif  // VECTOR_HPP
