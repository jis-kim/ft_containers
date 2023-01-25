/**
 * @file pair.hpp
 * @author jiskim
 * @brief pair
 * @date 2023-01-06
 *
 * @copyright Copyright (c) 2023
 */

#ifndef PAIR_HPP
#define PAIR_HPP

namespace ft {
template <typename T1, typename T2>
struct pair {
  typedef T1 first_type;
  typedef T2 second_type;

  T1 first;
  T2 second;

  pair(void) : first(), second() {}

  template <typename T, typename U>
  pair(const pair<T, U>& pr) : first(pr.first), second(pr.second) {}

  pair(const T1& a, const T2& b) : first(a), second(b) {}

  ~pair(void) {}

  pair& operator=(const pair& pr) {
    first = pr.first;
    second = pr.second;
    return *this;
  }
};

template <typename T1, typename T2>
bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <typename T1, typename T2>
bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return !(lhs == rhs);
}

template <typename T1, typename T2>
bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  // first 가 작거나 first 는 같은데 second 가 작으면 true
  return lhs.first < rhs.first ||
         (!(rhs.first < lhs.first) && lhs.second < rhs.second);
}

template <typename T1, typename T2>
bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return !(rhs < lhs);
}

template <typename T1, typename T2>
bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return rhs < lhs;
}

template <typename T1, typename T2>
bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return !(lhs < rhs);
}

template <typename T1, typename T2>
pair<T1, T2> make_pair(T1 x, T2 y) {
  return pair<T1, T2>(x, y);
}

}  // namespace ft

#endif  // PAIR_HPP
