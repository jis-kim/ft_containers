
#include "pair.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

void pair_test(void) {
  std::cout << ft::make_pair(10, 12) << '\n';
  std::cout << '\n';

  ft::pair<int, int> pr(1111, 312412);
  std::cout << pr << '\n';
  std::cout << ft::make_pair("abc", "defg") << '\n';
}
