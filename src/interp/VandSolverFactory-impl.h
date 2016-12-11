#ifndef NTRP_INTERP_VAND_SOLVER_FACTORY_H
#error "Do not include this file. This is used internally by VandSolverFactory.h"
#endif

/**
 * @file VandSolverFactory-impl.h
 */


#include "complexity/AlgEnum.h"
#include "complexity/PerfAlg.h"
#include "math/vandermonde/QuadSolver.h"
#include "math/vandermonde/FFTSolver.h"
#include "util/log.h"

#include <boost/log/attributes/named_scope.hpp>

#define LOG_MODULE ::logging::LogModuleEnum::VANDERMONDE

namespace interp {

namespace detail {

int getQuadFFTDiff(long double* diff, long double size, int nSolve);

} // namespace detail

template <class F>
VandSolverFactory<F>::VandSolverFactory(VandSolveTypeEnum choice)
    : choice_(choice) {
  LTRC_ << "Creating Vandermonde factory for " << choice;
}

/******************************************************************************/
template <class F>
boost::shared_ptr<math::vandermonde::Solver<F> >
VandSolverFactory<F>::newSolver(unsigned int size, const F* entries,
		int nSolve) const {
  BOOST_LOG_FUNCTION();
  LTRC_ << __FUNCTION__ << " Size: " << size;

  typedef boost::shared_ptr<math::vandermonde::Solver<F> > SolverPtr;
  typedef typename complexity::PerfAlg<
    complexity::NLogN2Equation,
    math::vandermonde::FFTSolver<F> > PerfFFTSolver;
  typedef typename complexity::PerfAlg<
    complexity::QuadEquation,
    math::vandermonde::QuadSolver<F> > PerfQuadSolver;

  switch (choice_.getId()) {
    case VandSolveTypeEnum::QUAD: {
      LTRC_ << "Building Quad Vandermonde of size: " << size;
      return SolverPtr(new PerfQuadSolver(
          complexity::AlgEnum::VAL_OF_VAND_QUAD_SOLVE, size, entries));
    }
    case VandSolveTypeEnum::FFT: {
      LTRC_ << "Building FFT Vandermonde of size: " << size;
      return SolverPtr(new PerfFFTSolver(
          complexity::AlgEnum::VAL_OF_VAND_FFT_SOLVE, size, entries));
    }
    case VandSolveTypeEnum::BEST: {

      LTRC_ << "Building BEST Vandermonde of size: " << size;
      long double diff;
      if (0 != detail::getQuadFFTDiff(&diff,size,nSolve)) {
        return boost::shared_ptr<math::vandermonde::Solver<F> >();
      }
      if (diff > 0) {
        LDBG_ << "Built QUAD Vandermonde";
        return SolverPtr(new PerfQuadSolver(
            complexity::AlgEnum::VAL_OF_VAND_QUAD_SOLVE, size, entries));
      } else {
        LDBG_ << "Built FFT Vandermonde";
        return SolverPtr(new PerfFFTSolver(
            complexity::AlgEnum::VAL_OF_VAND_FFT_SOLVE, size, entries));
      }
    }
    case VandSolveTypeEnum::UNKNOWN:
    default: {
      LWRN_ << "Unknown vandermonde factory option " << choice_;
      return boost::shared_ptr<math::vandermonde::Solver<F> >();
    }
  }
}

template <class F>
const char* VandSolverFactory<F>::getAlgName(unsigned int size,
		int nSolve) const {
  switch (choice_.getId()) {
    case VandSolveTypeEnum::QUAD: {
      return complexity::AlgEnum::VAL_OF_VAND_QUAD_SOLVE;
    }
    case VandSolveTypeEnum::FFT: {
      return complexity::AlgEnum::VAL_OF_VAND_FFT_SOLVE;
    }
    case VandSolveTypeEnum::BEST: {
      long double diff;
      if (0 != detail::getQuadFFTDiff(&diff,size,nSolve)) {
        return NULL;
      }
      if (diff < 0) {
        return complexity::AlgEnum::VAL_OF_VAND_FFT_SOLVE;
      } else {
        return complexity::AlgEnum::VAL_OF_VAND_QUAD_SOLVE;
      }
    }
    default:
      break;
  }
  return NULL;
}

}  // namespace interp

#undef LOG_MODULE
