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

#include <fstream>
#include <iostream>

#include "ReturnValue.h"

#include "context/maple/Driver.h"
#include "platform/Platform.h"
#include "run/FieldBenchmarks.h"
#include "run/Interpolation.h"
#include "runprofile/factory/CommandLine.h"
#include "runprofile/RunProfile.h"
#include "runprofile/util.h"
#include "util/MultiIndexMap.h"
#include "util/Timer.h"

#define LOG_MODULE ::logging::LogModuleEnum::MAIN
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
  os << "  Report bugs, suggestions or comments to "
     << Platform::getSite() << std::endl
     << std::endl;
}

typedef enum {
  FILE_OPEN = 0,
  FILE_EXISTS = 1,
  FILE_OPEN_ERROR = 2
} OpenFileStatus;


OpenFileStatus openFileForWritting(
  std::ofstream* stream, const std::string& filename, bool newFile = true) {
  if (newFile) {
    // Try to open the file
    std::ofstream ofile(filename.c_str(), std::ios::in);
    if (!ofile.fail()) {
      return FILE_EXISTS;
    }
    ofile.close();
  }
  stream->open(filename.c_str(), std::ios::out);
  if (stream->fail()) {
    // LERR_ << "Error opening file '" << filename << "' for writing.";
    return FILE_OPEN_ERROR;
  }
  return FILE_OPEN;
}

int runMachineInfo(
    const runprofile::RunProfile& /* profile */,
    util::MultiIndexMap* /* mmap */) {

  return 1;
}


int runBenchmark(
    const runprofile::RunProfile& /* profile */, util::MultiIndexMap* mmap) {

  run::FieldBenchmarks fb;

  LAPP1_ << "Measuring speed of Field operations: ";
  fb.run(&((*mmap)["benchmark"]["field"]));

  return 0;
}


int runInterpolation(
    const runprofile::RunProfile& profile, util::MultiIndexMap* mmap) {

  boost::shared_ptr<runprofile::InterpProfile const> iprof
      = profile.getInterpProfile();

  if (!iprof) {
    LERR_ << "No interpolation profile is defined.";
    return ReturnValue::DID_NOT_RUN;
  }

  util::MultiIndexMap& m = *mmap;

  LAPP1_ << "Parsing input file '" << profile.getInputFileName() << "' ...";

  // Parse maple input file into context
  boost::shared_ptr<context::InterpContext> ctxtPtr(
      new context::InterpContext());
  maple::Driver parseDriver;
  //parseDriver.setDebugScanner(true);
  //parseDriver.setDebugParser(true);

  boost::timer tm;
  if (!parseDriver.parse_file(profile.getInputFileName(), ctxtPtr)) {
    LERR_ << "Could not parse input file: " << profile.getInputFileName();
    return ReturnValue::PARSE_ERROR;
  }
  m["input"]["parsetime"] = tm.elapsed();

  LAPP1_ << "Done parsing. Got " << ctxtPtr->getNumberOfStatements()
         << " statements to execute.";

  m["input"]["problem"]["numberofstatements"] = ctxtPtr->getNumberOfStatements();

  LAPP1_ << "Total nVars "
         << ctxtPtr->getNumberOfVariables()
         << " variables to interpolate.";

  int nVars = ctxtPtr->getNumberOfVariables();
  m["input"]["problem"]["variable"]["total"] = nVars ;
  std::ostringstream varsStr;
  for (int i = 0; i < nVars; ++i) {
    const std::string varName = ctxtPtr->getVarName(i);
    m["input"]["problem"]["variable"]["list"] += varName;
    if (i) {
      varsStr << ",";
    }
    varsStr << varName;
  }
  LAPP1_ << "Variables " << varsStr.str();

  run::Interpolation interp(iprof);

  interp.run(ctxtPtr, &(m["interpolation"]));

  return 0;
}


