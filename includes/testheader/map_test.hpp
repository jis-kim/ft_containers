/**
 * @file map_test.hpp
 * @author jiskim
 * @brief ft map + std map
 * @date 2023-01-18
 *
 * @copyright Copyright (c) 2023
 */


#ifndef MAP_TEST_HPP_
#define MAP_TEST_HPP_

#include <iostream>

template <typename MapIter>
void print_map(MapIter begin, MapIter end) {
  std::cout << "map : ";
  for (MapIter it = begin; it != end; ++it) {
    std::cout << it->first << " ";
  }
  std::cout << std::endl;
}

#endif  // MAP_TEST_HPP_
