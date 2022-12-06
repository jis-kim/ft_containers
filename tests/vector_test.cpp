#include "vector.hpp"

#include <iostream>
#include <string>
#include <vector>

int main(void) {
  std::vector<int> int_vec;

  std::cout << "begin : " << int_vec.begin().base() << "\n";
  std::cout << "end : " << int_vec.end().base() << "\n";
  std::cout << "capacity: " << int_vec.capacity() << "\n";

  for (int i = 0; i < 10; ++i) {
    int_vec.push_back(i);
  }

  int_vec.clear();
  std::cout << "\nafter insert & clear vector\n";
  std::cout << "\nbegin : " << int_vec.begin().base() << "\n";
  std::cout << "end : " << int_vec.end().base() << "\n";
  std::cout << "capacity: " << int_vec.capacity() << "\n";

  ft::vector<int> ft_int_vec;
  std::cout << "\nft::vector defalut constructor \n";
  std::cout << "\nbegin : " << ft_int_vec.begin().base() << "\n";
  std::cout << "end : " << ft_int_vec.end().base() << "\n";
  std::cout << "capacity: " << ft_int_vec.capacity() << "\n";
}
