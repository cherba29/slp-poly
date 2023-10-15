#ifndef NTRP_COMPLEXITY_PERF_H
#define NTRP_COMPLEXITY_PERF_H

/**
 * @file  Declaration of Perf
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

#include "Tracker.h"
#include "util/log.h"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/timer.hpp>

#define LOG_MODULE ::logging::LogModuleEnum::COMPLEXITY

namespace complexity {

typedef boost::function<long double(long double size)> TestFunction;

/**
 * @brief Performance predictor and gatherer
 * This class is designed to allow flexible gathering of preformance information
 * for the purposes of future performance predictions.
 * Example
 * @code
 * {
 *   Perf t("MainAlg",n,50);
 *   std::cout << "Predicted time for the algorithm is " << t.predicted() <<
 * std::endl;
 *
 *   // Invoke Algorithm routine 50 times
 *
 *   std::cout << "Actual time taken " << t.current() << std::endl;
 * } // Invoke destructor of t, registering yet another measurement for MainAlg
 * @endcode
 */
class Perf {
  typedef void (
      Perf::*bool_type)() const;  // private boolean type as ptr to mem fun
 public:
  /**
   * @brief Constructor
   */
  Perf(const std::string& alg, double size, int nTimes);

  /**
   * @brief Destructor
   */
  ~Perf();

  /**
   * @brief Return predicted time in seconds
   */
  double predicted() const;

  static const Equation* getEquation(const std::string& alg);
  /**
   * @brief Return time passed since construction of this object
   */
  double current() const;

  static int registerAlg(const std::string& alg,
                         boost::shared_ptr<TrackWithEqn> eqn);

  template <typename EQN>
  static int registerAlg(const std::string& alg);

  static boost::shared_ptr<TrackWithEqn> getTracker(const std::string& alg);

  static double predicted(const std::string& alg, double size, int nTimes);

  static void exercise(const std::string& alg, TestFunction f, double maxTime);

  /**
   * @brief Return true if this enum is set to some valid value.
   */
  operator bool_type() const {
    return tracker_ ? &Perf::this_type_does_not_support_comparisons : 0;
  }

 private:
  boost::shared_ptr<TrackWithEqn> tracker_;
  double size_;
  int nTimes_;
  boost::timer tm_;

  void this_type_does_not_support_comparisons() const {}
};  // class Perf

template <typename EQN>
int Perf::registerAlg(const std::string& alg) {
  boost::shared_ptr<TrackWithEqn> trc = Perf::getTracker(alg);
  if (trc) {
    return -1;
  }
  boost::shared_ptr<TrackWithEqn> tracker(new Tracker<EQN>());

  Perf::registerAlg(alg, tracker);
  return 0;
}

}  // namespace complexity

#undef LOG_MODULE

#endif  // NTRP_COMPLEXITY_PERF_H
