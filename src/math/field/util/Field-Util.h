#ifndef NTRP_MATH_FIELD_UTIL_H
#define NTRP_MATH_FIELD_UTIL_H

/**
 * @file: Field-Util.h Implementation of utilit functions for Field32.
 */

#include "math/Base.h"

#include <boost/scoped_array.hpp>
#include <istream>
#include <ostream>

namespace math {
namespace field {
namespace util {

/*****************************************************************************/
/**
 * @brief convert nul terminated decimal string (without spaces) to a field
 * element
 * @return false if parse error has occurred, in which case value pointed by val
 * is guaranteed not have been changed
 */
template <typename F>
bool fromString(const char* digits, F* val) {
  F radix;
  radix.template setTo<10>();
  bool neg = false;

  F result;
  result.template setTo<0>();

  // Get starting digit index
  switch (*digits) {
    case '-':
      neg = true;
    /* no break */
    case '+':
      ++digits;
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      break;
    default:
      return false;
  }
  if (!*digits) {  // Empty String?
    return false;
  }

  for (; *digits; ++digits) {
    switch (*digits) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        result *= radix;
        result += F(*digits - '0');
        break;
      default:
        return false;
    }
  }

  if (neg) {
    result.negate();
  }
  *val = result;
  return true;
}

/**
 * @brief Compute determinant via LUdecomposition (Crout's Algorithm).
 *
 * @param dest Reference to destination where result will be stored
 * @param first Pointer to first element of the matrix.
 * @param size Matrix size where number of rows and columns is same
 * @throws none
 * None are thrown.
 *
 * See reference "Numerical Recipies in C"
 * by W.H. Press, S.A. Teukolsky, W.T. Vellerling and B.P. Flannery
 *
 * @remarks
 *   This implementation is for a pricise Field
 *
 * @see
 */
template <typename F>
__forceinline void det(F* dest, F* first, int size) {
#define ent(i, j) (*(first + ((i) * (size) + (j))))
  int i, j, k, idxNonZero;
  F sum, div;
  dest->template setTo<1>();
  for (j = 0; j < size; j++) {
    for (i = 0; i < j; i++) {
      sum = ent(i, j);
      for (k = 0; k < i; k++) {
        sum -= ent(i, k) * ent(k, j);
      }
      ent(i, j) = sum;
    }
    idxNonZero = -1;         // Search for non-zero pivot
    for (; i < size; i++) {  // i = j already
      sum = ent(i, j);
      for (k = 0; k < j; k++) {
        sum -= ent(i, k) * ent(k, j);
      }
      ent(i, j) = sum;
      if (!sum.template is<0>()) {
        idxNonZero = i;
      }
    }
    if (ent(j, j).template is<0>()) {  // Need to interchange rows
      if (idxNonZero < 0) {            // Singular matrix ?
        dest->template setTo<0>();
        return;
      }
      // Interchange rows
      for (k = 0; k < size; k++) {
        ent(idxNonZero, k).swap(ent(j, k));
      }
      dest->negate();
    }
    const F& div = ent(j, j);
    for (i = j + 1; i < size; i++) ent(i, j) /= div;
  }
  // Multiply diagonal elements
  for (i = 0; i < size; i++) *dest *= ent(i, i);
#undef ent
}

/******************************************************************/
template <typename F>
__forceinline void det2p(F* dest, F* first, int size) {
  int twoPow = math::intlog(size);
#define ent(i, j) (*(first + ((i << twoPow) + (j))))
  int i, j, k, idxNonZero;
  F sum, div;
  dest->template setTo<1>();
  for (j = 0; j < size; ++j) {
    for (i = 0; i < j; ++i) {
      sum = ent(i, j);
      for (k = 0; k < i; ++k) sum -= ent(i, k) * ent(k, j);
      ent(i, j) = sum;
    }
    idxNonZero = -1;         // Search for non-zero pivot
    for (; i < size; ++i) {  // i = j already
      sum = ent(i, j);
      for (k = 0; k < j; ++k) {
        sum -= ent(i, k) * ent(k, j);
      }
      ent(i, j) = sum;
      if (!sum.template is<0>()) {
        idxNonZero = i;
      }
    }
    if (ent(j, j).template is<0>()) {  // Need to interchange rows
      if (idxNonZero < 0) {            // Singular matrix ?
        dest->template setTo<0>();
        return;
      }
      // Interchange rows
      for (k = 0; k < size; k++) {
        ent(idxNonZero, k).swap(ent(j, k));
      }
      dest->negate();
    }
    const F& div = ent(j, j);
    for (i = j + 1; i < size; i++) ent(i, j) /= div;
  }
  // Multiply diagonal elements
  for (i = 0; i < size; i++) *dest *= ent(i, i);
#undef ent
}

/**
 * @brief Initialize random field32 element generator to given seed.
 *        Same seed produces same random number sequence.
 */
//__forceinline void randomInit(unsigned int seed) {
// rg.RandomInit((uint32) seed);
//}
/*****************************************************************************/
template <typename F>
inline void setFill(F* dest, const F& val, int count) {
  for (F* last = dest + count; dest != last; ++dest) {
    *dest = val;
  }
}
/*****************************************************************************/
template <typename F>
inline void setFill2P(F* dest, const F& val, int size) {
  int p = math::intlog(size);
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      *(dest + ((i << p) | j)) = val;
    }
  }
}
/*****************************************************************************/
template <typename F>
void field2Stream(std::ostream& os, const F& f) {
  boost::scoped_array<char> buffer(new char[F::getMaxStringRepLength()]);
  f.toString(buffer.get());
  std::operator<<(os, buffer.get());
}
/*****************************************************************************/
template <typename F>
void fieldFromStream(std::istream& is, F& f) {
  std::string digits;
  is >> std::ws;
  char nextChar = is.get();
  bool negated = (nextChar == '-');
  if ((nextChar == '-') || (nextChar == '+'))
    nextChar = is.get();
  else if (!isdigit(nextChar)) {
    is.putback(nextChar);
    is.clear(std::ios::failbit);
    // return is;
  }
  while (isdigit(nextChar)) {
    digits.insert(digits.size(), 1, nextChar);
    nextChar = is.get();
  }
  is.putback(nextChar);
  if (math::field::util::fromString<F>(digits.c_str(), &f)) {
    if (negated) {
      f.negate();
    }
  } else {
    is.setstate(std::ios::failbit);
  }
}
/*****************************************************************************/
}  // namespace util

