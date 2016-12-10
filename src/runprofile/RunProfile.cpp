/**
 * @file RunProfile.cpp Implementation of RunProfile
 */

#include "runprofile/RunProfile.h"
#include "runprofile/InterpProfile.h"

#include <iomanip>
#include <sstream>


namespace runprofile {

RunProfile::RunProfile()
    : action_(ActionEnum::UNKNOWN), overwrite_(false) {
  for (int i = 0; i < logging::LogModuleEnum::NUM_ENUMS; ++i) {
    logLevel_[i] = logging::LogLevelEnum::WARNING;
  }
  logLevel_[logging::LogModuleEnum::APPLICATION] = logging::LogLevelEnum::INFO2;
}

RunProfile::~RunProfile() {
}

void RunProfile::setInterpProfile(boost::shared_ptr<InterpProfile>& iprof) {
  interpProfile_ = iprof;
}

std::ostream& operator << (std::ostream& os, const RunProfile& profile) {
  os << " Log levels " << std::endl;
  for (int i = 0; i < logging::LogModuleEnum::NUM_ENUMS; i++) {
    logging::LogModuleEnum module(i);
    os << std::setw(20) << std::right << module << ": "
       <<  profile.getLogLevel(module) << std::endl;
  }

  if (profile.getTraceFileName()) {
    os << "        Trace file: " << *(profile.getTraceFileName()) << std::endl;
  } else {
    os << "        Trace file: not set" << std::endl;
  }
  if (profile.getInfoFileName()) {
    os << "   Runtime info file: " << *(profile.getInfoFileName()) << std::endl;
  } else {
    os << "   Runtime info file: not set" << std::endl;
  }
  os << "     Input file name: " << profile.getInputFileName() << std::endl;
  os << "    Output file name: " << profile.getOutputFileName() << std::endl;
  os << "           Overwrite: "
     << ((profile.isOverwrite()) ? "Yes" : "No") << std::endl;

  const boost::shared_ptr<InterpProfile const>& iprof
      = profile.getInterpProfile();

  if (iprof) {
    if (0
#ifdef FIELD_DYNAMIC32
        || iprof->getFieldOption() == FieldOptionEnum::DYNAMIC32
#endif
#ifdef FIELD_CRT32
        || iprof->getFieldOption() == FieldOptionEnum::CRT32
#endif
#ifdef FIELD_DYNAMIC64
        || iprof->getFieldOption() == FieldOptionEnum::DYNAMIC64
#endif
#ifdef FIELD_CRT64
        || iprof->getFieldOption() == FieldOptionEnum::CRT64
#endif
        ) {
      std::ostringstream oss;
      const std::vector<uint64_t>& primes = iprof->getFieldPrimes();
      for (unsigned int i = 0; i < primes.size(); ++i) {
        if (i) oss << ',';
        oss << primes[i];
      }
      os << "               Field: " << iprof->getFieldOption().toString()
         << " " << oss.str() << std::endl;
    } else {
      os << "               Field: " << iprof->getFieldOption().toString() << std::endl;
    }
    if (iprof->getRandomSeed())
      os << "         Random seed: " << *(iprof->getRandomSeed()) << std::endl;
    else
      os << "         Random seed: not set" << std::endl;
    os << "        Verify stage: " << iprof->getVerifyStage() << std::endl;;
    os << "        Verify final: " << iprof->getVerifyFinal() << std::endl;;
    os << "               Order: " << iprof->getVarOrder() << std::endl;;
    os << "      Univariate Alg: " << iprof->getUnivInterpAlg() << std::endl;;
    os << "     Vandermonde Alg: " << iprof->getVandermondeSolver() << std::endl;;
    os << "             Pruning: " << iprof->getPrunningChoice() << std::endl;;
    if (iprof->getSaveFileName()) {
      os << "           Save file: " << *(iprof->getSaveFileName());
    } else {
      os << "           Save file: not set";
    }
  }
  return os;
}

}  // namespace runprofile
