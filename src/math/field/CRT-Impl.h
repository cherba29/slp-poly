#ifndef NTRP_MATH_FIELD_CRT_H
#error "Do not include this file. This is used internaly by CRT.h"
#endif

/**
 * @file CRT-Impl.h Implementation of math::CRT
 */

#include "math/util/prime.h"
#include "util/SmartAssert.h"

namespace math::field {

template <typename UIntType>
unsigned int CRT<UIntType>::nPrimes_ = 0;

template <typename UIntType>
typename CRT<UIntType>::ubase_type* CRT<UIntType>::primes_ = NULL;

template <typename UIntType>
typename CRT<UIntType>::ubase_type* CRT<UIntType>::mixmuls_ = NULL;

template <typename UIntType>
boost::scoped_array<CRT<UIntType> > CRT<UIntType>::primRoots;

template <typename UIntType>
boost::scoped_array<CRT<UIntType> > CRT<UIntType>::primRootInvs;

//** Default Constructor *******************************************************
template <typename UIntType>
__forceinline CRT<UIntType>::CRT()
    : vals_(new ubase_type[CRT<UIntType>::nPrimes_]) {}

//******************************************************************************
/*
template <typename UIntType>
__forceinline CRT<UIntType>::CRT(typename CRT<UIntType>::ubase_type val)
  : vals_(new ubase_type[CRT<UIntType>::nPrimes_]) {
    for (unsigned int i = 0; i < nPrimes_; ++i) {
      vals_[i] = val % (primes_[i]);
    }
}
*/
//******************************************************************************
template <typename UIntType>
__forceinline CRT<UIntType>::CRT(typename CRT<UIntType>::sbase_type val)
    : vals_(new ubase_type[CRT<UIntType>::nPrimes_]) {
  if (val < 0) {
    for (unsigned int i = 0; i < nPrimes_; ++i)
      vals_[i] = primes_[i] - ((-val) % primes_[i]);
  } else {
    for (unsigned int i = 0; i < nPrimes_; ++i) vals_[i] = val % primes_[i];
  }
}

//** Copy constructor **********************************************************
template <typename UIntType>
__forceinline CRT<UIntType>::CRT(const CRT<UIntType>& right)
    : vals_(new ubase_type[CRT<UIntType>::nPrimes_]) {
  for (unsigned int i = 0; i < nPrimes_; i++) {
    vals_[i] = right.vals_[i];
  }
}

//** Destructor ****************************************************************
template <typename UIntType>
__forceinline CRT<UIntType>::~CRT() {
  delete[] vals_;
}

//******************************************************************************
/*
template <typename UIntType>
template <typename CRT<UIntType>::sbase_type VAL> __forceinline
void CRT<UIntType>::setTo()  {
  if (VAL < 0) {
    for (unsigned int i = 0; i < nPrimes_; i++)
      vals_[i] = primes_[i] - ((-VAL) % primes_[i]);
  } else {
    for (unsigned int i = 0; i < nPrimes_; i++)
      vals_[i] = VAL % primes_[i];
  }
}
*/
// Specialize
template <>
template <>
__forceinline void CRT<uint32_t>::setTo<0>() {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    vals_[i] = 0;
  }
}

template <>
template <>
__forceinline void CRT<uint64_t>::setTo<0>() {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    vals_[i] = 0;
  }
}

template <>
template <>
__forceinline void CRT<uint32_t>::setTo<1>() {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    vals_[i] = 1;
  }
}

template <>
template <>
__forceinline void CRT<uint64_t>::setTo<1>() {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    vals_[i] = 1;
  }
}

template <>
template <>
__forceinline void CRT<uint32_t>::setTo<-1>() {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    vals_[i] = primes_[i] - 1;
  }
}
template <>
template <>
__forceinline void CRT<uint64_t>::setTo<-1>() {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    vals_[i] = primes_[i] - 1;
  }
}

