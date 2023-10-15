#ifdef FIELD_BC32
/**
 * @file BC32.cpp Run interpolation using two hardcoded primes
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

typedef math::field::Field_uint2x<uint32_t, math::Prime<uint32_t>::B,
                                  math::Prime<uint32_t>::C>
    field;

int Interpolation::runBC32(
    const boost::shared_ptr<context::InterpContext>& ctxtPtr,
    util::MultiIndexMap* m) {
  LAPP_ << "Interpolating with BC32 (Hardcoded choice B & C primes): "
        << field::getName();

  return runWithField<field>(ctxtPtr, m);
}

int FieldBenchmarks::runBC32(util::MultiIndexMap* m) {
  LAPP_ << "Benchmarking with BC32 (Hardcoded B & C primes): "
        << field::getName();

  return runWithField<field>(m);
}

}  // namespace run

#endif  // FIELD_BC32
