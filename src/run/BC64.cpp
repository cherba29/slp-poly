#ifdef FIELD_BC64
/**
 * @file BC64.cpp Run interpolation using two hardcoded primes
 *
 */

#include "FieldBenchmarks.h"
#include "Interpolation.h"
#include "math/field/Field_uint2x.h"
#include "math/util/prime.h"
#include "util/SmartAssert.h"
#include "util/log.h"

#define LOG_MODULE ::logging::Module::RUN

namespace run {

typedef math::field::Field_uint2x<uint64_t, math::Prime<uint64_t>::B,
                                  math::Prime<uint64_t>::C>
    field;

int Interpolation::runBC64(
    const boost::shared_ptr<context::InterpContext>& ctxtPtr,
    util::MultiIndexMap* m) {
  LAPP_ << "Interpolating with BC64 (Hardcoded choice B & C primes): "
        << field::getName();

  return runWithField<field>(ctxtPtr, m);
}

int FieldBenchmarks::runBC64(util::MultiIndexMap* m) {
  LAPP_ << "Benchmarking with BC64 (Hardcoded B & C primes): "
        << field::getName();

  return runWithField<field>(m);
}

}  // namespace run

#endif  // FIELD_BC64
