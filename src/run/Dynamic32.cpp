#ifdef FIELD_DYNAMIC32
/**
 * @file Dynamic32.cpp Run interpolation dynamically specified prime
 *
 */

#include "Interpolation.h"
#include "FieldBenchmarks.h"
#include "math/field/Field.h"

#define LOG_MODULE ::logging::Module::RUN

namespace run {

typedef math::field::Field<uint32_t> field;

int Interpolation::runDynamic32(
         const boost::shared_ptr<context::InterpContext>& ctxtPtr,
         util::MultiIndexMap* m) {

  const std::vector<uint64_t>& primes = interpProfile_->getFieldPrimes();

  ASSERT0(primes.size() == 1)(primes.size())
    .msg("There should be exactly one prime specified for this option");

  ASSERT0(primes[0] < 0xFFFFFFFFUL)(primes[0])
    .msg("Value must fit into 32-bit integer");

  field::setPrime(static_cast<field::ubase_type>(primes[0]));

  return runWithField<field>(ctxtPtr, m);
}


int FieldBenchmarks::runDynamic32(util::MultiIndexMap* m) {

  field::setPrime(math::Prime<uint32_t>::A);

  LAPP_ << "Benchmarking with Dynamic32: " << field::getName();

  return runWithField<field>(m);
}


} // namespace run

#endif // FIELD_DYNAMIC32
