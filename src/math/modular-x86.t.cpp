#include "math/util/modular-x86.h"
#include "math/util/prime.h"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

BOOST_AUTO_TEST_SUITE( modular_x86 )

typedef boost::mpl::list<
        uint32_t
#ifdef __LP64__
        ,uint64_t
#endif
    > UIntTypes;

BOOST_AUTO_TEST_CASE_TEMPLATE( mul_add_test, U, UIntTypes ) {
  U a = math::ModOp<U>::add(2,4,5);
  BOOST_CHECK_EQUAL( a, 1 );

  U p = math::Prime<U>::A;

  U x = math::ModOp<U>::add(p-2,p-3,p);
  BOOST_CHECK_EQUAL( x, p-5 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( sub_mod_test, U, UIntTypes ) {
  U a = math::ModOp<U>::sub(2,3,5);
  BOOST_CHECK_EQUAL( a, 4 );

  U p = math::Prime<U>::A;

  U x = math::ModOp<U>::sub(p-2,p-3,p);
  BOOST_CHECK_EQUAL( x, 1 );
}


BOOST_AUTO_TEST_CASE_TEMPLATE( mul_mod_test, U, UIntTypes ) {
  U a = math::ModOp<U>::mul(2,3,5);
  BOOST_CHECK_EQUAL( a, 1 );

  U p = math::Prime<U>::A;

  U x = math::ModOp<U>::mul(p-2,p-3,p);
  BOOST_CHECK_EQUAL( x, 6 );
}


BOOST_AUTO_TEST_CASE_TEMPLATE( mul_add_mod_test, U, UIntTypes ) {
  U a = math::ModOp<U>::mul_add(2,3,4,5);
  BOOST_CHECK_EQUAL( a, 0 );

  U p = math::Prime<U>::A;

  U x = math::ModOp<U>::mul_add(p-2,p-3,p-6,p);
  BOOST_CHECK_EQUAL( x, 0 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( sub_mul_mod_test, U, UIntTypes ) {
  U a = math::ModOp<U>::sub_mul(2,3,4,5);
  BOOST_CHECK_EQUAL( a, 1 );

  U p = math::Prime<U>::A;

  U x = math::ModOp<U>::sub_mul(p-2,p-4,(p-1)/2,p);
  BOOST_CHECK_EQUAL( x, p-1 );
}


BOOST_AUTO_TEST_CASE_TEMPLATE( pow_mod_test, U, UIntTypes ) {
  U a = math::ModOp<U>::pow(2,3,5);
  BOOST_CHECK_EQUAL( a, 3 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( sqrt_mod_test, U, UIntTypes) {
   U a = math::ModOp<U>::sqrt(1,5);
   BOOST_CHECK_EQUAL( a, 1 );

   U p = math::Prime<U>::A;
   U x = math::ModOp<U>::sqrt(p-1,p);
   U y = math::ModOp<U>::mul(x,x,p);
   BOOST_CHECK_EQUAL( y, p-1 );
}


BOOST_AUTO_TEST_CASE_TEMPLATE( multi_mod_test, U, UIntTypes ) {
  U primes[] = //{ math::Prime<U>::A, math::Prime<U>::B };
  { 3, 5, 7, 11 };
  const size_t N = sizeof(primes)/sizeof(*primes);

  U mixMuls[N];
  math::MultiMod<U>::mixedMults(mixMuls, primes, N);
  BOOST_CHECK_EQUAL(mixMuls[0], 1);
  BOOST_CHECK_EQUAL(mixMuls[1], 2); // 3*2 mod 5 == 1
  BOOST_CHECK_EQUAL(mixMuls[2], 1); // (3*5) * 1 mod 7 == 1
  BOOST_CHECK_EQUAL(mixMuls[3], 2); // (3*5*7) * 2 mod 11 == 1

  math::MultiMod<U> crt(primes, N);

  U rads[N];
  U crts[N] = {1, 1, 1, 1};
  crt.crtToMixedRadix(rads, crts);
  BOOST_CHECK_EQUAL(rads[0], 1);
  BOOST_CHECK_EQUAL(rads[1], 0);
  BOOST_CHECK_EQUAL(rads[2], 0);
  BOOST_CHECK_EQUAL(rads[3], 0);

  BOOST_CHECK_EQUAL(false, crt.mixedRadixIsNegative(rads));

  char buf[20];
  int len = crt.mixedRadixToString(buf,sizeof(buf),rads,10);
  BOOST_CHECK_EQUAL(len, 1);
  BOOST_CHECK_EQUAL(buf[0], '1');
  BOOST_CHECK_EQUAL(buf[1], '\0');
}


BOOST_AUTO_TEST_SUITE_END()


bool init_unit_test() {
  return true;
}
