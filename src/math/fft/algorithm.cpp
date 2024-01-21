/**
 * @file algorithm.cpp Fast Fourier Transform algorithms.
 *
 * $URL$
 * $Id$
 */
#include "algorithm.h"

namespace math::fft {

// Find the bit-reversed permutation of 2^logsize items
unsigned int* bitrevPerm(unsigned int logsize) {
  unsigned int i;
  unsigned int mask;
  unsigned int rev = 0;
  unsigned int size = 1 << logsize;

  unsigned int* perm = new unsigned int[size];
  for (i = 0; i < size - 1; i++) {
    perm[i] = rev;
    for (mask = size >> 1; rev >= mask; mask >>= 1) {
      rev -= mask;
    }
    rev += mask;
  }
  perm[size - 1] = size - 1;
  return perm;
}

}  // namespace math::fft
