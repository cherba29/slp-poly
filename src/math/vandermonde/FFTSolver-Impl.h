#ifndef NTRP_MATH_VANDERMONDE_FFT_SOLVER_H
#error "Do not include this file. This is used internaly by FFTSolver.h"
#endif

/**
 * @file FFTSolver-Impl.h
 * @brief Implementation of Fast Fourier Vandermonde solver
 *
 * $URL$
 * $Id$
 */

#include "math/fft/algorithm.h"
#include "util/SmartAssert.h"

namespace math {
namespace vandermonde {

template <class F>
FFTSolver<F>::FFTSolver(unsigned int size, const F* entries)
    : size_(size), logsize_(math::intlog(size_)) {  // size_ < 2^logsize_

  ASSERT0(size > 0).msg("FFTVandermonde<F> cant initialize with zero size");

  unsigned int i;

  // Compute B(z)=prod_i (z-entries[i]) using tree multiplication using FFT

  // Build subproducts (and inverses of reverses)
  // page 281 of Modern Computer algebra
  subprods_ = new F*[logsize_ + 1];  // +1 in case size is not power of 2,
                                     // extra step will be needed.

  F* subprod = subprods_[0] = new F[size_];
  for (i = 0; i < size_; ++i) {
    subprod[i] = -entries[i];
  }

  if (logsize_ > 0) {
    subprods_[1] = new F[size_];
    for (i = 0; i < size_; i += 2) {
      if (i + 1 >= size_)
        subprods_[1][i] = subprod[i];
      else {
        subprods_[1][i] = subprod[i] * subprod[i + 1];
        subprods_[1][i + 1] = subprod[i] + subprod[i + 1];
      }
    }

    for (unsigned int k = 2, len = 2; k <= logsize_; ++k, len <<= 1) {
      subprods_[k] = new F[size_];
      math::fft::rootsToPolyStage<F>(size_, len, subprods_[k - 1],
                                     subprods_[k]);
    }
  }
  // subprods[logsize] - has prod_i (x-entries[i])

  // Precalculate scalars alpha_i = prod_{i<>j}(entries[i]-entries[j])
  // also alpha_i = B'(entries[i])
  // evaluate d( _subprods[_logsize](x) )/dx at entries

  // Compute B'(z)
  boost::scoped_array<F> deriv(new F[size_]);
  for (i = 0; i < size_ - 1; ++i) {
    deriv[i] = subprods_[logsize_][i + 1] * F(i + 1);
  }
  deriv[size_ - 1].setTo(size_);

  scalars_ = new F[size_];
  this->evaluate(deriv.get(), scalars_);

  for (i = 0; i < size_ - 1; ++i) {
    ASSERT0(scalars_[i].hasInverse())
    (i)(scalars_[i])(size_ - 1).msg(
        "FFTVandermonde.solveTranspose element has no inverse ");
  }
}

template <typename F>
FFTSolver<F>::~FFTSolver() {
  for (unsigned int i = 0; i <= logsize_; i++) {
    delete[] subprods_[i];
  }
  delete[] subprods_;
  delete[] scalars_;
}

template <typename F>
void FFTSolver<F>::solveTranspose(const F* values, F* result) {
  F* right = new F[2 * size_ + 1];
  F* prod = new F[2 * size_ + 1];

  unsigned int i;
  for (i = 0; i < size_; i++) {
    prod[i] = subprods_[logsize_][i];
    right[size_ - i] = values[i];
  }
  prod[size_].template setTo<1>();
  right[0].template setTo<0>();
  for (++i; i <= (size_ << 1); ++i) {
    prod[i].template setTo<0>();
    right[i].template setTo<0>();
  }
  math::fft::polyProduct<F>(2 * size_ + 1, prod, right, prod);

  evaluate(prod + (size_ + 1), result);

  delete[] right;
  delete[] prod;

  for (unsigned int i = 0; i < size_; i++) {
    result[i] /= scalars_[i];
  }
}

// See for reference Modern Computer Algebra Algorithm 10.7 on page 283
// n = 2^k
// 1. if n == 1 then return coeff[0]
// 2. r_0 = f % M_{k-1},0, r_1 = f % M_{k-1},1
// 3. evaluate r_0 on entries for i=0,...,n/2-1
// 4. evaluate r_1 on entries for i=n/2,...,n-1
// 5. return r_0(e_0),...,r_0(e_{n/2-1}),r_1(e_{n/2}),...r_1(e_{n-1})
// M_{k-1,0} = prod_{i=0}^{n/2-1} (x-e_i) i.e. subprod_[k-1] coeffs 0...n/2-1
// M_{k-1,1} = prod_{i=n/2{^{n-1} (x-e_i) i.e. subprod_[k-1] coeffs n/2...n-1

template <typename F>
void FFTSolver<F>::evaluate(const F* coeffs, F* result) {
  for (unsigned int i = 0; i < size_; i++) {
    result[i] = coeffs[i];
  }

  F* inters = new F[size_];

  for (unsigned int k = logsize_; k > 1; k--) {
    unsigned int len = 1 << (k - 1);
    F* curDiv = new F[len + 1];
    F* curRem = new F[2 * len];
    for (unsigned int i = 0; i < size_; i += 2 * len) {
      if (i + len >= size_) {
        continue;
      }
      unsigned int deg = (i + 2 * len < size_) ? len : size_ - i - len;

      for (unsigned int j = 0; j < deg; j++) {
        curDiv[j] = subprods_[k - 1][i + len + j];
      }
      curDiv[deg].template setTo<1>();

      math::fft::polyDivMod<F>(len + deg, result + i, deg + 1, curDiv, curRem);

      for (unsigned int j = 0; j < deg; j++) {
        inters[i + len + j] = curRem[j];
      }
      for (unsigned int j = 0; j < len; j++) {
        curDiv[j] = subprods_[k - 1][i + j];
      }
      curDiv[len].template setTo<1>();

      math::fft::polyDivMod<F>(len + deg, result + i, len + 1, curDiv, curRem);

      for (unsigned int j = 0; j < len; j++) {
        inters[i + j] = curRem[j];
      }
    }
    delete[] curDiv;
    delete[] curRem;
    for (unsigned int j = 0; j < size_; j++) {
      result[j] = inters[j];
    }
  }
  delete[] inters;
  F xCoeff;
  for (unsigned int i = 0; i < size_ - 1; i += 2) {
    xCoeff = result[i + 1];
    result[i + 1] = result[i] - xCoeff * subprods_[0][i + 1];
    result[i] -= xCoeff * subprods_[0][i];
  }
}

}  // namespace vandermonde
}  // namespace math
