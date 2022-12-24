
#ifndef PRINT_VECTOR_HPP
#define PRINT_VECTOR_HPP

#include <fstream>
#include <iostream>

#define L_CYAN "\033[1;96m"
#define RESET "\033[0m"

#define PRINT(...) std::cout, L_CYAN, __VA_ARGS__, RESET, "\n"

template <typename T>
std::ostream& operator,(std::ostream& out, const T& t) {
  out << t;
  return out;
}

template <typename T>
void print_vector(T it, T ite) {
  while (it != ite) {
    std::cout << *it << ", ";
    ++it;
    if (it == ite) std::cout << "\n";
  }
}

template <typename vec>
void print_vector(vec& v) {
  PRINT("vector info (size, capacity) : (", v.size(), ", ", v.capacity(), ")");
}

#endif