ReturnValue saveData(
    const util::MultiIndexMap& storage,
    const std::string& filename,
    bool shouldOverwrite) {

  // Try to open file now before computation starts
  std::ofstream ofile;
  OpenFileStatus status = openFileForWritting(&ofile,
      filename, !shouldOverwrite);

  switch(status) {
    case FILE_OPEN: // ok
      break;
    case FILE_EXISTS:
      LERR_ << "File '" << filename << "' already exist. "
               "Will not overwrite unless -w option is used.";
      return ReturnValue::FILE_ALREADY_EXISTS;
    case FILE_OPEN_ERROR:
      LERR_ << "Error opening file '" << filename
            << "' for writing.";
      return ReturnValue::OUT_FILE_OPEN_ERROR;
    default:
      LERR_ << "Unknown status opening file '" << filename
            << "' for writing.";
      return ReturnValue::UNKNOWN;
  }

  LAPP1_ << "Writing out result into file";
  ofile << storage;  // write out all results to file
  ofile.close();
  LAPP_ << "Done.";

  return ReturnValue::SUCCESS;
}


int main(int argc, char* argv[]) {
  // Initialize default logging.
  // The sink can be reconfigured later depending on command line arguments.
  boost::shared_ptr<logging::sink_t> log_sink = logging::init();

  std::cout << Platform::getApplicationName()
            << " - " << Platform::getApplicationDescription();
  std::cout << " Version " << Platform::getVersionStr()
            << " Commit " << Platform::getCommitHash()
            << std::endl << std::endl;

  ReturnValue status = ReturnValue::UNKNOWN;

  // Parse command line arguments
  try {
    std::unique_ptr<runprofile::RunProfile> profile
        = runprofile::factory::CommandLine::getRunProfile(argc,argv);

    // Set logging level for each module
    std::vector<
      std::pair<logging::LogModuleEnum, logging::LogLevelEnum>
    > moduleLogLevels;
    for (int i = 0; i < logging::LogModuleEnum::NUM_ENUMS; i++) {
      moduleLogLevels.push_back(std::make_pair(
        logging::LogModuleEnum(i),
        profile->getLogLevel(logging::LogModuleEnum(i))
      ));
    }
    logging::setModuleLogLevels(log_sink, moduleLogLevels);
    logging::setLogTags(log_sink, profile->getLogTags());

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

    util::MultiIndexMap m;

    m << *profile;

    m["tool"]["name"] = Platform::getApplicationName();
    m["tool"]["description"] = Platform::getApplicationDescription();
    m["tool"]["version"]["major"] = Platform::getMajorVersion();
    m["tool"]["version"]["minor"] = Platform::getMinorVersion();
    m["tool"]["version"]["subminor"] = Platform::getBugFixVersion();
    m["tool"]["version"]["subminor"] = Platform::getBugFixVersion();
    m["tool"]["build"]["commit_hash"] = Platform::getCommitHash();
    m["tool"]["build"]["date"] = Platform::getCompileDate();
    m["tool"]["build"]["compiler"]["name"] = Platform::getCompilerName();
    m["tool"]["build"]["compiler"]["version"] = Platform::getCompilerVersion();
    m["tool"]["build"]["compiler"]["flags"] = Platform::getCompilerFlags();
    m["tool"]["site"] = Platform::getSite();
    m["tool"]["license"] = Platform::getLicenseInfo();

    m["execution"]["start"] = util::getCurrentTimeString();
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
        status = (0 != runBenchmark(*profile, &m))
            ? ReturnValue::UNKNOWN : ReturnValue::SUCCESS;
        break;
      case runprofile::ActionEnum::MACHINE_INFO:
        status = (0 != runMachineInfo(*profile, &m))
            ? ReturnValue::UNKNOWN : ReturnValue::SUCCESS;
        break;
      case runprofile::ActionEnum::INTERPOLATE:
        status = (0 != runInterpolation(*profile, &m))
            ? ReturnValue::UNKNOWN : ReturnValue::SUCCESS;
        break;
      default:
        throw std::logic_error("Unknown/unimplemented action");
    }

    m["execution"]["status"] = status.toString();
    m["execution"]["end"] = util::getCurrentTimeString();

    if (profile->getOutputFileName().size() > 0) {
      saveData(m, profile->getOutputFileName(), profile->isOverwrite());
    }

  } catch (const std::exception& e) {
    LERR_ << e.what() << std::endl;
    status = ReturnValue::BAD_CMD_LINE_OPTION;
  }
  // catch (...) { } // comment out to core dump on unexpected exceptions

  return status.getId();
}
