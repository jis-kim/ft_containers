/**
 * @file type_traits.hpp
 * @author jiskim
 * @brief template meta programming library
 * @date 2022-12-17
 *
 * @copyright Copyright (c) 2022
 */

#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include "iterator.hpp"

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
// !SECTION: enable_if

// SECTION: integral_constant
/**
 * @brief v 를 static 인자로 가지기 위한 class
 * 컴파일 단계에서도 값을 저장해서 사용할 수 있다.
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
// !SECTION: integral_constant

// SECTION: remove_cv
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
// !SECTION: remove_cv

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
// !SECTION: is_integral

// SECTION: is_*_iterator

template <typename Base, typename Derived>
struct _is_base_of {
 private:
  struct no {};
  struct yes {
    no m[2];
  };

  /**
   * @brief Derived* -> Base* 로 형변환이 가능할 경우
   *
   * @param Base* Base* 로 형변환 시도.
   * @return yes size 가 2 인 no 배열을 리턴한다.
   */

  static yes test(Base *);

  /**
   * @brief yes 를 return 하는 경우를 제외한 나머지.
   *
   * @param ... 가변인자 (우선순위 낮음)
   * @return no size 가 1 (아무것도 없는 struct 의 size 는 1이다.)
   */
  static no test(...);

 public:
  /**
   * @brief test 의 overload resolution 을 이용해서 형변환 가능 여부에 따라
   * 리턴값의 size 를 달리한다.
   * size로 yes 와 no 를 구분한다.
   *
   */
  static const bool value =
      sizeof(test(static_cast<Derived *>(0))) == sizeof(yes);
};

template <typename Base, typename Derived>
struct is_base_of
    : public integral_constant<bool, _is_base_of<Base, Derived>::value> {};

// SECTION: iterator_category_t
/**
 * @brief iterator category 가 있으면 yes, 없으면 no 를 리턴한다.
 *
 * @tparam Iter 검증할 iterator
 */
template <typename Iter>
struct _has_iterator_category {
 private:
  struct no {};
  struct yes {
    no m[2];
  };

  template <typename T>
  static yes test(typename T::iterator_category *);

  template <typename T>
  static no test(...);

 public:
  static const bool value = sizeof(test<Iter>(0)) == sizeof(yes);
};

/**
 * @brief general _iterator_category_t
 *
 * @tparam B
 * @tparam Iter
 */
template <bool B, typename Iter>
struct _iterator_category_t {
  typedef void type;
};

/**
 * @brief specialization for true (has iterator_category)
 *
 * @tparam Iter
 */
template <typename Iter>
struct _iterator_category_t<true, Iter> {
  typedef typename Iter::iterator_category type;
};

template <typename Iter>
struct iterator_category_t {
  typedef typename _iterator_category_t<_has_iterator_category<Iter>::value,
                                        Iter>::type type;
};

template <typename Iter>
struct iterator_category_t<Iter *> {
  typedef typename std::random_access_iterator_tag type;
};
// !SECTION: iterator_category_t

// SECTION: iterator categorize
template <typename InputIterator>
struct is_input_iterator
    : public is_base_of<std::input_iterator_tag,
                        typename iterator_category_t<InputIterator>::type> {};

template <typename ForwardIterator>
struct is_forward_iterator
    : public is_base_of<std::forward_iterator_tag,
                        typename iterator_category_t<ForwardIterator>::type> {};

template <typename BidirectionalIterator>
struct is_bidirectional_iterator
    : public is_base_of<
          std::bidirectional_iterator_tag,
          typename iterator_category_t<BidirectionalIterator>::type> {};

template <typename RandomAccessIterator>
struct is_random_access_iterator
    : public is_base_of<
          std::random_access_iterator_tag,
          typename iterator_category_t<RandomAccessIterator>::type> {};
// !SECTION: iterator categorize

}  // namespace ft
#endif  // TYPE_TRAITS_HPP
