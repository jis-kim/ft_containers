/**
 * @file algorithm.hpp
 * @author jiskim
 * @brief
 * @date 2022-12-16
 *
 * @copyright Copyright (c) 2022
 */

#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

namespace ft {

template <typename T>
const T& min(const T& a, const T& b) {
  return (a < b ? a : b);
}

template <typename T>
const T& max(const T& a, const T& b) {
  return (a > b ? a : b);
}

}  // namespace ft

#endif  // ALGORITHM_HPP
