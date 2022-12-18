#include <iostream>
// #include <type_traits>

// struct A {};

// class B {};
#include <vector>

#include "type_traits.hpp"

int main(void) {
  // std::cout << "is_class type traits test!!\n";

  // std::cout << "class : " << std::boolalpha << std::is_class<A>::value
  //           << ", struct : " << std::is_class<B>::value << '\n';

  std::cout
      << "==================random access iterator test==================\n";
  std::vector<int> v;
  std::cout
      << "vector<int> iterator is random access iterator? : " << std::boolalpha
      << ft::is_random_access_iterator<std::vector<int>::iterator>::type::value
      << '\n';
  std::cout << "vector<int> iterator is input iterator? : " << std::boolalpha
            << ft::is_input_iterator<std::vector<int>::iterator>::type::value
            << '\n';
}
