#ifndef NTRP_COMPLEXITY_PERFALG_H
#define NTRP_COMPLEXITY_PERFALG_H

/**
 * @file PerfAlg.h Declaration of PerfAlg
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
#include "util/log.h"

namespace complexity {

template <typename EQN, typename T>
class PerfAlg : public T {
 public:
  PerfAlg(const char* name) : name_(name) { Perf::registerAlg<EQN>(name_); }

  template <typename A1>
  PerfAlg(const char* name, A1 a1) : T(a1), name_(name) {
    Perf::registerAlg<EQN>(name_);
  }

  template <typename A1, typename A2>
  PerfAlg(const char* name, A1 a1, A2 a2) : T(a1, a2), name_(name) {
    Perf::registerAlg<EQN>(name_);
  }

  template <typename A1, typename A2, typename A3>
  PerfAlg(const char* name, A1 a1, A2 a2, A3 a3) : T(a1, a2, a3), name_(name) {
    Perf::registerAlg<EQN>(name_);
  }

  const std::string& getName() const { return name_; }

  double predicted(double size, int nTimes) const {
    return Perf::predicted(name_, size, nTimes);
  }

 private:
  std::string name_;
};  // class PerfAlg

}  // namespace complexity

#endif  // NTRP_COMPLEXITY_PERFALG_H
