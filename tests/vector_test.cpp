#include "vector.hpp"

#include <unistd.h>

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

static int num = 0;
struct BASIC {
  int* a;
  BASIC(void) {
    a = new int(3);
    std::cout << "basic basic\n";
  }
  BASIC(const BASIC& src) {
    if (num >= 5) {
      num = 0;
      throw std::logic_error("throw test");
    }
    ++num;
    a = new int(*(src.a));
    std::cout << "copy constructor\n";
  }
  BASIC& operator=(const BASIC& rhs) {
    if (num >= 5) {
      num = 0;
      throw std::logic_error("throw test");
    }
    ++num;
    delete a;
    a = new int(*(rhs.a));
    std::cout << "operator=\n";
    return *this;
  }

  ~BASIC(void) {
    std::cout << "BASIC destructor called\n";
    delete a;
  }
};

int main(void) {
  std::vector<int> int_vec;

  std::cout << "begin : " << int_vec.begin().base() << "\n";
  std::cout << "end : " << int_vec.end().base() << "\n";
  std::cout << "capacity: " << int_vec.capacity() << "\n";

  for (int i = 0; i < 10; ++i) {
    int_vec.push_back(i);
  }

  // int_vec.clear();
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
  //  가능하다. 딱히 destruct 해 줄 필요는 없어보이는데?.. 근데 int 라서 그럴
  //  수도 있다. 얘가 destory 가 되냐?
  //  22.12.11) sanitizer 돌리면 얘도 에러가 난다.

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

  std::cout << "============= basic guarantee test =============\n";
  // assign is basic guarantee
  std::cout << "\n\nstd::vector !!!!!\n\n";
  {
    BASIC abc;
    std::vector<BASIC> basic_vec(10);
    basic_vec.clear();
    try {
      std::cout << "size of basic vector : " << basic_vec.size()
                << ", capacity : " << basic_vec.capacity() << '\n';
      basic_vec.assign(8, abc);
      std::cout << "size of basic vector : " << basic_vec.size()
                << ", capacity : " << basic_vec.capacity() << '\n';
      std::cout << "success?\n";
    } catch (...) {
      std::cout << "catch!\n";
      std::cout << "size of basic vector : " << basic_vec.size()
                << ", capacity : " << basic_vec.capacity() << '\n';
    }
  }

  std::cout << "\n\nft_vector !!!!!\n\n";
  {
    BASIC abc;
    num = -10;
    ft::vector<BASIC> ft_basic_vec(10, abc);
    ft_basic_vec.clear();
    try {
      std::cout << "size of ft vector : " << ft_basic_vec.size()
                << ", capacity : " << ft_basic_vec.capacity() << '\n';
      ft_basic_vec.assign(8, abc);
      std::cout << "size of ft vector : " << ft_basic_vec.size()
                << ", capacity : " << ft_basic_vec.capacity() << '\n';
      std::cout << "success?\n";
    } catch (...) {
      std::cout << "catch!\n";
      std::cout << "size of ft vector : " << ft_basic_vec.size()
                << ", capacity : " << ft_basic_vec.capacity() << '\n';
    }
  }

  // std::cout << system("leaks ft_containers") << "\n";

  std::cout << "\n\nSFINAE test\n";
  // itereator 를 이용한 consturctor 에 enable_if 를 적용하지 않은 경우
  // 그리고 n, val 의 타입이 동일한 경우 어떤 타입의 생성자가 호출될 것인가?
  ft::vector<int>((size_t)10, 3);  // 의도 : vector(n, val) 호출
}
