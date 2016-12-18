#ifndef NTRP_MATH_FIELD_UINT2X32_H_
#define NTRP_MATH_FIELD_UINT2X32_H_

/**
 * @file: Field_uint2x32.h interface for the Field_uint2x32 class.
 */

#include <math/Base.h>
#include <math/util/random.h> 
#include <math/util/static.h>
#include <boost/scoped_array.hpp>

namespace math {
namespace field {
/**
 * @brief Finite field of statically specified modulo.
 * Use these primes for fast fourier transform,
 *   to which max input size is limited by fourier order 2^ORD
 *  1. 30 - 3221225473 = 2^30 * 3 + 1
 *  2. 28 - 3489660929 = 2^28 * 13 + 1
 *  3. 27 - 3892314113 = 2^27 * 29 + 1
 *  4. 27 - 2281701377 = 2^27 * 17 + 1
 *  5. 27 - 2013265921 = 2^27 * 15 + 1
 *  6. 26 - 2885681153 = 2^26 * 43 + 1
 *  7. 26 - 2483027969 = 2^26 * 27 + 1
 *  8. 26 - 1811939329 = 2^26 * 9 + 1
 *  9. 26 - 469762049  = 2^26 * 7 + 1
 * 10. 25 - 4194304001 = 2^25 * 125 + 1
 * 11. 25 - 2717908993 = 2^25 * 81 + 1
 * 12. 25 - 2113929217 = 2^25 * 63 + 1
 * ...
 * This class is optimized for arithmetic modulo hardcoded 32-bit prime.
 * It overloads all arithmetic operations and can be instantiated from integers
 * and strings.
 */

template <uint32_t MOD1,uint32_t MOD2>
class Field_uint2x32 {
public:
  typedef typename TypeInfo<uint32_t>::signed_type sbase_type;
  typedef typename TypeInfo<uint32_t>::unsigned_type ubase_type;

  //### Initializers ###########################################################
  /**
   * @brief Default constructor, constructs without initialization
   */
  __forceinline Field_uint2x32() { }

  /**
   * @brief Initialize field from an unsigned integer,
   *        internally will reduce modulo.
   * @param v unsigned integer which can be larger than modulus
   * @see setTo
   */
  __forceinline explicit Field_uint2x32(ubase_type v);

  /**
   * @brief initalize field from signed integer.
   * @see setTo
   */
  __forceinline explicit Field_uint2x32(sbase_type v);

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
  template <int VAL>
  __forceinline void setTo() {
    val1_ = mod<VAL,MOD1>::VALUE;
    val2_ = mod<VAL,MOD2>::VALUE;
  }

  /**
   * @brief Set field to dynamic integer value
   * @see operator=(int), template <int VAL> setTo()
   */
  __forceinline void setTo(sbase_type v);

  /**
   * @brief Inititialize this instance to random field element.
   */
  __forceinline Field_uint2x32& random();

  /**
   * @brief Inititialize this instance to random field element, for which
   * hasInverse is true.
   * @see hasInverse, random
   */
  __forceinline Field_uint2x32& invertableRandom();

  //### Property checks ########################################################
  /**
   * @brief Returns true if this instance equivalent to given integer.
   */
  template <int VAL> __forceinline
  bool is() const {
    return val1_ == mod<VAL,MOD1>::VALUE && val2_ == mod<VAL,MOD2>::VALUE;
  }

  /**
   * @brief Returns true if this instance equivalent to given integer.
   */
  __forceinline
  bool is(sbase_type val) const {
    return (val>=0)
        ? ((val1_ == val % MOD1) && (val2_ == val % MOD2))
        : ((val1_ == (MOD1 - ((-val) % MOD1)))
            && (val2_ == (MOD2 - ((-val) % MOD2))));
  }

  /**
   * @brief Returns true if value is smaller or equal to size()/2, also 0
   * @see size()
   * @see isNegative()
   */
  __forceinline bool isPositive() const {
    std::pair<ubase_type,ubase_type> rads = mixedRadix();
    return rads.first < MOD1 - rads.first;
   }

  /**
   * @brief Returns true if value is larger than size()/2
   * @see size()
   * @see isPositive()
   */
  __forceinline bool isNegative() const {
    std::pair<ubase_type,ubase_type> rads = mixedRadix();
    return (rads.first >= MOD1 - rads.first);
  }

