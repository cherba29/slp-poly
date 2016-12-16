#ifndef NTRP_MATH_FIELD_UINT_H_
#define NTRP_MATH_FIELD_UINT_H_

/**
 * @file: Field_uint.h interface for the hardcoded prime field class.
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
 *  1. 30 - 3221225473 = 2^30 * 3 + 1
 *  2. 28 - 3489660929 = 2^28 * 13 + 1
 *  3. 27 - 3892314113 = 2^27 * 29 + 1
 * This class is optimized for arithmetic modulo hardcoded 64-bit prime.
 * It overloads all arithmetic operations and can be instantiated from integers
 * and strings.
 */

template <typename UIntType, UIntType MOD>
class Field_uint {
public:
  typedef typename TypeInfo<UIntType>::signed_type sbase_type;
  typedef typename TypeInfo<UIntType>::unsigned_type ubase_type;

  //### Initializers ########################################################
  /**
   * @brief Default constructor, constructs without initialization
   */
  __forceinline Field_uint() { }

  /**
   * @brief Initialize field from an unsigned integer,
   *        internally will reduce modulo.
   * @param v unsigned integer which can be larger than modulus
   * @see setTo
   */
  //__forceinline explicit Field_uint(ubase_type v);

  /**
   * @brief initalize field from signed integer.
   * @see setTo
   */
  __forceinline explicit Field_uint(sbase_type v);

  /*
   * @brief Copy constructor
   */
  //__forceinline Field_uint64(const Field_uint64& right);

  /*
   * @brief Destructor
   */
  //__forceinline ~Field_uint64() { }

  /**
   * @brief Set to statically know constant value.
   * @see operator=(int)
   */
  template <sbase_type VAL> __forceinline
  void setTo() { val_ = mod<VAL,MOD>::VALUE; }

  /**
   * @brief Set field to dynamic integer value
   * @see operator=(int), template <int VAL> setTo()
   */
  __forceinline void setTo(sbase_type v);

  /**
   * @brief Inititialize this instance to random field element.
   */
  __forceinline Field_uint& random();

  /**
   * @brief Inititialize this instance to random field element, for which
   * hasInverse is true.
   * @see hasInverse, random
   */
  __forceinline Field_uint& invertableRandom();

  //### Property checks ########################################################
  /**
   * @brief Returns true if this instance equivalent to given integer.
   */
  template <int VAL> __forceinline
  bool is() const { return val_ == mod<VAL,MOD>::VALUE; }

  /**
   * @brief Returns true if this instance equivalent to given integer.
   */
  __forceinline
  bool is(sbase_type val) const {
    return (val>=0) ? (val_==val % MOD) : (val_ == (MOD - ((-val) % MOD)));
  }

  /**
   * @brief Returns true if value is smaller or equal to size()/2, also 0
   * @see size()
   * @see isNegative()
   */
  __forceinline
  bool isPositive() const { return val_ <= POS; }

  /**
   * @brief Returns true if value is larger than size()/2
   * @see size()
   * @see isPositive()
   */
  __forceinline bool isNegative() const { return val_ > POS; }

  /**
   * @brief Does this element has multiplicative inverse.
   * For a field only 0 does not have it.
   * For a domain many non-zero elements will have no inverse.
   */
  __forceinline bool hasInverse() const {	return !is<0>(); }

  //### Modifiers ##############################################################

  /**
   * @brief Turn this instance to its additive inverse
   * @see operator-()
   * @see operator-=(Field)
   */
  __forceinline Field_uint& negate() { if (val_)	val_ = MOD - val_; return *this; }

  /**
   * @brief Swap values of two fields elements
   */
  __forceinline void swap(Field_uint& f) {
    ubase_type tmp = val_;
    val_ = f.val_;
    f.val_ = tmp;
  }

  /**
   * @brief set field element to its multiplicative inverse
   *        The algorithm is based on Euclidian GCD algorithm
   */
  __forceinline Field_uint& invert();

  /**
   *  @brief Squares itself.
   *  @return reference to itself
   */
  __forceinline Field_uint& sqr();

