/**
 * @file iterator_traits.hpp
 * @author jiskim
 * @brief
 * @date 2022-11-18
 *
 * @copyright Copyright (c) 2022
 */

#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cstddef>   // ptrdiff_t
#include <iterator>  // std::random_access_iterator_tag

namespace ft {

//// SECTION : iterator base class
//// don't have any funtionality of iterator
// template <typename Category, class T, class Distance = ptrdiff_t,
//           class Pointer = T*, class Reference = T&>
// struct iterator {
//   typedef T value_type;
//   typedef Distance diffrence_type;
//   typedef Pointer pointer;
//   typedef Reference reference;
//   typedef Category iterator_category;
// };

// SECTION : iterator_traits
template <typename Iterator>
struct iterator_traits {
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
  typedef typename Iterator::iterator_category iterator_category;
};

template <typename T>
class iterator_traits<T*> {
 public:
  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef std::random_access_iterator_tag iterator_category;
};

template <typename T>
class iterator_traits<const T*> {
 public:
  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef const T* pointer;
  typedef const T& reference;
  typedef std::random_access_iterator_tag iterator_category;
};

}  // namespace ft

#endif  // ITERATOR_HPP
