#ifndef NTRP_COMPLEXITY_EQUATION_H_
#define NTRP_COMPLEXITY_EQUATION_H_

/**
 * @file ComplexityEquation.h
 * @brief Abstract interface definition for ComplexityEquation class
 *
 */

#define LOG_MODULE ::logging::LogModuleEnum::COMPLEXITY
#include "util/log.h"

#include <boost/log/attributes/named_scope.hpp>
#include <cmath>
#include <ostream>

namespace complexity {

/**
 * @brief Abstrction for complexity equation
 */

class Equation {
 public:
  /**
   * @brief return number of terms in this equation
   */
  virtual int getNTerms() const = 0;

  /**
   * @brief Get array of coefficients
   */
  virtual long double* getCoeffs() = 0;
  virtual const long double* getCoeffs() const = 0;

  /**
   * @brief For printing purpose return name of each term
   */
  virtual const char** getTermNames() const = 0;

  /**
   * @brief Evaluate each term storing result in array, array must be large
   * enough to hold all there terms.
   * @param[in] n - evaluation point
   * @param[out] vals - array of size getNTerms
   */
  virtual void evaluateTermsAt(long double n, long double* vals) const = 0;

  /**
   * @brief Evaluate equation at given point
   * @see evaluateTermsAt
   * @param[in] n - evaluation point
   * @return value of the equation at given point
   */
  virtual long double evaluateAt(long double n) const = 0;

  /**
   * @brief Evaluate the derivative of the equation at given point
   * @see evaluateAt
   */
  virtual long double evaluateDerivAt(long double n) const = 0;

  /**
   * @brief find n so that val = evaluateAt(n) via
   * Newton-Raphson Method, since usually these equations are
   * monotonically growing, without zero derivative
   * @return solution if found otherwise a negative number,
   *           if failed to find a solution.
   */
  virtual long double solveFor(long double val) const {
    BOOST_LOG_FUNCTION();
    LTRC_ << " val: " << val;
    const int MAX_ITER = 30;  // maximum number of iterations
    long double xStart = 0;
    long double xEnd;

    // find upper bound on the root
    for (xEnd = 16; (xEnd > 0) && (val >= evaluateAt(xEnd)); xEnd *= 2)
      ;

    long double root = (xEnd - xStart) / 2;  // initial guess
    long double dx, f, df;
    LTRC_ << "root: " << root << ", xStart: " << xStart << ", xEnd: " << xEnd;
    for (int i = 0; i < MAX_ITER; i++) {
      f = evaluateAt(root) - val;
      df = evaluateDerivAt(root);
      dx = f / df;
      LTRC_ << i << " f: " << f << " df: " << df << "  dx: " << dx;
      root -= dx;
      LTRC_ << i << "  root: " << root;
      if (root < xStart || root > xEnd) {
        return -2;
      }                         // root jumped out
      if (std::fabs(dx) < 1.0)  // more than enough for integral approx
        return root;
    }
    return -1;  // could not narrow down the root
  }

  virtual ~Equation() {}