  /**
   * @brief Does this element has multiplicative inverse.
   * For a field only 0 does not have it.
   * For a domain many non-zero elements will have no inverse.
   */
  __forceinline bool hasInverse() const {	return val1_ && val2_; }

  //### Modifiers ##############################################################

  /**
   * @brief Turn this instance to its additive inverse
   * @see operator-()
   * @see operator-=(Field)
   */
  __forceinline Field_uint2x32<MOD1,MOD2>& negate() {
    if (val1_)	val1_ = MOD1 - val1_;
    if (val2_)	val2_ = MOD2 - val2_;
    return *this;
  }

  /**
   * @brief Swap values of two fields elements
   */
  __forceinline void swap(Field_uint2x32& f) { 
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
  __forceinline Field_uint2x32<MOD1,MOD2>& invert();

  /**
   *  @brief Squares itself.
   *  @return reference to itself
   */
  __forceinline Field_uint2x32<MOD1,MOD2>& sqr();

  /**
   * @brief Raise this instance to given power.
   *        Number of steps equal to the order of highest non-zero bit
   *        in the integer.
   * @see getPow
   */
  __forceinline Field_uint2x32<MOD1,MOD2>& powerBy(unsigned int p);

  //** Calculators **********************************************************
  /**
   * @brief Shanks-Tonelli algorithm to find square root of a mod m
   * @return number k for which k*k == f if exists, otherwise 0
   */
  __forceinline Field_uint2x32 getSqrt() const;

  /**
   * @brief Returns the multiplicative inverse of f.
   * @see inverse()
   */
  __forceinline Field_uint2x32 getInverse() const {
    Field_uint2x32 A(*this);
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
  __forceinline Field_uint2x32 getPow(unsigned int p) const;

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
  __forceinline Field_uint2x32 operator-() const;

  /**
   * @brief Adds right to this instance.
   */
  __forceinline Field_uint2x32& operator+=(const Field_uint2x32& right);

  /**
   *  @brief Subtracts right from this instance
   *  @see subtract
   *  @return reference to this instance
   */
  __forceinline Field_uint2x32& operator-=(const Field_uint2x32& right);

  /**
   * @brief Multiplies this instance by right
   * @return reference to this instance
   */
  __forceinline Field_uint2x32& operator*=(const Field_uint2x32& m);

  /**
   * @brief Divides this instance by right.
   */
  __forceinline Field_uint2x32& operator/=(const Field_uint2x32& right) {
    return operator*=(right.getInverse());
  }

  /**
   * @brief Adds this instance with given argument and returns new instance.
   * @return new instance which is sum of this and given
   * @see add
   * @see operator+=
   */
  template <uint32_t M1,uint32_t M2>
  friend Field_uint2x32<M1,M2> operator+(const Field_uint2x32<M1,M2>& left,
                                         const Field_uint2x32<M1,M2>& right);

  /**
   * @brief Subtract value from this instance returning the difference
   * @see subtract
   */
  template <uint32_t M1,uint32_t M2>
  friend Field_uint2x32<M1,M2> operator-(const Field_uint2x32<M1,M2>& left,
                                         const Field_uint2x32<M1,M2>& right);

  /**
   * @brief Returns (left * right) % modulus.
   */
  template <uint32_t M1,uint32_t M2>
  friend Field_uint2x32<M1,M2> operator*(const Field_uint2x32<M1,M2>& left,
                                         const Field_uint2x32<M1,M2>& right);

  /**
   * @brief Returns new instance which is division of this by given instance.
   * @see divide
   */
  template <uint32_t M1,uint32_t M2>
  friend Field_uint2x32<M1,M2> operator/(const Field_uint2x32<M1,M2>& left,
                                         const Field_uint2x32<M1,M2>& right);

  /**
   * @brief Returns true iff this instance and right are equal.
   *        Identical to equals member function
   * @see operator!=
   */
  template <uint32_t M1,uint32_t M2>
  friend bool operator==(const Field_uint2x32<M1,M2>& left,
                         const Field_uint2x32<M1,M2>& right);

  /**
   * @brief Returns true iff this instance and right are not equal.
   * @see operator==
   */
  template <uint32_t M1,uint32_t M2>
  friend bool operator!=(const Field_uint2x32<M1,M2>& left,
                         const Field_uint2x32<M1,M2>& right);

  //#### Static members ########################################################

  /** Maximum string representation for any element of the field */
  static int getMaxStringRepLength() {
    return sizeof(ubase_type)*2*5;
      /* 5 dec digits per byte with +/- with null terminator */
  }
  static int getMaxDebugStringRepLength() { return sizeof(ubase_type)*2*5; }

  /**
   * @brief Returns the largest number this class can represent
   */
  static long double getSize() { return static_cast<long double>(MOD1)*MOD2; }

  /**
   * @brief Initializes the class's random number generator.
   */
  //static void randomInit(unsigned int seed);

  __forceinline static
  void add(Field_uint2x32<MOD1,MOD2>* dest,
           const Field_uint2x32<MOD1,MOD2>& src1,
           const Field_uint2x32<MOD1,MOD2>& src2);

  __forceinline static
  void subtract(Field_uint2x32<MOD1,MOD2>* dest,
                const Field_uint2x32<MOD1,MOD2>& src1,
                const Field_uint2x32& src2);

  __forceinline static
  void neg(Field_uint2x32<MOD1,MOD2>* dest,
           const Field_uint2x32<MOD1,MOD2>& src);

  __forceinline static
  void multiply(Field_uint2x32* dest,
                const Field_uint2x32& src1, const Field_uint2x32& src2);

  __forceinline static
  void divide(Field_uint2x32* dest,
              const Field_uint2x32& src1,
              const Field_uint2x32& src2);

  __forceinline static
  void pow(Field_uint2x32* dest, const Field_uint2x32& src, unsigned int p);

  __forceinline static Field_uint2x32* alloc(unsigned int size);
  __forceinline static void dealloc(Field_uint2x32*);

  __forceinline static Field_uint2x32* allocSq(unsigned int size);
  __forceinline static Field_uint2x32* allocSq2p(unsigned int size);

  __forceinline static const Field_uint2x32* getPrimRoots() {
      if (!initialized) init();
      return primRoots;
  }
  __forceinline static const Field_uint2x32* getPrimRootInvs() {
    if (!initialized) init();
    return primRootInvs;
  }

  __forceinline static const char* getName() {
    const int BUF_SIZE = 80;
    static char buff[BUF_SIZE] = "Field_uint2x32XX_";
    buff[12] = '0' + ((sizeof(ubase_type) * 8) / 10) % 10;
    buff[13] = '0' + (sizeof(ubase_type) * 8) % 10;
    ubase_type num = MOD1;
    int i = BUF_SIZE-2;
    while(num > 0) {
      buff[i--] = '0' + (num % 10);
      num /= 10;
    }
    buff[i--]='_';
    num = MOD2;
    while(num > 0) {
      buff[i--] = '0' + (num % 10);
      num /= 10;
    }

    int j;
    for (j = 15, i++; i < BUF_SIZE - 1; i++, j++) {
      buff[j] = buff[i];
    }
    //buff[j++] = ']';
    buff[j] = 0;
    return buff;
  }

  __forceinline static const char* getId() {
    return getName();
  }


private:
  /** Values above POS are considered negative, and below positive */
  static const ubase_type POS1 = (MOD1 - 1) / 2;
  static const ubase_type POS2 = (MOD2 - 1) / 2;

//  static base_type modulus1; /**< nonmodifyable but cannot be declared const,
//                                  storing prime in memory location,
//                                  needed for some asm instructions */
  typedef fourier_order<MOD1> fourier1;
  typedef fourier_order<MOD2> fourier2;
  static Field_uint2x32 primRoots[min<fourier1::ORDER,fourier2::ORDER>::value];
  static Field_uint2x32 primRootInvs[min<fourier1::ORDER,fourier2::ORDER>::value];

  static bool initialized;
  static bool init();

  ubase_type val1_; /**< actual value in the field */
  ubase_type val2_; /**< actual value in the field */

  std::pair<ubase_type,ubase_type> mixedRadix() const {
    std::pair<ubase_type,ubase_type> rads;
    rads.first = val1_;
    rads.second = val2_;
    if (rads.second >= rads.first)
      rads.second -= rads.first;
    else
      rads.second += MOD2 - rads.first;
    rads.second = ModOp<ubase_type>::mul(rads.second, MOD1%MOD2, MOD2);
    return rads;
  }

};
}  // namespace field
}  // namespace math

#include "Field_uint2x32-Impl.h"

#endif  // NTRP_MATH_FIELD_UINT2X32_H_
