#ifndef NTRP_INTERP_INTERPOLATOR_H
#define NTRP_INTERP_INTERPOLATOR_H

/**
 * @file Interpolator.h Definition of interp::Interpolator
 */

#include "interp/VarOrderEnum.h"
#include "interp/PrunningEnum.h"

#include "algebra/Polynomial.h"
#include "algebra/PartialPolynomial.h"

#include "math/util/prime.h"

#include "VandSolverFactory.h"
#include "Newton.h"

#include <boost/shared_ptr.hpp>

namespace interp {

template <typename F>
class Interpolator {
public:

  /**
   * @brief Contruct interpolator
   * @param vf Vandermonde matrix solver factory
   * @param polOrderSortBy Use polynomial order for internal algorithm
   * @param prn use pruning technique to reduce number of terms in
   *        intermediate polynomials
   * @param verifyStage - at each stage of randomized interpolation algorithm
   *        verify that partial result matched black box that many times
   * @param verifyFinal - verify final polynomial against black box on that
   *        many random points
   */
  Interpolator(
      boost::shared_ptr<VandSolverFactory<F> > vf,
      const typename algebra::PartialPolynomial<F>::OrderByFunctor&
          polOrderSortBy,
      PrunningEnum prn,
      int verifyStage, int verifyFinal)
    : vf_(vf), polOrderSortBy_(polOrderSortBy), prn_(prn),
      verifyStage_(verifyStage), verifyFinal_(verifyFinal) { }

  /**
   * @brief  Interpolate evaluator
   *
   * @param eval reference to multivariate evaluator eval(x_0,...x_n-1) in
   *        n variables
   * @param ppol (if specified) partial polynomial pol(x0,...,x_m-1) in
   *        m variables where m < n and moreover
   *        eval(x_0,...,x_m-1,v_m...v_n-1) === pol(x_0,...,x_m-1)
   *        for some fixed v_m,...,v_n-1
   * @return polynomial such that pol == eval on all points x_i
   */
  template <typename E>
  boost::shared_ptr<algebra::Polynomial<F> const>
  interpolate(E& evaluator,
              boost::shared_ptr<algebra::PartialPolynomial<F> > ppol
                  = boost::shared_ptr<algebra::PartialPolynomial<F> >());

  ~Interpolator() {  }
/*
  void setVarOrder(const eval::VarOrder& ord) {
    std::vector<int> varPerm(nVars+1);
    switch(ord) {
      case InterpProfile::Prunning::NONE: {
        varPerm.pop_back(); // Not homogenizing
        for (int i = 0; i < nVars; ++i) { varPerm[i] = stats[i].varIdx;  }
        break;
      }
      case InterpProfile::Prunning::ORIG_ORDER: {
        varPerm[0] = nVars; // If homogenizing, hom var will be first
        for (int i = 1; i <= nVars; ++i) { varPerm[i] = stats[i-1].varIdx;  }
        break;
      }
      case InterpProfile::Prunning::MAX_LAST:{
        varPerm[0] = nVars; // If homogenizing, hom var will be first
        // find max deg idx and place it last
        int maxDeg = 0; int maxIdx = -1; int i;
        for (i = 0; i < nVars; ++i) {
          if (maxDeg < stats[i-1].pol->getMaxDegree(0)) {
            maxDeg = stats[i-1].pol->getMaxDegree(0);
            maxIdx = i;
          }
        }
        int skip = varPerm[nVars] = i; // Place it last
        for (int i = 1; i < nVars; ++i) { // Copy remaining in the order requested
          if (i==skip) continue;
          varPerm[i] = stats[i-1].varIdx;
        }
        break;
      }
      default: {
        LERR_ << "Bad prunning option";
        throw std::runtime_exception("Bad prunning option");
      }
    }
    if (!evalPtr->swapInVariableOrder(&varPerm)) {
      LERR_ << "Could not set variable order";
    }
  }
  */
private:
  /**
   * @brief Helper function to interpolate single variable
   */
  template <typename E>
  boost::shared_ptr<algebra::PartialPolynomial<F> >
  sparseInterpolateStage(
      E& evaluator,
      const algebra::PartialPolynomial<F>& polA,
      const algebra::PartialPolynomial<F>& polB);

  boost::shared_ptr<VandSolverFactory<F> > vf_;

  /**
   * @brief Chosen ordering on partially interpolated polynomials
   */
  typename algebra::PartialPolynomial<F>::PtrOrder polOrderSortBy_;

  PrunningEnum prn_;

  unsigned int verifyStage_;
  unsigned int verifyFinal_;
};

}  // namespace interp

#include "Interpolator-impl.h"

#endif  // NTRP_INTERP_INTERPOLATOR_H
