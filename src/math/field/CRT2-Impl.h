#ifndef NTRP_MATH_FIELD_CRT2_H
#error "Do not include this file. This is used internaly by CRT2.h"
#endif

/**
 * @file CRT2-Impl.h Implementation of math::CRT2
 */

#include "util/SmartAssert.h"
#include "math/util/prime.h"

namespace math {
namespace field {

//template <typename UIntType>
//unsigned int CRT2<UIntType>::multMod.getSize() = 0;

//template <typename UIntType>
//typename CRT2<UIntType>::ubase_type* CRT2<UIntType>::multMod = NULL;

//template <typename UIntType>
//typename CRT2<UIntType>::ubase_type* CRT2<UIntType>::mixmuls_ = NULL;

template <typename UIntType>
MultiMod<UIntType> CRT2<UIntType>::multMod;

template <typename UIntType>
boost::scoped_array<CRT2<UIntType> > CRT2<UIntType>::primRoots;

template <typename UIntType>
boost::scoped_array<CRT2<UIntType> > CRT2<UIntType>::primRootInvs;

//** Default Constructor *******************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>::CRT2()
  : vals_(new ubase_type[multMod.getSize()]) {}

//******************************************************************************
/*
template <typename UIntType>
__forceinline CRT2<UIntType>::CRT2(typename CRT2<UIntType>::ubase_type val)
  : vals_(new ubase_type[CRT2<UIntType>::multMod.getSize()]) {
    for (unsigned int i = 0; i < multMod.getSize(); ++i) {
      vals_[i] = val % (multMod[i]);
    }
}
*/
//******************************************************************************
template <typename UIntType>
__forceinline CRT2<UIntType>::CRT2(typename CRT2<UIntType>::sbase_type val)
  : vals_(new ubase_type[multMod.getSize()]) {
  if (val < 0) {
    for (unsigned int i = 0; i < multMod.getSize(); i++)
      vals_[i] = multMod[i] - ((-val)%multMod[i]);
  } else {
    for (unsigned int i = 0; i < multMod.getSize(); i++)
      vals_[i] = val%multMod[i];
  }
}

//** Copy constructor **********************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>::CRT2(const CRT2<UIntType>& right)
  : vals_(new ubase_type[multMod.getSize()]) {
  for (unsigned int i = 0; i < multMod.getSize(); i++) {
    vals_[i] = right.vals_[i];
  }
}

//** Destructor ****************************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>::~CRT2() {
  delete[] vals_;
}

//******************************************************************************
/*
template <typename UIntType>
template <typename CRT2<UIntType>::sbase_type VAL> __forceinline
void CRT2<UIntType>::setTo()  {
  if (VAL < 0) {
    for (unsigned int i = 0; i < multMod.getSize(); i++)
      vals_[i] = multMod[i] - ((-VAL) % multMod[i]);
  } else {
    for (unsigned int i = 0; i < multMod.getSize(); i++)
      vals_[i] = VAL % multMod[i];
  }
}
*/
// Specialize
template <> template <> __forceinline
void CRT2<uint32_t>::setTo<0>()  {
  for (size_t i = 0; i < multMod.getSize(); ++i) { vals_[i] = 0; }
}

template <> template <>
__forceinline void CRT2<uint64_t>::setTo<0>()  {
  for (size_t i = 0; i < multMod.getSize(); ++i) { vals_[i] = 0; }
}

template <> template <>
__forceinline void CRT2<uint32_t>::setTo<1>()  {
  for (size_t i = 0; i < multMod.getSize(); ++i) { vals_[i] = 1; }
}

template <> template <>
__forceinline void CRT2<uint64_t>::setTo<1>()  {
  for (size_t i = 0; i < multMod.getSize(); ++i) { vals_[i] = 1; }
}


template <> template <>
__forceinline void CRT2<uint32_t>::setTo<-1>()  {
  for (size_t i = 0; i < multMod.getSize(); ++i) { vals_[i] = multMod[i]-1; }
}
template <> template <>
__forceinline void CRT2<uint64_t>::setTo<-1>()  {
  for (size_t i = 0; i < multMod.getSize(); ++i) { vals_[i] = multMod[i]-1; }
}

//******************************************************************************
template <typename UIntType> __forceinline
void CRT2<UIntType>::setTo(typename CRT2<UIntType>::sbase_type val) {
  if (val >= 0) {
    for (size_t i = 0; i < multMod.getSize(); i++) {
      vals_[i] = val % (multMod[i]);
    }
  } else {
    for (size_t i = 0; i < multMod.getSize(); i++) {
      vals_[i] = multMod[i] - ((-val)%multMod[i]);
    }
  }
}
//******************************************************************************
template <typename UIntType> __forceinline
void CRT2<UIntType>::random() {
  for (size_t i = 0; i < multMod.getSize(); ++i) {
    vals_[i] = math::random<typename CRT2<UIntType>::ubase_type>::get()
        % multMod[i];
  }
}
//******************************************************************************
template <typename UIntType> __forceinline
void CRT2<UIntType>::invertableRandom() {
  do { this->random(); } while (!this->hasInverse());
}
//### Property checks ########################################################
template <> template <> __forceinline
bool CRT2<uint32_t>::is<0>() const {
  for (size_t i = 0; i < multMod.getSize(); ++i) {
    if (vals_[i] != 0) return false;
  }
  return true;
}
template <> template <>
__forceinline bool CRT2<uint64_t>::is<0>() const {
  for (size_t i = 0; i < multMod.getSize(); ++i) {
    if (vals_[i] != 0) return false;
  }
  return true;
}

template <> template <>
__forceinline bool CRT2<uint32_t>::is<1>() const {
  for (size_t i = 0; i < multMod.getSize(); i++) {
    if (vals_[i] != 1) return false;
  }
  return true;
}
template <> template <>
__forceinline bool CRT2<uint64_t>::is<1>() const {
  for (size_t i = 0; i < multMod.getSize(); i++) {
    if (vals_[i] != 1) return false;
  }
  return true;
}

template <> template <>
__forceinline bool CRT2<uint32_t>::is<-1>() const {
  for (size_t i = 0; i < multMod.getSize(); ++i) {
    if (vals_[i] != multMod[i]-1) return false;
  }
  return true;
}
template <> template <>
__forceinline bool CRT2<uint64_t>::is<-1>() const {
  for (size_t i = 0; i < multMod.getSize(); ++i) {
    if (vals_[i] != multMod[i]-1) return false;
  }
  return true;
}

template <typename UIntType>
template <int VAL> __forceinline
bool CRT2<UIntType>::is() const {
  if (VAL>=0) {
    for (size_t i = 0; i < multMod.getSize(); ++i)
      if (vals_[i] != VAL%multMod[i]) return false;
    return true;
  } else {
    for (size_t i = 0; i < multMod.getSize(); ++i)
      if (vals_[i] != multMod[i] - ((-VAL) % multMod[i])) return false;
    return true;
  }
}

//******************************************************************************
template <typename UIntType> __forceinline
bool CRT2<UIntType>::is(typename CRT2<UIntType>::sbase_type val) const {
if (val>=0) {
    for (size_t i = 0; i < multMod.getSize(); i++)
      if (vals_[i] != val%multMod[i]) return false;
    return true;
  } else {
    for (size_t i = 0; i < multMod.getSize(); i++)
      if (vals_[i] != multMod[i]-((-val)%multMod[i])) return false;
    return true;
  }
}

//******************************************************************************
template <typename UIntType> __forceinline
bool CRT2<UIntType>::isPositive() const { return !isNegative(); }

//******************************************************************************
template <typename UIntType> __forceinline
bool CRT2<UIntType>::isNegative() const {
  boost::scoped_array<ubase_type> rads(new ubase_type[multMod.getSize()]);
  multMod.crtToMixedRadix(rads.get(),vals_);
  return multMod.mixedRadixIsNegative(rads.get());
}

//******************************************************************************
template <typename UIntType> __forceinline
bool CRT2<UIntType>::hasInverse() const {
  for (unsigned int i = 0; i < multMod.getSize(); ++i) {
    if (vals_[i] == 0) { return false;  }
  }
  return true;
}

//### Modifiers ################################################################
template <typename UIntType> __forceinline
void CRT2<UIntType>::negate() {
  for (unsigned int i = 0; i < multMod.getSize(); ++i) {
    if (vals_[i] != 0) { vals_[i] = multMod[i] - vals_[i]; }
  }
}

//******************************************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>& CRT2<UIntType>::swap(CRT2<UIntType>& f) {
  ubase_type* tmp = vals_;
  vals_ = f.vals_;
  f.vals_ = tmp;
  return *this;
}

