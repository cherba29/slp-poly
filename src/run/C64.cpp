#ifdef FIELD_C64
/**
 * @file C64.cpp Run interpolation using first single hardcode prime
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
typedef math::field::Field_uint<uint64_t,math::Prime<uint64_t>::C> field;

int Interpolation::runC64(
    const boost::shared_ptr<context::InterpContext>& ctxtPtr,
    util::MultiIndexMap* m
) {
  LAPP_ << "Interpolating with C64 (Hardcoded choice C prime): "
        << field::getName();

  return runWithField<field>(ctxtPtr, m);
}

int FieldBenchmarks::runC64(util::MultiIndexMap* m) {
  LAPP_ << "Benchmarking with C64 (Hardcoded choice C prime): "
        << field::getName();

  return runWithField<field>(m);
}

} // namespace run

#endif // FIELD_C64
