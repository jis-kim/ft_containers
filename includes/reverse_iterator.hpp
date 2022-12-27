/**
 * @file reverse_iterator.hpp
 * @author jiskim
 * @brief reverse direction to bidirectional or random access iterator
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022
 */

#ifndef REVERSE_ITERATOR_HPP
#define REVERSE_ITERATOR_HPP

#include "iterator_traits.hpp"

namespace ft {
template <typename Iter>
class reverse_iterator {
 private:
  Iter _current;

 public:
  typedef iterator_traits<Iter> traits_type;

  typedef Iter iterator_type;
  typedef typename traits_type::difference_type difference_type;
  typedef typename traits_type::value_type value_type;
  typedef typename traits_type::pointer pointer;
  typedef typename traits_type::reference reference;
  typedef typename traits_type::iterator_category iterator_category;

  // TODO : member function
  /**
   * constructor
   * base
   * operator*
   * +
   * ++
   * +=
   * -
   * --
   * -=
   * ->
   * []
   */
  // same as copy_constructor of base iterator

  reverse_iterator(void) {}

  explicit reverse_iterator(iterator_type it) : _current(it) {}

  template <typename Iter2>
  reverse_iterator(const reverse_iterator<Iter2>& rev_it)
      : _current(rev_it.base()) {}

  iterator_type base(void) const { return _current; }

  reference operator*(void) const {
    iterator_type tmp = _current;
    return *--tmp;
  }

  reverse_iterator operator+(difference_type n) const {
    return reverse_iterator(_current - n);
  }

  reverse_iterator& operator++(void) {
    --_current;
    return *this;
  }

  reverse_iterator operator++(int) {
    reverse_iterator tmp = *this;
    --_current;
    return tmp;
  }

  reverse_iterator& operator+=(difference_type n) {
    _current -= n;
    return *this;
  }

  reverse_iterator operator-(difference_type n) const {
    return reverse_iterator(_current + n);
  }

  reverse_iterator& operator--(void) {
    ++_current;
    return *this;
  }

  reverse_iterator operator--(int) {
    reverse_iterator tmp = *this;
    ++_current;
    return tmp;
  }

  reverse_iterator& operator-=(difference_type n) {
    _current += n;
    return *this;
  }

  reference operator[](difference_type n) const { return *(*this + n); }

  pointer operator->(void) const { return &(operator*()); }
};

// TODO : non-member
/**
 * relational operators
 * operator+
 * operator-
 *
 */

template <typename Iter1, typename Iter2>
bool operator==(const reverse_iterator<Iter1>& lhs,
                const reverse_iterator<Iter2>& rhs) {
  return lhs.base() == rhs.base();
}

template <typename Iter1, typename Iter2>
bool operator!=(const reverse_iterator<Iter1>& lhs,
                const reverse_iterator<Iter2>& rhs) {
  return lhs.base() != rhs.base();
}

template <typename Iter1, typename Iter2>
bool operator<(const reverse_iterator<Iter1>& lhs,
               const reverse_iterator<Iter2>& rhs) {
  return lhs.base() > rhs.base();
}

template <typename Iter1, typename Iter2>
bool operator<=(const reverse_iterator<Iter1>& lhs,
                const reverse_iterator<Iter2>& rhs) {
  return lhs.base() >= rhs.base();
}

template <typename Iter1, typename Iter2>
bool operator>(const reverse_iterator<Iter1>& lhs,
               const reverse_iterator<Iter2>& rhs) {
  return lhs.base() < rhs.base();
}

template <typename Iter1, typename Iter2>
bool operator>=(const reverse_iterator<Iter1>& lhs,
                const reverse_iterator<Iter2>& rhs) {
  return lhs.base() <= rhs.base();
}

template <typename Iter>
reverse_iterator<Iter> operator+(
    typename reverse_iterator<Iter>::difference_type n,
    const reverse_iterator<Iter>& rev_it) {
  return reverse_iterator<Iter>(rev_it.base() - n);
}

template <typename Iter1, typename Iter2>
typename reverse_iterator<Iter1>::difference_type operator-(
    const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
  return rhs.base() - lhs.base();
}

template <typename Iter>
reverse_iterator<Iter> operator-(
    typename reverse_iterator<Iter>::difference_type n,
    const reverse_iterator<Iter>& rev_it) {
  return reverse_iterator<Iter>(rev_it.base() + n);
}

}  // namespace ft

#endif  // REVERSE_ITERATOR_HPP
