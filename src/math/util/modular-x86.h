#ifndef NTRP_MATH_MODULAR_X86_H
#define NTRP_MATH_MODULAR_X86_H

/**
 * @file: modular-x86.h modulo arithmetic.
 */

#include "math/Base.h"
#include "util/SmartAssert.h"

#include <algorithm>
#include <boost/scoped_array.hpp>

namespace math {

template <typename UIntType>
struct UIntTypeInfo;

template <>
struct UIntTypeInfo<uint32_t> {
  typedef int32_t signed_type;
  typedef int64_t signed_type2x;
  typedef uint32_t unsigned_type;
  typedef uint64_t unsigned_type2x;
};

template <>
struct UIntTypeInfo<uint64_t> {
  typedef int64_t signed_type;
  typedef uint64_t unsigned_type;
#ifdef __LP64__
  typedef int128_t signed_type2x;
  typedef uint128_t unsigned_type2x;
#endif
};

template <typename UIntType>
struct ModOp {
  /**
   * @brief Assuming l < m and r < m
   */
  static __forceinline UIntType add(UIntType l, UIntType r, UIntType m) {
    UIntType val = m - l;
    return (val > r) ? m - (val - r) : r - val;
  }

  /**
   * @brief Assuming l < m and r < m
   */
  static __forceinline UIntType sub(UIntType l, UIntType r, UIntType m) {
    if (l >= r) {
      return l - r;
    }
    UIntType diff = (r - l);
    if (diff <= m) return m - diff;
    return m - (diff % m);
    // return (l >= r)? l - r : m - (r - l);
  }

  static __forceinline bool eq(UIntType l, UIntType r, UIntType m) {
    return (l % m == r % m);
  }

  /**
   * @brief Compute l*r mod m
   */
  static __forceinline UIntType mul(UIntType l, UIntType r, UIntType m) {
    return (static_cast<typename UIntTypeInfo<UIntType>::unsigned_type2x>(l) *
            r) %
           m;
  }

  static __forceinline UIntType mul_add(UIntType l, UIntType r, UIntType a,
                                        UIntType m) {
    typename UIntTypeInfo<UIntType>::unsigned_type2x q = l;
    q *= r;
    q += a;
    return q % m;
  }

  /**
   * @brief Compute ( (l-r)*a ) % m
   */
  static __forceinline UIntType sub_mul(UIntType l, UIntType r, UIntType a,
                                        UIntType m) {
    typename UIntTypeInfo<UIntType>::unsigned_type2x q = sub(l, r, m);
    q *= a;
    return q % m;
  }

  /**
   * @brief Inverse modulo a prime
   * Extended Euclidean GCD algorithm to find inverse of a in Z/mZ
   */
  static __forceinline UIntType inv(UIntType a, UIntType m) {
    ASSERT1(a != 0)(a)(m).msg("Can't invert a mod m");

    UIntType ui = 0;
    UIntType ur = m;
    UIntType vi = 1;
    UIntType vr = a;
    UIntType ti;
    UIntType tr;
    UIntType q;
    while (vr) {
      q = ModOp::mul(ur / vr, vi, m);
      if ((ti = ui) >= q)
        ti -= q;
      else
        ti += m - q;
      tr = ur % vr;
      ui = vi;
      ur = vr;
      vi = ti;
      vr = tr;
    }

    ASSERT0(ur == 1)(a)(m).msg("No inverse exists for a mod m");

    return ui;
  }

  /**
   * @brief Find left^right mod m
   */
  static __forceinline UIntType pow(UIntType left, UIntType right, UIntType m) {
    UIntType base = left;
    UIntType n = right;
    UIntType result = 1;
    while (1) {
      if (n & 1) {
        result = ModOp::mul(result, base, m);
      }
      n >>= 1;
      if (n == 0) {
        return result;
      }
      base = ModOp::mul(base, base, m);
    }
    return result;
  }
  /**
   * Compute a^(2^p)
   */
  static __forceinline UIntType pow2p(UIntType a, long p, UIntType m) {
    while (p--) {
      a = ModOp::mul(a, a, m);
    }
    return a;
  }

