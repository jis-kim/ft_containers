
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
  std::cout << "init 0 capacity : " << a.capacity() << '\n';
  a.push_back(1);

  std::vector<int> b;
  b.push_back(2);

  swap(a, b);

  std::cout << a[0] << std::endl;
  std::cout << b[0] << std::endl;

  std::cout << "\ncheck vector capacity\n";
  std::cout << "push 1 element capacity : " << a.capacity() << '\n';

  std::vector<int> hihi(10, 1);

  std::cout << "init 10 capacity, size : " << hihi.capacity() << ", "
            << hihi.size() << '\n';

  hihi.push_back(11);
  std::cout << "after push back capacity, size : " << hihi.capacity() << ", "
            << hihi.size() << '\n';
}
