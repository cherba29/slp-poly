#include <math/Base.h>
#include <operation/field/setup.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Base)

BOOST_AUTO_TEST_CASE(intlog) {
  unsigned int p2w = math::intlog(0);
  BOOST_CHECK_EQUAL(p2w, 0);
  p2w = math::intlog(1);
  BOOST_CHECK_EQUAL(p2w, 0);
  p2w = math::intlog(2);
  BOOST_CHECK_EQUAL(p2w, 1);
  for (unsigned int i = 0; i < sizeof(int) * 8; ++i) {
    p2w = math::intlog(1 << i);
    BOOST_CHECK_EQUAL(p2w, i);
  }
  for (unsigned int i = 1; i < sizeof(int) * 8; ++i) {
    p2w = math::intlog((1 << i) + 1);
    BOOST_CHECK_EQUAL(p2w, i + 1);
  }
  for (unsigned int i = 2; i < sizeof(int)*8; i++) {
    p2w = math::intlog((1 << i) - 1);
    BOOST_CHECK_EQUAL(p2w, i);
  }
  // Check when every bit is 1
  p2w = math::intlog(static_cast<unsigned>(-1));
  BOOST_CHECK_EQUAL(p2w, sizeof(int) * 8);
}

BOOST_AUTO_TEST_SUITE_END()
