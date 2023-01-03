#include "testheader/print_vector.hpp"
// #include "vector.hpp"
#include <iostream>

const std::string& test(void) {
  const std::string a =
      "ajsdflkajsdlkfjaslkdfjlksadjlfkdsm,"
      "nckjxhvkjadlkfjaewoiurlkdjflkjasdlkfjasl;dkjfl;"
      "kasdjflkasdjfknsdjkfjsladkjflksdflkasdjfljasl;"
      "fjsadlkfjsdlkfjlsdkjflaksdjfjhsdjmvcx,"
      "mvjoiweufjkdsjflkjaslkdfjasklfjlkjflsjafkljsf;lhasjkfhajkyehjkadsn,"
      "mfhcjkxyhfliewyjk";

  return a;
}

const std::string a = "abcdefg";

enum hi { first = true, second };

int main(void) {
  // type_traits_test();
  vector_test();
  //  vector_iterator_test();

  // const std::string b = test();
  //  std::cout << b << '\n';
  std::cout << first << ", " << second << ", " << '\n';
}
