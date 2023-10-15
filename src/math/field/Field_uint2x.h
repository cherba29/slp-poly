#ifndef NTRP_MATH_FIELD_UINT2X_H_
#define NTRP_MATH_FIELD_UINT2X_H_

/**
 * @file: Field_uint2x.h interface for the Field_uint2x class.
 */

#include "math/Base.h"
#include "math/util/random.h"
#include "math/util/static.h"

#include <boost/scoped_array.hpp>

namespace math {
namespace field {
/**
 * @brief Finite field of statically specified modulo.
 * Use these primes for fast fourier transform,
 *   to which max input size is limited by fourier order 2^ORD
 */

template <typename UIntType, UIntType MOD1, UIntType MOD2>
class Field_uint2x {
 public:
  typedef typename TypeInfo<UIntType>::signed_type sbase_type;
  typedef typename TypeInfo<UIntType>::unsigned_type ubase_type;

  //### Initializers ########################################################
  /**
   * @brief Default constructor, constructs without initialization
   */
  __forceinline Field_uint2x() {}

  /**
   * @brief Initialize field from an unsigned integer,
   *        internally will reduce modulo.
   * @param v unsigned integer which can be larger than modulus
   * @see setTo
   */
  //__forceinline explicit Field_uint2x(ubase_type v);

  /**
   * @brief initalize field from signed integer.
   * @see setTo
   */
  __forceinline explicit Field_uint2x(sbase_type v);

  /*
   * @brief Copy constructor
   */
  //__forceinline Field_uint(const Field_uint& right);

  /*
   * @brief Destructor
   */
  //__forceinline ~Field_uint() { }

  /**
   * @brief Set to statically know constant value.
   * @see operator=(int)
   */
  template <sbase_type VAL>
  __forceinline void setTo() {
    val1_ = mod<VAL, MOD1>::VALUE;
    val2_ = mod<VAL, MOD2>::VALUE;
  }

  /**
   * @brief Set field to dynamic integer value
   * @see operator=(int), template <int VAL> setTo()
   */
  __forceinline void setTo(sbase_type v);

  /**
   * @brief Inititialize this instance to random field element.
   */
  __forceinline Field_uint2x& random();

  /**
   * @brief Inititialize this instance to random field element, for which
   * hasInverse is true.
   * @see hasInverse, random
   */
  __forceinline Field_uint2x& invertableRandom();

  //### Property checks ########################################################
  /**
   * @brief Returns true if this instance equivalent to given integer.
   */
  template <sbase_type VAL>
  __forceinline bool is() const {
    return val1_ == mod<VAL, MOD1>::VALUE && val2_ == mod<VAL, MOD2>::VALUE;
  }

  /**
   * @brief Returns true if this instance equivalent to given integer.
   */
  __forceinline bool is(sbase_type val) const {
    return (val >= 0) ? ((val1_ == val % MOD1) && (val2_ == val % MOD2))
                      : ((val1_ == (MOD1 - ((-val) % MOD1))) &&
                         (val2_ == (MOD2 - ((-val) % MOD2))));
  }

  /**
   * @brief Returns true if value is smaller or equal to size()/2, also 0
   * @see size()
   * @see isNegative()
   */
  __forceinline bool isPositive() const {
    std::pair<ubase_type, ubase_type> rads = mixedRadix();
    return (rads.first < MOD1 - rads.first);
  }

  /**
   * @brief Returns true if value is larger than size()/2
   * @see size()
   * @see isPositive()
   */
  __forceinline bool isNegative() const {
    std::pair<ubase_type, ubase_type> rads = mixedRadix();
    return (rads.first >= MOD1 - rads.first);
  }

  /**
   * @brief Does this element has multiplicative inverse.
   * For a field only 0 does not have it.
   * For a domain many non-zero elements will have no inverse.
   */
  __forceinline bool hasInverse() const { return val1_ && val2_; }

  //### Modifiers ##############################################################

  /**
   * @brief Turn this instance to its additive inverse
   * @see operator-()
   * @see operator-=(Field)
   */
  __forceinline Field_uint2x& negate() {
    if (val1_) val1_ = MOD1 - val1_;
    if (val2_) val2_ = MOD2 - val2_;
    return *this;
  }

