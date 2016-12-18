#ifdef FIELD_AB64
/**
 * @file AB64.cpp Run interpolation using two hardcoded primes
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

typedef math::field::Field_uint2x<uint64_t,math::Prime<uint64_t>::A,math::Prime<uint64_t>::B> field;

int Interpolation::runAB64(
    const boost::shared_ptr<context::InterpContext>& ctxtPtr,
    util::MultiIndexMap* m
) {

  LAPP_ << "Interpolating with AB64 (Hardcoded choice A & B primes): "
        << field::getName();

  return runWithField<field>(ctxtPtr, m);
}

int FieldBenchmarks::runAB64(util::MultiIndexMap* m) {
  LAPP_ << "Benchmarking with AB64 (Hardcoded A & B primes): "
        << field::getName();

  return runWithField<field>(m);
}


} // namespace run

#endif // FIELD_AB64
