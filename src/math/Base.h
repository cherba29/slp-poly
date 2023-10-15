#ifndef NTRP_MATH_BASE_H
#define NTRP_MATH_BASE_H

/**
 * @file: Base.h basic type definitions.
 */

#include <boost/static_assert.hpp>

#ifndef __LP64__
#ifdef _WIN64
#define __LP64__ 1
#endif
#endif

#ifdef _MSC_VER
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#ifdef __LP64__
typedef __int128 int128_t;
typedef unsigned __int128 u_int128_t;
#endif
#else  // g++
#include <stdint.h>
#define __forceinline __inline__
typedef int int32_t __attribute__((__mode__(__SI__)));
typedef unsigned int uint32_t __attribute__((__mode__(__SI__)));
typedef int int64_t __attribute__((__mode__(__DI__)));
typedef unsigned int uint64_t __attribute__((__mode__(__DI__)));

#ifdef __LP64__
typedef int int128_t __attribute__((mode(TI)));
typedef unsigned int uint128_t __attribute__((mode(TI)));
#endif
#endif

BOOST_STATIC_ASSERT(sizeof(int32_t) == 4);
BOOST_STATIC_ASSERT(sizeof(uint32_t) == 4);
BOOST_STATIC_ASSERT(sizeof(uint64_t) == 8);
BOOST_STATIC_ASSERT(sizeof(int64_t) == 8);
#ifdef __LP64__
BOOST_STATIC_ASSERT(sizeof(uint128_t) == 16);
#endif

template <typename IntType>
struct TypeInfo {};

template <>
struct TypeInfo<int32_t> {
  typedef int32_t signed_type;
  typedef uint32_t unsigned_type;
  typedef int64_t signed2x_type;
  typedef uint64_t unsigned2x_type;
};

template <>
struct TypeInfo<uint32_t> {
  typedef int32_t signed_type;
  typedef uint32_t unsigned_type;
  typedef int64_t signed2x_type;
  typedef uint64_t unsigned2x_type;
};

template <>
struct TypeInfo<int64_t> {
  typedef int64_t signed_type;
  typedef uint64_t unsigned_type;
#ifdef __LP64__
  typedef int128_t signed2x_type;
  typedef uint128_t unsigned2x_type;
#endif
};

template <>
struct TypeInfo<uint64_t> {
  typedef int64_t signed_type;
  typedef uint64_t unsigned_type;
#ifdef __LP64__
  typedef int128_t signed2x_type;
  typedef uint128_t unsigned2x_type;
#endif
};

/**
 * @brief Basic math constructs and algorithms
 */
namespace math {

/**
 * @brief Returns the smallest integer k for which 2^k >= n.
 */
inline unsigned int intlog(unsigned int n) {
  unsigned int result = 0;
  while ((static_cast<uint64_t>(1) << result) < n) ++result;
  return result;
}

namespace field {
/**
 * @brief Compute fourier order of given number (odd prime)
 * @param n unsigned integer (up to 64-bits)
 * @return largest k, where n = 2^k*m+1, where m is odd
 */
inline unsigned int getPrimeFourierOrder(uint64_t n) {
  unsigned int result = 0;
  for (--n; !(n & 1) && n; ++result, n >>= 1)
    ;
  return result;
}

//**************************************************************************
template <typename F>
unsigned int getFourierOrder() {
  F newOmega;
  newOmega.template setTo<-1>();
  F omega = newOmega;
  // Maximum depth is is limited by max i where 2^i < Field size
  unsigned int depth;
  for (depth = 1; newOmega.hasInverse(); ++depth) {
    omega = newOmega;
    newOmega = omega.getSqrt();
  }
  depth--;
  return depth;
}
//**************************************************************************
template <typename F>
unsigned int computePrimitiveRoots(F newOmega, F* primRoots, F* primRootInvs,
                                   unsigned int count) {
  unsigned int depth;
  for (depth = 0; newOmega.hasInverse() && depth < count; ++depth) {
    primRoots[depth] = newOmega;
    primRootInvs[depth] = newOmega.getInverse();
    newOmega = newOmega.getSqrt();  // If sqrt does not exist sets to 0
  }
  return depth;
}

}  // namespace field
}  // namespace math

#endif  // NTRP_MATH_BASE_H
