#ifdef FIELD_AC32
/**
 * @file AC32.cpp Run interpolation using two hardcoded primes
 *
 */

#include "Interpolation.h"
#include "FieldBenchmarks.h"

#include "math/field/Field_uint2x.h"
#include "math/util/prime.h"

#include "util/SmartAssert.h"
#include "util/log.h"

#define LOG_MODULE ::logging::Module::RUN

namespace run {

typedef math::field::Field_uint2x<uint32_t,math::Prime<uint32_t>::A,math::Prime<uint32_t>::C> field;

int Interpolation::runAC32(
    const boost::shared_ptr<context::InterpContext>& ctxtPtr,
    util::MultiIndexMap* m
) {

  LAPP_ << "Interpolating with AC32 (Hardcoded choice A & C primes): "
        << field::getName();

  return runWithField<field>(ctxtPtr, m);
}

int FieldBenchmarks::runAC32(util::MultiIndexMap* m) {
  LAPP_ << "Benchmarking with AC32 (Hardcoded A & C primes): "
        << field::getName();

  return runWithField<field>(m);
}


} // namespace run

#endif // FIELD_AC32
