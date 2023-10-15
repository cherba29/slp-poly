#ifdef FIELD_B32
/**
 * @file B32.cpp Run interpolation using first single hardcode prime
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

typedef math::field::Field_uint<uint32_t, math::Prime<uint32_t>::B> field;

int Interpolation::runB32(
    const boost::shared_ptr<context::InterpContext>& ctxtPtr,
    util::MultiIndexMap* m) {
  LAPP1_ << "Interpolating with B32 (Hardcoded choice B prime) "
         << field::getName();

  return runWithField<field>(ctxtPtr, m);
}

int FieldBenchmarks::runB32(util::MultiIndexMap* m) {
  LAPP_ << "Benchmarking with B32 (Hardcoded choice A prime): "
        << field::getName();

  return runWithField<field>(m);
}

}  // namespace run

#endif  // FIELD_B32
