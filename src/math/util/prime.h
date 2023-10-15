#ifndef NTRP_MATH_UTIL_PRIME_H
#define NTRP_MATH_UTIL_PRIME_H

// _STDC_CONSTANT_MACROS=1 must be defined project wide
#include <boost/cstdint.hpp>  // for UINT64_C
//#endif // __STDC_CONSTANT_MACROS

#include "math/Base.h"
#include "modular-x86.h"
#include "random.h"

namespace math {

/**
 * @brief Well known high fourier order primes
 */
template <typename U>
struct Prime {};

template <>
struct Prime<uint32_t> {
  static uint32_t const A = 3221225473UL;  // order 30
  static uint32_t const B = 3489660929UL;  // order 28
  static uint32_t const C = 3892314113UL;  // order 27
};

// Extension ULL for 64-bit constants is compiler/system dependent
// Using macro defined in stdint
//#define UINT64_C(x) x##ULL

template <>
struct Prime<uint64_t> {
  static uint64_t const A = UINT64_C(15564440312192434177);  // order 59
  static uint64_t const B = UINT64_C(17726168133330272257);  // order 57
  static uint64_t const C = UINT64_C(13690942867206307841);  // order 57
};

/**
 * @brief Rabin-Miller probabilistic primality test
 * D. E. Knuth, _Seminumerical Algorithms_, 4.5.4, algorithm P (p. 395)
 * @param val unsigned integer to test if it is prime
 * @return true if val is prime with high probability.
 */
template <typename UIntType>
bool maybe_prime(UIntType a) {
  // Find n > 0, q odd, such that 2^n * q + 1 == a
  UIntType q = a - 1;
  unsigned int n = 0;
  while ((q & 1) == 0) {
    n++;
    q >>= 1;
  }

  // Choose x at random, and generate the sequence y[k] == x^(2^k)*q % a
  // If a is prime, either x^q == 1 or y[k] = a-1 for some k < n
  //  UIntType x = static_cast<UIntType>( rg.IRandom(2, a-1) ); // min 2, max
  //  a-1
  UIntType x = random<UIntType>::get(2, a - 1);
  UIntType y = ModOp<UIntType>::pow(x, q, a);
  for (unsigned int j = 0; j < n; ++j) {
    if (y == 1) return (j == 0);
    if (y == a - 1) return true;
    y = ModOp<UIntType>::mul(y, y, a);
  }
  return false;
}

/**
 * @brief Collection of hardcoded primes
 */
extern const uint32_t primes[];

/**
 * Total number of hardcoded primes
 */
extern const unsigned int NPRIMES;

}  // namespace math
#endif  // NTRP_MATH_UTIL_PRIME_H
