/**
 * @file Estimator.cpp Implementation of Estimator
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

#include "Estimator.h"

#include "math/Base.h"  // for uint64_t
#include "util/log.h"

#include <cmath>

#define LOG_MODULE ::logging::LogModuleEnum::COMPLEXITY

namespace complexity {

namespace {}  // anonymous namespace

void Estimator::run(double maxRunTime) {
  LDBG_ << "Estimating for " << maxRunTime << " seconds";
  if (!tracker_) {
    LWRN_ << "Tracker is not set!";
    return;
  }
  const unsigned int FACTOR2P = 1;  // 2^FACTORP - grow by this factor
  long double totTime = 0.0;
  long double size = static_cast<long double>(1 << FACTOR2P);  // initial size
  uint64_t maxIter  // compute bound on max number of iterations
      = static_cast<uint64_t>(std::log(static_cast<long double>(
                                  std::numeric_limits<uint64_t>::max())) /
                              std::log(size));
  LDBG_ << "MAX iterations: " << maxIter;

  int nMeasurements = 0;  // Keep track how many valid measurements were made

  for (uint64_t count = 0; totTime < maxRunTime && count < maxIter; ++count) {
    LDBG_ << "Testing on size: " << size;
    long double timeTaken = f_(size);
    if (timeTaken > 0.100) {  // bigger number results in better measurement
      // std::numeric_limits<long double>::epsilon()) {
      nMeasurements++;
      ;
      tracker_->addMeasurement(static_cast<long double>(size), timeTaken, size);
      LDBG_ << "Time taken: " << timeTaken;
      totTime += timeTaken;
    } else if (nMeasurements > 0) {
      // report if time was too low
      LWRN_ << "Estimator time is less than epsilon for size " << size
            << " skipping this measurement...";
    }
    if (totTime > 0.1) {
      long double factor = maxRunTime / totTime;
      if (factor > (1 << FACTOR2P)) {
        size *= (1 << FACTOR2P);
      } else {
        size = size * factor;
      }
    } else {
      size *= (1 << FACTOR2P);
    }
  }

  if (nMeasurements < tracker_->getEquation().getNTerms()) {
    LWRN_ << "Was not able to get accurate estimate in " << maxRunTime
          << " secs. Got " << nMeasurements << ", need "
          << tracker_->getEquation().getNTerms();
  }
}

}  // namespace complexity