/**
 * @brief Writes F's decimal form to output stream os
 */
template <typename F>
inline std::ostream& operator<<(std::ostream& os, const F& f) {
  math::field::util::field2Stream<F>(os, f);
  return os;
}

/**
 * @brief Reads F (in decimal form) from input stream is
 */
template <typename F>
inline std::istream& operator>>(std::istream& is, F& f) {
  math::field::util::fieldFromStream<F>(is, f);
  return is;
}

}  // namespace field
}  // namespace math

#endif  // NTRP_MATH_FIELD_UTIL_H

/*****************************************************************************/

/**
 * @brief Constructs an instance from the decimal form of a number.
 */
// Field32(const std::string& digits);

/**
 * @brief Set up new prime field. In this implementation does nothing.
 */
// static void setPrimes(uint32_t* primes, int n);

/**
 * @brief Initializes the class's static members.
 */
// static void init() {}

/**
 * @brief Returns true iff this instance and given argument are equal
 *        Identical to operator==
 * @see operator==
 * @see operator!=
 */
//__forceinline bool equals(Field32 v) const { return _val == v._val; }

/**
 * @brief convert field falue to a signed integer
 * @see toString
 * @see str
 */
//__forceinline int toInt() const {
//  return (_val <= POS) ? _val : (_val - MOD);
//}

