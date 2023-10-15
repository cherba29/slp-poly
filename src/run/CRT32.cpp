#ifdef FIELD_CRT32
/**
 * @file CRT32.cpp Run interpolation using set of primes
 *
 */

#include "FieldBenchmarks.h"
#include "Interpolation.h"
#include "math/field/CRT.h"
#include "math/util/prime.h"
#include "util/MultiIndexMap.h"
#include "util/SmartAssert.h"
#include "util/log.h"

#include <vector>

#define LOG_MODULE ::logging::LogModuleEnum::RUN

namespace run {

typedef math::field::CRT<uint32_t> field;
int Interpolation::runCRT32(
    const boost::shared_ptr<context::InterpContext>& ctxtPtr,
    util::MultiIndexMap* m) {
  const std::vector<uint64_t>& primes = interpProfile_->getFieldPrimes();

  ASSERT0(primes.size() > 0)
  (primes.size()).msg("There must be at least one value for CRT32 option");

  // conver to 32-bit vector
  std::vector<uint32_t> primes32;
  for (unsigned int i = 0; i < primes.size(); ++i) {
    primes32.push_back(static_cast<uint32_t>(primes[i]));
  }

  field::setPrimes(&primes32[0], primes32.size());

  LAPP_ << "Interpolating with CRT32 : " << field::getName();

  return runWithField<field>(ctxtPtr, m);
}

int FieldBenchmarks::runCRT32(util::MultiIndexMap* m) {
  uint32_t primes[] = {math::Prime<uint32_t>::A, math::Prime<uint32_t>::B,
                       math::Prime<uint32_t>::C};

  math::field::CRT<uint32_t>::setPrimes(primes,
                                        sizeof(primes) / sizeof(primes[0]));

  LAPP_ << "Benchmarking with CRT32 : " << field::getName();

  return runWithField<field>(m);
}

}  // namespace run

#endif  // FIELD_CRT32