//******************************************************************************
template <typename UIntType>
__forceinline void CRT<UIntType>::setTo(
    typename CRT<UIntType>::sbase_type val) {
  if (val >= 0) {
    for (unsigned int i = 0; i < nPrimes_; i++) {
      vals_[i] = val % (primes_[i]);
    }
  } else {
    for (unsigned int i = 0; i < nPrimes_; i++) {
      vals_[i] = primes_[i] - ((-val) % primes_[i]);
    }
  }
}
//******************************************************************************
template <typename UIntType>
__forceinline void CRT<UIntType>::random() {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    vals_[i] =
        math::random<typename CRT<UIntType>::ubase_type>::get() % primes_[i];
  }
}
//******************************************************************************
template <typename UIntType>
__forceinline void CRT<UIntType>::invertableRandom() {
  do {
    this->random();
  } while (!this->hasInverse());
}
//### Property checks ########################################################
template <>
template <>
__forceinline bool CRT<uint32_t>::is<0>() const {
  for (unsigned int i = 0; i < nPrimes_; i++) {
    if (vals_[i] != 0) return false;
  }
  return true;
}
template <>
template <>
__forceinline bool CRT<uint64_t>::is<0>() const {
  for (unsigned int i = 0; i < nPrimes_; i++) {
    if (vals_[i] != 0) return false;
  }
  return true;
}

template <>
template <>
__forceinline bool CRT<uint32_t>::is<1>() const {
  for (unsigned int i = 0; i < nPrimes_; i++) {
    if (vals_[i] != 1) return false;
  }
  return true;
}
template <>
template <>
__forceinline bool CRT<uint64_t>::is<1>() const {
  for (unsigned int i = 0; i < nPrimes_; i++) {
    if (vals_[i] != 1) return false;
  }
  return true;
}

template <>
template <>
__forceinline bool CRT<uint32_t>::is<-1>() const {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    if (vals_[i] != primes_[i] - 1) return false;
  }
  return true;
}
template <>
template <>
__forceinline bool CRT<uint64_t>::is<-1>() const {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    if (vals_[i] != primes_[i] - 1) return false;
  }
  return true;
}

template <typename UIntType>
template <int VAL>
__forceinline bool CRT<UIntType>::is() const {
  if (VAL >= 0) {
    for (unsigned int i = 0; i < nPrimes_; ++i)
      if (vals_[i] != VAL % primes_[i]) return false;
    return true;
  } else {
    for (unsigned int i = 0; i < nPrimes_; ++i)
      if (vals_[i] != primes_[i] - ((-VAL) % primes_[i])) return false;
    return true;
  }
}

//******************************************************************************
template <typename UIntType>
__forceinline bool CRT<UIntType>::is(
    typename CRT<UIntType>::sbase_type val) const {
  if (val >= 0) {
    for (unsigned int i = 0; i < nPrimes_; i++)
      if (vals_[i] != val % primes_[i]) return false;
    return true;
  } else {
    for (unsigned int i = 0; i < nPrimes_; i++)
      if (vals_[i] != primes_[i] - ((-val) % primes_[i])) return false;
    return true;
  }
}

//******************************************************************************
template <typename UIntType>
__forceinline bool CRT<UIntType>::isPositive() const {
  ubase_type* rads = mixedRadix();
  unsigned int msrad = nPrimes_ - 1;
  bool result = (rads[msrad] <= primes_[msrad] - rads[msrad]);
  delete[] rads;
  return result;
}
//************************************************************************
template <typename UIntType>
__forceinline bool CRT<UIntType>::isNegative() const {
  boost::scoped_array<ubase_type> rads(mixedRadix());
  for (int i = nPrimes_ - 1; i >= 0; --i) {
    UIntType half = primes_[i] >> 1;
    if (rads[i] > half) return true;
    if (rads[i] < half) return false;
  }
  return false;  // Exact middle is positive
  // bool result = (rads[msrad] > primes_[msrad] - rads[msrad]);
  // delete[] rads;
  // return result;
}

//******************************************************************************
template <typename UIntType>
__forceinline bool CRT<UIntType>::hasInverse() const {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    if (vals_[i] == 0) {
      return false;
    }
  }
  return true;
}

//### Modifiers ################################################################
template <typename UIntType>
__forceinline void CRT<UIntType>::negate() {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    if (vals_[i] != 0) {
      vals_[i] = primes_[i] - vals_[i];
    }
  }
}

//******************************************************************************
template <typename UIntType>
__forceinline CRT<UIntType>& CRT<UIntType>::swap(CRT<UIntType>& f) {
  ubase_type* tmp = vals_;
  vals_ = f.vals_;
  f.vals_ = tmp;
  return *this;
}

