#include <fstream>
#include <iostream>
// #include <type_traits>

// struct A {};

// class B {};
#include <list>
#include <map>
#include <string>
#include <vector>

#include "type_traits.hpp"
#include "vector.hpp"

#define L_CYAN "\033[1;96m"
#define RESET "\033[0m"

#define PRINT(...) std::cout, L_CYAN, __VA_ARGS__, RESET, "\n"

template <typename T>
void print_vector(T it, T ite) {
  while (it != ite) {
    std::cout << *it << ", ";
    ++it;
    if (it == ite) std::cout << "\n";
  }
}

template <typename vec>
void print_vector(vec &v) {
  PRINT("vector info (size, capacity) : (", v.size(), ", ", v.capacity(), ")");
}

int main(void) {
  // std::cout << "is_class type traits test!!\n";

  // std::cout << "class : " << std::boolalpha << std::is_class<A>::value
  //           << ", struct : " << std::is_class<B>::value << '\n';
  // print type id
  std::cout << "class : " << typeid(ft::vector<int>).name() << "\n";

  std::cout
      << "==================random access iterator test==================\n";
  std::vector<int> v;

  std::vector<int>::iterator it = v.begin();

  std::cout << "vector<int> iterator is input access iterator? : "
            << std::boolalpha
            << ft::is_base_of<std::forward_iterator_tag,
                              std::random_access_iterator_tag>::value
            << '\n';

  std::cout << "\n\n==================is_*_iterator test==================\n";
  std::cout << "\n\n1. vector iterator (random access iterator) test\n";

  std::cout
      << "vector<int> iterator is random access iterator? : " << std::boolalpha
      << ft::is_random_access_iterator<std::vector<int>::iterator>::type::value
      << '\n';

  std::cout
      << "vector<int> iterator is bidirectional access iterator? : "
      << std::boolalpha
      << ft::is_bidirectional_iterator<std::vector<int>::iterator>::type::value
      << '\n';

  std::cout << "vector<int> iterator is forward access iterator? : "
            << std::boolalpha
            << ft::is_forward_iterator<std::vector<int>::iterator>::type::value
            << '\n';

  std::cout << "vector<int> iterator is input access iterator? : "
            << std::boolalpha
            << ft::is_input_iterator<std::vector<int>::iterator>::type::value
            << '\n';

  std::cout << "\n\n2. list iterator (bidirectional iterator) test\n";

  std::cout
      << "list<int> iterator is random access iterator? : " << std::boolalpha
      << ft::is_random_access_iterator<std::list<int>::iterator>::type::value
      << '\n';

  std::cout
      << "list<int> iterator is bidirectional access iterator? : "
      << std::boolalpha
      << ft::is_bidirectional_iterator<std::list<int>::iterator>::type::value
      << '\n';

  std::cout << "list<int> iterator is forward access iterator? : "
            << std::boolalpha
            << ft::is_forward_iterator<std::list<int>::iterator>::type::value
            << '\n';

  std::cout << "list<int> iterator is input access iterator? : "
            << std::boolalpha
            << ft::is_input_iterator<std::list<int>::iterator>::type::value
            << '\n';

  std::cout << "\n\n3. map iterator (bidirectional iterator) test\n";

  std::cout << "map<int, int> iterator is random access iterator? : "
            << std::boolalpha
            << ft::is_random_access_iterator<
                   std::map<std::string, int>::iterator>::type::value
            << '\n';

  std::cout << "map<int, int> iterator is bidirectional access iterator? : "
            << std::boolalpha
            << ft::is_bidirectional_iterator<
                   std::map<std::string, int>::iterator>::type::value
            << '\n';

  std::cout << "map<int, int> iterator is forward access iterator? : "
            << std::boolalpha
            << ft::is_forward_iterator<
                   std::map<std::string, int>::iterator>::type::value
            << '\n';

  std::cout << "map<int, int> iterator is input access iterator? : "
            << std::boolalpha
            << ft::is_input_iterator<
                   std::map<std::string, int>::iterator>::type::value
            << '\n';

  std::cout << "\n\n4. map const iterator (bidirectional iterator) test\n";

  std::cout << "map<int, int> const iterator is random access iterator? : "
            << std::boolalpha
            << ft::is_random_access_iterator<
                   std::map<std::string, int>::const_iterator>::type::value
            << '\n';

  std::cout << "map<int, int> const iterator is bidirectional access iterator? "
               ": "
            << std::boolalpha
            << ft::is_bidirectional_iterator<
                   std::map<std::string, int>::const_iterator>::type::value
            << '\n';

  std::cout << "map<int, int> const iterator is forward access iterator? : "
            << std::boolalpha
            << ft::is_forward_iterator<
                   std::map<std::string, int>::const_iterator>::type::value
            << '\n';

  std::cout << "\n\n4. ifsteram iterator test (input iterator)\n";

  std::cout << "std::istream_iterator<std::string> is forward iterator? :"
            << std::boolalpha
            << ft::is_forward_iterator<
                   std::istream_iterator<std::string> >::type::value
            << '\n';

  std::cout
      << "std::istream_iterator<std::string> is input iterator? :"
      << std::boolalpha
      << ft::is_input_iterator<std::istream_iterator<std::string> >::type::value
      << '\n';

  {
    PRINT(
        "ft::vector<char> v((std::istreambuf_iterator<char>(source)), "
        "std::istreambuf_iterator<char>()) ");
    std::ifstream source("Makefile", std::ios::binary);
    ft::vector<char> v((std::istreambuf_iterator<char>(source)),
                       std::istreambuf_iterator<char>());
    print_vector(v.begin(), v.end());
    print_vector(v);
  }

  {
    ft::vector<char> v;
    std::ifstream source("Makefile", std::ios::binary);
    v.assign(std::istreambuf_iterator<char>(source),
             std::istreambuf_iterator<char>());
    print_vector(v.begin(), v.end());
    print_vector(v);
  }
}
