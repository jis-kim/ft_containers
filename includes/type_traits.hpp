/**
 * @file type_traits.hpp
 * @author jiskim
 * @brief metaprogramming library
 * @date 2022-12-17
 *
 * @copyright Copyright (c) 2022
 */

#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include "iterator_traits.hpp"

namespace ft {

// SECTION: enable_if
template <bool B, typename T = void>
struct enable_if {};

/**
 * @brief true 로 특수화 한 enable_if.
 * enable_if< 조건, T>::type 의 형태로 사용해서 true 일 때만 특정 함수가
 * 불리도록 한다.
 *
 * @tparam T
 */
template <typename T>
struct enable_if<true, T> {
  typedef T type;
};

// SECTION: integral_constant
/**
 * @brief v 를 static 인자로 가지기 위한 class
 * 컴파일 단계에서도 값을 쓰려는 편법
 *
 * @tparam T 타입
 * @tparam v 값
 */
template <typename T, T v>
struct integral_constant {
  static const T value = v;
  typedef T value_type;
  typedef integral_constant<T, v> type;
  operator value_type(void) const { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

// SECTION : is_same
template <typename T, typename U>
struct is_same : public false_type {};

template <typename T>
struct is_same<T, T> : public true_type {};

// SECTION : remove_cv
/**
 * @brief const, volatile 타입을 non-cv-qualified 타입으로 변환한다.
 * 이 과정이 없는데 is_integral 에 const, volatile 타입이 들어가면
 * false_type 이 나오게 된다.
 *
 * @tparam T
 */
template <typename T>
struct remove_cv {
  typedef T type;
};

template <typename T>
struct remove_cv<const T> {
  typedef T type;
};

template <typename T>
struct remove_cv<volatile T> {
  typedef T type;
};

template <typename T>
struct remove_cv<const volatile T> {
  typedef T type;
};

// SECTION: is_integral
/**
 * @brief integral 인지 아닌지 판별.
 * integral 이면 true_type, 아니면 false_type 을 상속받아 value 를 달리 가진다.
 * @tparam T integral 을 판별할 타입.
 */

template <typename T>
struct _is_integral : public false_type {};

template <>
struct _is_integral<bool> : public true_type {};

template <>
struct _is_integral<char> : public true_type {};

template <>
struct _is_integral<signed char> : public true_type {};

template <>
struct _is_integral<unsigned char> : public true_type {};

template <>
struct _is_integral<short> : public true_type {};

template <>
struct _is_integral<unsigned short> : public true_type {};

template <>
struct _is_integral<int> : public true_type {};

template <>
struct _is_integral<unsigned int> : public true_type {};

template <>
struct _is_integral<long> : public true_type {};

template <>
struct _is_integral<unsigned long> : public true_type {};

/**
 * @brief for public is_integral
 * remove_cv 로 cv 를 제거하고 _is_integral 로 판별한다.
 *
 * @tparam T
 */
template <typename T>
struct is_integral : public _is_integral<typename remove_cv<T>::type> {};

template <typename Iter>
struct _iterator_category_t {
  typedef typename iterator_traits<Iter>::iterator_category type;
};

// SECTION : is_*_iterator

template <typename B, typename D>
struct is_base_of {
 private:
  struct no {};
  struct yes {
    no m[2];
  };

  static yes test(B*);
  static no test(...);

 public:
  static const bool value = sizeof(test(static_cast<D*>(0))) == sizeof(yes);
};

template <typename RandomAccessIterator>
struct _is_random_access_iterator {
  typedef typename is_same<
      typename iterator_traits<RandomAccessIterator>::iterator_category,
      std::random_access_iterator_tag>::type type;
  static const bool value = type::value;
};

template <typename BidirectionalIterator>
struct _is_bidirectional_iterator {
  static const bool value =
      _is_random_access_iterator<BidirectionalIterator>::value ||
      is_same<
          typename iterator_traits<BidirectionalIterator>::iterator_category,
          std::random_access_iterator_tag>::value;
};
template <typename ForwardIterator>
struct _is_forward_iterator {
  static const bool value =
      _is_bidirectional_iterator<ForwardIterator>::value ||
      is_same<typename iterator_traits<ForwardIterator>::iterator_category,
              std::forward_iterator_tag>::value;
};

template <typename InputIterator>
struct _is_input_iterator {
  static const bool value =
      _is_forward_iterator<InputIterator>::value ||
      is_same<typename iterator_traits<InputIterator>::iterator_category,
              std::input_iterator_tag>::value;
};

template <typename OutputIterator>
struct _is_output_iterator {
  static const bool value =
      _is_forward_iterator<OutputIterator>::value ||
      is_same<typename iterator_traits<OutputIterator>::iterator_category,
              std::output_iterator_tag>::value;
};

// SECTION : iterator categorize
// template <typename InputIterator>
// struct is_input_iterator {
//  typedef typename _is_input_iterator<InputIterator>::type type;
//};

// template <typename OutputIterator>
// struct is_output_iterator {
//   typedef typename _is_output_iterator<OutputIterator>::type type;
// };

// template <typename ForwardIterator>
// struct is_forward_iterator {
//   typedef typename _is_forward_iterator<ForwardIterator>::type type;
// };

// template <typename BidirectionalIterator>
// struct is_bidirectional_iterator {
//   typedef typename _is_bidirectional_iterator<BidirectionalIterator>::type
//   type;
// };

template <typename RandomAccessIterator>
struct is_random_access_iterator {
  typedef typename is_same<
      typename iterator_traits<RandomAccessIterator>::iterator_category,
      std::random_access_iterator_tag>::type type;
  // typedef typename _is_random_access_iterator<RandomAccessIterator>::type
  // type;
};

template <typename Iter>
struct is_iterator {
  typedef typename is_same<typename _iterator_category_t<Iter>::type,
                           std::input_iterator_tag>::type type;
};

}  // namespace ft
#endif  // TYPE_TRAITS_HPP
