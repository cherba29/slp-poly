#ifndef NTRP_ALGEBRA_PARTIAL_POLYNOMIAL_H
#define NTRP_ALGEBRA_PARTIAL_POLYNOMIAL_H

/**
 * @file PartialPolynomial.h Declaration & Definition of
 * algebra::PartialPolynomial.
 */

#include "Polynomial.h"

#include "util/SmartAssert.h"

#include <boost/scoped_array.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>

namespace algebra {

template <typename F>
class PartialPolynomial {
  std::vector<F> fixedVals_;
  /** Set of variable indices making up the partial polynomial */
  std::vector<int> varIdxs_;
  boost::shared_ptr<Polynomial<F> > pol_;
public:
  /**
   * @brief Create partial polynomial
   * @param nVars - total number of variables including pol variables
   * @param fixedVals - values for fixed variables. fixedVals is of size nVars
   * @param varIdxs - variable indices making up this polynomial
   * @param pol - polynomial in non-fixed variables
   */
  PartialPolynomial(
      int nVars,
      const F* fixedVals,
      const std::vector<int>& varIdxs,
      boost::shared_ptr<Polynomial<F> > pol)
    : fixedVals_(nVars), varIdxs_(varIdxs), pol_(pol) {
    ASSERT1(varIdxs.size() == pol->getNumberOfVariables())(
        "Number of indices must match number of variables in the polynomial");
    std::copy(fixedVals, fixedVals+nVars, fixedVals_.begin());
  }

  /**
   * @brief Evaluate partial polynomial by mapping given values to
   * actual polynomial.
   */
  F operator()(const F* vals) const {
    boost::scoped_array<F> polVals(new F[varIdxs_.size()]);
    for (unsigned int i = 0; i < varIdxs_.size(); ++i) {
      polVals[i] = vals[varIdxs_[i]];
    }
    return (*pol_)(polVals.get());
  }

  /**
   * @brief get polynomial in non fixed variables
   */
  boost::shared_ptr<Polynomial<F> const> getPolynomial() const { return pol_; }

  /**
   * @brief Reset partial polynomial with new polynomial
   */
  void setPolynomial(const std::vector<int>& varIdxs,
                     const std::vector<F>& fixedVals,
                     const boost::shared_ptr<Polynomial<F> >& pol) {
    varIdxs_ = varIdxs;
    fixedVals_ = fixedVals;
    pol_ = pol;
  }

  /**
   *  @brief Get value of fixed ith variable
   */
  F getFixedVal(int idx) const { return fixedVals_[idx]; }

  /**
   * @brief get ith variable index
   */
  int getVarIdx(int idx) const { return varIdxs_[idx]; }

  int getTotalNumberOfVariables() const { return fixedVals_.size(); }

  // Ordering predicates for  PartialEvalPolynomial
  typedef boost::function2<
      bool, const PartialPolynomial&, const PartialPolynomial&> OrderByFunctor;

  typedef std::binary_function<
      const PartialPolynomial&, const PartialPolynomial&, bool> Compare;

  struct MinDegOrder : public Compare {
    bool operator()(const PartialPolynomial& x,
                    const PartialPolynomial& y) const {
      return x.pol_->getTotDegree() < y.pol_->getTotDegree();
    }
  };
  struct MaxDegOrder : public Compare {
    bool operator()(const PartialPolynomial<F>& x,
                    const PartialPolynomial<F>& y) const {
      return x.pol_->getTotDegree() > y.pol_->getTotDegree();
    }
  };
  struct OriginalOrder : public Compare {
    bool operator()(const PartialPolynomial<F>& x,
                    const PartialPolynomial<F>& y) const {
      return x.varIdxs_ < y.varIdxs_;
    }
  };
  struct MaxTermsOrder : public Compare {
    bool operator()(const PartialPolynomial<F>& x,
                    const PartialPolynomial<F>& y) const {
      return x.pol_->getNumberOfTerms() > y.pol_->getNumberOfTerms();
    }
  };
  struct MinTermsOrder : public Compare {
    bool operator()(const PartialPolynomial<F>& x,
                    const PartialPolynomial<F>& y) const {
      return x.pol_->getNumberOfTerms() < y.pol_->getNumberOfTerms();
    }
  };
  // define order for shared_ptr in terms of installed order by functor,
  // which can be any of the above predicates
  class PtrOrder : public std::binary_function<
      const boost::shared_ptr<PartialPolynomial>&,
      const boost::shared_ptr<PartialPolynomial>&,
      bool> {
    // Functor taking two ppols returning bool
    OrderByFunctor order_;
  public:
    PtrOrder():order_(MaxTermsOrder()) {}  // default order
    PtrOrder(const OrderByFunctor& ord) : order_(ord) {}

    bool operator()(const boost::shared_ptr<PartialPolynomial>& a,
                    const boost::shared_ptr<PartialPolynomial>& b) {
      return this->order_(*a,*b);
    }
  };
};

}  // namespace algebra

#endif  // NTRP_ALGEBRA_PARTIAL_POLYNOMIAL_H
