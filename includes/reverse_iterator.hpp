/**
 * @file reverse_iterator.hpp
 * @author jiskim
 * @brief
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022
 */

#ifndef REVERSE_ITERATOR_HPP
#define REVERSE_ITERATOR_HPP

#include "iterator.hpp"

namespace ft {
// TODO : 상속? cplusplus 는 iterator <- reverse_iterator 로
// iterator 를 상속받는다.
template <typename Iter>
class reverse_iterator {
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
};

// TODO : non-member
/**
 * relational operators
 * operator+
 * operator-
 *
 */

}  // namespace ft
#endif  // REVERSE_ITERATOR_HPP
