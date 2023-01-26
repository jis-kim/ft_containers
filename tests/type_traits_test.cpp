#include <iostream>
// #include <type_traits>

// struct A {};

// class B {};
#include <list>
#include <map>
#include <string>
#include <vector>

#include "testheader/vector_test.hpp"
#include "type_traits.hpp"
#include "vector.hpp"

void type_traits_test(void) {
  // std::cout << "is_class type traits test!!\n";

  // std::cout << "class : " << std::boolalpha << std::is_class<A>::value
  //           << ", struct : " << std::is_class<B>::value << '\n';
  // print type id
  std::cout << "class : " << typeid(ft::vector<int>).name() << "\n";

  std::cout
      << "==================random access iterator test==================\n";

  std::cout << "random access iterator is forward access iterator? : "
            << std::boolalpha
            << ft::is_base_of<std::forward_iterator_tag,
                              std::random_access_iterator_tag>::value
            << '\n';

  std::cout << "\n\n==================is_*_iterator test==================\n";
  std::cout << "\n\n1. vector iterator (random access iterator) test\n";

  std::cout
      << "vector<int> iterator is random access iterator? : " << std::boolalpha
      << ft::is_random_access_iterator<std::vector<int>::iterator>::type::value
      << '\n';

  std::cout
      << "vector<int> iterator is bidirectional access iterator? : "
      << std::boolalpha
      << ft::is_bidirectional_iterator<std::vector<int>::iterator>::type::value
      << '\n';

  std::cout << "vector<int> iterator is forward access iterator? : "
            << std::boolalpha
            << ft::is_forward_iterator<std::vector<int>::iterator>::type::value
            << '\n';

  std::cout << "vector<int> iterator is input access iterator? : "
            << std::boolalpha
            << ft::is_input_iterator<std::vector<int>::iterator>::type::value
            << '\n';

  std::cout << "\n\n2. list iterator (bidirectional iterator) test\n";

  std::cout
      << "list<int> iterator is random access iterator? : " << std::boolalpha
      << ft::is_random_access_iterator<std::list<int>::iterator>::type::value
      << '\n';

  std::cout
      << "list<int> iterator is bidirectional access iterator? : "
      << std::boolalpha
      << ft::is_bidirectional_iterator<std::list<int>::iterator>::type::value
      << '\n';

  std::cout << "list<int> iterator is forward access iterator? : "
            << std::boolalpha
            << ft::is_forward_iterator<std::list<int>::iterator>::type::value
            << '\n';

  std::cout << "list<int> iterator is input access iterator? : "
            << std::boolalpha
            << ft::is_input_iterator<std::list<int>::iterator>::type::value
            << '\n';

  std::cout << "\n\n3. map iterator (bidirectional iterator) test\n";

  std::cout << "map<int, int> iterator is random access iterator? : "
            << std::boolalpha
            << ft::is_random_access_iterator<
                   std::map<std::string, int>::iterator>::type::value
            << '\n';

  std::cout << "map<int, int> iterator is bidirectional access iterator? : "
            << std::boolalpha
            << ft::is_bidirectional_iterator<
                   std::map<std::string, int>::iterator>::type::value
            << '\n';

  std::cout << "map<int, int> iterator is forward access iterator? : "
            << std::boolalpha
            << ft::is_forward_iterator<
                   std::map<std::string, int>::iterator>::type::value
            << '\n';

  std::cout << "map<int, int> iterator is input access iterator? : "
            << std::boolalpha
            << ft::is_input_iterator<
                   std::map<std::string, int>::iterator>::type::value
            << '\n';

  std::cout << "\n\n4. map const iterator (bidirectional iterator) test\n";

  std::cout << "map<int, int> const iterator is random access iterator? : "
            << std::boolalpha
            << ft::is_random_access_iterator<
                   std::map<std::string, int>::const_iterator>::type::value
            << '\n';

  std::cout << "map<int, int> const iterator is bidirectional access iterator? "
               ": "
            << std::boolalpha
            << ft::is_bidirectional_iterator<
                   std::map<std::string, int>::const_iterator>::type::value
            << '\n';

  std::cout << "map<int, int> const iterator is forward access iterator? : "
            << std::boolalpha
            << ft::is_forward_iterator<
                   std::map<std::string, int>::const_iterator>::type::value
            << '\n';

  std::cout << "\n\n4. ifsteram iterator test (input iterator)\n";

  std::cout << "std::istream_iterator<std::string> is forward iterator? :"
            << std::boolalpha
            << ft::is_forward_iterator<
                   std::istream_iterator<std::string> >::type::value
            << '\n';

  std::cout
      << "std::istream_iterator<std::string> is input iterator? :"
      << std::boolalpha
      << ft::is_input_iterator<std::istream_iterator<std::string> >::type::value
      << '\n';

  {
    PRINT(
        "ft::vector<char> v((std::istreambuf_iterator<char>(source)), "
        "std::istreambuf_iterator<char>()) ");
    std::ifstream source("Makefile", std::ios::binary);

    std::cout << (ft::is_input_iterator<
                      std::istreambuf_iterator<char> >::type::value &&
                  !(ft::is_forward_iterator<
                      std::istreambuf_iterator<char> >::type::value))
              << '\n';

    std::cout << "\n\n===============Makefile print test "
                 "(constructor)================\n\n";
    std::istreambuf_iterator<char> it(source);
    std::cout
        << typeid(std::istreambuf_iterator<char>::iterator_category).name()
        << '\n';

    ft::vector<char> v(it, std::istreambuf_iterator<char>());
    print_vector(v.begin(), v.end());

    std::cout
        << "-------------------------------------------------------------\n";

    print_vector(v);
    std::cout
        << "\n\n===============Makefile print test end================\n\n";

    std::cout << "\n\n=============== >= forward iterator consturct "
                 "===============\n\n";

    ft::vector<char> v2(v.begin(), v.end());
    print_vector(v2.begin(), v2.end());
    print_vector(v2);
  }

  {
    std::cout << "\n\n===============Makefile printtest(assign) ==== == == == "
                 "== == ==\n\n ";
    ft::vector<char> v;
    std::ifstream source("Makefile", std::ios::binary);
    v.assign(std::istreambuf_iterator<char>(source),
             std::istreambuf_iterator<char>());
    print_vector(v.begin(), v.end());
    std::cout
        << "--------------------------------------------------------------\n";
    print_vector(v);

    std::cout
        << "\n\n===============Makefile print test end================\n\n";
  }

  {
    ft::vector<char> v;
    for (int i = 0; i < 10; ++i) {
      v.push_back('0' + i);
    }
    v.assign(10, 'a');
    system("leaks ft_containers");
    print_vector(v.begin(), v.end());
    print_vector(v);
    std::cout
        << "--------------------------------------------------------------\n";

    std::ifstream source("tests/aaa", std::ios::binary);
    std::vector<char> vec((std::istreambuf_iterator<char>(source)),
                          std::istreambuf_iterator<char>());

    print_vector(vec.begin(), vec.end());
    print_vector(vec);
    v.insert(v.begin() + 5, vec.begin(), vec.end());

    std::cout
        << "--------------------------------------------------------------\n";
    print_vector(v.begin(), v.end());
    print_vector(v);
    //  well done!
  }

  {
    ft::vector<char> v;
    for (int i = 0; i < 10; ++i) {
      v.push_back('0' + i);
    }
    print_vector(v.begin(), v.end());
    print_vector(v);
    std::cout
        << "--------------------------------------------------------------\n";

    std::ifstream source("Makefile", std::ios::binary);

    v.insert(v.begin() + 5, (std::istreambuf_iterator<char>(source)),
             std::istreambuf_iterator<char>());
    print_vector(v.begin(), v.end());
    print_vector(v);

    std::cout
        << "--------------------------------------------------------------\n";
  }

  //  system("leaks ft_containers");
}
