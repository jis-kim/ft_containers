/**
 * @file iterator_traits.hpp
 * @author jiskim
 * @brief iterator traits
 * @date 2022-11-18
 *
 * @copyright Copyright (c) 2022
 */

#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

#include <cstddef>   // ptrdiff_t
#include <iterator>  // std::random_access_iterator_tag

namespace ft {

template <typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
          typename _Pointer = _Tp*, typename _Reference = _Tp&>
struct iterator {
  typedef _Tp value_type;
  typedef _Distance difference_type;
  typedef _Pointer pointer;
  typedef _Reference reference;
  typedef _Category iterator_category;
};

// SECTION : iterator_traits
template <typename Iterator>
struct iterator_traits {
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
  typedef typename Iterator::iterator_category iterator_category;
};

// pointer 로 들어왔을 때도 작동해야 한다.
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

#endif  // ITERATOR_TRAITS_HPP