  /**
   * @brief Swap values of two fields elements
   */
  __forceinline void swap(Field_uint2x& f) {
    ubase_type tmp = val1_;
    val1_ = f.val1_;
    f.val1_ = tmp;
    tmp = val2_;
    val2_ = f.val2_;
    f.val2_ = tmp;
  }

  /**
   * @brief set field element to its multiplicative inverse
   *        The algorithm is based on Euclidian GCD algorithm
   */
  __forceinline Field_uint2x& invert();

  /**
   *  @brief Squares itself.
   *  @return reference to itself
   */
  __forceinline Field_uint2x& sqr();

  /**
   * @brief Raise this instance to given power.
   *        Number of steps equal to the order of highest non-zero bit
   *        in the integer.
   * @see getPow
   */
  __forceinline Field_uint2x& powerBy(unsigned int p);

  //** Calculators **********************************************************
  /**
   * @brief Shanks-Tonelli algorithm to find square root of a mod m
   * @return number k for which k*k == f if exists, otherwise 0
   */
  __forceinline Field_uint2x getSqrt() const;

  /**
   * @brief Returns the multiplicative inverse of f.
   * @see inverse()
   */
  __forceinline Field_uint2x getInverse() const {
    Field_uint2x A(*this);
    A.invert();
    return A;
  }

  /**
   * @brief Raise this instance to given power.
   *        Number of steps equal to the order of highest non-zero bit
   *        in the integer.
   * @return this^p
   * @see powerBy
   */
  __forceinline Field_uint2x getPow(unsigned int p) const;

  /**
   * @brief Convert this instance to string representation.
   * @param buffer - output and must be at least
   *                 getMaxStringRepLength characters long
   * @see getMaxStringRepLength
   */
  int toString(char* buffer) const;

  int toDebugString(char* buffer) const;

  //** Operators ************************************************************
  /*
   * @brief Assignment operator. Replaces the current instance with right
   */
  // Field_uint& operator=(const Field_uint& right);

  /**
   * @brief Returns the additive inverse of this instance
   * @see negate
   * @see operator-=()
   */
  __forceinline Field_uint2x operator-() const;

  /**
   * @brief Adds right to this instance.
   */
  __forceinline Field_uint2x& operator+=(const Field_uint2x& right);

  /**
   *  @brief Subtracts right from this instance
   *  @see subtract
   *  @return reference to this instance
   */
  __forceinline Field_uint2x& operator-=(const Field_uint2x& right);

  /**
   * @brief Multiplies this instance by right
   * @return reference to this instance
   */
  __forceinline Field_uint2x& operator*=(const Field_uint2x& m);

  /**
   * @brief Divides this instance by right.
   */
  __forceinline Field_uint2x& operator/=(const Field_uint2x& right) {
    return operator*=(right.getInverse());
  }

  /**
   * @brief Adds this instance with given argument and returns new instance.
   * @return new instance which is sum of this and given
   * @see add
   * @see operator+=
   */
  Field_uint2x operator+(const Field_uint2x& right) const;

  /**
   * @brief Subtract value from this instance returning the difference
   * @see subtract
   */
  Field_uint2x operator-(const Field_uint2x& right) const;

  /**
   * @brief Returns (left * right) % modulus.
   */
  Field_uint2x operator*(const Field_uint2x& right) const;

  /**
   * @brief Returns new instance which is division of this by given instance.
   * @see divide
   */
  Field_uint2x operator/(const Field_uint2x& right) const;

  /**
   * @brief Returns true iff this instance and right are equal.
   *        Identical to equals member function
   * @see operator!=
   */
  bool operator==(const Field_uint2x& right) const;

  /**
   * @brief Returns true iff this instance and right are not equal.
   * @see operator==
   */
  bool operator!=(const Field_uint2x& right) const;

  //#### Static members ########################################################

  /** Maximum string representation for any element of the field */
  static int getMaxStringRepLength() {
    return sizeof(ubase_type) * 2 * 5;
    /* 5 dec digits per byte with +/- with null terminator */
  }
  static int getMaxDebugStringRepLength() { return sizeof(ubase_type) * 2 * 5; }

