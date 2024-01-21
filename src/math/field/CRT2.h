#ifndef NTRP_MATH_FIELD_CRT2_H
#define NTRP_MATH_FIELD_CRT2_H

/**
 * @file CRT2.h Field over set of primes via Chinese Remaindering.
 */

#include "math/Base.h"
#include "math/util/random.h"

#include <boost/scoped_array.hpp>

namespace math::field {
/**
 * @brief Represents a number by its value % a sequence of primes < 2^32
 * The primes are set via static method setPrimes, and must be done before
 * very first usage.
 *
 * Modular arithmetic algorithms taken from Donald Knuth,
 * ! _Seminumerical Algorithms_, 3rd edition
 *
 * This class supports aribitrary number of primes
 *
 * @see Field_uint, Field32 - for single prime modular arithmetic
 * which support identical interface
 *
 */

template <typename UIntType>
class CRT2 {
public:
  typedef typename TypeInfo<UIntType>::signed_type sbase_type;
  typedef typename TypeInfo<UIntType>::unsigned_type ubase_type;

  /**
   * @brief Default constructor, constructs without initialization
   */
  CRT2();

  /**
   *  @brief Initialize field from an unsigned integer,
   *         internally will reduce modulo.
   *  @param v unsigned integer which can be larger than modulus
   *  @see setTo
   */
//  CRT2(base_type val);

  /**
   * @brief initalize field from signed integer.
   * @see setTo
   */
  __forceinline CRT2(sbase_type v);

  /**
   * @brief Copy constructor
   */
  __forceinline CRT2(const CRT2& right);

  /**
   * @brief Destructor
   */
  __forceinline ~CRT2();

  /**
   * @brief Set to statically know constant value.
   * @see operator=(int)
   */
  template <sbase_type VAL> __forceinline
  void setTo() {
    if (VAL < 0) {
      for (unsigned int i = 0; i < multMod.getSize(); ++i) {
        vals_[i] = multMod[i] - ((-VAL) % multMod[i]);
      }
    } else {
      for (unsigned int i = 0; i < multMod.getSize(); ++i) {
        vals_[i] = VAL % multMod[i];
      }
    }
  }

  /**
   * @brief Set field to dynamic integer value
   * @see operator=(int), template <int VAL> setTo()
   */
  __forceinline void setTo(sbase_type v);

  /**
   * @brief Inititialize this instance to random field element.
   */
  __forceinline void random();

  /**
   * @brief Inititialize this instance to random field element, for which
   * hasInverse is true.
   * @see hasInverse, random
   */
  __forceinline void invertableRandom();

  /**
   * @brief Returns true if this instance equivalent to given integer.
   */
  template <int VAL> __forceinline
  bool is() const;

  /**
   * @brief Returns true if this instance equivalent to given integer.
   */
  __forceinline bool is(sbase_type val) const;

  /**
   * @brief Returns true if value is smaller or equal to size()/2
   * @see size()
   * @see isNegative()
   */
  __forceinline bool isPositive() const;

  /**
   * @brief Returns true if value is larger than size()/2
   * @see size()
   * @see isPositive()
   */
  bool isNegative() const;

  /**
   * @brief Does this element has multiplicative inverse.
   * For a field only 0 does not have it.
   * For a domain many non-zero elements will have no inverse.
   */
  __forceinline bool hasInverse() const;

  /**
   * @brief Turn this instance to its additive inverse
   * @see operator-()
   * @see operator-=(Field)
   */
  __forceinline
   void negate();

  /**
   * @brief Swap values of two elements
   */
  __forceinline
  CRT2& swap(CRT2& f);

  /**
   * @brief set field element to its multiplicative inverse
   *        The algorithm is based on Euclidian GCD algorithm
   */
  CRT2& invert();

  /**
   * @brief Square itself
   * @return reference to itself
   */
  __forceinline CRT2& sqr();

  /**
   * @brief Raise this instance to given power.
   *        Number of steps equal to the order of highest non-zero bit
   *        in the integer.
   * @see getPow
   */
  CRT2& powerBy(unsigned int p);

  /**
   * @brief Shanks-Tonelli algorithm to find square root of a mod m
   * @return number k for which k*k == f if exists, otherwise 0
   */
  CRT2 getSqrt() const;

  /**
   * @brief Returns the multiplicative inverse of f
   * @see inverse()
   */
  __forceinline
  CRT2 getInverse() const { CRT2 A(*this); A.invert(); return A;  }

  /**
   * @brief Raise this instance to given power.
   *        Number of steps equal to the order of highest non-zero bit
   *        in the integer.
   * @return this^p
   * @see powerBy
   */
  CRT2 getPow(unsigned int p) const;

  /**
   * @brief Convert this instance to string representation.
   * @param buffer - output and must be at least
   *                 getMaxStringRepLength characters long
    @see getMaxStringRepLength
   */
  int toString(char* buffer) const;

