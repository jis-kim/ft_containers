#include <iostream>

/**
 * @brief Euclidean algorithm with TMP
 * 유클리드 호제법 (최대 공약수 구하기)
 *
 */

template <int X, int Y>
struct GCD {
  static const int value = GCD<Y, X % Y>::value;
};

// specification in X
template <int X>
struct GCD<X, 0> {
  static const int value = X;
};

template <int N, int D = 1>
struct Ratio {
 private:
  const static int _gcd = GCD<N, D>::value;

 public:
  typedef Ratio<N / _gcd, D / _gcd> type;
  static const int num = N / _gcd;  // 분모
  static const int den = D / _gcd;  // 분자
};

template <class R1, class R2>
struct _Ratio_add {
  // type 을 이용하여 연산
  typedef Ratio<R1::num * R2::den + R2::num * R1::den, R1::den * R2::den> type;
};

// 상속을 받아서 Ratio_add를 마치 Ratio 타입처럼 사용할 수 있게 된다.
template <class R1, class R2>
struct Ratio_add : _Ratio_add<R1, R2>::type {};

template <class R1, class R2>
struct _Ratio_substract {
  typedef Ratio<R1::num * R2::den - R2::num * R1::den, R1::den * R2::den> type;
};

template <class R1, class R2>
struct _Ratio_multiply {
  typedef Ratio<R1::num * R2::num, R1::den * R2::den> type;
};

template <class R1, class R2>
struct Ratio_multiply : _Ratio_multiply<R1, R2>::type {};

template <class R1, class R2>
struct _Ratio_divide {
  typedef Ratio<R1::num * R2::den, R1::den * R2::num> type;
};

template <class R1, class R2>
struct Ratio_divide : _Ratio_divide<R1, R2>::type {};

template <int X>
struct fib {
  typedef fib<X - 1> fib1;
  typedef fib<X - 2> fib2;
  static const int result = fib1::result + fib2::result;
};

// ㅇㅣ거 해줘야  template 인 줄 앎.
template <>
struct fib<1> {
  static const int result = 1;
};

template <>
struct fib<2> {
  static const int result = 1;
};

// template <int N>
// struct check_div<N, N / 2> {
//   static const bool result = (N % (N / 2) == 0);
// };

/**
 * @brief INT wrapper class
 *
 * @return
 */

template <int N>
struct INT {
  static const int num = N;
};

template <typename a, typename b>
struct divide {
  typedef INT<a::num / b::num> result;
};

template <typename a, typename b>
struct add {
  typedef INT<a::num + b::num> result;
};

typedef INT<1> one;
typedef INT<2> two;
typedef INT<3> three;

template <typename N, typename d>
struct check_div {
  static const bool result = (N::num % d::num == 0) ||
                             check_div<N, typename add<d, one>::result>::result;
};

template <typename N>
struct _is_prime {
  static const bool result = !check_div<N, two>::result;
};

template <>
struct _is_prime<two> {
  static const bool result = true;
};

template <>
struct _is_prime<three> {
  static const bool result = true;
};

/**
 * @brief 식별자가 '값'인지 '타입' 인지 컴파일 타임에 결정되어야 한다.
 *
 * @tparam N
 */
template <typename N>
struct check_div<N, typename divide<N, two>::result> {
  static const bool result = (N::num % (N::num / 2) == 0);
};

template <int N>
struct is_prime {
  static const bool result = _is_prime<INT<N> >::result;
};

int main() {
  typedef Ratio<2, 3> rat1;
  typedef Ratio<3, 2> rat2;

  typedef Ratio_add<rat1, rat2> rat3;
  std::cout << "gcd (12, 18) : " << GCD<12, 18>::value << std::endl;

  std::cout << "2/3 + 3/2 = " << rat3::num << " / " << rat3::den << std::endl;

  typedef Ratio_multiply<rat1, rat3> rat4;
  std::cout << "(2/3 + 3/2) * (2/3) = " << rat4::num << "/" << rat4::den
            << std::endl;

  std::cout << "fib 5 " << fib<5>::result << std::endl;

  std::cout << std::boolalpha;
  std::cout << "Is 2 prime ? :: " << is_prime<2>::result << std::endl;
  std::cout << "Is 10 prime ? :: " << is_prime<10>::result << std::endl;
  std::cout << "Is 11 prime ? :: " << is_prime<11>::result << std::endl;
  std::cout << "Is 61 prime ? :: " << is_prime<61>::result << std::endl;

  return 0;
}
