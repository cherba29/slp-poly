#ifndef NTRP_COMPLEXITY_TRACKER_H
#define NTRP_COMPLEXITY_TRACKER_H

/**
 * @file Tracker.h Declaration of Tracker
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

#include "Equation.h"

#include "util/log.h"
//#include <boost/concept/assert.hpp>
#include <map>
#include <set>

#define LOG_MODULE ::logging::LogModuleEnum::COMPLEXITY

namespace complexity {

class Measurement {
public:
  long double size;
  long double value;
  long double weight;
  Measurement(long double s, long double v, long double w) :
      size(s), value(v), weight(w) {}
};

inline bool operator<(const Measurement& a, const Measurement& b) {
  if (a.size == b.size) {
    if (a.weight == b.weight) {
      return a.value < b.value;
    }
    return a.weight < b.weight;
  }
  return a.size < b.size;
}

/**
 * @brief Non-templatized performance tracker.
 * @see Tracker
 */
class TrackWithEqn {
//  typedef std::multimap<long double, long double> MeasurementSet;
  typedef std::set<Measurement> MeasurementSet;
  MeasurementSet measurements_;

  Equation& eqn_;
  bool dirty_;
public:

  TrackWithEqn(Equation& eqn) : eqn_(eqn), dirty_(true) {}

  /**
   * @brief Add new measurement.
   *        Complexity equation will be recalculated when getEuation is called.
   * @param size Current problem size
   * @param timeVal Observed time taken to solve the problem of this size
   * @param type - not used in this implementation. Here to be used by derived
   *        classes in case types of various measurements need to be tracked.
   */
  void addMeasurement(
      long double size, long double timeVal, long double weight);

  /**
   * @brief Return complexity equation, if new measurements were added
   *        equation is recalculated.
   * @see addMeasurement
   */
  const Equation& getEquation() const;

  /**
   * @brief return residual error which is sqrt of sum squared of
   * residual errors of each measurement
   */
  long double getResidual() const;
};

/**
 * @brief Responsible for collecting timed measurements and computing
 *        corresponding complexity equation.
 * Example
 * @code
 *   Tracker<BigOQuadEquation> t;
 *   boost::timer tm;
 *   // Call algorithm with complexity n^2 on input of size n
 *   t.addMeasurement(n, tm.elapsed());
 *   // ...
 *   // Check to see how much time it will take to solve problem of size m
 *   // with same algorithm
 *   std::cout << "Predicted time for size " << m << " is "
 *             << t.getEquation().evaluateAt(m) << std::endl;
 * @endcode
 * Note that at least few measurements must be observed before meaningful
 * prediction times can be produced.
 */
template <typename EqnT>
class Tracker : public TrackWithEqn {
//  BOOST_CONCEPT_ASSERT((Convertable<EqnT,Equation>));
  EqnT eqn_;

public:

  Tracker() : TrackWithEqn(eqn_) {  }
  //Tracker(EqnT eqn) : TrackWithEqn(eqn_), eqn_(eqn) {  }
  ~Tracker() {  }

}; // class Tracker

} // close namespace complexity



#undef LOG_MODULE


#endif  // #ifndef NTRP_COMPLEXITY_TRACKER_H
