#ifdef FIELD_CRT64
/**
 * @file CRT32.cpp Run interpolation using set of primes
 *
 */

#include "FieldBenchmarks.h"
#include "Interpolation.h"
#include "math/field/CRT.h"
#include "math/util/prime.h"
#include "util/SmartAssert.h"
#include "util/log.h"

#include <vector>

#define LOG_MODULE ::logging::Module::RUN

namespace run {

typedef math::field::CRT<uint64_t> field;

int Interpolation::runCRT64(
    const boost::shared_ptr<context::InterpContext>& ctxtPtr,
    util::MultiIndexMap* m) {
  const std::vector<uint64_t>& primes = interpProfile_->getFieldPrimes();

  ASSERT0(primes.size() > 0)
  (primes.size()).msg("There must be at least one value for CRT64 option");

  field::setPrimes(&primes[0], primes.size());

  LAPP_ << "Interpolating with CRT64 : " << field::getName();

  return runWithField<field>(ctxtPtr, m);
}

int FieldBenchmarks::runCRT64(util::MultiIndexMap* m) {
  uint64_t primes[] = {math::Prime<uint64_t>::A, math::Prime<uint64_t>::B,
                       math::Prime<uint64_t>::C};

  math::field::CRT<uint64_t>::setPrimes(primes,
                                        sizeof(primes) / sizeof(primes[0]));

  LAPP_ << "Benchmarking with CRT64 : " << field::getName();

  return runWithField<field>(m);
}

}  // namespace run
#endif  // FIELD_CRT64
