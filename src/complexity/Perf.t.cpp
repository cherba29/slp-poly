/**
 * @file Perf.t.h Test driver for Perf component
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

#include "Perf.h"

#include "complexity/Estimator.h"
#include "complexity/AlgEnum.h"

#include <boost/timer.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( complexity )

using namespace complexity;
#define LOG_MODULE ::logging::LogModuleEnum::COMPLEXITY

BOOST_AUTO_TEST_CASE( test_perf_not_found ) {

  int n = 1000;
  Perf t(AlgEnum::VAL_OF_VAND_QUAD_SETUP,n,50);

  BOOST_CHECK(!t);

  //BOOST_CHECK( t.predicted() > 0 );
}

class QuadSetup {
public:
  double operator()(double size) {
    boost::timer tm;
    tm.restart();

    for (int n = 0; n < size; ++n) {
      double x = 1.001;
      for (int i = 0; i < 1000; ++i) { x *= x; }
    }
    return tm.elapsed();
  }
};

BOOST_AUTO_TEST_CASE(test_perf) {
  boost::shared_ptr<TrackWithEqn> tracker(new Tracker<LinearEquation>());

  LDBG_ << "Registering the algorithm";
  Perf::registerAlg(AlgEnum::VAL_OF_VAND_QUAD_SETUP,tracker);

  double n = 10000;
  int nTimes = 5;
  QuadSetup eval;

  LDBG_ << "Estimating for 1 sec";
  Estimator est(eval, tracker);
  est.run(1.0); // run for that many seconds

  n = tracker->getEquation().solveFor(.5); // n which is solvable in 1 seconds
  double prevDiff = 10;
  LDBG_ << "In 1 sec solvable size " << n;

  for (int i = 1; i < nTimes; ++i) {
    LDBG_ << "Equation " << i << ": " << tracker->getEquation()
          << " residual " << tracker->getResidual();
    Perf t(AlgEnum::VAL_OF_VAND_QUAD_SETUP,i*n,nTimes-i);

    BOOST_CHECK(t);
    BOOST_CHECK( t.predicted() >= 0 );

    double actual = 0;
    for (int j = 0; j < nTimes-i; ++j) {
      actual += eval(i*n); // Add one more measurement
    }
    double diff = fabs(actual - t.predicted());
    BOOST_CHECK_MESSAGE(diff < 1 || diff < prevDiff*1.05, // with in 5%
        "Actual: " << actual
        << " Predicted: " << t.predicted()
        << " Difference " << diff
        << " should be smaller than prev " << prevDiff);
    prevDiff = diff;
  }
  LDBG_ << "Final Equation: " << tracker->getEquation()
        << " residual " << tracker->getResidual();
}

BOOST_AUTO_TEST_SUITE_END()


bool init_unit_test() {
  return true;
}
