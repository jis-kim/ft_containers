/**
 * @file map_test.cpp
 * @author jiskim
 * @brief
 * @date 2023-01-24
 *
 * @copyright Copyright (c) 2023
 */

#include "map.hpp"

#include <iostream>

#include "math.h"
#include "testheader/tree_test.hpp"

#define NUM 1000
#define RANGE 1000000

void map_test(void) {
  typedef ft::map<int, std::string> map_type;
  typedef map_type::value_type value_type;
  typedef map_type::iterator iterator;

  map_type map;
  iterator map_it;

  std::cout << "\n\n============= map random insert test ==============\n";

  srand(time(NULL));
  for (int i = 0; i < NUM; ++i) {
    pair<iterator, bool> result = map.insert(value_type(rand() % RANGE, "a"));
    std::cout << "insert " << (*(result.first)).first << " : "
              << ((result.second) ? "successed" : "failed") << '\n';
  }

  std::cout << "\n\ntotal success = " << map.size() << " of " << NUM << "\n\n";

  std::cout << "\n\n================================ < tree "
               "info > ================================\n\n";

  print_rb_tree<value_type>(map.end());

  map.erase(map.begin(), map.end());

  std::cout << "size : " << map.size() << '\n';
  print_rb_tree<value_type>(map.end());
}
