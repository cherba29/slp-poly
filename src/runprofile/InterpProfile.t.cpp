#include "runprofile/InterpProfile.h"

#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(interp_profile)

using namespace ::runprofile;

BOOST_AUTO_TEST_CASE(setFieldOption_basic) {
  InterpProfile profile;

  profile.setFieldOption("A64");
#ifdef FIELD_A64
  BOOST_CHECK_EQUAL(profile.getFieldOption(), FieldOptionEnum::A64);
#endif
}

BOOST_AUTO_TEST_SUITE_END()


bool init_unit_test() {
  return true;
}