//******************************************************************************
template <typename UIntType>
__forceinline CRT<UIntType>& CRT<UIntType>::invert() {
  ASSERT1(hasInverse()) (*this)(nPrimes_).msg("Has no inverse");
  unsigned int i;
  // try {
  for (i = 0; i < nPrimes_; i++)
    vals_[i] = ModOp<UIntType>::inv(vals_[i], primes_[i]);
  //}
  // catch (...) {
  //  this->setTo<0>();
  // No Inverse
  // ASSERT0(false)(i)(_vals[i])(_primes[i]).msg("Value has no inverse");
  //}
  return *this;
}

//******************************************************************************
template <typename UIntType>
__forceinline void CRT<UIntType>::add(CRT* dest, const CRT& src1,
                                      const CRT& src2) {
  *dest = src1;
  *dest += src2;
}

//******************************************************************************
template <typename UIntType>
__forceinline void CRT<UIntType>::subtract(CRT* dest, const CRT& src1,
                                           const CRT& src2) {
  *dest = src1;
  *dest -= src2;
}

//******************************************************************************
template <typename UIntType>
__forceinline void CRT<UIntType>::neg(CRT* dest, const CRT& src) {
  *dest = src;
  dest->negate();
}

//******************************************************************************
template <typename UIntType>
__forceinline void CRT<UIntType>::multiply(CRT* dest, const CRT& src1,
                                           const CRT& src2) {
  *dest = src1;
  *dest *= src2;
}

//******************************************************************************
template <typename UIntType>
__forceinline void CRT<UIntType>::divide(CRT<UIntType>* dest,
                                         const CRT<UIntType>& src1,
                                         const CRT<UIntType>& src2) {
  CRT<UIntType>::multiply(dest, src1, src2.getInverse());
}

//******************************************************************************
template <typename UIntType>
__forceinline void CRT<UIntType>::pow(CRT<UIntType>* dest,
                                      const CRT<UIntType>& src,
                                      unsigned int p) {
  *dest = src;
  dest->powerBy(p);
}

//******************************************************************************
template <typename UIntType>
__forceinline bool operator==(const CRT<UIntType>& left,
                              const CRT<UIntType>& right) {
  for (unsigned int i = 0; i < CRT<UIntType>::nPrimes_; ++i) {
    if (left.vals_[i] != right.vals_[i]) {
      return false;
    }
  }
  return true;
}

//******************************************************************************
// Returns (left + right) % each modulus
template <typename UIntType>
__forceinline CRT<UIntType> operator+(const CRT<UIntType>& left,
                                      const CRT<UIntType>& right) {
  CRT<UIntType> result = left;
  return result += right;
}

//******************************************************************************
//! Returns (left - right) % each modulus
template <typename UIntType>
__forceinline CRT<UIntType> operator-(const CRT<UIntType>& left,
                                      const CRT<UIntType>& right) {
  CRT<UIntType> result = left;
  return result -= right;
}

//******************************************************************************
//! Returns (left * right) % each modulus
template <typename UIntType>
__forceinline CRT<UIntType> operator*(const CRT<UIntType>& left,
                                      const CRT<UIntType>& right) {
  CRT<UIntType> result = left;
  return result *= right;
}

//******************************************************************************
template <typename UIntType>
__forceinline CRT<UIntType> CRT<UIntType>::operator-() const {
  CRT<UIntType> result;  // = CRT<UIntType>(0);
  for (unsigned int i = 0; i < nPrimes_; ++i)
    result.vals_[i] = primes_[i] - vals_[i];
  return result;
}

//******************************************************************************
template <typename UIntType>
__forceinline CRT<UIntType>& CRT<UIntType>::operator=(
    const CRT<UIntType>& right) {
  for (unsigned int i = 0; i < nPrimes_; i++) {
    vals_[i] = right.vals_[i];
  }
  return *this;
}

//******************************************************************************
template <typename UIntType>
__forceinline CRT<UIntType>& CRT<UIntType>::operator+=(
    const CRT<UIntType>& right) {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    if (vals_[i] >= primes_[i] - right.vals_[i]) {
      vals_[i] -= primes_[i] - right.vals_[i];
    } else {
      vals_[i] += right.vals_[i];
    }
  }
  return *this;
}