  /**
   * @brief Square root modulo a prime
   * Shanks-Tonelli algorithm to find square root of a mod m
   */
  static __forceinline UIntType sqrt2(UIntType a, UIntType mod) {
    // Euler's criterion: root exists iff a^((mod-1)/2) % mod == 1
    const UIntType POS = (mod - 1) >> 1;
    if (ModOp::pow(a, POS, mod) != 1) return 0;

    // Find n > 0, q odd, such that 2^n * q + 1 == mod
    UIntType q = mod - 1;
    unsigned int n = 0;
    while ((q & 1) == 0) {
      ++n;
      q >>= 1;
    }

    // Find u such that u^((mod-1)/2) % mod != 1
    // about half satisfy such a condition
    UIntType u = 2;
    while (ModOp::pow(u, POS, mod) == 1) ++u;

    // Initialize g, x, b, r
    UIntType g = ModOp::pow(u, q, mod);            // g = u^q
    UIntType t = ModOp::pow(a, (q - 1) / 2, mod);  // t == val^(q-1)/2
    UIntType x = ModOp::mul(t, a, mod);            // x == a^(q+1)/2
    UIntType b = ModOp::mul(t, x, mod);            // b == a^q;
    int r = n, m;

    while (b != 1) {
      // Find m such that b^2^m % mod == 1
      t = b;
      for (m = 0; t != 1; ++m) {
        t = ModOp::mul(t, t, mod);  // t *= t
      }
      t = g;
      for (--r; r > m; --r) {
        t = ModOp::mul(t, t, mod);  // t *= t
      }
      // r == m, t == g^2^(r-m-1)
      x = ModOp::mul(x, t, mod);  // x *= t
      g = ModOp::mul(t, t, mod);  // g = t^2
      b = ModOp::mul(b, g, mod);  // b *= g
    }
    if (x > POS) x = mod - x;  // return positive value
    return x;
  }

  /**
   * @brief Square root modulo a prime
   * Shanks-Tonelli algorithm to find square root of a mod m
   * @param a - an element to compute square root of
   * @param m - prime number
   * @return x such that x^2=a mod m or 0 if no square root exists
   */

  static __forceinline UIntType sqrt(UIntType a, UIntType mod) {
    // Euler's criterion: root exists iff a^((mod-1)/2) % mod == 1
    const UIntType POS = (mod - 1) >> 1;
    if (ModOp::pow(a, POS, mod) != 1) return 0;

    // Find e > 0, q odd, such that 2^e * q + 1 == mod
    UIntType q = mod - 1;
    unsigned int e = 0;
    while ((q & 1) == 0) {
      ++e;
      q >>= 1;
    }

    // Find generator z
    // Find n such that n^((mod-1)/2) % mod != 1
    // about half satisfy such a condition
    UIntType n = 2;
    while (ModOp::pow(n, POS, mod) == 1) ++n;
    UIntType z = ModOp::pow(n, q, mod);

    UIntType y = z;
    unsigned int r = e;
    unsigned int m;
    UIntType x = ModOp::pow(a, (q - 1) / 2, mod);
    UIntType b = ModOp::mul(a, x, mod);
    b = ModOp::mul(b, x, mod);  // b = a^(q-1)
    x = ModOp::mul(a, x, mod);  // x = a^(q+1)/2
    while (b != 1) {
      UIntType t = b;
      for (m = 0; (m <= r) && (t != 1); ++m) {
        t = ModOp::mul(t, t, mod);  // t *= t
      }
      ASSERT1(m < r)
      (m)(r)(b)(t)(e)(q)(n)(y)(a)(mod).msg(
          "this condition should not occur if there exist a root for a");
      t = ModOp::pow2p(y, r - m - 1, mod);
      y = ModOp::mul(t, t, mod);
      r = m;
      x = ModOp::mul(x, t, mod);
      b = ModOp::mul(b, y, mod);
    }
    if (x > POS) x = mod - x;  // return positive value
    return x;
  }
};  // struct ModOp

//============================================================================
template <typename UIntType>
class MultiMod {
  UIntType* mods_;
  UIntType* radneg_;
  UIntType* mixmuls_;
  size_t n_;
  void setPrimes(const UIntType* mods, size_t n) {
    std::copy(mods, mods + n, mods_);

    // Sort _primes in increasing order
    std::sort(mods_, mods_ + n);

    // Check if there are not duplicates
    const UIntType* prm;
    ASSERT1((prm = std::adjacent_find(mods_, mods_ + n)) == mods_ + n)
    (prm - mods_)(n).warn("Duplicate primes were passed in");

    mixedMults(mixmuls_, mods_, n_);
    radneg_[0] = mods_[0];
    for (UIntType i = 1; i < n_; ++i) {
      radneg_[i] = mods_[i] - 1;
      // ModOp<UIntType>::mul(
      // ModOp<UIntType>::sub(mods_[i], mods_[i-1], mods_[i]),
      // mixmuls_[i],mods_[i]
      //);
    }
  }

