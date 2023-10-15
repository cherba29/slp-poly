/**
 * @file Tracker.cpp Implementation of Tracker
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

#include "math/numeric/svd.h"
#include "util/log.h"

#include <boost/scoped_array.hpp>
#include <cmath>

#define LOG_MODULE ::logging::LogModuleEnum::COMPLEXITY

namespace complexity {

namespace {}  // anonymous namespace

void TrackWithEqn::addMeasurement(long double size, long double timeVal,
                                  long double weight) {
  unsigned int nTerms = eqn_.getNTerms();

  if (measurements_.size() > nTerms) {
    MeasurementSet::iterator it = measurements_.begin();
    if (it->weight < weight) {
      if (measurements_.size() > 2 * nTerms) {
        measurements_.erase(it);
      }
      measurements_.insert(Measurement(size, timeVal, weight));
      dirty_ = true;
    }
  } else {
    measurements_.insert(Measurement(size, timeVal, weight));
    dirty_ = true;
  }
}

const Equation& TrackWithEqn::getEquation() const {
  if (dirty_) {  // If new measurement exist

    // E x C = T, solve for C

    int nMeasurements = measurements_.size();
    int nTerms = eqn_.getNTerms();
    if (nTerms > nMeasurements) {  // Not enough measurements
      return eqn_;
    }

    // Allocate memory for E and T all in one shot
    boost::scoped_array<long double> Etmp(
        new long double[nMeasurements * (nTerms + 2)]);
    long double** E = reinterpret_cast<long double**>(Etmp.get());
    for (int i = 0; i < nMeasurements; ++i) {
      E[i] = Etmp.get() + nMeasurements + i * nTerms;
    }
    // Last nMeasurements elements
    long double* T = Etmp.get() + nMeasurements * (nTerms + 1);

    // Fill in E and T matrices from measurement map
    MeasurementSet::const_iterator it = measurements_.begin();
    for (int i = 0; it != measurements_.end(); ++it, ++i) {
      eqn_.evaluateTermsAt(it->size, E[i]);
      T[i] = it->value;
    }

    // Solve for coefficients using singular value decomposition
    // Note that logically we consider this object constant
    // we are just refining equation coefficients in light of new measurements
    TrackWithEqn* this_ = const_cast<TrackWithEqn*>(this);
    math::numeric::solveViaSVD(E, (this_->eqn_).getCoeffs(), T, nMeasurements,
                               nTerms);

    this_->dirty_ = false;
  }
  return eqn_;
}

long double TrackWithEqn::getResidual() const {
  const Equation& eqn = this->getEquation();
  MeasurementSet::const_iterator it = measurements_.begin();
  long double residual = 0.0;
  for (; it != measurements_.end(); ++it) {
    long double diff = eqn.evaluateAt(it->size) - it->value;
    residual += diff * diff;
  }
  return std::sqrt(residual);
}

}  // namespace complexity