//******************************************************************************
template <typename UIntType>
__forceinline CRT<UIntType>& CRT<UIntType>::operator-=(
    const CRT<UIntType>& right) {
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    if (vals_[i] >= right.vals_[i]) {
      vals_[i] -= right.vals_[i];
    } else {
      vals_[i] += primes_[i] - right.vals_[i];
    }
  }
  return *this;
}

//******************************************************************************
template <typename UIntType>
__forceinline CRT<UIntType>& CRT<UIntType>::operator*=(
    const CRT<UIntType>& right) {
  if (is<0>()) return *this;
  if (right.is<0>()) return operator=(right);

  for (unsigned int i = 0; i < nPrimes_; ++i) {
    vals_[i] = ModOp<UIntType>::mul(vals_[i], right.vals_[i], primes_[i]);
  }
  return *this;
}

//******************************************************************************
template <typename UIntType>
__forceinline CRT<UIntType>& CRT<UIntType>::sqr() {
  if (is<0>()) return *this;

  for (unsigned int i = 0; i < nPrimes_; ++i) {
    vals_[i] = ModOp<UIntType>::mul(vals_[i], vals_[i], primes_[i]);
  }
  return *this;
}

//******************************************************************************
template <typename UIntType>
__forceinline CRT<UIntType>* CRT<UIntType>::alloc(unsigned int size) {
  // Allocate space for CRT*size, and also do internal allocation in one shot
  char* cBuff =
      new char[size * (sizeof(ubase_type) * nPrimes_ + sizeof(CRT<UIntType>))];
  CRT<UIntType>* buff = reinterpret_cast<CRT<UIntType>*>(cBuff);
  // For each element adjust internal pointer
  for (unsigned int i = 0; i < size; ++i) {
    buff[i].vals_ =
        reinterpret_cast<ubase_type*>(cBuff + size * sizeof(CRT<UIntType>) +
                                      i * sizeof(ubase_type) * nPrimes_);
  }
  return buff;
}

//******************************************************************************
template <typename UIntType>
__forceinline void CRT<UIntType>::dealloc(CRT<UIntType>* buff) {
  delete[] reinterpret_cast<char*>(buff);
}

//******************************************************************************
template <typename UIntType>
__forceinline CRT<UIntType>* CRT<UIntType>::allocSq(unsigned int size) {
  return CRT<UIntType>::alloc(size * size);
}

//******************************************************************************
template <typename UIntType>
__forceinline CRT<UIntType>* CRT<UIntType>::allocSq2p(unsigned int size) {
  // Must support indexing [i<<szLog|j]
  const unsigned int szLog = math::intlog(size);
  const unsigned int sz2p = 1 << szLog;
  const unsigned int sz2p2 = sz2p << szLog;

  // Allocate space for CRT32*size, and also do internal allocation in one shot
  char* cBuff = new char[nPrimes_ * sizeof(ubase_type) * size * size +
                         sizeof(CRT<UIntType>) * sz2p2];
  cBuff += sz2p * sizeof(CRT<UIntType>);  // Skip object allocation space
  CRT<UIntType>* buff = reinterpret_cast<CRT<UIntType>*>(cBuff);
  // For each element adjust internal pointer
  unsigned int i;
  unsigned int j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      buff[(i << sz2p) + j]._vals = reinterpret_cast<ubase_type*>(
          cBuff + (i * size + j) * sizeof(ubase_type) * nPrimes_);
    }
    for (; j < sz2p; ++j) buff[(i << sz2p) + j].vals_ = NULL;
  }
  for (; i < sz2p; ++i)
    for (j = 0; j < sz2p; ++j) buff[(i << sz2p) + j].vals_ = NULL;

  return buff;
}

template <typename UIntType>
void CRT<UIntType>::reset() {
  delete[] primes_;
  primes_ = NULL;
  mixmuls_ = NULL;
  CRT<UIntType>::primRoots.reset();
  CRT<UIntType>::primRootInvs.reset();
}