  /**
   * @brief Compute intersection point n where a(n) == b(n)
   */
  static long double intersectsAt(const Equation& a, const Equation& b) {
    BOOST_LOG_FUNCTION();
    LTRC_ << "a=" << a << "  b=" << b;
    const int MAX_ITER = 30;               // maximum number of iterations
    long double xStart = (1 << MAX_ITER);  // initial large min guess
    long double xEnd = xStart * 2;         // initial max guess to be adjusted

    // Adjust max range
    int sign = (a.evaluateAt(xStart) > b.evaluateAt(xStart)) ? 1 : -1;
    int newSign = sign;
    int i;
    LTRC_ << "sign at " << xStart << " is " << sign;
    for (i = 0; (newSign == sign) && (i < MAX_ITER); xEnd *= 2, i++) {
      newSign = (a.evaluateAt(xEnd) > b.evaluateAt(xEnd)) ? 1 : -1;
      LTRC_ << "sign at " << xEnd << " is " << newSign;
    }
    if (i >= MAX_ITER) {  // initial guess was too high
      xEnd = 1024;
      LTRC_ << "initial guess was too high, reducing lower bound, setting xEnd "
               "to "
            << xEnd;
      for (i = 0; (newSign == sign) && (i < MAX_ITER); xStart /= 2, i++) {
        newSign = (a.evaluateAt(xStart) > b.evaluateAt(xStart)) ? 1 : -1;
        LTRC_ << "sign at " << xStart << " is " << newSign;
      }
      if (i >= MAX_ITER) return -3;
      xStart = 2 * xStart;
      xEnd = 2 * xStart;
    } else {
      xEnd = xEnd / 2;
      xStart = xEnd / 2;
    }
    long double root = (xEnd + xStart) / 2;  // initial guess
    long double dx, f, df;
    LTRC_ << "root: " << root << ", xStart: " << xStart << ", xEnd: " << xEnd;
    for (int i = 0; i < MAX_ITER; i++) {
      f = a.evaluateAt(root) - b.evaluateAt(root);
      df = a.evaluateDerivAt(root) - b.evaluateDerivAt(root);
      dx = f / df;
      LTRC_ << i << " f: " << f << " df: " << df << "  dx: " << dx;
      root -= dx;
      LTRC_ << i << "  root: " << root;
      if (root < xStart || root > xEnd) {
        return -2;
      }                         // root jumped out
      if (std::fabs(dx) < 0.1)  // more than enough for integral approx
        return root;
    }
    return -1;
  }
};

inline std::ostream& operator<<(std::ostream& os, const Equation& ce) {
  bool first = true;
  const long double* coeffs = ce.getCoeffs();
  const char** names = ce.getTermNames();
  for (int i = ce.getNTerms() - 1; i >= 0; --i) {
    if (coeffs[i] > 1e-9) {
      if (!first) os << "+";
      os << coeffs[i] << "*" << names[i];
      first = false;
    } else if (coeffs[i] < 1e-9) {
      os << coeffs[i] << "*" << names[i];
      first = false;
    }
  }
  return os;
}

class LinearEquation : public Equation {
  static const int NTERMS = 2;
  long double coeffs_[NTERMS];
  static const char* names_[];

 public:
  LinearEquation() {
    coeffs_[0] = 0.0;
    coeffs_[1] = 0.0;
  }
  virtual int getNTerms() const { return NTERMS; }
  virtual long double* getCoeffs() { return coeffs_; }
  virtual const long double* getCoeffs() const { return coeffs_; }
  virtual const char** getTermNames() const { return names_; }
  virtual void evaluateTermsAt(long double n, long double* vals) const {
    vals[0] = 1;
    vals[1] = n;
  }
  virtual long double evaluateAt(long double n) const {
    long double vals[NTERMS];
    evaluateTermsAt(n, vals);
    long double sum = 0.0;
    for (int i = 0; i < NTERMS; i++) {
      sum += vals[i] * coeffs_[i];
    }
    return sum;
  }
  virtual long double evaluateDerivAt(long double) const {
    // independent of parameter
    return coeffs_[1];
  }
};

class BigOLinearEquation : public Equation {
  static const int NTERMS = 1;
  long double coeffs_[NTERMS];
  static const char* names_[];

 public:
  BigOLinearEquation() { coeffs_[0] = 0.0; }

  virtual int getNTerms() const { return NTERMS; }

  virtual long double* getCoeffs() { return coeffs_; }

  virtual const long double* getCoeffs() const { return coeffs_; }

  virtual const char** getTermNames() const { return names_; }

  virtual void evaluateTermsAt(long double n, long double* vals) const {
    vals[0] = n;
  }

  virtual long double evaluateAt(long double n) const {
    long double vals[NTERMS];
    evaluateTermsAt(n, vals);
    long double sum = 0.0;
    for (int i = 0; i < NTERMS; i++) {
      sum += vals[i] * coeffs_[i];
    }
    return sum;
  }

  virtual long double evaluateDerivAt(long double) const {
    // independent of parameter
    return coeffs_[0];
  }
};

class QuadEquation : public Equation {
  static const int NTERMS = 3;
  long double coeffs_[NTERMS];
  static const char* names_[];

 public:
  QuadEquation() {
    coeffs_[0] = 0.0;
    coeffs_[1] = 0.0;
    coeffs_[2] = 0.0;
  }

  virtual int getNTerms() const { return NTERMS; }