/**
 * @brief return character string signed decimal representation for the value.
 *        Uses toInt
 * @see str
 * @see toInt
 */
// std::string toString() const;

// static std::string toStringRepresentation();

/*
* @brief Perform Gaussian elimination step on a row of matrix.
* This code computes B[i]= A[0]*B[i]-A[i]*B[0] for i=1..s-1
* Note B[0] = 0
* @param A address of base row of Field elements
* @param B address of row to eleminate
* @param s number of elements in the row
*

__forceinline static void EliminateRow(int * A, int * B, const int s)
{
#ifdef INLINE_ASM
#ifdef LINUX
//@todo impl linux compile inline assembly
// *A = 0*s;
// *B = 0;

#else
__asm {
//      rdtsc
  //      push edx
  //      push eax
  mov ecx, s //size of arrays
  mov esi, A //address of A
  mov edi, B //address of B
  mov ebx, MOD
  dec ecx //if size is less than zero,
  jle finish_loop //then quit
  top: mov eax, [edi] //get B[0]
  mul DWORD PTR [esi+4*ecx] //B[0]*A[ecx]
  div ebx
  push edx //push upper 32 bits of result onto stack
  mov eax, [esi] //get A[0]
  mul DWORD PTR [edi+ecx*4] //A[0]*B[ecx]
  div ebx
  pop eax
  sub edx, eax
  jnc not_negative
  add edx, MOD
  not_negative:
  mov [edi+ecx*4], edx //store remainder into B[ecx]
  dec ecx //decrement ecx
  jg top //jump to the top
  finish_loop:
  mov DWORD PTR [edi], 0 // Save zero into first element
  //      rdtsc
  //      pop ebx
  //      sub eax, ebx
  //          pop ebx
  //      sbb edx, ebx
}
#endif
#else
// Non ASM code
#endif
return;
}
*/
/*
 * @brief Perform Gaussian elimination step on a row of matrix.
 * This code computes B[i]= A[0]*B[i]-A[i]*B[0] for i=1..s-1
 * Note B[0] = 0
 * @param A address of base row of Field elements
 * @param B address of row to eleminate
 * @param s number of elements in the row


__forceinline static void EliminateRow2(int * A, int * B, int s) {
#ifdef INLINE_ASM
#ifdef LINUX
  //#error NOT_IMPLEMENTED
  //@todo impl  linux compile inline assembly
  // *A = 0*s;
  // *B = 0;

#else
  __asm {
    mov ecx, s //size of arrays
    mov esi, A //address of A
    mov edi, B //address of B
    mov ebx, MOD
    dec ecx //if size is less than zero,
    jle finish_loop2 //then quit
    top2: mov eax, [edi] // get B[0]
    mul DWORD PTR [esi+ecx*4] // edx:eax = B[0]*A[ecx]
    push edx
    mov ebx, eax
    mov eax, [esi] //get A[0]
    mul DWORD PTR [edi+ecx*4] //edx:eax = A[0]*B[ecx]
    //pop ebx
    sub eax, ebx
    pop ebx
    sbb edx, ebx
    js negative_number
    mov ebx, MOD // Compute Modulus
    div ebx
    jmp not_negative2
    negative_number:
    not eax // Compute two's complement
    not edx
    add eax, 1
    adc edx, 0
    mov ebx, MOD
    div ebx
    sub ebx, edx
    mov edx, ebx // MOD - remainder
    not_negative2:
    mov [edi+ecx*4], edx //store remainder into B[ecx]
    dec ecx //decrement ecx
    jg top2 //jump to the top
    finish_loop2:
    mov DWORD PTR [edi], 0 // Save zero into first element
  }
#endif
#else
             //int * aA = A;
             //int * aB = B;
             //int ss = s;
  //#error NOT_IMPLEMENTED
#endif
  return;
}
*/
