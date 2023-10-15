#ifndef NTRP_OPERATION_BENCHMARK_H_
#define NTRP_OPERATION_BENCHMARK_H_

#include "complexity/Equation.h"
#include "complexity/Tracker.h"
#include "eval/Evaluator.h"

#include <boost/shared_ptr.hpp>
#include <boost/timer.hpp>

namespace operation {
namespace eval {

template <typename F>
class Evaluator {
  ::eval::Evaluator<F>& bbeval_;

 public:
  typedef complexity::LinearEquation ComplexityEquation;

  Evaluator(::eval::Evaluator<F>& bbeval) : bbeval_(bbeval) {}

  long double operator()(long double size) {
    int nVars = bbeval_.getNumberOfVariables();
    boost::shared_ptr<F> pointSet(F::alloc(nVars), F::dealloc);
    for (int i = 0; i < nVars; ++i) {
      pointSet.get()[i].random();
    }
    boost::timer tm;
    int i = 0;
    for (uint64_t n = 0; n < size; ++n, ++i) {
      pointSet.get()[(i % nVars)] *= pointSet.get()[((i + 1) % nVars)];
      bbeval_(pointSet.get());
    }
    return tm.elapsed();
  }
};

}  // namespace eval
}  // namespace operation

#endif  // NTRP_OPERATION_BENCHMARK_H_
