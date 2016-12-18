#ifdef FIELD_C32
/**
 * @file C32.cpp Run interpolation using first single hardcode prime
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

typedef math::field::Field_uint<uint32_t,math::Prime<uint32_t>::C> field;

int Interpolation::runC32(
         const boost::shared_ptr<context::InterpContext>& ctxtPtr,
         util::MultiIndexMap* m
) {
  LAPP1_ << "Interpolating with C32 (Hardcoded choice C prime) "
    << field::getName();

  return runWithField<field>(ctxtPtr, m);
}

int FieldBenchmarks::runC32(util::MultiIndexMap* m) {
  LAPP_ << "Benchmarking with C32 (Hardcoded choice C prime): "
    << field::getName();

  return runWithField<field>(m);
}


} // namespace run
#endif // FIELD_C32
