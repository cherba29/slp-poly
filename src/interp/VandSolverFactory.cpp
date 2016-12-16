/**
 * @file VandSolverFactory.cpp
 * @brief Non template implementation details of VandSolverFactory
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
 * $URL$
 * $Id$
 */


#include "VandSolverFactory.h"

#include "complexity/AlgEnum.h"
#include "complexity/Perf.h"
#include "complexity/Equation.h"

#include "util/log.h"

#define LOG_MODULE ::logging::LogModuleEnum::VANDERMONDE

namespace interp {
namespace detail {

int getQuadFFTDiff(long double* diff, long double size, int nSolve) {

  using namespace complexity;

  const Equation* quadSetupEqn = Perf::getEquation(
		  complexity::AlgEnum::VAL_OF_VAND_QUAD_SETUP);
  if (!quadSetupEqn) {
    LWRN_ << "Unknown performance of VandQuadSetupAlg";
    return -1;
  }
  const Equation* quadSolveEqn = Perf::getEquation(
		  complexity::AlgEnum::VAL_OF_VAND_QUAD_SOLVE);
  if (!quadSolveEqn) {
    LWRN_ << "Unknown performance of VandQuadSolveAlg";
    return -1;
  }
  const Equation* fftSetupEqn = Perf::getEquation(
		  complexity::AlgEnum::VAL_OF_VAND_FFT_SETUP);
  if (!fftSetupEqn) {
    LWRN_ << "Unknown performance of VandFFTSetupAlg";
    return -1;
  }
  const Equation* fftSolveEqn = Perf::getEquation(
		  complexity::AlgEnum::VAL_OF_VAND_FFT_SOLVE);
  if (!fftSolveEqn) {
    LWRN_ << "Unknown performance of VandFFTSolveAlg";
    return -1;
  }


  long double quadTotTime = std::max<long double>(quadSetupEqn->evaluateAt(size)
      + quadSolveEqn->evaluateAt(size) * nSolve, 0.0);
  long double fftTotTime = std::max<long double>(fftSetupEqn->evaluateAt(size)
      + fftSolveEqn->evaluateAt(size) * nSolve, 0.0);

  LTRC_ << "... quadTime: " << quadTotTime << " for size " << size;
  LTRC_ << "   setup eqn: " << *quadSetupEqn;
  LTRC_ << "   solve eqn: " << *quadSolveEqn;
  LTRC_ << "...  fftTime: " << fftTotTime << " for size " << size;
  LTRC_ << "   setup eqn: " << *fftSetupEqn;
  LTRC_ << "   solve eqn: " << *fftSolveEqn;

  *diff = quadTotTime - fftTotTime;
  return 0;
}

} // namespace detail

} // namespace interp
