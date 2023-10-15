#ifdef FIELD_A64
/**
 * @file A64.cpp Run interpolation using first single hardcode prime
 *
 */

#include "FieldBenchmarks.h"
#include "Interpolation.h"
#include "math/field/Field_uint.h"
#include "math/util/prime.h"
#include "util/SmartAssert.h"
#include "util/log.h"

#define LOG_MODULE ::logging::Module::RUN

namespace run {
typedef math::field::Field_uint<uint64_t, math::Prime<uint64_t>::A> field;

int Interpolation::runA64(
    const boost::shared_ptr<context::InterpContext>& ctxtPtr,
    util::MultiIndexMap* m) {
  LAPP_ << "Interpolating with A64 (Hardcoded choice A prime): "
        << field::getName();

  return runWithField<field>(ctxtPtr, m);
}

int FieldBenchmarks::runA64(util::MultiIndexMap* m) {
  LAPP_ << "Benchmarking with A64 (Hardcoded choice A prime): "
        << field::getName();

  return runWithField<field>(m);
}

}  // namespace run

#endif  // #ifdef FIELD_A64
