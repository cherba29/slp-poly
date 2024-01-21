#ifndef NTRP_MATH_STATIC_H
#define NTRP_MATH_STATIC_H

/**
 * @file: static.h static math template meta utilities.
 */

#include <boost/static_assert.hpp>
#include <limits>

namespace math {

/**
 * @brief Compute modulus of given number
 */
template <int64_t VAL, uint64_t MOD>
struct mod {
  static uint64_t const VALUE = (VAL >= 0)
                                    ? static_cast<uint64_t>(VAL) % MOD
                                    : MOD - static_cast<uint64_t>(-VAL) % MOD;

 private:
  mod();  // dont instantiate
};

/**
 * @brief Computes X*Y % MOD without overflow in prodmod<X,Y,MOD>::VALUE
 */
template <uint64_t X, uint64_t Y, uint64_t MOD>
class prodmod {
  static uint64_t const P = prodmod<X, Y / 2, MOD>::VALUE % MOD;
  static uint64_t const P2 =
      (P > MOD / 2) ? MOD - (MOD - P) - (MOD - P) : (P << 1);
  prodmod();  // dont instantiate
 public:
  static uint64_t const VALUE =
      (1 - Y % 2) * P2 + (Y % 2) * ((P2 <= (MOD - X)) * (MOD - (MOD - X - P2)) +
                                    (P2 > (MOD - X)) * (P2 - (MOD - X)));
};

template <uint64_t X, uint64_t MOD>
struct prodmod<X, 0, MOD> {
  static uint64_t const VALUE = 1;
};

template <uint64_t X, uint64_t MOD>
struct prodmod<X, 1, MOD> {
  static uint64_t const VALUE = X % MOD;
};

template <uint64_t Y, uint64_t MOD>
struct prodmod<0, Y, MOD> {
  static uint64_t const VALUE = 0;
};

template <uint64_t Y, uint64_t MOD>
struct prodmod<1, Y, MOD> {
  static uint64_t const VALUE = Y % MOD;
};

/**
 * @brief Compute i such that (N-1)=2^i*oddval in two_powers<N>::POWER
 */
template <uint64_t N>
class two_powers {
  two_powers();  // dont instantiate
 public:
  static uint64_t const POWER =
      ((1 - (N & 1)) * ((two_powers<N / 2>::POWER) + 1));
};
template <>
struct two_powers<1> {
  static uint64_t const POWER = 0;
};
template <>
struct two_powers<0> {
  static uint64_t const POWER = 0;
};

/**
 * @brief Compute N^P % MOD without overflow
 */
template <uint64_t N, uint64_t P, uint64_t MOD>
struct powermod {
  static uint64_t const Q = powermod<N, P / 2, MOD>::VALUE;
  static uint64_t const P2 = prodmod<Q, Q, MOD>::VALUE;
  static uint64_t const VALUE =
      (1 - (P & 1)) * P2 + (P & 1) * prodmod<N, P2, MOD>::VALUE;

 private:
  powermod();  // dont instantiate
};

template <uint64_t N, uint64_t MOD>
struct powermod<N, 1, MOD> {
  static uint64_t const VALUE = N;
};

template <uint64_t N, uint64_t MOD>
struct powermod<N, 0, MOD> {
  static uint64_t const VALUE = 1;
};

/**
 * @brief Compute fourier order of the odd (prime) number
 * in fourier_order::ORDER
 */
template <uint64_t N>
struct fourier_order {
  static uint64_t const ORDER = two_powers<N - 1>::POWER,
                        ODDMUL = (N - 1) / (static_cast<uint64_t>(1)
                                            << two_powers<N - 1>::POWER);

 private:
  fourier_order();  // dont instantiate
};

template <uint64_t p, uint64_t i>
class is_prime {
  is_prime();  // dont instantiate
 public:
  static uint64_t const prim =
      (p == 2) || ((p % i) && is_prime<(i > 2 ? p : 0), i - 1>::prim);
};

template <>
class is_prime<0, 0> {
 public:
  static uint64_t const prim = 1;
};

template <>
class is_prime<0, 1> {
 public:
  static uint64_t const prim = 1;
};

template <uint64_t A, uint64_t B>
struct min {
  static uint64_t const value = (A < B) ? A : B;
};

template <uint64_t A, uint64_t B>
struct max {
  static uint64_t const value = (A > B) ? A : B;
};

template <uint64_t V>
struct uint32_t_ {
  BOOST_STATIC_ASSERT(V <= INT64_C(0xFFFFFFFF));
  static uint32_t const value = static_cast<uint32_t>(V);
};

}  // namespace math

#endif  // NTRP_MATH_STATIC_H
