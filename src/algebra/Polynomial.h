#ifndef NTRP_ALGEBRA_POLYNOMIAL_H
#define NTRP_ALGEBRA_POLYNOMIAL_H

/**
 * @file Polynomial.h Definition of algebra::Polynomial
 */

#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace algebra {

template <typename F>
class Polynomial {
 public:
  /**
   * @brief Create polynomial with given variables and that many terms.
   * Initial polynomial coefficients/degrees are not set, and may contain
   * uninitialized values.
   */
  Polynomial(const std::vector<std::string>& varNames, unsigned int nTerms);

  ~Polynomial();

  /**
   * @brief get value of the coefficient of t-th term
   */
  F getCoeff(int t) const { return coeffs_[t]; }

  /**
   * @brief Compute value of t-th monomial given variable values
   */
  F monValue(int t, const F* vals) const;

  /**
   * @brief Evaluate polynomial at given set of values
   * @param vals - array of size nVars
   * @return value of the polynomial evaluated at vals;
   */
  F operator()(const F* vals) const;

  /**
   * @brief Return degree of varIdx variable in t-th term
   */
  int getDegree(int t, int varIdx) const {
    return *(terms_ + nVars_ * t + varIdx);
  }

  /**
   * @brief Compute maximum degree for given variable index
   */
  int getMaxDegree(int varIdx) const;

  /**
   * @brief Compute total degree of this polynomial O(nTerms*nvars)
   */
  int getTotDegree() const;

  /**
   * @brief Find minimum degree term in given variable and return its degree
   */
  int getMinDegree(int varIdx) const;

  /**
   * @brief Retunr number of allocated polynomial terms, note that there might
   * be zero terms which will be included in this count
   */
  unsigned int getNumberOfTerms() const { return nTerms_; }

  /**
   * @brief Get number of variables this polynomial represents
   */
  unsigned int getNumberOfVariables() const { return nVars_; }

  /**
   * @brief Get string representation of variable at index idx
   */
  const std::string& getVarName(int idx) const { return varNames_[idx]; }

  /**
   * @brief Return short string representation for this polynomial
   */
  std::string getShortName() const;

  /**
   * @brief set value of t-th coefficient
   */
  void setCoeff(int t, F val) { coeffs_[t] = val; }

  /**
   * @brief set degree of varIdx variable at t-th term
   */
  void setDegree(int t, int varIdx, int deg) {
    *(terms_ + nVars_ * t + varIdx) = deg;
  }

  /**
   * @brief Pack the polynomial removing any terms with zero coefficients
   */
  void removeZeroTerms();

 private:
  unsigned int nTerms_;
  unsigned int nVars_;
  std::vector<std::string> varNames_;
  int* terms_;  // 2D array of term degrees
  F* coeffs_;
};

/**
 * @brief Produce string representation of the polynomial
 */
template <typename F>
std::ostream& operator<<(std::ostream& os, const algebra::Polynomial<F>& p);

}  // namespace algebra

#include "Polynomial-impl.h"

#endif  // NTRP_ALGEBRA_POLYNOMIAL_H
