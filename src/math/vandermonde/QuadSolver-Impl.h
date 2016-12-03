#ifndef NTRP_MATH_VANDERMONDE_QUAD_SOLVER_H
#error "Do not include this file. This is used internaly by QuadSolver.h"
#endif

/**
 * @file QuadSolver-Impl.h
 * @brief Implementation of quadratic vandermonde solver
 */

#include <sstream>
#include <stdexcept>

namespace math {
namespace vandermonde {

template<class F>
QuadSolver<F>::QuadSolver(unsigned int size, const F* entries) {

  size_ = size;
  entries_ = new F[size_];
  rootPoly_ = new F[size_];

  // Lagrange polynomial rootPoly_(x) ==
  //   (x - entries[0])*...*(x - entries[size-1])
  F coeff;
  for (unsigned int i = 0; i < size; i++) {
    entries_[i] = entries[i];
    rootPoly_[i] = coeff = -entries[i];
    for (unsigned int j = i; j > 0; j--) {
      rootPoly_[j] += rootPoly_[j-1];
      rootPoly_[j-1] *= coeff;
    }
  }
}

template<class F>
QuadSolver<F>::~QuadSolver() {
  delete[] rootPoly_;
  delete[] entries_;
}

template<class F>
void QuadSolver<F>::solveTranspose(const F* values, F* result) {
  if (size_ <= 1) {
    result[0] = values[0];
    return;
  }
  // Invert the Vandermonde matrix
  F coeff;
  F div;
  for (unsigned int i = 0; i < size_; ++i) {
    coeff.template setTo<1>();
    div.template setTo<1>();
    result[i] = values[size_ - 1];
    for (unsigned int j = size_ - 1; j > 0; j--) {
      // coeff == coefficient of x^j of rootPoly_[i](x)
      //       == rootPoly_(x)/(x - entries_[i])
      coeff *= entries_[i];
      coeff += rootPoly_[j];
      div *= entries_[i];
      div += coeff;
      result[i] += values[j - 1] * coeff;
    }
    // div = _rootPoly[i](_entries[i])
    if (div.hasInverse()) { // @ todo avoid exception
      result[i] /= div;
    } else {
      std::ostringstream errs;
      errs << "QuadVandermonde.solveTranspose: divisor " << div << " has no inverse";
      throw std::runtime_error(errs.str());
    }
  }
}

template<class F>
void QuadSolver<F>::evaluate(const F* coeffs, F* result) {
  unsigned int i, j;
  for (j = 0; j < size_; ++j)
    result[j] = coeffs[size_ - 1];
  for (i = size_ - 1; i > 0; i--) {
    for (j = 0; j < size_; ++j) {
      result[j] *= entries_[j];
      result[j] += coeffs[i - 1];
    }
  }
}

} // namespace vandermonde
} // namespace math
