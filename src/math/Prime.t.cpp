#include "math/util/prime.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( prime )

BOOST_AUTO_TEST_CASE( maybe_prime ) {
  BOOST_TEST_MESSAGE("  Testing " << math::NPRIMES << " primes");
  for (unsigned int i = 0; i < math::NPRIMES; ++i) {
    BOOST_CHECK(math::maybe_prime(math::primes[i]));
  }
  //Field::base_type maxVal = static_cast<Field::base_type>(Field::getSize());
  //BOOST_CHECK(math::maybe_prime(maxVal));
}
BOOST_AUTO_TEST_SUITE_END()


bool init_unit_test() {
  return true;
}