 public:
  MultiMod() : mods_(NULL), radneg_(NULL), mixmuls_(NULL), n_(0) {}
  MultiMod(const UIntType* mods, size_t n)
      : mods_(new UIntType[3 * n]),
        radneg_(mods_ + 2 * n),
        mixmuls_(mods_ + n),
        n_(n) {
    setPrimes(mods, n);
  }

  void init(const UIntType* mods, size_t n) {
    mods_ = new UIntType[3 * n];
    mixmuls_ = mods_ + n;
    n_ = n;
    radneg_ = mods_ + 2 * n;
    setPrimes(mods, n);
  }
  void reset() {
    delete[] mods_;
    mods_ = NULL;
    mixmuls_ = NULL;
    n_ = 0;
  }
  UIntType operator[](int i) const { return mods_[i]; }
  size_t getSize() const { return n_; }

  /**
   * @brief Compute mixed radix representations of crts value using Garners
   * algorithm. Complexity of this algorithm is O(n^2) where n is number of
   * moduli. if crts_i = X mod m_i, then X = rads_0 +
   * rads_1*m_0+rads_2*m_0*m_1+...+rads_n*m_0...m_{n-1} This algorithm uses
   * precomputed mixedmultipliers see setPrimes().
   *
   * @param rads - (output) mixed radix represention of length n (constructor
   * param)
   * @param crts - (input) number in modulo represention of length n
   */
  void crtToMixedRadix(UIntType* rads, const UIntType* crts) const {
    rads[0] = crts[0];
    for (int k = 1; k < static_cast<int>(n_); ++k) {
      UIntType tmp = rads[k - 1];
      for (int j = k - 2; j >= 0; --j) {
        // temp = (temp*mods[j] + rads[j]) % mods[k]
        tmp = ModOp<UIntType>::mul_add(tmp, mods_[j], rads[j], mods_[k]);
      }
      // rads[k] = ( (crts[k] - tmp) * mixmuls_[k] ) % mods[k]
      rads[k] = ModOp<UIntType>::sub_mul(crts[k], tmp, mixmuls_[k], mods_[k]);
    }
  }

  /**
   * @brief Given a number in mixed radix representation divide it by
   * unsigned integer and return corresponding remainder. Note that
   * resulting rads might will be in strict mixed radix representation
   * in the sence that rads[i] < mods_[i].
   * @param rads - (input/output) mixed radix representation
   * @param div - divider
   * @return - remainder
   */
  UIntType divModRadix(UIntType* rads, bool* isZero, UIntType div) const {
    typename UIntTypeInfo<UIntType>::unsigned_type2x quo = 0;
    *isZero = true;
    for (unsigned int i = n_ - 1; i > 0; i--) {
      quo += rads[i];
      rads[i] = static_cast<UIntType>(quo / div);
      *isZero = (*isZero && (rads[i] == 0));
      quo %= div;
      quo *= mods_[i - 1];
    }
    quo += rads[0];
    rads[0] = static_cast<UIntType>(quo / div);
    return quo % div;
  }

