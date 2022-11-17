
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

////void endl(std::ostream& os) { os << "testst\n"; }

// int main() {
//   std::cout << std::endl;
//   operator<<(std::cout, "hihi");  // argument

int main(void) {
  std::vector<int> a;
  a.push_back(1);

  std::vector<int> b;
  b.push_back(2);

  swap(a, b);

  std::cout << a[0] << std::endl;
  std::cout << b[0] << std::endl;
}
