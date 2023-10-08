/**
 * @file Newton.t.h Test driver for newton interpolation algorithm
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
 */

#include "interp/Newton.h"
#include "operation/field/setup.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(newton)

BOOST_AUTO_TEST_CASE_TEMPLATE(newton_alg, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // avoid unreferenced local variable warning

  BOOST_TEST_MESSAGE("  Testing " << F::getName());

  int const MAX_DEG = 10;
  F x[MAX_DEG + 1];
  F y[MAX_DEG + 1];
  // Assume polynomial f = sum (i+1) x^i
  for (int i = 0; i <= MAX_DEG; ++i) {
    F sum;
    sum.template setTo<0>();
    x[i].setTo(i+1);
    for (int j = 0; j <= MAX_DEG; ++j) {
      sum += x[i].getPow(j) * F(j+1);
    }
    y[i] = sum;
  }
  boost::scoped_array<F> coeffs(new F[MAX_DEG+1]);
  interp::newton(MAX_DEG, x, y, coeffs.get());

  // verify result
  for (int i = 0; i <= MAX_DEG; ++i) {
    BOOST_CHECK_MESSAGE(coeffs[i] == F(i+1),
        " f[i]==F(i+1) with i=" << i
        << " f[i]=" << coeffs[i] << " F(i+1)" << F(i+1));
  }
}

BOOST_AUTO_TEST_SUITE_END()
