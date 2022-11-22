
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

////void endl(std::ostream& os) { os << "testst\n"; }

// int main() {
//   std::cout << std::endl;
//   operator<<(std::cout, "hihi");  // argument

// test class for assignment operator
template <typename T>
struct A {
  T a;

  operator void *() { return NULL; }
};

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

  A<int> ta;
  A<int> tb;

  ta.a = 10;
  tb = ta;

  std::cout << "ta, tb " << ta.a << ", " << tb.a << '\n';
  ta.a = 13;
  std::cout << "ta, tb " << ta.a << ", " << tb.a << '\n';

  A<int *> pa;
  A<int *> pb;

  int fortest = 123;
  pa.a = &fortest;
  pb = pa;
  std::cout << "ta, tb " << pa.a << ", " << pb.a << '\n';
  std::cout << "value : pa, pb " << *(pa.a) << ", " << *(pb.a) << '\n';
  fortest = 345;
  std::cout << "pa, pb " << pa.a << ", " << pb.a << '\n';
  std::cout << "pa, pb " << *(pa.a) << ", " << *(pb.a) << '\n';

  A<int> ca;
  ca.a = 3;
  std::cout << (void *)ca << '\n';

  std::vector<int>::iterator it = hihi.begin();

  // if (it == it.base()) {
  // }
}
