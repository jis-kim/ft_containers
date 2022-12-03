#include <iostream>
#include <vector>

#include "vector.hpp"

int main(void) {
  int array[5] = {1, 2, 3, 4, 5};

  ft::vector_iterator<int *> it(array);

  std::cout
      << "\n================= increment operator test =================\n";
  for (int i = 0; i < 5; ++i) {
    std::cout << "i is " << i << ", it is " << *it << '\n';
    it++;
  }
  --it;

  std::cout
      << "\n================= decrement operator test =================\n";
  for (int i = 0; i < 5; ++i) {
    std::cout << "i is " << i << ", it is " << *it << '\n';
    it--;
  }
  ++it;

  std::cout << "\nreference & insertion & subscript";
  std::cout << "it[3] is " << it[3] << '\n';
  it[3] = 100;
  std::cout << "it[3] after insertion is " << it[3] << '\n';

  std::cout << "\n================= original iterator =================\n";
  for (int i = 0; i < 5; ++i) {
    std::cout << "i is " << i << ", it is " << *it << '\n';
    it++;
  }
  it -= 5;

  ft::vector_iterator<int *> tmp(it);
  it += 2;
  std::cout << "it += 2 ([2]): " << *it << '\n';

  ft::vector_iterator<int *> *it_pointer = &it;

  std::cout << "\nbase : " << it_pointer->base() << '\n';
  std::cout << "tmp base : " << tmp.base() << '\n';

  std::cout << "\ncopied to tmp (default copy constructor) \n\n";
  for (int i = 0; i < 5; ++i) {
    std::cout << "i is " << i << ", tmp is " << *tmp << '\n';
    tmp++;
  }

  std::cout << "\ntmp(5) - 3 : " << *(tmp - 3) << "\n";
  tmp = &tmp[-2];
  std::cout << "\ntmp(2) + 1 : " << *(tmp + 1) << "\n";
  tmp = it;  // assignment operation
  std::cout << "\n3 + tmp(2) : " << *(2 + tmp) << '\n';
  *tmp = 1111;

  it = &it[-1];

  for (int i = 0; i < 5; ++i) {
    std::cout << "i is " << i << ", tmp is " << *it << '\n';
    it++;
  }

  std::vector<int> vec = {1, 2, 3, 4, 5};
  std::cout << "\nvec is 1, 2, 3, 4, 5 \n";

  std::vector<int>::iterator vit = vec.begin();
  std::cout << "\nexecute vit += 3\n\n";
  vit += 3;

  std::cout << "vit - 2 : " << *(vit - 2) << '\n';
  std::cout << "vit[-2] : " << vit[-2] << '\n';  // -도 되는게 정상
}
