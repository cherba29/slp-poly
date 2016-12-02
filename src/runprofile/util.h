#ifndef NTRP_RUNPROFILE_UTIL_H
#define NTRP_RUNPROFILE_UTIL_H

#include "runprofile/RunProfile.h"
#include "runprofile/InterpProfile.h"
#include "util/MultiIndexMap.h"
#include "util/log.h"

#define LOG_MODULE ::logging::LogModuleEnum::MAIN

/**
 * @brief Specifies external configuration parameters for this application
 */
namespace runprofile {

/**
 * @brief Set of utilities used by runprofile package
 */
namespace util {

//void setLogging(const RunProfile& profile) {
//  std::vector<std::pair<logging::LogModuleEnum,logging::LogLevelEnum> moduleLogLevels;
//  for (int i = 0; i < logging::LogModuleEnum::NUM_ENUMS; i++) {
//    moduleLogLevels.push_back(std::make_pair(
//      logging::LogModuleEnum(i),
//      profile.getLogLevel(module)
//    ));
//  setModuleLogLevels(moduleLogLevels);
////    logging::LogModuleEnum module(i);
////    logging::g_l_filter()->set_enabled(module, profile.getLogLevel(module));
//  }
//
//  setLogTags(profile.getLogTags());
//
//}

} // namespace util
} // namespace runprofile

::util::MultiIndexMap& operator<<(
    ::util::MultiIndexMap& m,
    const runprofile::RunProfile& profile) {
  if (!profile.getOutputFileName().empty()) {
    m["output"]["filename"] = profile.getOutputFileName();
  }
  if (!profile.getInputFileName().empty()) {
    m["input"]["filename"] = profile.getInputFileName();
  }
  boost::shared_ptr<runprofile::InterpProfile const> iprof = profile.getInterpProfile();
  if (iprof) {
    if (iprof->getRandomSeed()) {
      m["input"]["randomseed"] = *(iprof->getRandomSeed());
    }
    ::util::MultiIndexMap& interp_m = m["input"]["parameter"]["interpolation"];
    interp_m["verify"]["stage"] = iprof->getVerifyStage();
    interp_m["verify"]["final"] = iprof->getVerifyFinal();
    interp_m["variable"]["order"] = iprof->getVarOrder().toString();
    interp_m["univariate"]["algorithm"] = iprof->getUnivInterpAlg().toString();
    interp_m["vandermonde"]["solver"] = iprof->getVandermondeSolver().toString();
    interp_m["pruning"] = iprof->getPrunningChoice().toString();
  }
  return m;
}


#undef LOG_MODULE
#endif  // NTRP_RUNPROFILE_UTIL_H
