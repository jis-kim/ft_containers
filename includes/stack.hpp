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

template <typename T, typename Container = vector<T> >
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
  explicit stack(const container_type& ctnr = container_type()) : c(ctnr) {}

  bool empty(void) const { return c.empty(); }

  size_type size(void) const { return c.size(); }

  value_type& top(void) { return c.back(); }

  const value_type& top(void) const { return c.back(); }

  void push(const value_type& val) { c.push_back(val); }

  void pop(void) { c.pop_back(); }

  template <typename T1, typename Container1>
  friend bool operator==(const stack<T1, Container1>& lhs,
                         const stack<T1, Container1>& rhs);

  template <typename T1, typename Container1>
  friend bool operator<(const stack<T1, Container1>& lhs,
                        const stack<T1, Container1>& rhs);
};

template <typename T, typename Container>
bool operator==(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return lhs.c == rhs.c;
}

template <typename T, typename Container>
bool operator!=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return !(lhs == rhs);
}

template <typename T, typename Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
  return lhs.c < rhs.c;
}

template <typename T, typename Container>
bool operator<=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return !(rhs < lhs);
}

template <typename T, typename Container>
bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
  return rhs < lhs;
}

template <typename T, typename Container>
bool operator>=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return !(lhs < rhs);
}
}  // namespace ft

#endif  // STACK_HPP
