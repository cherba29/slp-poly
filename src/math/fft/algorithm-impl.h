#ifndef INTERP_MATH_FOURIER_H
#error Do not include this file. This is used internaly by algorithm.h
#endif

/**
 * @file Fourier-Impl.h Implementation of math::Fourier
 *
 * $URL$
 * $Id$
 */

#include "math/Base.h"
#include "util/SmartAssert.h"

#include <boost/scoped_array.hpp>

namespace math::fft {
/******************************************************************************/
/* @brief
 * @param logsize - input size 2^logsize
 * @param roots - input, primitive roots of unity, will use logsize of them
 * @param result - input/output
 */
template <typename F>
void butterflies(unsigned int logsize, const F* roots, F* result) {
  const unsigned int size = (1 << logsize);
  F temp, root;

  for (unsigned int l = 0, spread = 1; l < logsize; ++l, spread <<= 1) {
    for (unsigned int i = 0; i < size; i += (spread << 1)) {
      temp = result[i + spread];
      result[i + spread] = result[i] - temp;
      result[i] += temp;
    }
    root = roots[l];  // Each recursive DFT uses l-th root of unity
    for (unsigned int j = 1; j < spread; ++j, root *= roots[l]) {
      for (unsigned int i = j; i < size; i += (spread << 1)) {
        temp = root * result[i + spread];
        result[i + spread] = result[i] - temp;
        result[i] += temp;
      }
    }
  }
}
/******************************************************************************/
// A radix-2 decimation-in-time (DIT) FFT
// Descrete Fourier Transform (DFT)
// Given vector coeffs returns vector result of size n=2^logsize
// such that
//  result[k] = sum_{j=0}^{n-1} coeffs[j] omega_n^{kj}
template <typename F>
void transform(unsigned int logsize, const F* coeffs, F* result) {
  if (logsize < 1) {
    result[0] = coeffs[0];
    return;
  }
  const unsigned int size = (1 << logsize);

  // Permute the input to bit-reversed order
  unsigned int* perm = bitrevPerm(logsize);
  for (unsigned int i = 0; i < size; i++) {
    result[i] = coeffs[perm[i]];
  }
  delete[] perm;

  // Butterfly inputs
  butterflies(logsize, F::getPrimRoots(), result);
}
/******************************************************************************/
template <typename F>
void inverse(unsigned int logsize, const F* points, F* result) {
  if (logsize < 1) {
    result[0] = points[0];
    return;
  }

  const unsigned int size = (1 << logsize);

  // Permute the input to bit-reversed order
  unsigned int* perm = bitrevPerm(logsize);
  unsigned int i;
  for (i = 0; i < size; i++) {
    result[i] = points[perm[i]];
  }
  delete[] perm;

  // Butterfly inputs
  butterflies(logsize, F::getPrimRootInvs(), result);
  F fSize(size);

  // This should never happen for proper field, this is verified by unit tests
  // as 2 is not a zero devisor
  ASSERT0(fSize.hasInverse())
      .msg("Does not have inverse in math::fft::inverse");

  F invsize = fSize.getInverse();
  for (i = 0; i < size; i++) {
    result[i] *= invsize;
  }
}
/******************************************************************************/
template <typename F>
void polyProduct(unsigned int nCoeffs, const F* left, const F* right, F* prod) {
  if (nCoeffs < 1) {
    return;
  }
  if (nCoeffs < 2) {
    F::multiply(&prod[0], left[0], right[0]);
    return;
  }
  // product will be twice as big hence +1
  const unsigned int logdeg = 1 + math::intlog(nCoeffs);
  const unsigned int limit = static_cast<unsigned int>(1) << logdeg;

  // Do single memory allocation, and ensure automatic deallocation
  boost::scoped_array<F> buffer(new F[(limit << 1) + limit]);  // 3 times
  F* coeffs = buffer.get();
  F* leftFT = coeffs + limit;
  F* rightFT = leftFT + limit;
  // F* coeffs = new F[limit];
  // F* leftFT = new F[limit];
  // F* rightFT = new F[limit];

  unsigned int i;
  // Copy over coefficients of left into double sized buffer.
  for (i = 0; i < nCoeffs; ++i) coeffs[i] = left[i];
  // Set higher degrees to 0
  for (; i < limit; ++i) coeffs[i].template setTo<0>();

  transform<F>(logdeg, coeffs, leftFT);  // do DFT

  // Repeat for 'right' polynomial coefficients
  for (i = 0; i < nCoeffs; ++i) coeffs[i] = right[i];
  // for (; i < limit; ++i) coeffs[i].template setTo<0>();
  transform<F>(logdeg, coeffs, rightFT);  // do DFT

  // Multiply transformed values
  for (i = 0; i < limit; ++i) leftFT[i] *= rightFT[i];

  inverse<F>(logdeg, leftFT, coeffs);

  // for (i = 0; i < (nCoeffs<<1); i++) prod[i] = coeffs[i];

  //  mod x^nCoeffs
  for (i = 0; i < nCoeffs; ++i) prod[i] = coeffs[i];
}
/******************************************************************************/
template <typename F>
void rootsToPolyStage(unsigned int nRoots, unsigned int maxdeg, const F* in,
                      F* out) {
  unsigned int i;
  unsigned int j;
  // unsigned int logdeg = math::intlog(maxdeg);

  const unsigned int maxdeg2 = maxdeg << 1;
  F* left = new F[maxdeg2];
  F* right = new F[maxdeg2];
  F* prod = new F[maxdeg2];

  for (i = 0; i < nRoots; i += maxdeg2) {
    if (i + maxdeg >= nRoots) {  // Just copy remaining
      for (j = i; j < nRoots; j++) {
        out[j] = in[j];
      }
    } else {
      for (j = 0; j < maxdeg; j++) {
        left[j] = in[i + j];
        if (i + maxdeg + j < nRoots) {
          right[j] = in[i + maxdeg + j];
        } else {
          if (i + maxdeg + j > nRoots) {
            right[j].template setTo<0>();
          } else {
            right[j].template setTo<1>();
          }
        }
        // right[j] = (i+maxdeg+j < nRoots)?
        //     in[i+maxdeg+j]: F((i+maxdeg+j > nRoots)? 0: 1);
        left[maxdeg + j].template setTo<0>();
        right[maxdeg + j].template setTo<0>();
      }
      // (sum(left[i]*x^i) + x^maxdeg) * (sum(right[i]*x^i) + x^maxdeg)
      //  == x^2*maxdeg +	x^maxdeg * (sum((left[i] + right[i])*x^i)
      // + sum(left[i]*x^i) * sum(right[i]*x^i)

      polyProduct<F>(maxdeg2, left, right, prod);

      for (j = 0; (i + j < nRoots) && (j < maxdeg2); j++) {
        out[i + j] = prod[j];
      }
      for (j = 0; (i + maxdeg + j < nRoots) && (j < maxdeg); j++) {
        out[i + maxdeg + j] += right[j];
      }
      if (i + maxdeg2 <= nRoots) {
        for (j = 0; j < maxdeg; j++) {
          out[i + maxdeg + j] += left[j];
        }
      }
    }
  }
  delete[] left;
  delete[] right;
  delete[] prod;
}

//*****************************************************************************
template <typename F>
void rootsToPoly(unsigned int nRoots, const F* roots, F* coeffs) {
  for (unsigned int i = 0; i < nRoots; i += 2) {
    if (i + 1 >= nRoots) {
      coeffs[i] = -roots[i];
    } else {
      coeffs[i] = roots[i] * roots[i + 1];
      coeffs[i + 1] = -roots[i] - roots[i + 1];
    }
  }

  unsigned int logsize = math::intlog(nRoots);

  for (unsigned int logdeg = 1; logdeg < logsize; logdeg++) {
    unsigned int deg = 1 << logdeg;
    rootsToPolyStage<F>(nRoots, deg, coeffs, coeffs);
  }
}

//*****************************************************************************

template <typename F>
void polyInvert(unsigned int nCoeffs, const F* poly, F* result) {
  F* prod = new F[nCoeffs];
  ASSERT1(poly[0].hasInverse())
      .msg(
          "Passed in polynomial has no invertible constant term, "
          "polynomial inverse does not exist");

  result[0] = poly[0].getInverse();
  for (unsigned int i = 1; i < nCoeffs; ++i) {
    result[i].template setTo<0>();
  }

  unsigned int newDeg;
  for (unsigned int oldDeg = 1, i = math::intlog(nCoeffs); i > 0;
       i--, oldDeg = newDeg) {
    newDeg = nCoeffs >> (i - 1);

    if ((newDeg << (i - 1)) < nCoeffs) {
      newDeg++;
    }

    polyProduct<F>(newDeg, poly, result, prod);  // prod % x^oldDeg == 1

    for (unsigned int j = oldDeg; j < newDeg; j++) {
      prod[j] = -prod[j];
    }

    polyProduct<F>(newDeg - oldDeg, prod + oldDeg, result, result + oldDeg);
    // inverse += x^oldDeg * inverse * (prod / x^oldDeg)
  }

  delete[] prod;
}

//*****************************************************************************

template <typename F>
void polyDivMod(unsigned int aNTerms, const F* a, unsigned int bNTerms,
                const F* b, F* remquo) {
  unsigned int i;
  for (i = 0; (i < aNTerms) && (i < bNTerms - 1); i++) {
    remquo[i] = a[i];
  }
  if (aNTerms < bNTerms) return;

  // Invert reverse of divisor
  unsigned int qNTerms = aNTerms - bNTerms + 1;
  F* tmp = new F[aNTerms];  // (temporary of max needed size)// qNTerms];
  F* bRevInverse = new F[qNTerms];
  for (i = 0; (i < qNTerms) && (i < bNTerms); i++) {
    tmp[i] = b[bNTerms - 1 - i];  // reverse b
  }
  for (; i < qNTerms; ++i) {
    tmp[i].template setTo<0>();
  }

  polyInvert<F>(qNTerms, tmp, bRevInverse);  // compute inverse of rev b

  // Calculate quotient
  for (i = 0; i < qNTerms; i++) {
    tmp[i] = a[aNTerms - 1 - i];
  }  // rev a

  polyProduct<F>(qNTerms, tmp, bRevInverse, tmp);

  for (i = 0; i < qNTerms; i++) {
    remquo[bNTerms - 1 + i] = tmp[qNTerms - 1 - i];
  }

  // delete[] tmp;
  delete[] bRevInverse;

  // Calculate remainder via r = a - b*q
  // F* rTemp = new F[aNTerms]; // rCoeffs <= lCoeffs
  // tmp <- b
  for (i = 0; i < bNTerms; ++i) {
    tmp[i] = b[i];
  }
  for (; i < aNTerms; ++i) {
    tmp[i].template setTo<0>();
  }

  // prod <- q
  F* prod = new F[aNTerms];  // qCoeffs == lCoeffs - rCoeffs+1
  for (i = 0; i < qNTerms; i++) {
    prod[i] = remquo[bNTerms - 1 + i];
  }
  for (; i < aNTerms; ++i) {
    prod[i].template setTo<0>();
  }

  polyProduct<F>(aNTerms, prod, tmp, prod);  // get b*q

  for (i = 0; i < bNTerms - 1; i++) {
    remquo[i] -= prod[i];  // r[i] = a[i] - (b*q)[i];
  }

  delete[] tmp;
  delete[] prod;
}

}  // namespace math::fft