  int toDebugString(char* buffer) const;

 	/**
	 * @brief Replaces the current instance with right
	 */
	CRT2& operator=(const CRT2& right);

  /**
   * @brief Returns the additive inverse of this instance
   * @see negate
   * @see operator-=()
   */
  CRT2 operator-() const;

  /**
   * @brief Adds right to this instance.
   */
  CRT2& operator+=(const CRT2& right);

  /**
   *  @brief Subtracts right from this instance
   *  @see subtract
   *  @return reference to this instance
   */
  CRT2& operator-=(const CRT2& right);

  /**
   * @brief Multiplies this instance by right,
   * implemented using x86 32-bit inline assembly.
   * @return reference to this instance
   */
  CRT2& operator*=(const CRT2& right);

  /**
   * @brief Divides this instance by right
   */
  CRT2& operator/=(const CRT2& right) {	return operator*=(right.getInverse()); }

 /**
   * @brief Adds this instance with given argument and returns new instance.
   * @return new instance which is sum of this and given
   * @see add
   * @see operator+=
   */
  template <typename U>
  friend __forceinline
  CRT2<U> operator+(const CRT2<U>& left, const CRT2<U>& right);


  /**
   * @brief Subtract value from this instance returning the difference
   * @see subtract
   */
  template <typename U>
  friend __forceinline
  CRT2<U> operator-(const CRT2<U>& left, const CRT2<U>& right);


  /**
   * @brief Returns (left * right) % each modulus.
   */
  template <typename U>
  friend __forceinline
  CRT2<U> operator*(const CRT2<U>& left, const CRT2<U>& right);


  /**
   * @brief Returns new instance which is division of this by given instance.
   * @see divide
   */
  template <typename U>
  friend __forceinline CRT2<U> operator/(const CRT2<U>& left, const CRT2<U>& right);


  /**
   * @brief Returns true iff this instance and right are equal.
   *        Identical to equals member function
   * @see operator!=
   */
  template <typename U>
  friend __forceinline bool operator==(const CRT2<U>& left, const CRT2<U>& right);


  /**
   * @brief Returns true iff this instance and right are not equal.
   * @see operator==
   */
  template <typename U>
  friend __forceinline bool operator!=(const CRT2<U>& left, const CRT2<U>& right);



  /** Maximum string representation for any element of the field */
  static int getMaxStringRepLength() {
    return multMod.getSize() * sizeof(ubase_type) * 5;
      /* 5 dec digits per byte with +/- with null terminator */
  }

  static int getMaxDebugStringRepLength() {
    return multMod.getSize() * sizeof(ubase_type) * 5 * 2;
  }

  /**
   * @brief Returns the largest number this class can represent.
   */
  static long double getSize();

  /**
   * @brief Initializes the class's random number generator.
   */
  static void randomInit(unsigned int seed);

  __forceinline
  static void add(CRT2* dest, const CRT2& src1, const CRT2& src2);

  __forceinline
  static void subtract(CRT2* dest, const CRT2& src1, const CRT2& src2);

  __forceinline
  static void neg(CRT2* dest, const CRT2& src);

  __forceinline
  static void multiply(CRT2* dest, const CRT2& src1, const CRT2& src2);

  __forceinline
  static void divide(CRT2* dest, const CRT2& src1, const CRT2& src2);

  __forceinline
  static void pow(CRT2* dest, const CRT2& src, unsigned int p);

  static CRT2* alloc(unsigned int size);

  static void dealloc(CRT2* buff);

  __forceinline
  static CRT2* allocSq(unsigned int size);

  __forceinline
  static CRT2* allocSq2p(unsigned int size);

  __forceinline
  static const CRT2* getPrimRoots() { return primRoots.get(); }

  __forceinline
  static const CRT2* getPrimRootInvs() { return primRootInvs.get(); }

  __forceinline
  static const char* getName() {
    const int BUF_SIZE = 80;
    static char buff[BUF_SIZE] = "CRT2XX_X_";
    buff[3] = '0' + ((sizeof(ubase_type) * 8) / 10) % 10;
    buff[4] = '0' + (sizeof(ubase_type) * 8)%10;
    buff[6] = '0' + multMod.getSize();
    int i = BUF_SIZE-2; // place it at the end
    int j;
    int lastPrimeIdx = multMod.getSize()-1;
    for (j = lastPrimeIdx; j >=0; --j) {
      if (j < lastPrimeIdx) {
        buff[i--] = '_';
      }
      ubase_type num = multMod[j];
      while(num > 0) {
        buff[i--] = '0' + (num % 10);
        num /= 10;
      }
    }
    // move over
    for (j = 8, i++; i < BUF_SIZE-1; ++i,++j) {
      buff[j] = buff[i];
    }
    //buff[j++] = ']';
    buff[j] = 0;
    return buff;
  }

  __forceinline
  static const char* getId() { return getName(); }

