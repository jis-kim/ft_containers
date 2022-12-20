#include <iostream>
// #include <type_traits>

// struct A {};

// class B {};
#include <vector>

#include "type_traits.hpp"
#include "vector.hpp"

class A {};
class B : public A {};

template <typename B, typename D>
struct is_base_of {
 private:
  struct no {};
  struct yes {
    no m[2];
  };

  static yes test(B*);
  static no test(...);

 public:
  static const bool value = sizeof(test(static_cast<D*>(0))) == sizeof(yes);
};

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
            << is_base_of<std::forward_iterator_tag,
                          std::random_access_iterator_tag>::value
            << '\n';

  std::cout
      << "\nvector<int> iterator is random access iterator? : "
      << std::boolalpha
      << ft::is_random_access_iterator<std::vector<int>::iterator>::type::value
      << '\n';
}
