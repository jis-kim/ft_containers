#include "vector.hpp"

#include <iostream>
#include <string>
#include <vector>

class A {
 public:
  int a;

  A(int num = int()) { a = num; }
};

std::ostream& operator<<(const std::ostream& os, const A& a) {
  return os << a.a;
}

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

  for (int i = 0; i < 10; ++i) {
    int_vec.push_back(i);
  }

  std::cout << "============ vector resize test ==============\n";
  // 1.10개 -> 5개로 resize 했을 때 뒤에 있는 요소 접근 가능한가?
  int_vec.resize(5);

  std::cout << "size : " << int_vec.size()
            << " , end + 3  : " << *(int_vec.end() + 3) << "\n";
  // 가능하다. 딱히 destruct 해 줄 필요는 없어보이는데?.. 근데 int 라서 그럴
  // 수도 있다. 얘가 destory 가 되냐?

  std::vector<A> a_vec;
  for (int i = 0; i < 10; ++i) {
    a_vec.push_back(A(i));
  }

  a_vec.resize(5);

  std::cout << "capacity : " << a_vec.capacity() << "\n";
  // segmentation fault!
  // std::cout << "size : " << a_vec.size()
  //          << " , end + 3  : " << *(a_vec.end() + 3) << "\n";

  std::cout << "============ vector at[n] out of range test =============\n";
  try {
    a_vec.at(10);
  } catch (const std::out_of_range& e) {
    std::cout << e.what() << '\n';
  }
}