  void mixedRadixNegate(UIntType* orads, const UIntType* irads) {
    for (unsigned int i = 0; i < n_; ++i) {
      orads[i] = ModOp<UIntType>::sub(radneg_[i], irads[i], mods_[i]);
    }
  }
  bool mixedRadixIsNegative(const UIntType* rads) {
    for (int i = n_ - 1; i >= 0; --i) {
      UIntType half = mods_[i] >> 1;
      if (rads[i] > half) return true;
      if (rads[i] < half) return false;
    }
    return false;  // Exact middle is positive
  }

  int mixedRadixToString(char* buf, int max_buf_size, const UIntType* rads,
                         unsigned int base) {
    unsigned int highRad = n_;
    // TODO: fix base parameter.
    const UIntType radix = 10;
    typename UIntTypeInfo<UIntType>::unsigned_type2x quo;

    boost::scoped_array<UIntType> nrads(new UIntType[highRad]);
    bool negated = mixedRadixIsNegative(rads);
    if (negated) {
      mixedRadixNegate(nrads.get(), rads);
    } else {
      std::copy(rads, rads + highRad, nrads.get());
    }
    int pos = 0;
    max_buf_size -= 2;
    while (highRad > 0 && pos < max_buf_size) {
      quo = 0;
      for (unsigned int i = highRad - 1; i > 0; i--) {
        quo += nrads[i];
        nrads[i] = static_cast<UIntType>(quo / radix);
        quo %= radix;
        quo *= mods_[i - 1];
      }
      quo += nrads[0];
      nrads[0] = static_cast<UIntType>(quo / radix);

      buf[pos++] = '0' + quo % radix;
      while ((highRad > 0) && (nrads[highRad - 1] == 0)) highRad--;
    }

    if (pos >= max_buf_size) {  // there was not enough space
      buf[0] = 0;
      return 0;
    }

    // Do not negate 0
    if (negated) {
      buf[pos++] = '-';
    }
    buf[pos] = '\0';
    std::reverse(buf, buf + pos);

    return pos;
  }
  /**
   * @brief Garner's algorithm to compute multipliers for mixed radix
   * conversion. When converting a number in modulo form to mixed radix
   * representation (prod_i=0^{k-1} m_i)^{-1} mod m_k need to be computed. They
   * need to be computed only once, hence are provided here as separate
   * function.
   * @param mixmuls - (output) mixed multiplier for 0,...,n-1
   * @param mods - (input) array of relatively co-prime integers of size n
   * @param n - (input) size of the mixmuls and mods arrays
   * @see crtToMixedRadix
   */
  static void mixedMults(UIntType* mixmuls, const UIntType* mods, size_t n) {
    mixmuls[0] = 1;
    for (size_t k = 1; k < n; ++k) {
      UIntType prod = mods[0] % mods[k];
      for (size_t i = 1; i < k; ++i) {
        prod = ModOp<UIntType>::mul(prod, mods[i], mods[k]);
      }
      mixmuls[k] = ModOp<UIntType>::inv(prod, mods[k]);
    }
    /*

        UIntType prod = 1;
        for (size_t i = 1; i < n; ++i) {
          for (size_t j = i; j > 0; --j) {
            prod = ModOp<UIntType>::mul(
              prod,
              ModOp<UIntType>::inv(mods[j-1], mods[i]),
              mods[i]);
          }
          mixmuls[i-1] = prod;
        }
    */
  }

  ~MultiMod() {
    delete[] mods_;  // deletes both, mods and mixmuls
  }
};

}  // namespace math
#endif  // NTRP_MATH_MODULAR_X86_H
