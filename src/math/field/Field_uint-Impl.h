#ifndef NTRP_MATH_FIELD_UINT_H_
#error "Do not include this file. This is used internally by Field_uint.h"
#endif

/**
 * @file Field_uint64-Impl.h Implementation of math::Field_uint64<MOD>
 */

#include "math/util/modular-x86.h"

#include <cstdio> // for snprintf

namespace math {
namespace field {

// Constructor for base type
//template <typename UIntType, UIntType MOD> __forceinline
//Field_uint<UIntType,MOD>::Field_uint(typename Field_uint<UIntType,MOD>::ubase_type v) {
//  val_ = v % MOD;
//}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>::Field_uint(typename Field_uint<UIntType,MOD>::sbase_type v) {
  val_ = (v < 0) ? (MOD - ((-v)%MOD)) : v % MOD;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::setTo(typename Field_uint<UIntType,MOD>::sbase_type v) {
  if (v < 0) {
    if (static_cast<uint64_t> (-v) < MOD)
      val_ = MOD + v;
    else
      val_ = MOD - (v + MOD);
  } else {
    if (static_cast<uint64_t> (v) < MOD)
      val_ = v;
    else
      val_ = v - MOD;
  }
}
//*****************************************************************************
template <typename UIntType, UIntType MOD>__forceinline
Field_uint<UIntType,MOD>&
Field_uint<UIntType,MOD>::operator+=(const Field_uint<UIntType,MOD>& right) {
  val_ = MOD - val_; // negate
  if (val_ > right.val_) {
	  val_ -= right.val_;
	  val_ = MOD - val_; // negate
  } else {
	  val_ = right.val_ - val_;
  }
  return *this;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>
Field_uint<UIntType,MOD>::operator+(const Field_uint<UIntType,MOD>& right) const {
  Field_uint<UIntType,MOD> A;
  A.val_ = MOD - val_;
  if (A.val_ > right.val_) {
    A.val_ -= right.val_;
    A.val_ = MOD - A.val_;
  } else {
    A.val_ = right.val_ - A.val_;
  }
  return A;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>
Field_uint<UIntType,MOD>::operator-(const Field_uint<UIntType,MOD>& right) const {
  Field_uint<UIntType,MOD> A;
  if (val_ >= right.val_)
    A.val_ = val_ - right.val_;
  else
    A.val_ = MOD - (right.val_ - val_);
  return A;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>
Field_uint<UIntType,MOD>::operator*(const Field_uint<UIntType,MOD>& right) const {
    typename TypeInfo<UIntType>::unsigned2x_type q = val_;
    q *= right.val_;
    Field_uint<UIntType,MOD> r;
    r.val_ = q % MOD;
    return r;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>
Field_uint<UIntType,MOD>::operator/(const Field_uint<UIntType,MOD>& right) const {
  return ((right.getInverse()) *= *this);
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>&
Field_uint<UIntType,MOD>::random() {
  val_ = math::random<typename Field_uint::ubase_type>::get() % MOD;
  return *this;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>& Field_uint<UIntType,MOD>::invertableRandom() {
  do { random(); } while (is<0>()); return *this;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::divide(Field_uint<UIntType,MOD>* dest,
            const Field_uint<UIntType,MOD>& src1, const Field_uint<UIntType,MOD>& src2) {
   Field_uint<UIntType,MOD>::multiply(dest, src1, src2.getInverse());
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
bool Field_uint<UIntType,MOD>::operator==(const Field_uint<UIntType,MOD>& right) const {
  return val_ == right.val_;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
bool Field_uint<UIntType,MOD>::operator!=(const Field_uint<UIntType,MOD>& right) const {
  return val_ != right.val_;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>
Field_uint<UIntType,MOD>::operator-() const {
  Field_uint<UIntType,MOD> r;
	if (val_)	r.val_ = MOD - val_;
  else	r.val_ = 0;
  return r;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>&
Field_uint<UIntType,MOD>::operator-=(const Field_uint<UIntType,MOD>& right) {
  if (val_ >= right.val_)	val_ -= right.val_;
  else val_ = MOD - (right.val_ - val_);
  return *this;
}

//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>&
Field_uint<UIntType,MOD>::operator*=(const Field_uint<UIntType,MOD>& m) {
  typename TypeInfo<UIntType>::unsigned2x_type q = val_;
  q *= m.val_;
  val_ = q % MOD;
	return *this;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>
Field_uint<UIntType,MOD>::getPow(ubase_type p) const {
  Field_uint<UIntType,MOD> A;
  if (p == 1) { A.val_ = val_; return A; }
  A.val_ = 1;
  if (p == 0) {	return A;	}

  Field_uint<UIntType,MOD> r(*this);
  //r.val_ = val_;

  ubase_type pw = p;
  for (;;) {
    if (pw & 1)	A *= r;
    pw >>= 1;
    if (!pw) break;
    r.sqr();
  };
  return A;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>& Field_uint<UIntType,MOD>::sqr() {
  typename TypeInfo<UIntType>::unsigned2x_type q = val_;
  q *= val_;
  val_ = q % MOD;
	return *this;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD>__forceinline
Field_uint<UIntType,MOD>& Field_uint<UIntType,MOD>::invert() {
  ASSERT0(val_ != 0).msg("Can't invert zero element");
	ubase_type c = MOD;
	ubase_type d = val_;
	sbase_type c2 = 0;
	sbase_type d2 = 1;
	ubase_type q, r;
	sbase_type  r2;
	while (d) {
		q = c / d;
		r = c % d;
		r2 = c2 - q * d2;
		c = d;
		c2 = d2;
		d = r;
		d2 = r2;
	}
	val_ = (c2 < 0)?(MOD + c2):c2;

	ASSERT1(val_ != 0).msg("Inversion resulted in zero");
	return *this;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>& Field_uint<UIntType,MOD>::powerBy(unsigned int p) {
  if (p == 1) {	return *this;	}
  if (p == 0) { val_ = 1;	return *this;	}

  Field_uint<UIntType,MOD> r(*this);
  val_ = 1;
  for (;;) {
    if (p & 1) operator*=(r);
    p >>= 1;
    if (!p)	break;
    r.sqr();
  }
  return *this;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>
Field_uint<UIntType,MOD>::getSqrt() const {
  Field_uint<UIntType,MOD> a;
  a.val_ = ModOp<UIntType>::sqrt(val_,MOD);
  return a;
}

//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::multiply(Field_uint<UIntType,MOD>* dest,
                                  const Field_uint<UIntType,MOD>& src1,
                                  const Field_uint<UIntType,MOD>& src2) {
  typename TypeInfo<UIntType>::unsigned2x_type q = src1.val_;
  BOOST_STATIC_ASSERT(sizeof(q) >= (sizeof(src1.val_)<<1));
  q *= src2.val_;
  dest->val_ = q % MOD;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::multiply(Field_uint<UIntType,MOD>* dest,
                                  const Field_uint<UIntType,MOD>* src1,
                                  const Field_uint<UIntType,MOD>* src2, int n) {
  Field_uint<UIntType,MOD>* last = dest + n;
  for (; dest != last; ++dest, ++src1, ++src2) {
    typename TypeInfo<UIntType>::unsigned2x_type q = src1->val_;
    q *= src2->val_;
    dest->val_ = q % MOD;
  }
}

//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::multiply(Field_uint<UIntType,MOD>* dest,
                                  const Field_uint<UIntType,MOD>* src1,
                                  const Field_uint<UIntType,MOD>& src2, int n) {
  Field_uint<UIntType,MOD>* last = dest + n;
  for (; dest != last; ++dest, ++src1) {
    typename TypeInfo<UIntType>::unsigned2x_type q = src1->val_;
    q *= src2.val_;
    dest->val_ = q % MOD;
  }
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::multiply(Field_uint<UIntType,MOD>* dest,
                                  const Field_uint<UIntType,MOD>* src1,
                                  int n) {
  Field_uint<UIntType,MOD>* last = dest + n;
  for (; dest != last; ++dest, ++src1) {
    typename TypeInfo<UIntType>::unsigned2x_type q = dest->val_;
    q *= src1->val_;
    dest->val_ = q % MOD;
  }
}

//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::add(Field_uint<UIntType,MOD>* dest,
                                   const Field_uint<UIntType,MOD>& src1,
                                   const Field_uint<UIntType,MOD>& src2) {
	dest->val_ = MOD - src1.val_;
	if (dest->val_ > src2.val_) {
		dest->val_ -= src2.val_;
		dest->val_ = MOD - dest->val_;
	} else {
		dest->val_ = src2.val_ - dest->val_;
	}
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::add(Field_uint<UIntType,MOD>* dest,
                                   const Field_uint<UIntType,MOD>* src1,
                                   const Field_uint<UIntType,MOD>* src2, int n) {
  Field_uint<UIntType,MOD>* last = dest + n;
	for (; dest != last; ++dest, ++src1, ++src2) {
    dest->val_ = MOD - src1->val_;
	  if (dest->val_ > src2->val_) {
		  dest->val_ -= src2->val_;
		  dest->val_ = MOD - dest->val_;
	  } else {
		  dest->val_ = src2->val_ - dest->val_;
	  }
  }
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::add(Field_uint<UIntType,MOD>* dest,
                                   const Field_uint<UIntType,MOD>* src1,
                                   const Field_uint<UIntType,MOD>& src2, int n) {
  Field_uint<UIntType,MOD>* last = dest + n;
	for (; dest != last; ++dest, ++src1) {
    dest->val_ = MOD - src1->val_;
	  if (dest->val_ > src2.val_) {
		  dest->val_ -= src2.val_;
		  dest->val_ = MOD - dest->val_;
	  } else {
		  dest->val_ = src2.val_ - dest->val_;
	  }
  }
}
//*****************************************************************************
//template <typename UIntType, UIntType MOD> __forceinline
//void Field_uint<UIntType,MOD>::add(Field_uint<UIntType,MOD>* dest,
//                                   const Field_uint<UIntType,MOD>* src1,
//                                   int n) {
//  Field_uint<UIntType,MOD>* last = dest + n;
//	for (; dest != last; ++dest, ++src1) {
//    dest->val_ = MOD - val_; // negate
//    if (dest->val_ > src->val_) {
//	    dest->val_ -= src1->val_;
//	    dest->val_ = MOD - val_; // negate
//    } else {
//	    dest->val_ = src1->val_ - val_;
//    }
//  }
//}

//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::subtract(Field_uint* dest,
                                        const Field_uint& src1,
                                        const Field_uint& src2) {
  if (src1.val_ >= src2.val_) {
	  dest->val_ = src1.val_ - src2.val_;
  } else {
	  dest->val_ = MOD - (src2.val_ - src1.val_);
  }
}
//*****************************************************************************
//template <typename UIntType, UIntType MOD> __forceinline
//void Field_uint<UIntType,MOD>::subtract(Field_uint* dest,
//                                        const Field_uint* src1,
//                                        const Field_uint* src2, int n) {
//  Field_uint<UIntType,MOD>* last = dest + n;
//	for (; dest != last; ++dest, ++src1, ++src2) {
//    if (src1->val_ >= src2->val_) {
//	    dest->val_ = src1->val_ - src2->val_;
//    } else {
//	    dest->val_ = MOD - (src2->val_ - src1->val_);
//    }
//  }
//}

//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::neg(Field_uint<UIntType,MOD>* dest,
                                   const Field_uint<UIntType,MOD>& src) {
  if (src.val_) {
		dest->val_ = MOD - src.val_;
  } else {
    dest->val_ = 0;
  }
}


//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::pow(Field_uint<UIntType,MOD>* dest,
                                   const Field_uint<UIntType,MOD>& src,
                                   unsigned int p) {
		if (p == 1) {
			dest->val_ = src.val_;
      return;
		}
		if (p == 0) {
			dest->val_ = 1;
      return;
		}
		Field_uint<UIntType,MOD> r;
		r.val_ = src.val_;
		dest->val_ = 1;
		unsigned int pw = p;
		for (;;) {
			if (pw & 1)
				*dest *= r;
			pw >>= 1;
			if (!pw)
				break;
			r.sqr();
		};
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>* Field_uint<UIntType,MOD>::alloc(unsigned int size) {
  return new Field_uint[size];
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
void Field_uint<UIntType,MOD>::dealloc(Field_uint* p) {
  delete[] p;
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>*
Field_uint<UIntType,MOD>::allocSq(unsigned int size) {
  return new Field_uint[size*size];
}
//*****************************************************************************
template <typename UIntType, UIntType MOD> __forceinline
Field_uint<UIntType,MOD>*
Field_uint<UIntType,MOD>::allocSq2p(unsigned int size) {
  const unsigned int szLog = math::intlog(size);
  return new Field_uint[static_cast<unsigned int>(1)<<(szLog<<1)];
}
//*****************************************************************************

//template <typename UIntType, UIntType MOD>
//typename Field_uint<UIntType,MOD>::ubase_type
//Field_uint<UIntType,MOD>::modulus = MOD;

template <typename UIntType, UIntType MOD>
Field_uint<UIntType,MOD>
Field_uint<UIntType,MOD>::primRoots[
  //fourier_order<Field_uint<UIntType,MOD>::sbase_type,MOD>::ORDER
  sizeof(Field_uint<UIntType,MOD>::sbase_type)*8
];

template <typename UIntType, UIntType MOD>
Field_uint<UIntType,MOD>
Field_uint<UIntType,MOD>::primRootInvs[
  //fourier_order<Field_uint<UIntType,MOD>::sbase_type,MOD>::ORDER
  sizeof(Field_uint<UIntType,MOD>::sbase_type)*8
];

template <typename UIntType, UIntType MOD>
bool Field_uint<UIntType,MOD>::init() {
  if (!Field_uint<UIntType,MOD>::initialized) {
    computePrimitiveRoots<Field_uint<UIntType,MOD> >(
        Field_uint<UIntType,MOD>(-1),
        primRoots,
        primRootInvs,
        sizeof(Field_uint<UIntType,MOD>::sbase_type)*8
        //fourier_order<Field_uint<UIntType,MOD>::sbase_type,MOD>::ORDER
        );
  }
  return true;
}

template <typename UIntType, UIntType MOD>
bool Field_uint<UIntType,MOD>::initialized = Field_uint<UIntType,MOD>::init();

template <typename UIntType, UIntType MOD>
void Field_uint<UIntType,MOD>::toString(char* buffer) const {
#ifdef __LP64__
   const char* formatStr = "%lld";
#else
   const char* formatStr = "%ld";
#endif

#ifdef _MSC_VER
  sprintf_s
#else
  snprintf
#endif
  (buffer, getMaxStringRepLength(), formatStr,
      (val_ <= POS) ? static_cast<long>(val_) : -static_cast<long>(MOD-val_));
}

}  // namespace field
}  // namespace math
