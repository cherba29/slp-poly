#ifndef MATH_BENCHMARK_FIELDSETUP_H_
#define MATH_BENCHMARK_FIELDSETUP_H_

#include "math/field/Field.h"

#include "math/field/util/Field-Util.h"

#include "math/field/Field_uint2x.h"
#include "math/field/Field_uint.h"
#include "math/field/CRT.h"
#include "math/field/CRT2.h"

#include <boost/mpl/list.hpp>

namespace math {


template <typename F>
struct FieldFixture {
  operator bool() const { return true; }
  //bool operator !() { return true; }
};

#ifdef __LP64__
template <>
struct FieldFixture<math::field::Field<uint64_t> > {
  FieldFixture() {
    math::field::Field<uint64_t>::setPrime(
      static_cast<math::field::Field<uint64_t>::ubase_type>(math::Prime<uint64_t>::A));
  }
  ~FieldFixture() {
    math::field::Field<uint64_t>::reset();
  }
  operator bool() const { return true; }
};
#endif

template <>
struct FieldFixture<math::field::Field<uint32_t> > {
  FieldFixture() {
    math::field::Field<uint32_t>::setPrime(
      static_cast<math::field::Field<uint32_t>::ubase_type>(math::Prime<uint32_t>::A));
  }
  ~FieldFixture() {
    math::field::Field<uint32_t>::reset();
  }
  operator bool() const { return true; }
};

template <>
struct FieldFixture<math::field::CRT<uint32_t> > {
  FieldFixture()  {
    uint32_t primes[] = {
        math::Prime<uint32_t>::A,
        math::Prime<uint32_t>::B,
        math::Prime<uint32_t>::C
    };

    math::field::CRT<uint32_t>::setPrimes(primes, sizeof(primes)/sizeof(primes[0]));
  }
  ~FieldFixture() {
    math::field::CRT<uint32_t>::reset();
  }
  operator bool() const { return true; }
};

template <>
struct FieldFixture<math::field::CRT2<uint32_t> > {
  FieldFixture()  {
    uint32_t primes[] = {
        math::Prime<uint32_t>::A,
        math::Prime<uint32_t>::B,
        math::Prime<uint32_t>::C
    };

    math::field::CRT2<uint32_t>::setPrimes(primes, sizeof(primes)/sizeof(primes[0]));
  }
  ~FieldFixture() {
    math::field::CRT2<uint32_t>::reset();
  }
  operator bool() const { return true; }
};

#ifdef __LP64__
template <>
struct FieldFixture<math::field::CRT<uint64_t>  > {
  FieldFixture()  {
    uint64_t primes[] = {
        math::Prime<uint64_t>::A,
        math::Prime<uint64_t>::B,
        math::Prime<uint64_t>::C
    };

    math::field::CRT<uint64_t>::setPrimes(primes, sizeof(primes)/sizeof(primes[0]));
  }
  ~FieldFixture() {
    math::field::CRT<uint64_t>::reset();
  }
  operator bool() const { return true; }
};

template <>
struct FieldFixture<math::field::CRT2<uint64_t>  > {
  FieldFixture()  {
    uint64_t primes[] = {
        math::Prime<uint64_t>::A,
        math::Prime<uint64_t>::B,
        math::Prime<uint64_t>::C
    };

    math::field::CRT2<uint64_t>::setPrimes(primes, sizeof(primes)/sizeof(primes[0]));
  }
  ~FieldFixture() {
    math::field::CRT2<uint64_t>::reset();
  }
  operator bool() const { return true; }
};
#endif

typedef boost::mpl::list<
    math::field::Field_uint<uint32_t,math::Prime<uint32_t>::A>,
    math::field::Field_uint<uint32_t,math::Prime<uint32_t>::B>,
    math::field::Field_uint<uint32_t,math::Prime<uint32_t>::C>,
    math::field::Field_uint2x<uint32_t,math::Prime<uint32_t>::A,math::Prime<uint32_t>::B>,
    math::field::Field<uint32_t>,
    math::field::CRT<uint32_t>,
    math::field::CRT2<uint32_t>
#ifdef __LP64__
  , math::field::Field_uint<uint64_t,math::Prime<uint64_t>::A>
  , math::field::Field_uint<uint64_t,math::Prime<uint64_t>::B>
  , math::field::Field_uint<uint64_t,math::Prime<uint64_t>::C>
  , math::field::Field_uint2x<uint64_t,math::Prime<uint64_t>::A,math::Prime<uint64_t>::B>
  , math::field::Field<uint64_t>
  , math::field::CRT<uint64_t>
  , math::field::CRT2<uint64_t>
#endif
> FieldTypes;

} // namespace math

#endif /* MATH_BENCHMARK_FIELDSETUP_H_ */
