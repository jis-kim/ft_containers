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

typedef ft::map<int, std::string> map_type;
typedef map_type::value_type value_type;
typedef map_type::iterator map_it;

void map_test(void) {
  map_type map;
  map_it it;

  std::cout << "\n\n================================ map N random insert test "
               "================================n";

  srand(time(NULL));
  for (int i = 0; i < NUM; ++i) {
    ft::pair<map_it, bool> result = map.insert(value_type(rand() % RANGE, "a"));
    // std::cout << "insert " << (*(result.first)).first << " : "
    //           << ((result.second) ? "successed" : "failed") << '\n';
  }

  std::cout << "\n\ntotal success = " << map.size() << " of " << NUM << "\n\n";

  std::cout << "\n\n================================ < tree "
               "info > ================================\n\n";

  // print_rb_tree(map.end());

  map.erase(map.begin(), map.end());

  std::cout << "map size after erase begin to end : " << map.size() << '\n';
  print_rb_tree(map.end());

  std::cout << "\n\n================================ map range construct test "
               "================================\n\n";
  value_type arr[10] = {value_type(1, "a"), value_type(2, "b"),
                        value_type(3, "c"), value_type(4, "d"),
                        value_type(5, "e"), value_type(6, "f"),
                        value_type(7, "g"), value_type(8, "h"),
                        value_type(9, "i"), value_type(10, "j")};

  map_type map2(arr, arr + 10);
  print_rb_tree(map2.end());
}
