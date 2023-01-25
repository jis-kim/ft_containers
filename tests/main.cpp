#include <iostream>
#include <map>
// #include <vector>
#include "testheader/tree_test.hpp"
#include "testheader/vector_test.hpp"
#include "vector.hpp"

const std::string a = "abcdefg";

enum hi { first = true, second };

int main(void) {
  type_traits_test();
  vector_test();
  vector_iterator_test();
  pair_test();
  tree_test();
  map_test();
  system("leaks ft_containers");
}