//******************************************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>& CRT2<UIntType>::invert() {
  ASSERT1(hasInverse())(*this)(multMod.getSize()).msg("Has no inverse");
  unsigned int i;
  //try {
    for (i = 0; i < multMod.getSize(); i++)
      vals_[i] = ModOp<UIntType>::inv(vals_[i], multMod[i]);
  //}
  //catch (...) {
  //  this->setTo<0>();
    // No Inverse
    //ASSERT0(false)(i)(_vals[i])(_primes[i]).msg("Value has no inverse");
  //}
  return *this;
}

//******************************************************************************
template <typename UIntType> __forceinline
void CRT2<UIntType>::add(CRT2* dest,
                         const CRT2& src1,
                         const CRT2& src2) {
  *dest = src1;
  *dest += src2;
}

//******************************************************************************
template <typename UIntType> __forceinline
void CRT2<UIntType>::subtract(CRT2* dest,
                              const CRT2& src1,
                              const CRT2& src2) {
  *dest = src1;
  *dest -= src2;
}

//******************************************************************************
template <typename UIntType> __forceinline
void CRT2<UIntType>::neg(CRT2* dest, const CRT2& src) {
  *dest = src;
  dest->negate();
}

//******************************************************************************
template <typename UIntType> __forceinline
void CRT2<UIntType>::multiply(CRT2* dest,
                             const CRT2& src1,
                             const CRT2& src2) {
  *dest = src1;
  *dest *= src2;
}

