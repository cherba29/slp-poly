#ifndef INTERP_MATH_FIELD_H
#error "Do not include this file. This is used internaly by Field.h"
#endif

/**
 * @file Field-Impl.h Implementation of math::Field32
 *
 */

#include "math/util/modular-x86.h"
#include "math/util/prime.h"
#include "math/util/random.h"

namespace math::field {

//*****************************************************************************
template <typename T>
__forceinline Field<T>::Field(typename Field<T>::sbase_type v) {
  val_ = (v < 0) ? (modulus + v)
                 : ((static_cast<unsigned>(v) < modulus) ? v : (v - modulus));
}
//*****************************************************************************
// template <typename T>
//__forceinline Field<T>::Field(unsigned int v) {
//  val_ = (v < modulus)?v:(v - modulus);
//}

//*****************************************************************************
template <typename T>
__forceinline Field<T>& Field<T>::operator+=(const Field<T>& right) {
  // Ensure no overflow
  val_ = modulus - val_;  // negate
  if (val_ > right.val_) {
    val_ -= right.val_;
    val_ = modulus - val_;  // negate
  } else {
    val_ = right.val_ - val_;
  }
  return *this;
}
//*****************************************************************************
/*template <typename T>
template <typename Field<T>::sbase_type VAL> __forceinline
void Field<T>::setTo() {
  if (VAL < 0) {
    if (static_cast<T> (-VAL) < modulus)
      val_ = modulus + VAL;
    else
      val_ = modulus - (VAL + modulus);
  } else {
    if (static_cast<T> (VAL) < modulus)
      val_ = VAL;
    else
      val_ = VAL - modulus;
  }
} */
//*****************************************************************************
template <typename T>
__forceinline void Field<T>::setTo(typename Field<T>::sbase_type v) {
  if (v < 0) {
    if (static_cast<T>(-v) < modulus)
      val_ = modulus + v;
    else
      val_ = modulus - (v + modulus);
  } else {
    if (static_cast<T>(v) < modulus)
      val_ = v;
    else
      val_ = v - modulus;
  }
}
//*****************************************************************************

// template <typename T>
// template <typename Field<T>::sbase_type VAL> __forceinline
// bool Field<T>::is() const {
//   return (VAL>0)?(val_==VAL%modulus):(val_==(modulus-((-VAL)%modulus)));
// }

template <>
template <>
__forceinline bool Field<uint64_t>::is<0>() const {
  return val_ == 0;
}

template <>
template <>
__forceinline bool Field<uint32_t>::is<0>() const {
  return val_ == 0;
}

template <>
template <>
__forceinline bool Field<uint64_t>::is<1>() const {
  return val_ == 1;
}

template <>
template <>
__forceinline bool Field<uint32_t>::is<1>() const {
  return val_ == 1;
}

template <>
template <>
__forceinline bool Field<uint64_t>::is<-1>() const {
  return val_ == modulus - 1;
}

template <>
template <>
__forceinline bool Field<uint32_t>::is<-1>() const {
  return val_ == modulus - 1;
}

template <typename T>
__forceinline bool Field<T>::is(typename Field<T>::sbase_type val) const {
  return (val >= 0) ? (val_ == val % modulus)
                    : (val_ == (modulus - ((-val) % modulus)));
}

template <typename T>
__forceinline bool Field<T>::hasInverse() const {
  return !is<0>();
}
//*****************************************************************************
template <typename T>
__forceinline Field<T> operator+(const Field<T>& left, const Field<T>& right) {
  Field<T> A;
  A.val_ = Field<T>::modulus - left.val_;
  if (A.val_ > right.val_) {
    A.val_ -= right.val_;
    A.val_ = Field<T>::modulus - A.val_;
  } else {
    A.val_ = right.val_ - A.val_;
  }
  return A;
}
//*****************************************************************************
template <typename T>
__forceinline Field<T> operator/(const Field<T>& left, const Field<T>& right) {
  return ((right.getInverse()) *= left);
}
//*****************************************************************************
template <typename T>
__forceinline Field<T> operator-(const Field<T>& left, const Field<T>& right) {
  Field<T> A;
  if (left.val_ >= right.val_)
    A.val_ = left.val_ - right.val_;
  else
    A.val_ = Field<T>::modulus - (right.val_ - left.val_);
  return A;
}

//*****************************************************************************
#ifdef __LP64__
template <>
__forceinline Field<uint64_t> operator*(const Field<uint64_t>& left,
                                        const Field<uint64_t>& right) {
  uint128_t q = left.val_;
  // result = ((uint128_t) x * y) >> 64;
  q *= right.val_;
  Field<uint64_t> v;
  v.val_ = q % Field<uint64_t>::modulus;
  return v;
}
#endif  // __LP64__

template <>
__forceinline Field<uint32_t> operator*(const Field<uint32_t>& left,
                                        const Field<uint32_t>& right) {
  uint64_t q = left.val_;
  // result = ((uint128_t) x * y) >> 64;
  q *= right.val_;
  Field<uint32_t> v;
  v.val_ = static_cast<uint32_t>(q % Field<uint32_t>::modulus);
  return v;
}
//*****************************************************************************
template <typename T>
__forceinline Field<T> Field<T>::operator-() const {
  Field<T> v;
  v.val_ = val_ ? (modulus - val_) : 0;
  return v;
}
//*****************************************************************************
template <>
__forceinline Field<uint32_t>& Field<uint32_t>::operator*=(
    const Field<uint32_t>& right) {
  uint64_t q = val_;
  q *= right.val_;
  val_ = q % Field<uint32_t>::modulus;
  return *this;
}

//#ifdef __LP64__
template <>
__forceinline Field<uint64_t>& Field<uint64_t>::operator*=(
    const Field<uint64_t>& right) {
  uint128_t q = val_;
  q *= right.val_;
  val_ = q % Field<uint64_t>::modulus;
  return *this;
}
//#endif
//*****************************************************************************
template <typename T>
__forceinline Field<T>& Field<T>::operator-=(const Field<T>& right) {
  if (val_ >= right.val_)
    val_ -= right.val_;
  else
    val_ = modulus - (right.val_ - val_);
  return *this;
}
//*****************************************************************************
template <typename T>
__forceinline bool operator==(const Field<T>& left, const Field<T>& right) {
  return left.val_ == right.val_;
}
//*****************************************************************************
template <typename T>
__forceinline bool operator!=(const Field<T>& left, const Field<T>& right) {
  return left.val_ != right.val_;
}
//*****************************************************************************
template <typename T>
__forceinline Field<T>& Field<T>::random() {
  val_ = math::random<typename Field<T>::ubase_type>::get() % modulus;
  return *this;
}
//*****************************************************************************
template <typename T>
__forceinline Field<T>& Field<T>::invertableRandom() {
  do {
    val_ = math::random<typename Field<T>::ubase_type>::get() % modulus;
  } while (!hasInverse());
  return *this;
}
//*****************************************************************************
template <typename T>
__forceinline void Field<T>::add(Field<T>* dest, const Field<T>& src1,
                                 const Field<T>& src2) {
  dest->val_ = modulus - src1.val_;
  if (dest->val_ > src2.val_) {
    dest->val_ -= src2.val_;
    dest->val_ = modulus - dest->val_;
  } else {
    dest->val_ = src2.val_ - dest->val_;
  }
}
//*****************************************************************************
template <typename T>
__forceinline void Field<T>::subtract(Field<T>* dest, const Field<T>& src1,
                                      const Field<T>& src2) {
  if (src1.val_ >= src2.val_)
    dest->val_ = src1.val_ - src2.val_;
  else
    dest->val_ = modulus - (src2.val_ - src1.val_);
}
//*****************************************************************************
template <typename T>
__forceinline void Field<T>::neg(Field<T>* dest, const Field<T>& src) {
  if (src.val_) {
    dest->val_ = modulus - src.val_;
  } else {
    dest->val_ = 0;
  }
}
//*****************************************************************************
#ifdef __LP64__
template <>
__forceinline void Field<uint64_t>::multiply(Field<uint64_t>* dest,
                                             const Field<uint64_t>& src1,
                                             const Field<uint64_t>& src2) {
  uint128_t q = src1.val_;
  BOOST_STATIC_ASSERT(sizeof(q) >= (sizeof(src1.val_) << 1));
  q *= src2.val_;
  dest->val_ = q % modulus;
}
#endif
template <>
__forceinline void Field<uint32_t>::multiply(Field<uint32_t>* dest,
                                             const Field<uint32_t>& src1,
                                             const Field<uint32_t>& src2) {
  uint64_t q = src1.val_;
  BOOST_STATIC_ASSERT(sizeof(q) >= (sizeof(src1.val_) << 1));
  q *= src2.val_;
  dest->val_ = q % modulus;
}
//*****************************************************************************
template <typename T>
__forceinline void Field<T>::divide(Field<T>* dest, const Field<T>& src1,
                                    const Field<T>& src2) {
  Field<T>::multiply(dest, src1, src2.getInverse());
}
//*****************************************************************************
template <typename T>
__forceinline Field<T> Field<T>::getPow(unsigned int p) const {
  Field<T> A;
  if (p == 1) {
    A.val_ = val_;
    return A;
  }
  A.val_ = 1;
  if (p == 0) {
    return A;
  }

  Field<T> r(*this);
  // r.val_ = val_;

  unsigned int pw = p;
  for (;;) {
    if (pw & 1) A *= r;
    pw >>= 1;
    if (!pw) break;
    r.sqr();
  };
  return A;
}
//*****************************************************************************
template <>
__forceinline Field<uint32_t>& Field<uint32_t>::sqr() {
  uint64_t q = val_;
  BOOST_STATIC_ASSERT(sizeof(q) >= (sizeof(val_) << 1));
  q *= val_;
  val_ = q % modulus;
  return *this;
}
#ifdef __LP64__
template <>
__forceinline Field<uint64_t>& Field<uint64_t>::sqr() {
  uint128_t q = val_;
  BOOST_STATIC_ASSERT(sizeof(q) >= (sizeof(val_) << 1));
  q *= val_;
  val_ = q % modulus;
  return *this;
}
#endif
//*****************************************************************************
template <>
__forceinline Field<uint32_t>& Field<uint32_t>::invert() {
  // val = 1/val;
  uint32_t c = modulus;
  uint32_t d = val_;
  // int c1 = 1, d1 = 0;
  int32_t c2 = 0, d2 = 1;
  uint32_t q, r;
  // int r1;
  int32_t r2;
  while (d) {
    q = c / d;
    r = c % d;
    // r1 = c1 - q*d1;
    r2 = c2 - q * d2;
    c = d;  // c1 = d1;
    c2 = d2;
    d = r;  // d1 = r1;
    d2 = r2;
  }
  if (c2 < 0)
    val_ = modulus + c2;
  else
    val_ = c2;
  //*this = Domain(c2) * (*this);
  return *this;
}

template <>
__forceinline Field<uint64_t>& Field<uint64_t>::invert() {
  // val = 1/val;
  uint64_t c = modulus;
  uint64_t d = val_;
  // int c1 = 1, d1 = 0;
  int64_t c2 = 0, d2 = 1;
  uint64_t q, r;
  // int r1;
  int64_t r2;
  while (d) {
    q = c / d;
    r = c % d;
    // r1 = c1 - q*d1;
    r2 = c2 - q * d2;
    c = d;  // c1 = d1;
    c2 = d2;
    d = r;  // d1 = r1;
    d2 = r2;
  }
  if (c2 < 0)
    val_ = modulus + c2;
  else
    val_ = c2;
  //*this = Domain(c2) * (*this);
  return *this;
}

//*****************************************************************************
template <typename T>
__forceinline Field<T>& Field<T>::powerBy(unsigned int p) {
  if (p == 1) {
    return *this;
  }
  if (p == 0) {
    val_ = 1;
    return *this;
  }

  Field<T> r(*this);
  val_ = 1;
  for (;;) {
    if (p & 1) operator*=(r);
    p >>= 1;
    if (!p) break;
    r.sqr();
  }
  return *this;
}
//*****************************************************************************
template <typename T>
__forceinline Field<T> Field<T>::getSqrt() const {
  Field<T> v;
  v.val_ = ModOp<typename Field<T>::ubase_type>::sqrt(val_, modulus);
  return v;
}
//*****************************************************************************
template <typename T>
__forceinline void Field<T>::pow(Field<T>* dest, const Field<T>& src,
                                 unsigned int p) {
  if (p == 1) {
    dest->val_ = src.val_;
    return;
  }
  if (p == 0) {
    dest->val_ = 1;
    return;
  }
  Field<T> r;
  r.val_ = src.val_;
  dest->val_ = 1;
  unsigned int pw = p;
  for (;;) {
    if (pw & 1) *dest *= r;
    pw >>= 1;
    if (!pw) break;
    r.sqr();
  };
}
//*****************************************************************************
template <typename T>
__forceinline Field<T>* Field<T>::allocSq(unsigned int size) {
  return new Field<T>[size * size];
}
//*****************************************************************************
template <typename T>
__forceinline Field<T>* Field<T>::allocSq2p(unsigned int size) {
  const unsigned int szLog = math::intlog(size);
  return new Field<T>[static_cast<unsigned int>(1) << (szLog << 1)];
}
//*****************************************************************************

template <>
__forceinline unsigned int getFourierOrder<Field<uint32_t> >() {
  static unsigned int order =
      math::field::getPrimeFourierOrder(Field<uint32_t>::getModulus());
  return order;
}

template <>
__forceinline unsigned int getFourierOrder<Field<uint64_t> >() {
  static unsigned int order =
      math::field::getPrimeFourierOrder(Field<uint64_t>::getModulus());
  return order;
}

template <typename T>
typename Field<T>::ubase_type Field<T>::modulus = 0;

template <typename T>
typename Field<T>::ubase_type Field<T>::pos = 0;

template <typename T>
boost::scoped_array<Field<T> > Field<T>::primRoots;
template <typename T>
boost::scoped_array<Field<T> > Field<T>::primRootInvs;

template <typename T>
void Field<T>::reset() {
  Field<T>::modulus = 0;
  Field<T>::pos = 0;
  Field<T>::primRoots.reset();
  Field<T>::primRootInvs.reset();
}

template <typename T>
void Field<T>::setPrime(typename Field<T>::ubase_type mod) {
  ASSERT0(maybe_prime<typename Field<T>::ubase_type>(mod))
  (mod).msg("Passed in value is not a prime");
  Field<T>::modulus = mod;
  Field<T>::pos = modulus >> 1;
  //*(const_cast<base_type*>(&modulus)) = mod;
  ubase_type const fourierOrder = getFourierOrder<Field<T> >();
  ASSERT1(fourierOrder > 15)
  (fourierOrder).warn("Fourier order of the field is very small");
  primRoots.reset(new Field<T>[fourierOrder]);
  primRootInvs.reset(new Field<T>[fourierOrder]);
  computePrimitiveRoots<Field<T> >(Field<T>(-1), primRoots.get(),
                                   primRootInvs.get(), fourierOrder);
}

template <typename T>
int Field<T>::toString(char* buffer) const {
  /* This introduces dependence on boost::format and std::string
    // Convert number to string
    std::string str = (boost::format("%1%") % ((val_ <= pos) ?
      static_cast<long>(val_) : -static_cast<long>(modulus-val_))).str();

    ASSERT1(str.size() < Field32::getMaxStringRepLength())
      (srt.size())(Field32::getMaxStringRepLength())
      .msg("Element cannot be represented in max established length for string
    representation. Something went really wrong!");

    str.copy(buffer,str.size());
    buffer[str.size()]='\0';
  */

  const int maxStrLen = Field<T>::getMaxStringRepLength() - 1;
  int currPos = -1;
  ubase_type val;
  if (val_ > pos) {
    buffer[++currPos] = '-';
    val = modulus - val_;
  } else {
    val = val_;
  }
  int start = currPos + 1;
  do {
    buffer[++currPos] = '0' + val % 10;
    val /= 10;
  } while (val > 0 && currPos < maxStrLen);
  buffer[++currPos] = '\0';

  for (int i = start, end = currPos - 1; i < end; ++i, --end) {
    std::swap(buffer[i], buffer[end]);
  }

  //#ifdef _MSC_VER
  //  int len = sprintf_s(buffer, maxStrLen, "%lld",
  //    (val_ <= pos) ? static_cast<long long>(val_) : -static_cast<long
  //    long>(modulus-val_));
  //#else
  //  int len = snprintf(buffer, maxStrLen, "%lld",
  //    (val_ <= pos) ? static_cast<long long>(val_) : -static_cast<long
  //    long>(modulus-val_));
  //  buffer[maxStrLen-1] = '\0';
  //#endif
  return currPos;
}

template <typename T>
int Field<T>::toDebugString(char* buffer) const {
  /* This introduces dependence on boost::format and std::string
    // Convert number to string
    std::string str = (boost::format("%1%") % ((val_ <= pos) ?
      static_cast<long>(val_) : -static_cast<long>(modulus-val_))).str();

    ASSERT1(str.size() < Field32::getMaxStringRepLength())
      (srt.size())(Field32::getMaxStringRepLength())
      .msg("Element cannot be represented in max established length for string
    representation. Something went really wrong!");

    str.copy(buffer,str.size());
    buffer[str.size()]='\0';
  */
  const int maxStrLen = Field<T>::getMaxStringRepLength() - 1;
  int currPos = -1;
  ubase_type val;
  if (val_ > pos) {
    buffer[++currPos] = '-';
    val = modulus - val_;
  } else {
    val = val_;
  }
  while (val > 0 && currPos < maxStrLen) {
    buffer[++currPos] = val % 10;
    val /= 10;
  }
  buffer[++currPos] = '\0';
  return currPos;
}

}  // namespace math::field
