/**
 * @file  Implementation of Perf
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

#include "Perf.h"
#include "Estimator.h"
#include "Tracker.h"

#include "util/log.h"

#include <boost/shared_ptr.hpp>

#define LOG_MODULE ::logging::LogModuleEnum::COMPLEXITY


namespace complexity {

namespace {

typedef std::map<std::string,boost::shared_ptr<TrackWithEqn> > Name2TrackerMap;
Name2TrackerMap trackedAlgs;

} // anonymous namespace

Perf::Perf(const std::string& alg, double size, int nTimes)
    : size_(size), nTimes_(nTimes) {
  Name2TrackerMap::iterator it = trackedAlgs.find(alg);
  if (it != trackedAlgs.end()) {
    tracker_ = it->second;
  }
  LTRC_ << "Starting perf monitor for " << alg
        << " size=" << size
        << ", ntimes=" << nTimes
        << " predicted " << this->predicted();
  tm_.restart();
}

Perf::~Perf() {
  if (tracker_) {
    double measurement = tm_.elapsed()/nTimes_;
    LDBG_ << "Adding measurement of " << measurement << " for size " << size_;
    tracker_->addMeasurement(size_,measurement,size_);
  }
}

double Perf::predicted() const {
  if (tracker_) {
    return nTimes_*tracker_->getEquation().evaluateAt(size_);
  } else {
    return std::numeric_limits<double>::quiet_NaN();
  }
}

double Perf::predicted(const std::string& alg, double size, int nTimes) {
  Name2TrackerMap::iterator it = trackedAlgs.find(alg);
  if (it == trackedAlgs.end()) {
    return std::numeric_limits<double>::quiet_NaN();
  }
  return nTimes*(it->second)->getEquation().evaluateAt(size);
}

double Perf::current() const {
  return tm_.elapsed();
}

int Perf::registerAlg(
    const std::string& alg, boost::shared_ptr<TrackWithEqn> eqn) {
  std::pair<Name2TrackerMap::iterator,bool> inserted
      = trackedAlgs.insert(std::make_pair(alg,eqn));
  return inserted.second ? 0 : -1;
}

boost::shared_ptr<TrackWithEqn> Perf::getTracker(const std::string& alg) {
  Name2TrackerMap::iterator it = trackedAlgs.find(alg);
  if (it == trackedAlgs.end()) {
    return boost::shared_ptr<TrackWithEqn>();
  }
  return it->second;
}

const Equation* Perf::getEquation(const std::string& alg) {
  Name2TrackerMap::iterator it = trackedAlgs.find(alg);
  if (it == trackedAlgs.end()) {
    return NULL;
  }
  return &(it->second->getEquation());
}

void Perf::exercise(const std::string& alg, TestFunction f, double maxTime) {
  Name2TrackerMap::iterator it = trackedAlgs.find(alg);
  if (it == trackedAlgs.end()) {
    return;
  }
  Estimator est(f,it->second);
  est.run(maxTime);
}

}  // namespace complexity
