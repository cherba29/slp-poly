#ifndef INTERP_MATH_FOURIER_H
#define INTERP_MATH_FOURIER_H

/**
 * @file algorithm.h
 * @todo doc the file
 *
 * $URL$
 * $Id$
 */

#include "util/log.h"
#include <cstdlib>


namespace math {

/**
 * @brief Fast Fourier Transform related algorithms
 */
namespace fft {

/**
 * @brief Find the bit-reversed permutation of 2^logsize items.
 * For example if logsize == 3 then permutation is
 *   0,4,2,6,1,5,3,7, i.e. bit pattern of an index is reversed.
 * Used by FFT algorithm.
 * @param logsize - total number of elements 2^logsize
 * @return allocated array of size 2^logsize
 */
unsigned int* bitrevPerm(unsigned int logsize);


/**
 * @brief Decimate-in-time fast Fourier transform
 * permute coefficients and apply butterflies on primitive roots
 * Complexity O(2^logsize)
 * @param coeffs input array of coeffcients of size 2^logsize
 * @param result output preallocated array of size 2^logsize
 */
template <typename F>
void transform(unsigned int logsize, const F* coeffs, F* result);

/**
 * Decimate-in-time fast inverse Fourier transform
 */
template <typename F>
void inverse(unsigned int logsize, const F* coeffs, F* result);

/**
 * Given coefficients of two polynomials of degree < nCoeffs,
 * computes coefficients of their product % x^nCoeffs
 * i.e. fast convolution
 */
template <typename F>
void polyProduct(unsigned int nCoeffs, const F* left, const F* right, F* prod);

/**
 * @brief Implementation of stage building subproducts algorithm described in
 * page 281 Modern Computer Algebra
 *
 * Given nRoots/maxdeg monic polynomials of degree maxdeg in in where
 * in[i*maxdeg+k] == term k of poly i, stores product j == poly 2j * poly 2j+1
 * in out where out[j*2*maxdeg+k] == term k of product j
 */
template <typename F>
void rootsToPolyStage(unsigned int nRoots, unsigned int maxdeg, const F* in, F* out);

/**
 * Given nRoots roots, interpolates monic polynomial with those roots
 */
template <typename F>
void rootsToPoly(unsigned int nRoots, const F* roots, F* coeffs);

/**
 * Given a polynomial of degree < nCoeffs, returns its inverse % x^nCoeffs
 */
template <typename F>
void polyInvert(unsigned int nCoeffs, const F* poly, F* result);

/**
 * Given polynomials left (of degree < lCoeffs) and right (of degree < rCoeffs),
 * divides left by right and returns coeffs of remainder followed by coeffs of
 * quotient in the array remquo
 */
template <typename F>
void polyDivMod(unsigned int lCoeffs, const F* left,
      unsigned int rCoeffs, const F* right, F* remquo);

} // namespace fourier

} // namespace math


namespace math {

/**
 * @brief Fast Fourier Transform algorithms.
 *        Taken from Joachim von zur Gathen & Juergen Gerhard,
 * _Modern Computer Algebra_, Cambridge University Press, 2003, page 221.
 */
template <class F>
class Fourier {
	//F* primRoots;		    /**< Index n holds 2^(n+1)th primitive root of 1 in field F */
	//F* invRoots;		    /**< Index n holds inv(primRoots[n]) */
	//unsigned int depth;	/**< Size of primRoots == Fourier Order of the Field */

  /**
   * @todo Document Fourier::butterflies
   */
	void butterflies(unsigned int logsize, const F* const roots, F* result);

public:
	//Fourier() : primRoots(NULL), invRoots(NULL), depth(0) {}

//	~Fourier() {
//		delete[] primRoots;
//		delete[] invRoots;
//	}

	/**
	 * Computes and allocates roots of unity with its inverses
   * Must be called before object of this class is used
	 */
	//void init();

	/**
   * Returns the Fourier order of the field F
   * @return Maximum n such that (x^2)^n = -1, number of primitive roots
   */
	//unsigned int getOrder() { return depth; }

	/**
   * Decimate-in-time fast Fourier transform
   */
	void transform(unsigned int logsize, const F* coeffs, F* result);

	/**
   * Decimate-in-time fast inverse Fourier transform
   */
	void inverse(unsigned int logsize, const F* coeffs, F* result);

	/**
   * Given coefficients of two polynomials of degree < nCoeffs,
	 * computes coefficients of their product % x^nCoeffs
   */
	void polyProduct(unsigned int nCoeffs, const F* left, const F* right, F* prod);

	/**
   * Given nRoots/maxdeg monic polynomials of degree maxdeg in in where
	 * in[i*maxdeg+k] == term k of poly i, stores product j == poly 2j * poly 2j+1
	 * in out where out[j*2*maxdeg+k] == term k of product j
   */
	void rootsToPolyStage(unsigned int nRoots, unsigned int maxdeg, const F* in, F* out);

	/**
   * Given nRoots roots, interpolates monic polynomial with those roots
   */
	void rootsToPoly(unsigned int nRoots, const F* roots, F* coeffs);

	/**
   * Given a polynomial of degree < nCoeffs, returns its inverse % x^nCoeffs
   */
	void polyInvert(unsigned int nCoeffs, const F* poly, F* result);

	/**
   * Given polynomials left (of degree < lCoeffs) and right (of degree < rCoeffs),
	 * divides left by right and returns coeffs of remainder followed by coeffs of
	 * quotient in the array remquo
   */
	void polyDivMod(unsigned int lCoeffs, const F* left,
				unsigned int rCoeffs, const F* right, F* remquo);
};

}

#include "algorithm-impl.h"

#endif // INTERP_MATH_FOURIER_H
