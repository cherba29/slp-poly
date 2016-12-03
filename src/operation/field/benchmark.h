#ifndef NTRP_OPERATION_FIELD_BENCHMARK_H_
#define NTRP_OPERATION_FIELD_BENCHMARK_H_

/**
 * @file FieldBenchmark.h
 * @brief Field Benchmark class definition
 *
 */

#include "FieldOperations.h"
#include "math/Base.h"

#include "complexity/Equation.h"
#include "complexity/Tracker.h"

#define LOG_MODULE ::logging::LogModuleEnum::OPERATION
#include "util/log.h"

#include <boost/timer.hpp>

namespace operation {
namespace field {

template <typename F>
class Add {
public:
  typedef complexity::LinearEquation ComplexityEquation;

  long double operator()(long double size) {
    F test1; test1.invertableRandom();
    F test2; test2.invertableRandom();
    F test3; F test4;

    uint64_t count = static_cast<uint64_t>(size) >> 4;
    boost::timer tm;

    for (uint64_t i = 0; i < count; i++) {
      test3 = test1 + test2;  test4 = test2 + test3;
      test1 = test3 + test4;  test2 = test1 + test4;
      test3 = test2 + test4;  test4 = test1 + test3;
      test1 = test3 + test4;  test2 = test1 + test4;
      test3 = test1 + test2;  test4 = test2 + test3;
      test1 = test3 + test4;  test2 = test1 + test4;
      test3 = test2 + test4;  test4 = test1 + test3;
      test1 = test3 + test4;  test2 = test1 + test4;
    }

    return tm.elapsed();
  }
};

template <typename F>
class Subtract {
public:
  typedef complexity::LinearEquation ComplexityEquation;

  long double operator()(long double size) {
    F test1; test1.invertableRandom();
    F test2; test2.invertableRandom();
    F test3; F test4;

    uint64_t count = static_cast<uint64_t>(size) >> 4;
    boost::timer tm;
    for (uint64_t i = 0; i < count; ++i) {
      test3 = test1 - test2;  test4 = test2 - test3;
      test1 = test3 - test4;	test2 = test1 - test4;
      test3 = test2 - test4;	test4 = test1 - test3;
      test1 = test3 - test4;	test2 = test1 - test4;
      test3 = test1 - test2;  test4 = test2 - test3;
      test1 = test3 - test4;  test2 = test1 - test4;
      test3 = test2 - test4;  test4 = test1 - test3;
      test1 = test3 - test4;  test2 = test1 - test4;
    }

    return tm.elapsed();
  }
};

template <typename F>
class Multiply {
public:
  typedef complexity::LinearEquation ComplexityEquation;

  long double operator()(long double size) {
    LTRC_ << "Estimating operation muliply of size " << size;
    F test1; test1.invertableRandom();
    F test2; test2.invertableRandom();
    F test3; F test4;

    uint64_t count = static_cast<uint64_t>(size) >> 5;
    boost::timer tm;

    for (uint64_t i = 0; i < count; i++) {
      test3 = test2 * test1; test1 = test2 * test2;
      test4 = test2 * test1; test1 = test2 * test2;
      test2 = test2 * test1; test1 = test3 * test2;
      test2 = test2 * test1; test1 = test2 * test4;
      test2 = test2 * test1; test1 = test3 * test2;
      test2 = test2 * test1; test1 = test2 * test4;
      test2 = test2 * test1; test1 = test3 * test2;
      test2 = test2 * test1; test1 = test2 * test4;
      test3 = test2 * test1; test1 = test2 * test2;
      test4 = test2 * test1; test1 = test2 * test2;
      test2 = test2 * test1; test1 = test3 * test2;
      test2 = test2 * test1; test1 = test2 * test4;
      test2 = test2 * test1; test1 = test3 * test2;
      test2 = test2 * test1; test1 = test2 * test4;
      test2 = test2 * test1; test1 = test3 * test2;
      test2 = test2 * test1; test1 = test2 * test4;
    }

    return tm.elapsed();
  }
};

template <typename F>
class Divide {
public:
  typedef complexity::LinearEquation ComplexityEquation;

  long double operator()(long double size) {
    F test1; test1.invertableRandom();
    F test2; test2.invertableRandom();
    F test3; F test4;

    uint64_t count = static_cast<uint64_t>(size) >> 4;
    boost::timer tm;
    tm.restart();

    for (uint64_t i = 0; i < count; i++) {
      test2 /= test1;
      test1 /= test2;
      test2 /= test1;
      test1 /= test2;
      test2 /= test1;
      test1 /= test2;
      test2 /= test1;
      test1 /= test2;
      test2 /= test1;
      test1 /= test2;
      test2 /= test1;
      test1 /= test2;
      test2 /= test1;
      test1 /= test2;
      test2 /= test1;
      test1 /= test2;
  	}

    return tm.elapsed();
  }
};

template <typename F>
class Inverse {
public:
  typedef complexity::LinearEquation ComplexityEquation;

  long double operator()(long double size) {
    F test1; test1.invertableRandom();
    F test2; test2.invertableRandom();
    F test3; F test4;

    uint64_t count = static_cast<uint64_t>(size) >> 4;
    boost::timer tm;
    tm.restart();

    for (uint64_t i = 0; i < count; i++) {
      test1.invert();
      test2.invert();
      test1.invert();
      test2.invert();
      test1.invert();
      test2.invert();
      test1.invert();
      test2.invert();
      test1.invert();
      test2.invert();
      test1.invert();
      test2.invert();
      test1.invert();
      test2.invert();
      test1.invert();
      test2.invert();
    }

    return tm.elapsed();
  }
};

template <typename F>
class Power {
public:
  typedef complexity::LinearEquation ComplexityEquation;

  long double operator()(long double size) {
    F test1; test1.invertableRandom();
    F test2; test2.invertableRandom();
    F test3; F test4;

    unsigned int count = static_cast<uint64_t>(size) >> 4;
    boost::timer tm;
    tm.restart();

    for (uint64_t i = 0; i < count; i++) {
      test1.powerBy(1 << (4-1));
      test2.powerBy(1 << (8-1));
      test1.powerBy(1 << (12-1));
      test2.powerBy(1 << (16-1));
      test1.powerBy(1 << (4-1));
      test2.powerBy(1 << (8-1));
      test1.powerBy(1 << (12-1));
      test2.powerBy(1 << (16-1));
      test1.powerBy(1 << (4-1));
      test2.powerBy(1 << (8-1));
      test1.powerBy(1 << (12-1));
      test2.powerBy(1 << (16-1));
      test1.powerBy(1 << (4-1));
      test2.powerBy(1 << (8-1));
      test1.powerBy(1 << (12-1));
      test2.powerBy(1 << (16-1));
    }

    return tm.elapsed();
  }
};

}  // namespace field
}  // namespace operation


#undef LOG_MODULE
#endif  // NTRP_OPERATION_FIELD_BENCHMARK_H_
