/**
 * @file map.hpp
 * @author jiskim
 * @brief
 * @date 2023-01-02
 *
 * @copyright Copyright (c) 2023
 */

#ifndef MAP_HPP_
#define MAP_HPP_

#include <functional>  // std::less
#include <memory>      //std::allocator

namespace ft {

template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Alloc = std::allocator<std::pair<const Key, T>>>
class map {
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef const T* const_pointer;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  map(void) {}
  ~map(void) {}
};

}  // namespace ft

#endif  // MAP_HPP_