template <typename UIntType>
void CRT<UIntType>::setPrimes(const ubase_type* prs, int n) {
  ASSERT1(n > 0)(n).msg("There must be at least one prime.");
  ASSERT1(n > 1)(n).warn("CRT is not most efficient option for single prime.");

  // Check if all prime
  const ubase_type* prm;

  ASSERT1((prm = std::find_if(
               prs, prs + n,
               std::not1(std::ptr_fun(maybe_prime<ubase_type>)))) == prs + n)
  (prm - prs)(n).debug("A value in passed in array is not a prime");

  delete[] primes_;
  // delete[] _mixmuls;

  nPrimes_ = n;
  primes_ = new ubase_type[nPrimes_ << 1];
  mixmuls_ = primes_ + n;  // new uint32_t[_nPrimes-1];

  // Copy primes
  std::copy(prs, prs + n, primes_);

  // Sort _primes in increasing order
  std::sort(primes_, primes_ + nPrimes_);

  // Check if there are not duplicates
  ASSERT1((prm = std::adjacent_find(primes_, primes_ + n)) == primes_ + n)
  (prm - primes_)(n).warn("Duplicate primes were passed in");

  // Compute Mixed mul
  for (unsigned int i = 1; i < nPrimes_; ++i) {
    ubase_type prod = 1;
    for (unsigned int j = i; j > 0; --j) {
      prod = ModOp<ubase_type>::mul(prod, primes_[j - 1], primes_[i]);
    }
    mixmuls_[i - 1] = ModOp<ubase_type>::inv(prod, primes_[i]);
  }
  const ubase_type fourierOrder = getFourierOrder<CRT<UIntType> >();
  ASSERT1(fourierOrder > 15)
  (fourierOrder)(1 << fourierOrder)
      .warn("Fourier order of the field is very small");
  primRoots.reset(new CRT<UIntType>[fourierOrder]);
  primRootInvs.reset(new CRT<UIntType>[fourierOrder]);

  computePrimitiveRoots<CRT<UIntType> >(-1, primRoots.get(), primRootInvs.get(),
                                        fourierOrder);
}

//******************************************************************************
template <typename UIntType>
long double CRT<UIntType>::getSize() {
  long double result = 1.0;
  for (unsigned int i = 0; i < nPrimes_; i++) result *= primes_[i];
  return result;
}

//******************************************************************************
// D. E. Knuth, Seminumerical Algorithms, 4.3.2, ex 7 (p. 293)
template <typename UIntType>
typename CRT<UIntType>::ubase_type* CRT<UIntType>::mixedRadix() const {
  ubase_type* rads = new ubase_type[nPrimes_];
  ubase_type eval;
  unsigned int i, j;
  for (i = 0; i < nPrimes_; ++i) {
    rads[i] = vals_[i];
  }
  for (i = 1; i < nPrimes_; ++i) {
    eval = rads[i - 1];
    for (j = i - 1; j > 0; --j) {
      eval = ModOp<ubase_type>::mul_add(eval, primes_[j - 1], rads[j - 1],
                                        primes_[i]);
      // if (eval >= primes_[i] - rads[j-1])
      //   eval -= primes_[i] - rads[j-1];
      // else
      //   eval += rads[j-1];
    }
    // if (rads[i] >= eval)
    //   rads[i] -= eval;
    // else
    //   rads[i] += primes_[i] - eval;
    rads[i] =
        ModOp<ubase_type>::sub_mul(rads[i], eval, mixmuls_[i - 1], primes_[i]);
  }
  return rads;
}

