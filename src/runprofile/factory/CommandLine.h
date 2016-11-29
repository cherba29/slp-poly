#ifndef NTRP_RUNPROFILE_FACTORY_COMMANDLINE_H
#define NTRP_RUNPROFILE_FACTORY_COMMANDLINE_H

/**
 * @file CommandLine.h Command line parser / RunProfile factory.
 */

#include "runprofile/RunProfile.h"

#include <memory>

namespace runprofile {
namespace factory {

class CommandLine {
  CommandLine();
  ~CommandLine();
public:
  /**
   * @brief Construct runprofile given command line parameters
   * Can throw exception if command line does not follow excepted format.
   */
  static std::unique_ptr<RunProfile> getRunProfile(int argc, char* argv[]);
};

class CommandLineException : public std::exception {
public:
};

class ConflictingOptionException : public CommandLineException {
};

class RequiredOptionException : public CommandLineException {
};

class BadOptionException : public CommandLineException {
};

}  // namespace factory
}  // namespace runprofile
#endif  // NTRP_RUNPROFILE_FACTORY_COMMANDLINE_H
