#include <iostream>
#include <vector>

int main(void) {
  std::vector<int> v(10, 10);

  for (auto i : v) {
    std::cout << i << std::endl;
  }

  const int i = 190;

  v.push_back(i);
  for (auto i : v) {
    std::cout << i << std::endl;
  }
}
