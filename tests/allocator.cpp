#include <iostream>
#include <memory>

int main(void) {
  std::allocator<int> a, b;

  // int* asdf;
  // try {
  //   a.construct(asdf, 1);
  // } catch (std::exception& e) {
  //   std::cout << e.what() << '\n';
  // }

  if (a == b) std::cout << "equal!\n";
  int* p = a.allocate(0);
  std::cout << "\n\nallocate (0) address: " << p << '\n';
  std::cout << "p dereference : " << *p << '\n';
}