//******************************************************************************
template <typename UIntType> __forceinline
void CRT2<UIntType>::divide(CRT2<UIntType>* dest,
                            const CRT2<UIntType>& src1,
                            const CRT2<UIntType>& src2) {
  CRT2<UIntType>::multiply(dest, src1, src2.getInverse());
}

//******************************************************************************
template <typename UIntType> __forceinline
void CRT2<UIntType>::pow(CRT2<UIntType>* dest,
                         const CRT2<UIntType>& src,
                         unsigned int p) {
  *dest = src;
  dest->powerBy(p);
}

//******************************************************************************
template <typename UIntType> __forceinline
bool operator==(const CRT2<UIntType>& left, const CRT2<UIntType>& right) {
  for (unsigned int i = 0; i < CRT2<UIntType>::multMod.getSize(); ++i) {
    if (left.vals_[i] != right.vals_[i]) {
      return false;
    }
  }
  return true;
}

//******************************************************************************
// Returns (left + right) % each modulus
template <typename UIntType> __forceinline
CRT2<UIntType> operator+(const CRT2<UIntType>& left,
                         const CRT2<UIntType>& right) {
  CRT2<UIntType> result = left;
  return result += right;
}

//******************************************************************************
//! Returns (left - right) % each modulus
template <typename UIntType> __forceinline
CRT2<UIntType> operator-(const CRT2<UIntType>& left,
                         const CRT2<UIntType>& right) {
  CRT2<UIntType> result = left;
  return result -= right;
}

//******************************************************************************
//! Returns (left * right) % each modulus
template <typename UIntType> __forceinline
CRT2<UIntType> operator*(const CRT2<UIntType>& left,
                         const CRT2<UIntType>& right) {
  CRT2<UIntType> result = left;
  return result *= right;
}

//******************************************************************************
template <typename UIntType> __forceinline
CRT2<UIntType> CRT2<UIntType>::operator-() const {
  CRT2<UIntType> result; // = CRT2<UIntType>(0);
  for (unsigned int i = 0; i < multMod.getSize(); ++i)
    result.vals_[i] = multMod[i] - vals_[i];
  return result;
}

//******************************************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>& CRT2<UIntType>::operator=(const CRT2<UIntType>& right) {
  for (unsigned int i = 0; i < multMod.getSize(); i++) {
    vals_[i] = right.vals_[i];
  }
  return *this;
}

//******************************************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>& CRT2<UIntType>::operator+=(const CRT2<UIntType>& right) {
  for (unsigned int i = 0; i < multMod.getSize(); ++i) {
    if (vals_[i] >= multMod[i] - right.vals_[i]) {
      vals_[i] -= multMod[i] - right.vals_[i];
    } else {
      vals_[i] += right.vals_[i];
    }
  }
  return *this;
}

//******************************************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>& CRT2<UIntType>::operator-=(const CRT2<UIntType>& right) {
  for (unsigned int i = 0; i < multMod.getSize(); ++i) {
    if (vals_[i] >= right.vals_[i]) {
      vals_[i] -= right.vals_[i];
    } else {
      vals_[i] += multMod[i] - right.vals_[i];
    }
  }
  return *this;
}

//******************************************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>& CRT2<UIntType>::operator*=(const CRT2<UIntType>& right) {
  if (is<0>()) return *this;
  if (right.is<0>()) return operator=(right);

  for (unsigned int i = 0; i < multMod.getSize(); ++i) {
    vals_[i] = ModOp<UIntType>::mul(vals_[i], right.vals_[i], multMod[i]);
  }
  return *this;
}

