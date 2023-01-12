#include <iostream>
#include <map>
// #include <vector>

#include "map.hpp"
#include "testheader/print_vector.hpp"
#include "vector.hpp"

const std::string a = "abcdefg";

enum hi { first = true, second };

int main(void) {
  type_traits_test();
  vector_test();
  vector_iterator_test();

  ft::vector<const int> const_vec;

  std::allocator<const int> alloc = const_vec.get_allocator();

  alloc.construct(&(*(const_vec.begin())));
  const int ad = 10;
  std::cout << alloc.address(ad) << '\n';

  pair_test();

  ft::map<int, std::string> map;
}
