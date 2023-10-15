#ifndef NTRP_INTERP_NEWTON_H
#define NTRP_INTERP_NEWTON_H

/**
 * @file Newton.h
 * @brief univariate interpolation algorithms.
 */

#include "algebra/Polynomial.h"
#include "util/log.h"

#define LOG_MODULE ::logging::LogModuleEnum::INTERPOLATOR

namespace interp {
/**
 * @brief Zippel, _Effective Polynomial Computation_, p.217
 *        Quadratic algorithm
 *        Interpolates a polynomial of degree <= maxdeg from maxdeg+1 points
 *        (in x) and values (in y), placing its coefficients in f
 *
 * @param maxdeg - size of x, y and f arrays
 * @param x - array of evaluation points
 * @param y - array of values at evaluation points, i.e. y[i] = poly(x[i])
 * @param f - output (must be preallocated of size maxdeg+1),
 *            result will be such which satisfies poly = sum f[i]*x^i
 */

template <typename F>
void newton(unsigned int maxdeg, const F* x, const F* y, F* f) {
  F qVal;
  F fVal;
  F mult;
  // Allocate coefficient array on the stack.
#if _MSC_VER || defined __clang__
  F* q = static_cast<F*>(alloca(sizeof(F) * (maxdeg + 1)));
#else
  F q[maxdeg + 1];
#endif
  for (unsigned int i = 0; i <= maxdeg; ++i) {
    // Evaluate f and q at x[i]
    q[i].template setTo<1>();
    qVal.template setTo<1>();
    f[i].template setTo<0>();
    fVal.template setTo<0>();
    for (unsigned int j = i; j > 0; --j) {
      qVal *= x[i];
      qVal += q[j - 1];
      fVal *= x[i];
      fVal += f[j - 1];
    }
    // Add multiple of q to f, then multiply q by (x - _points[i])
    mult = (y[i] - fVal);
    mult /= qVal;
    for (unsigned int j = i; j > 0; --j) {
      f[j] += q[j] * mult;
      q[j] *= -x[i];
      q[j] += q[j - 1];
    }
    f[0] += q[0] * mult;
    q[0] *= -x[i];
  }
}

template <typename F, typename E>
boost::shared_ptr<algebra::Polynomial<F> > newtonInterpolateUnivariateEvaluator(
    unsigned int maxdeg, const std::string& varName, E& evaluator) {
  unsigned int nTerms = maxdeg + 1;
  // Use stack allocation for these arrays.
#if _MSC_VER || defined __clang__
  F* xPoints = static_cast<F*>(alloca(sizeof(F) * nTerms));
  F* yPoints = static_cast<F*>(alloca(sizeof(F) * nTerms));
  F* coeffs = static_cast<F*>(alloca(sizeof(F) * nTerms));
#else
  F xPoints[nTerms];
  F yPoints[nTerms];
  F coeffs[nTerms];
#endif
  LTRC_ << "Newton interpolating " << varName << " up to deg " << maxdeg;
  for (unsigned int i = 0; i < nTerms; ++i) {
    xPoints[i].invertableRandom();
    yPoints[i] = evaluator(xPoints + i);
    LTRC_ << "  x=" << xPoints[i] << "  y=" << yPoints[i];
  }
  interp::newton(maxdeg, xPoints, yPoints, coeffs);

  int nNonZeroTerms = 0;

  for (unsigned int i = 0; i < nTerms; ++i) {
    if (!coeffs[i].template is<0>()) ++nNonZeroTerms;
  }
  // Univariate polynomial
  std::vector<std::string> varNames(1, varName);

  boost::shared_ptr<algebra::Polynomial<F> > ppol =
      boost::shared_ptr<algebra::Polynomial<F> >(
          new algebra::Polynomial<F>(varNames, nNonZeroTerms));

  for (unsigned int i = 0, t = 0; i < nTerms; ++i) {
    if (!coeffs[i].template is<0>()) {
      ppol->setCoeff(t, coeffs[i]);
      ppol->setDegree(t, 0, i);
      ++t;
    }
  }
  return ppol;
}

}  // namespace interp
#undef LOG_MODULE
#endif  // NTRP_INTERP_NEWTON_H
