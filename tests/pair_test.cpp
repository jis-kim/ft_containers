
#include "pair.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

template <typename T, typename U>
void print_pair(ft::pair<T, U> pr) {
  std::cout << "[ " << pr.first << ", " << pr.second << " ]" << std::endl;
}

void pair_test(void) {
  print_pair(ft::make_pair(10, 12));
  std::cout << '\n';

  ft::pair<int, int> pr(1111, 312412);
  print_pair(ft::make_pair("abc", "defg"));

  print_pair(pr);
}
