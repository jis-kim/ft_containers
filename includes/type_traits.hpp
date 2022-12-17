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

// SECTION: is_integral
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

/**
 * @brief integral 인지 아닌지 판별.
 * integral 이면 true_type, 아니면 false_type 을 상속받아 value 를 달리 가진다.
 * @tparam T integral 을 판별할 타입.
 */
template <typename T>
struct is_integral : public false_type {};

template <>
struct is_integral<bool> : public true_type {};

template <>
struct is_integral<char> : public true_type {};

template <>
struct is_integral<signed char> : public true_type {};

template <>
struct is_integral<unsigned char> : public true_type {};

template <>
struct is_integral<short> : public true_type {};

template <>
struct is_integral<unsigned short> : public true_type {};

template <>
struct is_integral<int> : public true_type {};

template <>
struct is_integral<unsigned int> : public true_type {};

template <>
struct is_integral<long> : public true_type {};

template <>
struct is_integral<unsigned long> : public true_type {};

// SECTION : iterator categorize
/**
 * @brief iterator 의 카테고리를 나타내는 타입.
 * iterator 의 카테고리는 input, output, forward, bidirectional, random-access
 * 로 나뉜다.
 *
 * @tparam Category 카테고리
 * @tparam T 타입
 * @tparam Distance 거리
 * @tparam Pointer 포인터
 * @tparam Reference 참조
 */

}  // namespace ft
#endif  // TYPE_TRAITS_HPP
