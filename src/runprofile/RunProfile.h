#ifndef SLP_POLY_RUNPROFILE_H
#define SLP_POLY_RUNPROFILE_H

/**
 * @file RunProfile.h Definition of RunProfile and InterpProfile classes.
 */

#include "runprofile/ActionEnum.h"
#include "util/LogLevelEnum.h"
#include "util/LogModuleEnum.h"
#include "util/LogTagEnum.h"

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

#include <string>


namespace runprofile {

class InterpProfile;  // InterpProfile.h
class RunProfile {
public:
  RunProfile();
  ~RunProfile();

  /** Should overwrite existing output file */
  bool isOverwrite() const { return overwrite_; }
  /** Set whether to overwrite existing output file */
  void setOverwrite(bool val)  { overwrite_ = val; }

  void setInputFileName(const std::string& filename) {
    inputFileName_ = filename;
  }
  void setOutputFileName(const std::string& filename) {
    outputFileName_ = filename;
  }
  void setTraceFileName(const std::string& filename) {
    traceFileName_ = filename;
  }
  void setInfoFileName(const std::string& filename) {
    infoFileName_ = filename;
  }

  std::string getCommandLine() const;
  std::string getUsageHelp() const;
  std::string getUsageHelp(ActionEnum action) const;

  ActionEnum getAction() const { return action_; }
  void setAction(ActionEnum a) { action_ = a; }

  const logging::LogLevelEnum& getLogLevel(logging::LogModuleEnum m) const {
    return logLevel_[m.getIndex()];
  }

  void setLogLevel(logging::LogModuleEnum module, const logging::LogLevelEnum& level) {
    logLevel_[module.getIndex()] = level;
  }

  const std::vector<logging::LogTagEnum>& getLogTags() const {
    return logTags_;
  }

  void addLogTag(const logging::LogTagEnum& tag) {
    logTags_.push_back(tag);
  }

  boost::shared_ptr<InterpProfile const> getInterpProfile() const {
    return interpProfile_;
  }

  void setInterpProfile(boost::shared_ptr<InterpProfile>& iprof);

  const std::string& getInputFileName() const {
    return inputFileName_;
  }

  const std::string& getOutputFileName() const {
    return outputFileName_;
  }

  const boost::optional<std::string>& getTraceFileName() const {
    return traceFileName_;
  }

  const boost::optional<std::string>& getInfoFileName() const {
    return infoFileName_;
  }

  const std::string& getOptionDescription() const {
    return description_;
  }

  void setOptionDescription(const std::string& desc) {
    description_ = desc;
  }
private:
  std::string description_;
  ActionEnum action_;
  logging::LogLevelEnum logLevel_[logging::LogModuleEnum::NUM_ENUMS];
  std::vector<logging::LogTagEnum> logTags_;

  boost::shared_ptr<InterpProfile> interpProfile_;

  std::string inputFileName_;
  std::string outputFileName_;
  bool overwrite_;              /**< Overwrite output file ? */

  boost::optional<std::string> traceFileName_;
  boost::optional<std::string> infoFileName_;
};

std::ostream& operator << (std::ostream& os, const RunProfile& profile);

}  // namespace runprofile

#endif  // SLP_POLY_RUNPROFILE_H