  /**
   * @brief Raise this instance to given power.
   *        Number of steps equal to the order of highest non-zero bit
   *        in the integer.
   * @see getPow
   */
  __forceinline Field_uint& powerBy(unsigned int p);

  //** Calculators **********************************************************
  /**
   * @brief Shanks-Tonelli algorithm to find square root of a mod m
   * @return number k for which k*k == f if exists, otherwise 0
   */
  __forceinline Field_uint getSqrt() const;

  /**
   * @brief Returns the multiplicative inverse of f.
   * @see inverse()
   */
  __forceinline Field_uint getInverse() const {
    Field_uint A(*this);
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
  __forceinline Field_uint getPow(ubase_type p) const;

  /**
   * @brief Convert this instance to string representation.
   * @param buffer - output and must be at least
   *                 getMaxStringRepLength characters long
   * @see getMaxStringRepLength
   */
  void toString(char* buffer) const;

  void toDebugString(char* buffer) const { toString(buffer); }

  //** Operators ************************************************************
  /*
   * @brief Assignment operator. Replaces the current instance with right
   */
  // Field_uint64& operator=(const Field_uint64& right);

  /**
   * @brief Returns the additive inverse of this instance
   * @see negate
   * @see operator-=()
   */
  __forceinline Field_uint operator-() const;

  /**
   * @brief Adds right to this instance.
   */
  __forceinline Field_uint& operator+=(const Field_uint& right);

  /**
   *  @brief Subtracts right from this instance
   *  @see subtract
   *  @return reference to this instance
   */
  __forceinline Field_uint& operator-=(const Field_uint& right);

  /**
   * @brief Multiplies this instance by right
   * @return reference to this instance
   */
  __forceinline Field_uint& operator*=(const Field_uint& m);

  /**
   * @brief Divides this instance by right.
   */
  __forceinline Field_uint& operator/=(const Field_uint& right) {
    return operator*=(right.getInverse());
  }

  /**
   * @brief Adds this instance with given argument and returns new instance.
   * @return new instance which is sum of this and given
   * @see add
   * @see operator+=
   */
//  template <typename U, U M>
//  friend Field_uint<U,M> operator+(const Field_uint<U,M>& left,
//                                   const Field_uint<U,M>& right);

  Field_uint operator+(const Field_uint& right) const;
  /**
   * @brief Subtract value from this instance returning the difference
   * @see subtract
   */
//  template <typename U, U M>
//  friend Field_uint<U,M> operator-(const Field_uint<U,M>& left,
//                                   const Field_uint<U,M>& right);
  Field_uint operator-(const Field_uint& right) const;
  /**
   * @brief Returns (left * right) % modulus.
   */
  //template <typename U, U M>
  //friend Field_uint<U,M> operator*(const Field_uint<U,M>& left,
  //                                 const Field_uint<U,M>& right);
  Field_uint operator*(const Field_uint& right) const;

  /**
   * @brief Returns new instance which is division of this by given instance.
   * @see divide
   */
  //template <typename U, U M>
  //friend Field_uint<U,M> operator/(const Field_uint<U,M>& left,
  //                                 const Field_uint<U,M>& right);
  Field_uint operator/(const Field_uint& right) const;

  /**
   * @brief Returns true iff this instance and right are equal.
   *        Identical to equals member function
   * @see operator!=
   */
  //template <typename U, U M>
  //friend bool operator==(const Field_uint<U,M>& left,
  //                       const Field_uint<U,M>& right);
  bool operator==(const Field_uint& right) const;

  /**
   * @brief Returns true iff this instance and right are not equal.
   * @see operator==
   */
  //template <typename U, U M>
  //friend bool operator!=(const Field_uint<U,M>& left,
  //                       const Field_uint<U,M>& right);
  bool operator!=(const Field_uint& right) const;

  //#### Static members ########################################################

  /** Maximum string representation for any element of the field */
  static int getMaxStringRepLength() {
    return sizeof(ubase_type)*5;
      /* 5 dec digits per byte with +/- with null terminator */
  }
  static int getMaxDebugStringRepLength() { return sizeof(ubase_type)*5; }

  /**
   * @brief Returns the largest number this class can represent
   */
  static long double getSize() { return MOD; }

  /**
   * @brief Initializes the class's random number generator.
   */
  static void randomInit(unsigned int seed);

  __forceinline static
  void add(Field_uint* dest,
           const Field_uint& src1, const Field_uint& src2);

  __forceinline static
  void add(Field_uint* dest,
           const Field_uint* src1, const Field_uint* src2, int n);

  __forceinline static
  void add(Field_uint* dest,
           const Field_uint* src1, const Field_uint& src2, int n);

    __forceinline static
  void add(Field_uint* dest,
           const Field_uint* src1, int n);

  __forceinline static
  void subtract(Field_uint* dest,
                const Field_uint& src1, const Field_uint& src2);

    __forceinline static
  void subtract(Field_uint* dest,
                const Field_uint* src1, const Field_uint& src2, int n);

  __forceinline static
  void subtract(Field_uint* dest,
                const Field_uint* src1, const Field_uint* src2, int n);

  __forceinline static
  void subtract(Field_uint* dest,
                const Field_uint* src1, int n);

  __forceinline static
  void neg(Field_uint* dest, const Field_uint& src);

  __forceinline static
  void neg(Field_uint* dest, const Field_uint* src, int n);

  __forceinline static
  void multiply(Field_uint* dest,
                const Field_uint& src1, const Field_uint& src2);

  __forceinline static
  void multiply(Field_uint* dest,
                const Field_uint* src1, const Field_uint* src2, int n);

  __forceinline static
  void multiply(Field_uint* dest,
                const Field_uint* src1, const Field_uint& src2, int n);

  __forceinline static
  void multiply(Field_uint* dest,
                const Field_uint* src1, int n);

  __forceinline static
  void divide(Field_uint* dest,
              const Field_uint& src1, const Field_uint& src2);

  __forceinline static
  void divide(Field_uint* dest,
              const Field_uint* src1, int n);

  __forceinline static
  void divide(Field_uint* dest,
              const Field_uint* src1, const Field_uint* src2, int n);

  __forceinline static
  void divide(Field_uint* dest,
              const Field_uint* src1, const Field_uint& src2, int n);

  __forceinline static
  void pow(Field_uint* dest, const Field_uint& src, unsigned int p);

  __forceinline static
  void pow(Field_uint* dest, const Field_uint* src, unsigned int p, int n);

  __forceinline static Field_uint* alloc(unsigned int size);
  __forceinline static void dealloc(Field_uint*);

  __forceinline static Field_uint* allocSq(unsigned int size);
  __forceinline static Field_uint* allocSq2p(unsigned int size);

  __forceinline static const Field_uint* getPrimRoots() {
      if (!initialized) init();
      return primRoots;
  }
  __forceinline static const Field_uint* getPrimRootInvs() {
    if (!initialized) init();
    return primRootInvs;
  }


  __forceinline static const char* getName() {
    static bool nameSet = false;
    static char buff[] = "Field_uintXX_XXXXXXXXXXXXXXXXXXXXXXX";
    if (!nameSet) {
      buff[10] = '0' + ((sizeof(ubase_type) *8) / 10) % 10;
      buff[11] = '0' + (sizeof(ubase_type) *8) % 10;
      UIntType mod = MOD;
      int i;
      for (i = 13; mod; ++i) {
        buff[i] = '0' + (mod % 10);
        mod /= 10;
      }
      buff[i] = '\0'; --i;
      for (int start = 13; start < i; ++start, --i) {
        std::swap(buff[start], buff[i]);
      }
      nameSet = true;
    }
    return buff;
  }

  __forceinline static const char* getId() {
    return getName();
  }


private:
  /** Values above POS are considered negative, and below positive */
  static const ubase_type POS = (MOD - 1) / 2;

  // Only 2^nth roots are listed. Total number is bounded by log of the size of the Field
  static Field_uint primRoots[sizeof(sbase_type) * 8];
  static Field_uint primRootInvs[sizeof(sbase_type) * 8];

  // Flag whether primitive roots have been computed
  static bool initialized;
  static bool init();

  ubase_type val_; /**< actual value in the field */
};

}  // namespace field
}  // namespace math

#include "Field_uint-Impl.h"

#endif  // NTRP_MATH_FIELD_UINT64_H_