//******************************************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>& CRT2<UIntType>::sqr() {
  if (is<0>()) return *this;

  for (unsigned int i = 0; i < multMod.getSize(); ++i) {
    vals_[i] = ModOp<UIntType>::mul(vals_[i], vals_[i], multMod[i]);
  }
  return *this;
}

//******************************************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>* CRT2<UIntType>::alloc(unsigned int size) {
  // Allocate space for CRT2*size, and also do internal allocation in one shot
  char* cBuff = new char[size * (sizeof(ubase_type)
      * multMod.getSize() + sizeof(CRT2<UIntType>))];
  CRT2<UIntType>* buff = reinterpret_cast<CRT2<UIntType>*>(cBuff);
  // For each element adjust internal pointer
  for (unsigned int i = 0; i < size; ++i) {
    buff[i].vals_ = reinterpret_cast<ubase_type*>(
        cBuff + size * sizeof(CRT2<UIntType>)
      + i * sizeof(ubase_type) * multMod.getSize());
  }
  return buff;
}

//******************************************************************************
template <typename UIntType> __forceinline
void CRT2<UIntType>::dealloc(CRT2<UIntType>* buff) {
  delete[] reinterpret_cast<char*>(buff);
}

//******************************************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>* CRT2<UIntType>::allocSq(unsigned int size) {
  return CRT2<UIntType>::alloc(size*size);
}

//******************************************************************************
template <typename UIntType> __forceinline
CRT2<UIntType>* CRT2<UIntType>::allocSq2p(unsigned int size) {
  // Must support indexing [i<<szLog|j]
  const unsigned int szLog = math::intlog(size);
  const unsigned int sz2p = 1<<szLog;
  const unsigned int sz2p2 = sz2p<<szLog;

  // Allocate space for CRT232*size, and also do internal allocation in one shot
  char* cBuff = new char[multMod.getSize() * sizeof(ubase_type)
                         * size * size + sizeof(CRT2<UIntType>) * sz2p2];
  cBuff += sz2p * sizeof(CRT2<UIntType>);  // Skip object allocation space
  CRT2<UIntType>* buff = reinterpret_cast<CRT2<UIntType>*>(cBuff);
  // For each element adjust internal pointer
  unsigned int i;
  unsigned int j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      buff[(i << sz2p) + j]._vals = reinterpret_cast<ubase_type*>(
        cBuff + (i * size + j) * sizeof(ubase_type) * multMod.getSize()
      );
    }
    for (; j < sz2p; ++j) buff[(i << sz2p)+j].vals_ = NULL;
  }
  for (; i < sz2p; ++i)
    for (j = 0; j < sz2p; ++j) buff[(i << sz2p) + j].vals_ = NULL;

  return buff;
}

template <typename UIntType>
void CRT2<UIntType>::reset() {
  multMod.reset();
  CRT2<UIntType>::primRoots.reset();
  CRT2<UIntType>::primRootInvs.reset();
}

template <typename UIntType>
void CRT2<UIntType>::setPrimes(const ubase_type* prs, int n) {

  ASSERT1(n>0)(n).msg("There must be at least one prime.");
  ASSERT1(n>1)(n).warn("CRT2 is not most efficient option for single prime.");

  // Check if all prime
  const ubase_type* prm;

  ASSERT1((prm = std::find_if(
      prs, prs + n,
      std::not1(std::ptr_fun(maybe_prime<ubase_type>)) )) == prs + n)
    (prm - prs)(n).debug("A value in passed in array is not a prime");

  multMod.init(prs,n);

  const ubase_type fourierOrder = getFourierOrder<CRT2<UIntType> >();
  ASSERT1(fourierOrder > 15)(fourierOrder)(1 << fourierOrder)
      .warn("Fourier order of the field is very small");
  primRoots.reset(new CRT2<UIntType>[fourierOrder]);
  primRootInvs.reset(new CRT2<UIntType>[fourierOrder]);

  computePrimitiveRoots<CRT2<UIntType> >(
      -1, primRoots.get(), primRootInvs.get(), fourierOrder);
}

//******************************************************************************
template <typename UIntType>
long double CRT2<UIntType>::getSize() {
  long double result = 1.0;
  for (unsigned int i = 0; i < multMod.getSize(); i++) {
    result *= multMod[i];
  }
  return result;
}