  /**
   * @brief Replaces _primes with the contents of the argument.
   * @see init
   */
  static void setPrimes(const ubase_type* primes, int n);
  static void reset();

private:
  //static unsigned int nPrimes_; /**< the number of prime moduli */
  //static ubase_type* primes_;     /**< the prime moduli */
  //static ubase_type* mixmuls_;    /**< mixmuls_[i] == product(primes_[j] | j <- 0..i) % primes_[i+1] */
  static MultiMod<UIntType> multMod;

  static boost::scoped_array<CRT2> primRoots;
  static boost::scoped_array<CRT2> primRootInvs;

  /**
   * @brief Returns a mixed-radix representation of this instance
   *        D. E. Knuth, _Seminumerical Algorithms_, 4.3.2, ex 7 (p. 293)
   */
  //ubase_type* mixedRadix() const;
  ubase_type* vals_;              /**< _vals[i] == number % _primes[i] */
};

/**
 * @brief Returns (left + right) % each modulus
 */
template <typename U> __forceinline
CRT2<U> operator+(const CRT2<U>& left, const CRT2<U>& right);

/**
 * @brief Returns (left - right) % each modulus.
 */
template <typename U> __forceinline
CRT2<U> operator-(const CRT2<U>& left, const CRT2<U>& right);

/**
 * @brief Returns (left * right) % each modulus.
 */
template <typename U> __forceinline
CRT2<U> operator*(const CRT2<U>& left, const CRT2<U>& right);

/**
 * @brief Returns (left / right) % each modulus.
 */
template <typename U> __forceinline
CRT2<U> operator/(const CRT2<U>& left, const CRT2<U>& right);

/**
 * @brief Returns true iff this instance and right are equal.
 *        Identical to equals member function
 * @see equals
 * @see operator!=
 */
template <typename U> __forceinline
bool operator==(const CRT2<U>& left, const CRT2<U>& right);

/**
 * @brief Returns true iff this instance and right are not equal.
 * @see operator==
 * @see equals
 */
template <typename U> __forceinline
bool operator!=(const CRT2<U>& left, const CRT2<U>& right) {
  return !(operator==(left,right));
}


/**
 * @brief Returns isNegative(left-right).
 */
template <typename U> __forceinline
bool operator<(const CRT2<U>& left, const CRT2<U>& right);

/**
 * @brief Returns !isNegative(right-left).
 */
template <typename U> __forceinline
bool operator<=(const CRT2<U>& left, const CRT2<U>& right);

/**
 * @brief Returns isNegative(right-left).
 */
template <typename U> __forceinline
bool operator>(const CRT2<U>& left, const CRT2<U>& right);

/**
 * @brief Returns !isNegative(left-right).
 */
template <typename U> __forceinline
bool operator>=(const CRT2<U>& left, const CRT2<U>& right);

//! Returns !isNegative(left-right)
template <typename U> __forceinline
bool operator>=(const CRT2<U>& left, const CRT2<U>& right) {
  return !(left - right).isNegative();
}

//! Returns isNegative(left-right)
template <typename U> __forceinline
bool operator<(const CRT2<U>& left, const CRT2<U>& right) {
  return (left - right).isNegative();
}

//! Returns !isNegative(right-left)
template <typename U> __forceinline
bool operator<=(const CRT2<U>& left, const CRT2<U>& right) {
  return !(right - left).isNegative();
}

//! Returns isNegative(right-left)
template <typename U> __forceinline
bool operator>(const CRT2<U>& left, const CRT2<U>& right) {
  return (right - left).isNegative();
}

//! Returns (left / right) % each modulus
template <typename U> __forceinline
CRT2<U> operator/(const CRT2<U>& left, const CRT2<U>& right) {
  CRT2<U> result = left;
  return result /= right;
}

/**
 * @brief Reads f (in decimal form) from is
 */
//std::istream& operator>>(std::istream& is, PrimeField& f);

/**
 * @brief Writes f's decimal form to os
 */
//std::ostream& operator<<(std::ostream& os, const PrimeField& f);

	/**
	 * @brief Constructs an instance from the decimal form of a number.
	 */
//	CRT232(const std::string& digits);

	/**
	 * @brief Initializes the class's static members.
	 */
  /*
	static void init() {
		if (!_nPrimes) {
		  _nPrimes = 1;
		  _primes = new uint32_t[_nPrimes];
		  _primes[0] = (uint32_t) 0xFFF00001; // 4293918721;
		  _mixmuls = new uint32_t[_nPrimes - 1];
		}
	} */

	/**
	 * @brief Deallocates _primes
	 */
  /*
	static void clearPrimes() {
		delete[] _primes;
		delete[] _mixmuls;
	}
  */
	/**
	 * Writes _primes to os
	 *  this static member should not be part of this class
	 */
	//static void showPrimes(std::ostream& os);

}  // namespace math::field

#include "CRT2-Impl.h"

#endif  // NTRP_MATH_FIELD_CRT2_H
