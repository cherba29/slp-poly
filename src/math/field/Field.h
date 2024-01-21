#ifndef INTERP_MATH_FIELD_H
#define INTERP_MATH_FIELD_H

#include "math/Base.h"

#include <boost/scoped_array.hpp>

/**
 * @file: Field.h interface for the Field<Uint> class.
 */

namespace math::field {

template <typename T>
class Field {
 public:
  typedef typename TypeInfo<T>::signed_type sbase_type;
  typedef typename TypeInfo<T>::unsigned_type ubase_type;

  Field() {}  // Do not set to 0
  // Field(base_type v);
  explicit Field(sbase_type v);
  // Field(unsigned int v);

  template <sbase_type VAL>
  void setTo() {
    if (VAL < 0) {
      if (static_cast<T>(-VAL) < modulus)
        val_ = modulus + VAL;
      else
        val_ = modulus - (VAL + modulus);
    } else {
      if (static_cast<T>(VAL) < modulus)
        val_ = VAL;
      else
        val_ = VAL - modulus;
    }
  }

  void setTo(sbase_type v);
  Field& random();
  Field& invertableRandom();

  template <sbase_type VAL>
  bool is() const {
    return (VAL >= 0) ? (val_ == VAL % modulus)
                      : (val_ == (modulus - ((-VAL) % modulus)));
  }
  bool is(sbase_type val) const;
  bool isPositive() const { return val_ <= pos; }
  bool isNegative() const { return val_ > pos; }
  bool hasInverse() const;

  Field& negate() {
    if (val_) val_ = modulus - val_;
    return *this;
  }
  Field& swap(Field& v) {
    ubase_type tmp = val_;
    val_ = v.val_;
    v.val_ = tmp;
    return *this;
  }
  Field& invert();
  Field& sqr();
  Field& powerBy(unsigned int p);

  Field getSqrt() const;
  Field getInverse() const {
    Field A(*this);
    A.invert();
    return A;
  }
  Field getPow(unsigned int p) const;

  int toString(char* buffer) const;
  int toDebugString(char* buffer) const;

  Field operator-() const;

  Field& operator+=(const Field& right);
  Field& operator-=(const Field& right);
  Field& operator*=(const Field& right);
  Field& operator/=(const Field& right) {
    return operator*=(right.getInverse());
  }

  template <typename IntType>
  friend Field<IntType> operator+(const Field<IntType>& left,
                                  const Field<IntType>& right);

  template <typename IntType>
  friend Field<IntType> operator-(const Field<IntType>& left,
                                  const Field<IntType>& right);

  template <typename IntType>
  friend Field<IntType> operator*(const Field<IntType>& left,
                                  const Field<IntType>& right);

  template <typename IntType>
  friend Field<IntType> operator/(const Field<IntType>& left,
                                  const Field<IntType>& right);

  template <typename IntType>
  friend bool operator==(const Field<IntType>& left,
                         const Field<IntType>& right);

  template <typename IntType>
  friend bool operator!=(const Field<IntType>& left,
                         const Field<IntType>& right);

  static int getMaxStringRepLength() { return sizeof(ubase_type) * 5; };
  static T getSize() { return modulus; };

  static void neg(Field* dest, const Field& src);
  static void add(Field* dest, const Field& src1, const Field& src2);
  static void subtract(Field* dest, const Field& src1, const Field& src2);
  static void multiply(Field* dest, const Field& src1, const Field& src2);
  static void divide(Field* dest, const Field& src1, const Field& src2);
  static void pow(Field* dest, const Field& src, unsigned int p);

  static Field* alloc(unsigned int size) { return new Field[size]; }
  static void dealloc(Field* buff) { delete[] buff; }
  static Field* allocSq(unsigned int size);
  static Field* allocSq2p(unsigned int size);

  static const Field* getPrimRoots() { return primRoots.get(); }
  static const Field* getPrimRootInvs() { return primRootInvs.get(); }

  __forceinline static const char* getName() {
    static const char* name = computeName();
    return name;
  }

  __forceinline static const char* getId() {
    static const char* name = getName();  // todo: will change to computeId
    return name;
  }

  static void setPrime(ubase_type mod);
  static ubase_type getModulus() { return modulus; }
  static void reset();

 private:
  static /* const */ ubase_type pos;
  static /* const */ ubase_type modulus;
  static boost::scoped_array<Field> primRoots;
  static boost::scoped_array<Field> primRootInvs;
  ubase_type val_;

  static const char* computeName() {
    const int BUF_SIZE = 80;
    static char buff[BUF_SIZE] = "FieldXX_";
    buff[5] = '0' + ((sizeof(ubase_type) * 8) / 10) % 10;
    buff[6] = '0' + (sizeof(ubase_type) * 8) % 10;
    ubase_type num = modulus;
    int i = BUF_SIZE - 2;  // place it at the end
    while (num > 0) {
      buff[i--] = '0' + (num % 10);
      num /= 10;
    }
    int j;  // move over
    for (j = 8, i++; i < BUF_SIZE - 1; ++i, ++j) {
      buff[j] = buff[i];
    }
    buff[j] = '\0';
    return buff;
  }
};
}  // namespace math::field

#include "Field-Impl.h"

#endif /* INTERP_MATH_FIELD_H */