  virtual long double* getCoeffs() { return coeffs_; }

  virtual const long double* getCoeffs() const { return coeffs_; }

  virtual const char** getTermNames() const { return names_; }

  virtual void evaluateTermsAt(long double n, long double* vals) const {
    vals[0] = 1;
    vals[1] = n;
    vals[2] = n * n;
  }

  virtual long double evaluateAt(long double n) const {
    long double vals[NTERMS];
    evaluateTermsAt(n, vals);
    long double sum = 0.0;
    for (int i = 0; i < NTERMS; i++) {
      sum += vals[i] * coeffs_[i];
    }
    return sum;
  }

  virtual long double evaluateDerivAt(long double n) const {
    return 2.0 * n * coeffs_[2] + coeffs_[1];
  }
};

class BigOQuadEquation : public Equation {
  static const int NTERMS = 1;
  long double coeffs_[NTERMS];
  static const char* names_[];

 public:
  BigOQuadEquation() { coeffs_[0] = 0.0; }

  virtual int getNTerms() const { return NTERMS; }

  virtual long double* getCoeffs() { return coeffs_; }

  virtual const long double* getCoeffs() const { return coeffs_; }

  virtual const char** getTermNames() const { return names_; }

  virtual void evaluateTermsAt(long double n, long double* vals) const {
    vals[0] = n * n;
  }

  virtual long double evaluateAt(long double n) const {
    long double vals[NTERMS];
    evaluateTermsAt(n, vals);
    long double sum = 0.0;
    for (int i = 0; i < NTERMS; i++) {
      sum += vals[i] * coeffs_[i];
    }
    return sum;
  }

  virtual long double evaluateDerivAt(long double n) const {
    return 2.0 * n * coeffs_[0];
  }
};

class NLogN2Equation : public Equation {
  static const int NTERMS = 4;
  long double coeffs_[NTERMS];
  static const char* names_[];

 public:
  NLogN2Equation() {
    coeffs_[0] = 0.0;
    coeffs_[1] = 0.0;
    coeffs_[2] = 0.0;
    coeffs_[3] = 0.0;
  }

  virtual int getNTerms() const { return NTERMS; }

  virtual long double* getCoeffs() { return coeffs_; }

  virtual const long double* getCoeffs() const { return coeffs_; }

  virtual const char** getTermNames() const { return names_; }

  virtual void evaluateTermsAt(long double n, long double* vals) const {
    long double lg = std::log(n);
    vals[3] = n * lg * lg;
    vals[2] = n * lg;
    vals[1] = n;
    vals[0] = 1;
  }

  virtual long double evaluateAt(long double n) const {
    long double vals[NTERMS];
    evaluateTermsAt(n, vals);
    long double sum = 0.0;
    for (int i = 0; i < NTERMS; i++) {
      sum += vals[i] * coeffs_[i];
    }
    return sum;
  }

  virtual long double evaluateDerivAt(long double n) const {
    long double lg = std::log(n);
    return (lg * lg + 2 * lg) * coeffs_[3] + (lg + 1) * coeffs_[2] + coeffs_[1];
  }
};

class BigONLogN2Equation : public Equation {
  static const int NTERMS = 1;
  long double coeffs_[NTERMS];
  static const char* names_[];

 public:
  virtual int getNTerms() const { return NTERMS; }

  virtual long double* getCoeffs() { return coeffs_; }

  virtual const long double* getCoeffs() const { return coeffs_; }

  virtual const char** getTermNames() const { return names_; }

  virtual void evaluateTermsAt(long double n, long double* vals) const {
    long double lg = std::log(n);
    vals[0] = n * lg * lg;
  }

  virtual long double evaluateAt(long double n) const {
    long double vals[NTERMS];
    evaluateTermsAt(n, vals);
    long double sum = 0.0;
    for (int i = 0; i < NTERMS; i++) {
      sum += vals[i] * coeffs_[i];
    }
    return sum;
  }

  virtual long double evaluateDerivAt(long double n) const {
    long double lg = std::log(n);
    return (lg * lg + 2 * lg) * coeffs_[0];
  }
};

}  // namespace complexity

#undef LOG_MODULE
#endif  // NTRP_COMPLEXITY_EQUATION_H_
