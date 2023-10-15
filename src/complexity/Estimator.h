#ifndef NTRP_COMPLEXITY_ESTIMATOR_H
#define NTRP_COMPLEXITY_ESTIMATOR_H

/**
 * @file Estimator.h Declaration of Estimator
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

#include "complexity/Equation.h"
#include "complexity/Tracker.h"
#include "util/log.h"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#define LOG_MODULE ::logging::LogModuleEnum::COMPLEXITY

namespace complexity {

typedef boost::function<long double(long double size)> TestFunction;
/**
 * @brief
 * Example
 * @code
 *   Estimator t;
 *   // ...
 * @endcode
 */
class Estimator {
  TestFunction f_;
  boost::shared_ptr<TrackWithEqn> tracker_;

 public:
  /**
   * @brief Constructor
   */
  Estimator(TestFunction f, boost::shared_ptr<TrackWithEqn> t)
      : f_(f), tracker_(t) {}

  /**
   * @brief Destructor
   */
  ~Estimator() {}

  /**
   * @brief Run supplied functor for given amount of time.
   * This routine will try to make measurements using complexity
   * tracker
   */
  void run(double maxRunTime);

  boost::shared_ptr<TrackWithEqn> getTracker() { return tracker_; }

};  // class Estimator

}  // namespace complexity

#undef LOG_MODULE

#endif  // NTRP_COMPLEXITY_ESTIMATOR_H
