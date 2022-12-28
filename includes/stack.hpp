/**
 * @file stack.hpp
 * @author jiskim
 * @brief
 * @date 2022-12-28
 *
 * @copyright Copyright (c) 2022
 */

#ifndef STACK_HPP
#define STACK_HPP

#include "vector.hpp"

namespace ft {

template <class T, class Container = vector<T> >
class stack {
 public:
  typedef Container container_type;
  typedef typename Container::value_type value_type;
  typedef typename Container::size_type size_type;
  typedef typename Container::reference reference;
  typedef typename Container::const_reference const_reference;

 protected:
  container_type c;

 public:
  explicit stack(const container_type& c = container_type()) {}

  bool empty() const { return c.empty(); }
};

}  // namespace ft

#endif  // STACK_HPP
