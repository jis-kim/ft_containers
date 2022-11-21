#include <iostream>

#include "vector.hpp"

int main(void) {
  int array[5] = {1, 2, 3, 4, 5};

  ft::vector_iterator<int*> it(array);

  ft::vector_iterator<int*> tmp(it);

  std::cout
      << "\n================= increment operator test =================\n";
  for (int i = 0; i < 4; ++i) {
    std::cout << "i is " << i << ", it is " << *it << '\n';
    it++;
  }

  std::cout
      << "\n================= decrement operator test =================\n";
  for (int i = 0; i < 4; ++i) {
    std::cout << "i is " << i << ", it is " << *it << '\n';
    it--;
  }

  std::cout << "\nreference & insertion & subscript";
  std::cout << "it[3] is " << it[3] << '\n';
  it[3] = 100;
  std::cout << "it[3] after insertion is " << it[3] << '\n';

  it += 2;
}
