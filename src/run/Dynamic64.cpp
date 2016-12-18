#ifdef FIELD_DYNAMIC64
/**
 * @file Dynamic64.cpp Run interpolation using single 64-bit hardcode prime
 *
 * $LastChangedDate: 2010-11-02 08:33:01 -0400 (Tue, 02 Nov 2010) $
 * $Date: 2010-11-02 08:33:01 -0400 (Tue, 02 Nov 2010) $
 * $LastChangedRevision: 567 $
 * $Revision: 567 $
 * $LastChangedBy: artas $
 * $Author: artas $
 * $URL$
 * $Id$
 */

#include "Interpolation.h"
#include "FieldBenchmarks.h"
#include "math/field/Field.h"

#include "util/SmartAssert.h"
#include "util/log.h"

#define LOG_MODULE ::logging::Module::RUN

namespace run {

typedef math::field::Field<uint64_t> field;

int Interpolation::runDynamic64(
  const boost::shared_ptr<context::InterpContext>& ctxtPtr,
  util::MultiIndexMap* m)
{

  const std::vector<uint64_t>& primes = interpProfile_->getFieldPrimes();
  ASSERT0(primes.size() == 1)(primes.size())
    .msg("There should be exactly one prime specified for this option");
  field::setPrime(primes[0]);

  return runWithField<field>(ctxtPtr, m);
}

int FieldBenchmarks::runDynamic64(util::MultiIndexMap* m) {

  field::setPrime(math::Prime<uint64_t>::A);

  LAPP_ << "Benchmarking with Dynamic32: "
    << field::getName();

  return runWithField<field>(m);
}

} // namespace run
#endif //FIELD_DYNAMIC64

