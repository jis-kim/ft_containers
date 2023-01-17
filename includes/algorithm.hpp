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

template <typename T>
void _swap(T& a, T& b) {
  T tmp(a);
  a = b;
  b = tmp;
}

template <typename InputIterator1, typename InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
  while (first1 != last1) {
    if (!(*first1 == *first2)) {
      return false;
    }
    ++first1;
    ++first2;
  }
  return true;
}

template <typename InputIterator1, typename InputIterator2,
          typename BinaryPredicate>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
           BinaryPredicate pred) {
  while (first1 != last1) {
    if (!pred(*first1, *first2)) {
      return false;
    }
    ++first1;
    ++first2;
  }
  return true;
}

template <typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2) {
  while (first1 != last1) {
    if (first2 == last2 || *first2 < *first1) {
      return false;
    } else if (*first1 < *first2) {
      return true;
    }
    ++first1;
    ++first2;
  }
  return (first2 != last2);
}

template <typename InputIterator1, typename InputIterator2, typename Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             Compare comp) {
  while (first1 != last1) {
    if (first2 == last2 || comp(first2, *first1)) {
      return false;
    } else if (comp(*first1, *first2)) {
      return true;
    }
    ++first1;
    ++first2;
  }
  return (first2 != last2);
}

}  // namespace ft

#endif  // ALGORITHM_HPP
