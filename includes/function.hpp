/**
 * @file function.hpp
 * @author jiskim
 * @brief decalre functors
 * @date 2023-01-10
 *
 * @copyright Copyright (c) 2023
 */

#ifndef FUNCTION_HPP_
#define FUNCTION_HPP_

namespace ft {
template <typename Pair>
struct _SelectKey {
  typename Pair::first_type operator()(const Pair& x) const { return x.first; }

  const typename Pair::first_type& operator()(const Pair& x) const {
    return x.first;
  }
};

template <typename T>
struct _Identity {
  T& operator()(T& x) const { return x; }
  const T& operator()(const T& x) const { return x; }
};

}  // namespace ft

#endif  // FUNCTION_HPP_
