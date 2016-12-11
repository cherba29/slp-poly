#ifndef NTRP_ALGEBRA_POLYNOMIAL_H
#error "Do not include this file. This is used internally by Polynomial.h"
#endif

/**
 * @file Polynomial-Impl.h
 */

#include <sstream>

namespace algebra {

template <typename F>
Polynomial<F>::Polynomial(
    const std::vector<std::string>& varNames, unsigned int nTerms)
  : nTerms_(nTerms),
    nVars_(varNames.size()),
    varNames_(varNames),
    terms_((nTerms && nVars_) ? new int[nTerms * nVars_] : NULL),
    coeffs_((nTerms) ? new F[nTerms] : NULL) {
  //for (unsigned int i = 0; i < nTerms; ++i) {
  //  for (unsigned int j = 0; j < nVars_; ++j) {
  //    terms_[nVars_*i+j] = 0;
  //  }
  //}
}

template <typename F>
Polynomial<F>::~Polynomial() {
  delete[] terms_;
  delete[] coeffs_;
}

template <typename F>
F Polynomial<F>::monValue(int t, const F* vals) const {
  F prod; prod.template setTo<1>();
  const int* term = terms_+t*nVars_;
  for (unsigned int i = 0; i < nVars_; i++) {
    prod *= vals[i].getPow(term[i]);
  }
  return prod;
}

template <typename F>
F Polynomial<F>::operator()(const F* vals) const {
  F sum; sum.template setTo<0>();
  F prod;
  const int* term = terms_;
  for (unsigned int i = 0; i < nTerms_; ++i, term+=nVars_) {
    prod = coeffs_[i];
    for (unsigned int j = 0; j < nVars_; ++j) {
      prod *= vals[j].getPow(term[j]);
    }
    sum += prod;
  }
  return sum;
}

template <typename F>
int Polynomial<F>::getMaxDegree(int varIdx) const {
  if (nTerms_ < 1) return -1;
  const int* term = terms_+varIdx;
  int max = *term; // Degree in first term
  for (unsigned int i = 1; i < nTerms_; ++i,term+=nVars_) {
    max = std::max(max, *term);
  }
  return max;
}

template <typename F>
int Polynomial<F>::getTotDegree() const {
  int max = -1;
  const int* term = terms_;
  for (unsigned int i = 0; i < nTerms_; ++i) {
    int deg = 0;
    for (unsigned int j = 0; j < nVars_; ++j, ++term) {
      deg+=*term;
    }
    max = std::max(max, deg);
  }
  return max;
}

template <typename F>
std::string Polynomial<F>::getShortName() const {
  std::ostringstream oss;  oss << '[';
  for (size_t i = 0; i < nVars_; i++) {
    if (i) { oss << ","; }
    oss << varNames_[i];
  }
  oss << ']';
  return oss.str();
}

template <typename F>
int Polynomial<F>::getMinDegree(int varIdx) const {
  if (nTerms_ < 1) return -1;
  const int* term = terms_+varIdx;
  int min = *term;
  for (unsigned int i = 1; i < nTerms_; ++i) {
    term+=nVars_ ;
    min = std::min(min, *term);
  }
  return min;
}

template <typename F>
void Polynomial<F>::removeZeroTerms() {
  unsigned int i = 0;
  for (unsigned int t = 0; t < nTerms_; ++t) {
    if (!coeffs_[t].template is<0>()) {
      if (t != i) { // copy t term to i term
        coeffs_[i] = coeffs_[t];
        for (unsigned int j = 0; j < nVars_; ++j) {
          *(terms_ + nVars_ * i + j) = *(terms_ + nVars_ * t + j);
        }
      }
      ++i;
    }
  }
  nTerms_ = i;
}

template <typename F>
std::ostream& operator << (std::ostream& os, const algebra::Polynomial<F>& p) {
  unsigned int n = p.getNumberOfTerms();
  unsigned int nVars = p.getNumberOfVariables();
//  for (int i = 0; i < n; i++) {
//    os << '+' << p.getCoeff(i);
//    for (int j = 0; j < nVars; j++) {
//      os << '*' << p.getVarName(j) << '^' << p.getDegree(i,j);
//    }
//  }
//  return os;
  for (unsigned int i = 0; i < n; i++) {
    bool firstItem = true;
    F coeff = p.getCoeff(i);
    if (coeff.template is<-1>()) {
      os << '-';
    }
    else if (coeff.template is<1>()) {
      if (i) {
        os << '+';
      }
    } else {
      if (coeff.isPositive() && i) {
        os << '+';
      }
      os << coeff;
      firstItem = false;
    }
    for (unsigned int j = 0; j < nVars; ++j) {
      int deg = p.getDegree(i,j);
      if (deg != 0) {
        if (firstItem) {
          firstItem = false;
        } else {
          os << '*';
        }
        os << p.getVarName(j);
        if (deg != 1) {
          os << '^' << deg;
        }
      }
    }
    if (firstItem) {
      os << 1;
    }
  }
  if (n < 1) {
    os << 0;
  }
  return os;
}

} // namespace algebra