  /**
   * @brief Returns the largest number this class can represent
   */
  static long double getSize() { return static_cast<long double>(MOD1) * MOD2; }

  /**
   * @brief Initializes the class's random number generator.
   */
  // static void randomInit(unsigned int seed);

  __forceinline static void add(Field_uint2x* dest, const Field_uint2x& src1,
                                const Field_uint2x& src2);

  __forceinline static void subtract(Field_uint2x* dest,
                                     const Field_uint2x& src1,
                                     const Field_uint2x& src2);

  __forceinline static void neg(Field_uint2x* dest, const Field_uint2x& src);

  __forceinline static void multiply(Field_uint2x* dest,
                                     const Field_uint2x& src1,
                                     const Field_uint2x& src2);

  __forceinline static void divide(Field_uint2x* dest, const Field_uint2x& src1,
                                   const Field_uint2x& src2);

  __forceinline static void pow(Field_uint2x* dest, const Field_uint2x& src,
                                unsigned int p);

  __forceinline static Field_uint2x* alloc(unsigned int size);
  __forceinline static void dealloc(Field_uint2x*);

  __forceinline static Field_uint2x* allocSq(unsigned int size);
  __forceinline static Field_uint2x* allocSq2p(unsigned int size);

  __forceinline static const Field_uint2x* getPrimRoots() {
    if (!initialized) init();
    return primRoots;
  }
  __forceinline static const Field_uint2x* getPrimRootInvs() {
    if (!initialized) init();
    return primRootInvs;
  }

  __forceinline static const char* getName() {
    const int BUF_SIZE = 80;
    static char buff[BUF_SIZE] = "Field_uint2x32XX_";
    buff[12] = '0' + ((sizeof(ubase_type) * 8) / 10) % 10;
    buff[13] = '0' + (sizeof(ubase_type) * 8) % 10;
    ubase_type num = MOD1;
    int i = BUF_SIZE - 2;
    while (num > 0) {
      buff[i--] = '0' + (num % 10);
      num /= 10;
    }
    buff[i--] = '_';
    num = MOD2;
    while (num > 0) {
      buff[i--] = '0' + (num % 10);
      num /= 10;
    }

    int j;
    for (j = 15, i++; i < BUF_SIZE - 1; i++, j++) {
      buff[j] = buff[i];
    }
    // buff[j++] = ']';
    buff[j] = 0;
    return buff;
  }

  __forceinline static const char* getId() { return getName(); }

 private:
  /** Values above POS are considered negative, and below positive */
  static const ubase_type POS1 = (MOD1 - 1) / 2;
  static const ubase_type POS2 = (MOD2 - 1) / 2;

  //  static base_type modulus1; /**< nonmodifyable but cannot be declared
  //  const,
  //                                  storing prime in memory location,
  //                                  needed for some asm instructions */
  typedef fourier_order<MOD1> fourier1;
  typedef fourier_order<MOD2> fourier2;
  static Field_uint2x primRoots[min<fourier1::ORDER, fourier2::ORDER>::value];
  static Field_uint2x
      primRootInvs[min<fourier1::ORDER, fourier2::ORDER>::value];

  static bool initialized;
  static bool init();

  ubase_type val1_; /**< actual value in the field */
  ubase_type val2_; /**< actual value in the field */

  std::pair<ubase_type, ubase_type> mixedRadix() const {
    std::pair<ubase_type, ubase_type> rads;
    rads.first = val1_;
    if (val2_ >= rads.first)
      rads.second = val2_ - rads.first;
    else
      rads.second = MOD2 - (rads.first - val2_);
    // @todo: Precompute inverse
    rads.second = ModOp<ubase_type>::mul(
        rads.second, ModOp<ubase_type>::inv(MOD1 % MOD2, MOD2), MOD2);
    return rads;
  }
  // mixmuls[1] = inv(MOD1 % MOD2, MOD2)
  // rads[0] = crt[0];
  // rads[1] = ((crt[1] - crt[0])*mixmul[1]) % MOD2
};
}  // namespace field
}  // namespace math

#include "Field_uint2x-Impl.h"

#endif  // NTRP_MATH_FIELD_UINT2X_H_
