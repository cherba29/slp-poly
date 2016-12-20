/**
 * @file MiltiIndexMap.t.cpp Test driver for MultiIndexMap class
 *
 * Copyright (C) 2010  Arthur D. Cherba
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "MultiIndexMap.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>


using namespace util;
using boost::test_tools::output_test_stream;

/******************************************************************************/
BOOST_AUTO_TEST_CASE( multiindex_empty_constructor ) {

  MultiIndexMap m;

  output_test_stream output;

  output << m;

  BOOST_CHECK(output.is_empty(false /* flush stream */));

}
/******************************************************************************/
BOOST_AUTO_TEST_CASE( multiindex_number ) {
  MultiIndexMap m;
  output_test_stream output;

  m = 1;

  output << m;

  BOOST_CHECK( !output.is_empty( false /* flush stream */) );
  BOOST_CHECK( output.check_length(1, false));
  BOOST_CHECK( output.is_equal("1", false));
}
/******************************************************************************/
BOOST_AUTO_TEST_CASE( multiindex_string ) {
  MultiIndexMap m;
  output_test_stream output;

  const char str[] = "Hello World";
  m = str;

  output << m;

  BOOST_CHECK(!output.is_empty( false /* flush stream */) );
  BOOST_CHECK(output.check_length(sizeof(str)+1, false));
  BOOST_CHECK(output.is_equal("\"Hello World\"", false));
}
/******************************************************************************/
BOOST_AUTO_TEST_CASE( multiindex_test_map ) {
  MultiIndexMap m;

  output_test_stream output;

  const char str[] = "Hello World";
  m["test"] = str;

  BOOST_CHECK_EQUAL(m["test"],str);

  output << m;

  const char expected[] = "\ntest: \"Hello World\"";

  BOOST_CHECK( !output.is_empty( false /* flush stream */) );
  BOOST_CHECK( output.check_length(sizeof(expected)-1, false));
  BOOST_CHECK( output.is_equal(expected, false));
}
/******************************************************************************/
BOOST_AUTO_TEST_CASE( multiindex_test_idx_map ) {
  MultiIndexMap m;
  output_test_stream output;

  const char str[] = "Hello World";
  m["test"]["subtest"] = str;

  BOOST_CHECK_EQUAL(m["test"]["subtest"],str);

  output << m;

  const char expected[] = "\ntest: \n  subtest: \"Hello World\"";

  BOOST_CHECK( !output.is_empty( false /* flush stream */) );
  BOOST_CHECK( output.check_length(sizeof(expected)-1, false));
  BOOST_CHECK( output.is_equal(expected, false));
}
/******************************************************************************/
BOOST_AUTO_TEST_CASE( multiindex_test_list ) {

  MultiIndexMap m;

  output_test_stream output;

  const char str[] = "Hello World";
  m += str;

  output << m;

  const char expected[] = "[ \"Hello World\" ]";

  BOOST_CHECK( !output.is_empty( false /* flush stream */) );
  BOOST_CHECK( output.check_length(sizeof(expected)-1, false));
  BOOST_CHECK( output.is_equal(expected, false));
}
/******************************************************************************/
BOOST_AUTO_TEST_CASE( multiindex_test_list2 ) {
  MultiIndexMap m;

  output_test_stream output;

  m += "x";
  m += "t_1";
  m += "t_2";

  output << m;

  const char expected[] = "[ x, t_1, t_2 ]";

  BOOST_CHECK( !output.is_empty( false /* flush stream */) );
  BOOST_CHECK( output.check_length(sizeof(expected)-1, false));
  BOOST_CHECK( output.is_equal(expected, false));
}


bool init_unit_test() {
  return true;
}
