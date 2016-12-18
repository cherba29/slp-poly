#ifndef NTRP_MATH_FIELD_UINT2X32_H_
#error "Do not include this file. This is used internaly by Field_uint2x32.h"
#endif

/**
 * @file Field_uint2x32-Impl.h Implementation of math::Field_uint2x32<MOD1,MOD2>
 */

namespace math {
namespace field {

//**************************************************************************************************
// Constructor for base type
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2>::Field_uint2x32(typename Field_uint2x32<MOD1,MOD2>::ubase_type v) {
  val1_ = v % MOD1;
  val2_ = v % MOD2;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2>::Field_uint2x32(typename Field_uint2x32<MOD1,MOD2>::sbase_type v) {
  if (v < 0) {
    val1_ = MOD1 - ((-v)%MOD1);
    val2_ = MOD2 - ((-v)%MOD2);
  } else {
    val1_ = v % MOD1;
    val2_ = v % MOD2;
  }
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
void Field_uint2x32<MOD1,MOD2>::setTo(
    typename Field_uint2x32<MOD1,MOD2>::sbase_type v) {
  if (v < 0) {
    val1_ = MOD1 - ((-v)%MOD1);
    val2_ = MOD2 - ((-v)%MOD2);
  } else {
    val1_ = v % MOD1;
    val2_ = v % MOD2;
  }
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2>__forceinline
Field_uint2x32<MOD1,MOD2>& Field_uint2x32<MOD1,MOD2>::operator+=(
    const Field_uint2x32<MOD1,MOD2>& right) {
  val1_ = MOD1 - val1_; // negate
  if (val1_ > right.val1_) {
    val1_ -= right.val1_;
    val1_ = MOD1 - val1_; // negate
  } else {
    val1_ = right.val1_ - val1_;
  }
  val2_ = MOD2 - val2_; // negate
  if (val2_ > right.val2_) {
    val2_ -= right.val2_;
    val2_ = MOD2 - val2_; // negate
  } else {
    val2_ = right.val2_ - val2_;
  }
  return *this;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2> operator+(const Field_uint2x32<MOD1,MOD2>& left,
                                    const Field_uint2x32<MOD1,MOD2>& right) {
  Field_uint2x32<MOD1,MOD2> A;
  A.val1_ = MOD1 - left.val1_;
  A.val2_ = MOD2 - left.val2_;
  if (A.val1_ > right.val1_) {
    A.val1_ -= right.val1_;
    A.val1_ = MOD1 - A.val1_;
  } else {
    A.val1_ = right.val1_ - A.val1_;
  }
  if (A.val2_ > right.val2_) {
    A.val2_ -= right.val2_;
    A.val2_ = MOD2 - A.val2_;
  } else {
    A.val2_ = right.val2_ - A.val2_;
  }
  return A;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2> operator-(const Field_uint2x32<MOD1,MOD2>& left,
                                    const Field_uint2x32<MOD1,MOD2>& right) {
  Field_uint2x32<MOD1,MOD2> A;
  if (left.val1_ >= right.val1_)
    A.val1_ = left.val1_ - right.val1_;
  else
    A.val1_ = MOD1 - (right.val1_ - left.val1_);

  if (left.val2_ >= right.val2_)
    A.val2_ = left.val2_ - right.val2_;
  else
    A.val2_ = MOD2 - (right.val2_ - left.val2_);
  return A;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2> operator*(
    const Field_uint2x32<MOD1,MOD2>& left,
    const Field_uint2x32<MOD1,MOD2>& right) {
  Field_uint2x32<MOD1,MOD2> A;
  uint64_t q1 = left.val1_;
  q1 *= right.val1_;
  A.val1_ = q1%MOD1;
  uint64_t q2 = left.val2_;
  q2 *= right.val2_;
  A.val2_ = q2%MOD2;
  return A;
}
//**************************************************************************************************
template <uint32_t MOD1, uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2>& Field_uint2x32<MOD1,MOD2>::random() {
  val1_ = math::random<typename Field_uint2x32::ubase_type>::get() % MOD1;
  val2_ = math::random<typename Field_uint2x32::ubase_type>::get() % MOD2;
  return *this;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2>& Field_uint2x32<MOD1,MOD2>::invertableRandom() {
  do { random(); } while (!val1_ || !val2_); return *this;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
void Field_uint2x32<MOD1,MOD2>::divide(Field_uint2x32<MOD1,MOD2>* dest,
                                       const Field_uint2x32<MOD1,MOD2>& src1,
                                       const Field_uint2x32<MOD1,MOD2>& src2) {
  Field_uint2x32<MOD1,MOD2>::multiply(dest, src1, src2.getInverse());
}

//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2> operator/(const Field_uint2x32<MOD1,MOD2>& left,
                                    const Field_uint2x32<MOD1,MOD2>& right) {
  return ((right.getInverse()) *= left);
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
bool operator==(const Field_uint2x32<MOD1,MOD2>& left,
                const Field_uint2x32<MOD1,MOD2>& right) {
  return left.val1_ == right.val1_ && left.val2_ == right.val2_;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2>
__forceinline bool operator!=(const Field_uint2x32<MOD1,MOD2>& left,
                              const Field_uint2x32<MOD1,MOD2>& right) {
  return left.val1_ != right.val1_ && left.val2_ != right.val2_;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2> Field_uint2x32<MOD1,MOD2>::operator-() const {
  Field_uint2x32<MOD1,MOD2> r;
  if (val1_)  r.val1_ = MOD1 - val1_;
  else  r.val1_ = 0;
  if (val2_)  r.val2_ = MOD2 - val2_;
  else  r.val2_ = 0;
  return r;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2>& Field_uint2x32<MOD1,MOD2>::operator-=(
    const Field_uint2x32<MOD1,MOD2>& right) {
  if (val1_ >= right.val1_)  val1_ -= right.val1_;
  else val1_ = MOD1 - (right.val1_ - val1_);
  if (val2_ >= right.val2_)  val2_ -= right.val2_;
  else val2_ = MOD2 - (right.val2_ - val2_);
  return *this;
}

//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2>& Field_uint2x32<MOD1,MOD2>::operator*=(
    const Field_uint2x32<MOD1,MOD2>& m) {
  uint64_t q1 = val1_;
  q1 *= m.val1_;
  val1_ = q1 % MOD1;
  uint64_t q2 = val2_;
  q2 *= m.val2_;
  val2_ = q2 % MOD2;
  return *this;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2> Field_uint2x32<MOD1,MOD2>::getPow(unsigned int p) const {
  Field_uint2x32<MOD1,MOD2> A;
  if (p == 1) { A.val1_ = val1_; A.val2_ = val2_; return A; }
  A.val1_ = 1;
  A.val2_ = 1;
  if (p == 0) {  return A;  }

  Field_uint2x32<MOD1,MOD2> r(*this);
  //r.val_ = val_;

  unsigned int pw = p;
  for (;;) {
    if (pw & 1)  A *= r;
    pw >>= 1;
    if (!pw) break;
    r.sqr();
  };
  return A;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2>& Field_uint2x32<MOD1,MOD2>::sqr() {
  uint64_t q1 = val1_;
  q1 *= val1_;
  val1_ = q1 % MOD1;

  uint64_t q2 = val2_;
  q2 *= val2_;
  val2_ = q2 % MOD2;

  return *this;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2>__forceinline
Field_uint2x32<MOD1,MOD2>& Field_uint2x32<MOD1,MOD2>::invert() {
  ASSERT1(hasInverse())(*this)(val1_)(val2_).msg("Has no inverse");
  val1_ = ModOp<uint32_t>::inv<Field_uint2x32::ubase_type>(val1_,MOD1);
  val2_ = ModOp<uint32_t>::inv<Field_uint2x32::ubase_type>(val2_,MOD2);
  return *this;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2>&
Field_uint2x32<MOD1,MOD2>::powerBy(unsigned int p) {
  if (p == 1) { return *this; }
  if (p == 0) { val1_ = 1;  val2_ = 1;  return *this;  }

  Field_uint2x32<MOD1,MOD2> r(*this);
  val1_ = 1; val2_ = 1;
  for (;;) {
    if (p & 1) operator*=(r);
    p >>= 1;
    if (!p)  break;
    r.sqr();
  }
  return *this;
}
//**************************************************************************************************
// Shanks-Tonelli algorithm solves x^2 == val % MOD
//  if MOD == 3 % 4 then  x == val^((MOD+1)/4) % MOD
//  else
//   1. Let MOD-1 = 2^FOURIER_ORDER*ODDMUL
//   2. Select UWIT such that there is no x, such that x^2 == UWIT % MOD
//   3. let b = val^((ODDMUL+1)/2) % MOD
//   4. let GPOW = UWIT^ODDMUL % MOD
//   5. Loop
//     1. Find lowest i, 0 <= i < FOURIER_ORDER such that
//        (b^2 val^(-1))^(2*i) == 1 % MOD
//        This can be done by stating with b^2 val^(-1) and squaring until result is 1
//     2. If i = 0 return b
//     3. Otherwise,
//        let b' = b GPOW^(2^(FOURIER_ORDER-i-1)) % MOD and repeat from step 4 with new b'

template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2> Field_uint2x32<MOD1,MOD2>::getSqrt() const {
  Field_uint2x32<MOD1,MOD2> x;
  x.val1_ = ModOp<Field_uint2x32::ubase_type>::sqrt(val1_, MOD1);
  x.val2_ = ModOp<Field_uint2x32::ubase_type>::sqrt(val2_, MOD2);
  if (val1_ && !x.val1_) x.val2_ = 0;
  else if (val2_ && !x.val2_) x.val1_ = 0;
  return x;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
void Field_uint2x32<MOD1,MOD2>::multiply(
    Field_uint2x32<MOD1,MOD2>* dest,
    const Field_uint2x32<MOD1,MOD2>& src1,
    const Field_uint2x32<MOD1,MOD2>& src2) {
  uint64_t q = src1.val1_;
  BOOST_STATIC_ASSERT(sizeof(q) >= (sizeof(src1.val1_)<<1));
  q *= src2.val1_;
  dest->val1_ = q % MOD1;
  uint64_t q2 = src1.val2_;
  q2 *= src2.val2_;
  dest->val2_ = q2 % MOD2;

}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
void Field_uint2x32<MOD1,MOD2>::add(Field_uint2x32<MOD1,MOD2>* dest,
                                  const Field_uint2x32<MOD1,MOD2>& src1,
                                  const Field_uint2x32<MOD1,MOD2>& src2) {
  dest->val1_ = MOD1 - src1.val1_;
  if (dest->val1_ > src2.val1_) {
    dest->val1_ -= src2.val1_;
    dest->val1_ = MOD1 - dest->val1_;
  } else {
    dest->val1_ = src2.val1_ - dest->val1_;
  }

  dest->val2_ = MOD2 - src1.val2_;
  if (dest->val2_ > src2.val2_) {
    dest->val2_ -= src2.val2_;
    dest->val2_ = MOD2 - dest->val2_;
  } else {
    dest->val2_ = src2.val2_ - dest->val2_;
  }

}
//**************************************************************************************************
template <uint32_t MOD1, uint32_t MOD2> __forceinline
void Field_uint2x32<MOD1,MOD2>::subtract(Field_uint2x32<MOD1,MOD2>* dest,
                                         const Field_uint2x32<MOD1,MOD2>& src1,
                                         const Field_uint2x32<MOD1,MOD2>& src2) {
  if (src1.val1_ >= src2.val1_) {
    dest->val1_ = src1.val1_ - src2.val1_;
  } else {
    dest->val1_ = MOD1 - (src2.val1_ - src1.val1_);
  }
  if (src1.val2_ >= src2.val2_) {
    dest->val2_ = src1.val2_ - src2.val2_;
  } else {
    dest->val2_ = MOD2 - (src2.val2_ - src1.val2_);
  }

}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
void Field_uint2x32<MOD1,MOD2>::neg(Field_uint2x32<MOD1,MOD2>* dest,
                                    const Field_uint2x32<MOD1,MOD2>& src) {
  if (src.val1_) {
    dest->val1_ = MOD1 - src.val1_;
  } else {
    dest->val1_ = 0;
  }
  if (src.val2_) {
    dest->val2_ = MOD2 - src.val2_;
  } else {
    dest->val2_ = 0;
  }
}


//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline void
Field_uint2x32<MOD1,MOD2>::pow(Field_uint2x32<MOD1, MOD2>* dest,
                               const Field_uint2x32<MOD1, MOD2>& src,
                               unsigned int p) {
  if (p == 1) {
    dest->val1_ = src.val1_;
    dest->val2_ = src.val2_;
    return;
  }
  if (p == 0) {
    dest->val1_ = 1;
    dest->val2_ = 1;
    return;
  }
  Field_uint2x32<MOD1,MOD2> r;
  r.val1_ = src.val1_;
  r.val2_ = src.val2_;
  dest->val1_ = 1;
  dest->val2_ = 1;
  unsigned int pw = p;
  for (;;) {
    if (pw & 1)
      *dest *= r;
    pw >>= 1;
    if (!pw)
      break;
    r.sqr();
  }
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2>* Field_uint2x32<MOD1,MOD2>::alloc(unsigned int size) {
  return new Field_uint2x32<MOD1,MOD2>[size];
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
void Field_uint2x32<MOD1,MOD2>::dealloc(Field_uint2x32<MOD1,MOD2>* p) {
  delete[] p;
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2>* Field_uint2x32<MOD1,MOD2>::allocSq(unsigned int size) {
  return new Field_uint2x32<MOD1,MOD2>[size*size];
}
//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2> __forceinline
Field_uint2x32<MOD1,MOD2>* Field_uint2x32<MOD1,MOD2>::allocSq2p(unsigned int size) {
  const unsigned int szLog = math::intlog(size);
  return new Field_uint2x32<MOD1,MOD2>[static_cast<unsigned int>(1)<<(szLog<<1)];
}
//**************************************************************************************************

template <uint32_t MOD1,uint32_t MOD2>
Field_uint2x32<MOD1,MOD2>
Field_uint2x32<MOD1,MOD2>::primRoots[
  min<Field_uint2x32<MOD1,MOD2>::fourier1::ORDER,
      Field_uint2x32<MOD1,MOD2>::fourier2::ORDER
     >::value
];

template <uint32_t MOD1,uint32_t MOD2>
Field_uint2x32<MOD1,MOD2>
Field_uint2x32<MOD1,MOD2>::primRootInvs[
  min<Field_uint2x32<MOD1,MOD2>::fourier1::ORDER,
      Field_uint2x32<MOD1,MOD2>::fourier2::ORDER
     >::value
];

template <uint32_t MOD1,uint32_t MOD2>
bool Field_uint2x32<MOD1,MOD2>::init() {
  if (!Field_uint2x32<MOD1,MOD2>::initialized) {
    computePrimitiveRoots<Field_uint2x32<MOD1,MOD2> >(
        Field_uint2x32<MOD1,MOD2>(-1),
        primRoots,
        primRootInvs,
        min<fourier1::ORDER,fourier2::ORDER>::value);
  }
  return true;
}

template <uint32_t MOD1,uint32_t MOD2>
bool Field_uint2x32<MOD1,MOD2>::initialized = Field_uint2x32<MOD1,MOD2>::init();

// @todo use boost format
#include <cstdio> // for snprintf

template <uint32_t MOD1,uint32_t MOD2>
int Field_uint2x32<MOD1,MOD2>::toDebugString(char* buffer) const {

#ifdef _MSC_VER
  return sprintf_s
#else
  return snprintf
#endif
  (buffer, getMaxStringRepLength(), "%ld,%ld",
      (val1_ <= POS1) ? static_cast<long>(val1_) : -static_cast<long>(MOD1 - val1_),
      (val2_ <= POS2) ? static_cast<long>(val2_) : -static_cast<long>(MOD2 - val2_));
}

//**************************************************************************************************
template <uint32_t MOD1,uint32_t MOD2>
int Field_uint2x32<MOD1,MOD2>::toString(char* buffer) const {

  std::pair<ubase_type,ubase_type> radsp = mixedRadix();
  bool negated = (radsp.second >= MOD2 - radsp.second);

  if (negated) {
    radsp = operator-().mixedRadix();
  }

  ubase_type primes[] = {MOD1, MOD2};
  ubase_type rads[] = {0, 0};
  rads[0] = radsp.first;
  rads[1] = radsp.second;

  const ubase_type radix = 10;
  //std::string digits;
  int pos = 0;
  unsigned int highRad = 2;
  uint64_t quo;

  while (highRad > 0) {
    quo = 0;
    for (unsigned int i = highRad-1; i > 0; i--) {
      quo += rads[i];
      rads[i] = static_cast<ubase_type>(quo / radix);
      quo %= radix;
      quo *= primes[i-1];
    }
    quo += rads[0];
    rads[0] = static_cast<ubase_type> (quo / radix);

    buffer[pos++] = '0' + quo % radix;
    while ((highRad > 0) && (rads[highRad - 1] == 0)) highRad--;
  }

  // Do not negate 0
  if (negated) {
    buffer[pos++] = '-';
  }
  buffer[pos] = '\0';
  std::reverse(buffer, buffer+pos);
  return pos;
}

}  // namespace field
}  // namespace math
