#ifdef FIELD_A32
/**
 * @file A32.cpp Run interpolation using first single hardcode prime.
 *
 */

#include "Interpolation.h"
#include "FieldBenchmarks.h"

#include "math/field/Field_uint.h"
#include "math/util/prime.h"

#include "util/SmartAssert.h"
#include "util/log.h"

#define LOG_MODULE ::logging::Module::RUN

namespace run {

typedef math::field::Field_uint<uint32_t, math::Prime<uint32_t>::A> field;

int Interpolation::runA32(
    const boost::shared_ptr<context::InterpContext>& ctxtPtr,
    util::MultiIndexMap* m) {

  LAPP_ << "Interpolating with A32 (Hardcoded choice A prime): "
        << field::getName();

  return this->runWithField<field>(ctxtPtr, m);
}

int FieldBenchmarks::runA32(util::MultiIndexMap* m) {
  LAPP_ << "Benchmarking with A32 (Hardcoded choice A prime): "
    << field::getName();

  return this->runWithField<field>(m);
}

} // namespace run
#endif // #ifdef FIELD_A32
