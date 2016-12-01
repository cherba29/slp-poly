/*
 * Copyright 2016 Arthur D. Cherba
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file main.cpp Entry point for the application.
 */

#include <iostream>

#include "ReturnValue.h"
#include "platform/Platform.h"
#include "runprofile/factory/CommandLine.h"
#include "runprofile/RunProfile.h"
#include "util/log.h"

void showVersion(std::ostream& os) {
  os << "  Version "
     << Platform::getMajorVersion() << "."
     << Platform::getMinorVersion() << "."
     << Platform::getBugFixVersion()
     << " built "  << Platform::getBuildTime()
     << " Revision " << Platform::getCommitHash()
     << " Committed on " << Platform::getCommitDateTime() << std::endl;
  os << "    Compiled with " << Platform::getCompilerName()
     << " version " << Platform::getCompilerVersion() << std::endl;
  os << "    Compile flags: " << Platform::getCompilerFlags() << std::endl
     << std::endl;
  os << "  Report bugs, suggestions or comments to "
     << Platform::getSite() << std::endl
     << std::endl;
  os << "  " << Platform::getSite() << std::endl
     << std::endl;
  os << Platform::getLicenseInfo() << std::endl
     << std::endl;
}


int main(int argc, char* argv[]) {
  logging::init();

  LOGGER(main);

  std::cout << Platform::getApplicationName()
            << " - " << Platform::getApplicationDescription();
  std::cout << " Version " << Platform::getVersionStr()
            << " Commit " << Platform::getCommitHash()
            << " " << Platform::getBuildType()
            << std::endl << std::endl;

  ReturnValue status = ReturnValue::UNKNOWN;

  // Parse command line arguments
  try {
    std::unique_ptr<runprofile::RunProfile> profile
        = runprofile::factory::CommandLine::getRunProfile(argc,argv);

    // TODO: set logging based on specified runprofile.

    LAPP1_ << "Run profile:" << std::endl << *profile;
 
    LAPP2_ << "Committed  on: " << Platform::getCommitDateTime();
    //LAPP2_ << "Compiled   on: " << Platform::getCompileDate() << " "
    //       << Platform::getCompileTime();
    LAPP2_ << "Compiled with: " << Platform::getCompilerName()<< " "
           << Platform::getCompilerVersion();
    LAPP2_ << "        flags: " << Platform::getCompilerFlags();
    //LAPP2_ << " Report bugs : " << Platform::getBugReportName() << " "
    //       << Platform::getBugReportEmail();
    LAPP2_ << " Project site: " << Platform::getSite();

    switch (profile->getAction().getId()) {
      case runprofile::ActionEnum::HELP:
        std::cerr << profile->getOptionDescription() << std::endl;
        status = ReturnValue::DID_NOT_RUN;
        break;
      case runprofile::ActionEnum::VERSION:
        showVersion(std::cerr);
        status = ReturnValue::SUCCESS;
        break;
      case runprofile::ActionEnum::BENCHMARK:
        //status = (0 == runBenchmark(*profile, &m))
        //    ? ReturnValue::UNKNOWN : ReturnValue::SUCCESS;
        break;
      case runprofile::ActionEnum::MACHINE_INFO:
        //status = (0 == runMachineInfo(*profile, &m))
        //    ? ReturnValue::UNKNOWN : ReturnValue::SUCCESS;
        break;
      case runprofile::ActionEnum::INTERPOLATE:
        //status = (0 == runInterpolation(*profile, &m))
        //    ? ReturnValue::UNKNOWN : ReturnValue::SUCCESS;
        break;
      default:
        throw std::logic_error("Unknown/unimplemented action");
    }

  } catch (const std::exception& e) {
    LERR_ << e.what() << std::endl;
    status = ReturnValue::BAD_CMD_LINE_OPTION;
  }
  // catch (...) { } // comment out to core dump on unexpected exceptions

  LTRC_ << "Trace message.";
  LDBG_ << "Debug message.";
  LINF2_ << "Info2 Message.";
  LINF1_ << "Info1 Message.";
  LINF_ << "Info Message.";
  LAPP2_ << "App2 Message.";
  LAPP1_ << "App2 Message.";
  LAPP_ << "App Message.";
  LWRN_ << "Warning Message.";
  LERR_ << "Error Message.";
  LFTL_ << "Fatal Message.";

  return status.getId();
}