//******************************************************************************
template <typename UIntType>
int CRT2<UIntType>::toDebugString(char* buffer) const {
#ifdef __LP64__
  const char* formatStr = "%ld";
#else
  const char* formatStr = "%d";
#endif

  int pos = toString(buffer);
  buffer[pos++] = '=';
  buffer[pos++] = '[';
  for (size_t i = 0; i < multMod.getSize(); ++i) {
    if (i) buffer[pos++] = ',';
#ifdef _MSC_VER
    pos += sprintf_s(buffer + pos, sizeof(vals_[0]) * 5, formatStr,
      (vals_[i] <= (multMod[i] >> 1))
          ? static_cast<sbase_type>(vals_[i])
          : -static_cast<sbase_type>(multMod[i] - vals_[i]));
#else
    pos += snprintf(buffer + pos, sizeof(vals_[0]) * 5, formatStr,
      (vals_[i] <= (multMod[i] >> 1))
          ? static_cast<sbase_type>(vals_[i])
          : -static_cast<sbase_type>(multMod[i] - vals_[i]));
#endif
  }
  buffer[pos++] = ']';
  buffer[pos] = '\0';
  return pos;
}
//************************************************************************
template <typename UIntType>
int CRT2<UIntType>::toString(char* buffer) const {
  boost::scoped_array<ubase_type> rads(new ubase_type[multMod.getSize()]);
  multMod.crtToMixedRadix(rads.get(),vals_);
  return multMod.mixedRadixToString(
      buffer, getMaxStringRepLength(), rads.get(), 10);
}

/*
template <typename UIntType>
int CRT2<UIntType>::toString(char* buffer) const {

  ubase_type* rads = mixedRadix();
  bool negated =
    (rads[multMod.getSize()-1] >= multMod[multMod.getSize()-1] - rads[multMod.getSize()-1]);

  if (negated) {
    delete[] rads;
    rads = operator-().mixedRadix();
  }

  const ubase_type radix = 10;
  //std::string digits;
  int pos = 0;
  unsigned int highRad = multMod.getSize();
  uint64_t quo;

  while (highRad > 0) {
    quo = 0;
    for (unsigned int i = highRad - 1; i > 0; i--) {
      quo += rads[i];
      rads[i] = static_cast<ubase_type>(quo / radix);
      quo %= radix;
      quo *= multMod[i-1];
    }
    quo += rads[0];
    rads[0] = static_cast<ubase_type> (quo / radix);

    buffer[pos++] = '0' + quo % radix;
    while ((highRad > 0) && (rads[highRad - 1] == 0)) highRad--;
  }
  delete[] rads;

  // Do not negate 0
  if (negated) {
    buffer[pos++] = '-';
  }
  buffer[pos] = '\0';
  std::reverse(buffer, buffer+pos);
  return pos;
}
*/
//******************************************************************************
template <typename UIntType>
CRT2<UIntType> CRT2<UIntType>::getPow(unsigned int right) const {
  CRT2<UIntType> base = *this;
  unsigned int n = right;
  CRT2<UIntType> result = 1;
  while (1) {
    if ((n&1) == 1)  // if last bit is 1
      result *= base;
    n >>= 1;  // divide by 2
    if (n == 0)
      return result;
    base.sqr();
  }
}

//******************************************************************************
template <typename UIntType>
CRT2<UIntType>& CRT2<UIntType>::powerBy(unsigned int right)  {
  CRT2<UIntType> base = *this;
  unsigned int n = right;
  //PrimeField  = PrimeField(1);
  this->setTo(1);
  while (1) {
    if ((n&1) == 1)  // if last bit is 1
      *this *= base;
    n >>= 1;  // divide by 2
    if (n == 0)
      break;
    base.sqr();
  }
  return *this;
}

//******************************************************************************
template <typename UIntType>
CRT2<UIntType> CRT2<UIntType>::getSqrt() const {
  unsigned int i;
  CRT2<UIntType> result = 0;
  //try {
    for (i = 0; i < multMod.getSize(); i++) {
      result.vals_[i] = ModOp<ubase_type>::sqrt(vals_[i], multMod[i]);
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
std::istream& operator>>(std::istream& is, CRT232& f)
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

std::ostream& operator<<(std::ostream& os, const CRT232& f)
{
	os << f.toString();
	return os;
}
*/
/*
template <typename UIntType>
void CRT232::randomInit(unsigned int seed) {
  math::random<uint32_t>::setSeed(seed);
  // rg.RandomInit((uint32_t) seed);
}
*/

}  // namespace field
}  // namespace math
