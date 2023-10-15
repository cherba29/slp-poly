#ifndef NTRP_MATH_BENCHMARK_VANDERMONDE_SOLVER_H_
#define NTRP_MATH_BENCHMARK_VANDERMONDE_SOLVER_H_

#include "complexity/Equation.h"
#include "complexity/Tracker.h"
#include "math/vandermonde/FFTSolver.h"
#include "math/vandermonde/QuadSolver.h"

#include <boost/scoped_array.hpp>
#include <boost/timer.hpp>

#ifdef max
#undef max
#endif

namespace operation {
namespace vandermonde {

template <typename F>
class QuadSetup {
 public:
  typedef complexity::QuadEquation ComplexityEquation;

  long double operator()(long double size) {
    if (size < 1) {
      return 0.0;
    }
    if (size > std::numeric_limits<unsigned int>::max()) {
      return 0;
    }
    unsigned int sz = static_cast<unsigned int>(size);
    boost::scoped_array<F> testEntries(new F[sz]);
    boost::scoped_array<F> result(new F[sz]);
    testEntries[0].template setTo<1>();
    for (unsigned int i = 1; i < sz; i++) {
      testEntries[i] = testEntries[i - 1] + testEntries[0];
    }

    boost::timer tm;
    tm.restart();
    math::vandermonde::QuadSolver<F> testVand(sz, testEntries.get());
    // testVand.solveTranspose(testEntries.get(), result.get());
    return tm.elapsed();
  }
};

template <typename F>
class QuadSolve {
 public:
  typedef complexity::QuadEquation ComplexityEquation;

  long double operator()(long double size) {
    if (size < 1) {
      return 0.0;
    }
    if (size > std::numeric_limits<unsigned int>::max()) {
      return 0;
    }
    unsigned int sz = static_cast<unsigned int>(size);
    boost::scoped_array<F> testEntries(new F[sz]);
    boost::scoped_array<F> result(new F[sz]);
    testEntries[0].template setTo<1>();
    for (unsigned int i = 1; i < sz; i++) {
      testEntries[i] = testEntries[i - 1] + testEntries[0];
    }

    boost::timer tm;
    math::vandermonde::QuadSolver<F> testVand(sz, testEntries.get());
    tm.restart();
    testVand.solveTranspose(testEntries.get(), result.get());

    return tm.elapsed();
  }
};

template <typename F>
class FFTSetup {
 public:
  typedef complexity::NLogN2Equation ComplexityEquation;

  long double operator()(long double size) {
    if (size < 1) {
      return 0.0;
    }
    if (size > std::numeric_limits<unsigned int>::max()) {
      return 0;
    }
    unsigned int sz = static_cast<unsigned int>(size);
    boost::scoped_array<F> testEntries(new F[sz]);
    boost::scoped_array<F> result(new F[sz]);
    testEntries[0].template setTo<1>();
    for (unsigned int i = 1; i < sz; i++) {
      testEntries[i] = testEntries[i - 1] + testEntries[0];
    }

    boost::timer tm;
    tm.restart();
    math::vandermonde::FFTSolver<F> testVand(sz, testEntries.get());
    // testVand.solveTranspose(testEntries.get(), result.get());
    return tm.elapsed();
  }
};

template <typename F>
class FFTSolve {
 public:
  typedef complexity::NLogN2Equation ComplexityEquation;

  long double operator()(long double size) {
    if (size < 1) {
      return 0.0;
    }
    if (size > std::numeric_limits<unsigned int>::max()) {
      return 0;
    }
    unsigned int sz = static_cast<unsigned int>(size);
    boost::scoped_array<F> testEntries(new F[sz]);
    boost::scoped_array<F> result(new F[sz]);
    testEntries[0].template setTo<1>();
    for (unsigned int i = 1; i < sz; i++) {
      testEntries[i] = testEntries[i - 1] + testEntries[0];
    }

    boost::timer tm;
    math::vandermonde::FFTSolver<F> testVand(sz, testEntries.get());
    tm.restart();
    testVand.solveTranspose(testEntries.get(), result.get());

    return tm.elapsed();
  }
};

}  // namespace vandermonde
}  // namespace operation

#endif  // NTRP_MATH_BENCHMARK_VANDERMONDE_SOLVER_H_