//************************************************************************
template <typename UIntType>
int CRT<UIntType>::toDebugString(char* buffer) const {
#ifdef __LP64__
  const char* formatStr = "%ld";
#else
  const char* formatStr = "%d";
#endif

  int pos = toString(buffer);
  buffer[pos++] = '=';
  buffer[pos++] = '[';
  for (unsigned int i = 0; i < nPrimes_; ++i) {
    if (i) buffer[pos++] = ',';
#ifdef _MSC_VER
    pos += sprintf_s(buffer + pos, sizeof(vals_[0]) * 5, formatStr,
                     (vals_[i] <= (primes_[i] >> 1))
                         ? static_cast<sbase_type>(vals_[i])
                         : -static_cast<sbase_type>(primes_[i] - vals_[i]));
#else
    pos += snprintf(buffer + pos, sizeof(vals_[0]) * 5, formatStr,
                    (vals_[i] <= (primes_[i] >> 1))
                        ? static_cast<sbase_type>(vals_[i])
                        : -static_cast<sbase_type>(primes_[i] - vals_[i]));
#endif
  }
  buffer[pos++] = ']';
  buffer[pos] = '\0';
  return pos;
}
//************************************************************************
template <typename UIntType>
int CRT<UIntType>::toString(char* buffer) const {
  ubase_type* rads = mixedRadix();
  bool negated;
  int i;
  for (i = nPrimes_ - 1; i >= 0; --i) {
    UIntType half = primes_[i] >> 1;
    if (rads[i] > half) {
      negated = true;
      break;
    }
    if (rads[i] < half) {
      negated = false;
      break;
    }
  }
  if (i < 0) negated = false;  // Exact middle is positive

  if (negated) {
    delete[] rads;
    rads = operator-().mixedRadix();
  }

  const ubase_type radix = 10;
  // std::string digits;
  int pos = 0;
  unsigned int highRad = nPrimes_;
  typename UIntTypeInfo<UIntType>::unsigned_type2x quo;

  while (highRad > 0) {
    quo = 0;
    for (unsigned int i = highRad - 1; i > 0; i--) {
      quo += rads[i];
      rads[i] = static_cast<ubase_type>(quo / radix);
      quo %= radix;
      quo *= primes_[i - 1];
    }
    quo += rads[0];
    rads[0] = static_cast<ubase_type>(quo / radix);

    buffer[pos++] = '0' + quo % radix;
    while ((highRad > 0) && (rads[highRad - 1] == 0)) highRad--;
  }
  delete[] rads;

  // Do not negate 0
  if (negated) {
    buffer[pos++] = '-';
  }
  buffer[pos] = '\0';
  std::reverse(buffer, buffer + pos);
  return pos;
}

//******************************************************************************
template <typename UIntType>
CRT<UIntType> CRT<UIntType>::getPow(unsigned int right) const {
  CRT<UIntType> base = *this;
  unsigned int n = right;
  CRT<UIntType> result = 1;
  while (1) {
    if ((n & 1) == 1)  // if last bit is 1
      result *= base;
    n >>= 1;  // divide by 2
    if (n == 0) return result;
    base.sqr();
  }
}

//******************************************************************************
template <typename UIntType>
CRT<UIntType>& CRT<UIntType>::powerBy(unsigned int right) {
  CRT<UIntType> base = *this;
  unsigned int n = right;
  // PrimeField  = PrimeField(1);
  this->setTo(1);
  while (1) {
    if ((n & 1) == 1)  // if last bit is 1
      *this *= base;
    n >>= 1;  // divide by 2
    if (n == 0) break;
    base.sqr();
  }
  return *this;
}

//******************************************************************************
template <typename UIntType>
CRT<UIntType> CRT<UIntType>::getSqrt() const {
  unsigned int i;
  CRT<UIntType> result = 0;
  // try {
  for (i = 0; i < nPrimes_; i++) {
    result.vals_[i] = ModOp<ubase_type>::sqrt(vals_[i], primes_[i]);
  }
  //}
  //	catch (uint32_t) {
  //    return 0;
  //		std::stringstream errs;
  //		errs << "Number has no square root: " << toString() << " [ ";
  //		for (i = 0; i < _nPrimes; i++) {
  //			errs << (unsigned int)(_vals[i]) << ' ';
  //		}
  //		errs << ']';
  //		throw std::runtime_error(errs.str());
  //}
  return result;
}

//******************************************************************************
/*
std::istream& operator>>(std::istream& is, CRT32& f)
{
  std::string digits;
        is >> std::ws;
        char nextChar = is.get();
        bool negated = (nextChar == '-');
        if ( (nextChar == '-') || (nextChar == '+') )
                nextChar = is.get();
        else if ( !isdigit(nextChar) ) {
                is.putback(nextChar);
                is.clear(std::ios::failbit);
                return is;
        }

        while ( isdigit(nextChar) ) {
                digits.insert(digits.size(), 1, nextChar);
                nextChar = is.get();
        }
        is.putback(nextChar);
        f = negated? -PrimeField(digits): PrimeField(digits);
        return is;
}

std::ostream& operator<<(std::ostream& os, const CRT32& f)
{
        os << f.toString();
        return os;
}
*/
/*
template <typename UIntType>
void CRT32::randomInit(unsigned int seed) {
  math::random<uint32_t>::setSeed(seed);
  // rg.RandomInit((uint32_t) seed);
}
*/

}  // namespace math::field
