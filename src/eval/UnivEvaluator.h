#ifndef NTRP_EVAL_UNIV_EVALUATOR_H
#define NTRP_EVAL_UNIV_EVALUATOR_H

/**
 * @file UnivEvaluator.h Definition of eval::UnivEvaluator
 */

#include "Evaluator.h"

namespace eval {

/**
 * @brief This class represents a black box function
 *        in single variable. Implements interface of Evaluator
 */
template <typename F>
class UnivEvaluator {
  F* vals_;
  Evaluator<F>& evaluator_;
  int idx_;

 public:
  UnivEvaluator(Evaluator<F>& e, int idx, const F* vals)
      : evaluator_(e), idx_(idx) {
    int nVars = evaluator_.getNumberOfVariables();
    // @todo: use uniqur_ptr.
    vals_ = new F[nVars];
    for (int i = 0; i < nVars; i++) {
      vals_[i] = vals[i];
    }
  }
  int getNumberOfVariables() const { return 1; }

  ~UnivEvaluator() { delete[] vals_; }

  F operator()(const F* val) {
    vals_[idx_] = *val;
    return evaluator_(vals_);
  }
};

}  // namespace eval

#endif  // NTRP_EVAL_UNIV_EVALUATOR_H
