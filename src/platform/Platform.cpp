/**
 * @file Platform.h implementation of platform namespace for collection of 
 * functions to query system specific information.
 *
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
 *
 */

#include "Platform.h"
#include <sstream>

namespace {

// @todo Use STLSoft to distinguish compilers.
#if defined(__GNUC__)
# if defined(__GNUC_PATCHLEVEL__)
const int COMPILER_VERSION = (__GNUC__ * 10000 
                            + __GNUC_MINOR__ * 100 
                            + __GNUC_PATCHLEVEL__);
# else
const int COMPILER_VERSION = (__GNUC__ * 10000 
                            + __GNUC_MINOR__ * 100)
# endif
const char* COMPILER_NAME = "GCC compiler";
#else
#ifdef _MSC_VER
const char* COMPILER_NAME = "Microsoft C++ compiler";
int COMPILER_VERSION  = _MSC_VER;
#else
const char* COMPILER_NAME = "Unknown";
int COMPILER_VERSION  = 0;
#endif /* defined _MSCVER */
#endif /* defined __GNUC__ */

const char* LICENSE_INFO = "Apache License 2.0";

}

const char* Platform::getApplicationName() {
  return "slp-poly";
}

const char* Platform::getApplicationDescription() {
  return "Polynomial Interpolator (slp-poly)";
}

int Platform::getMajorVersion() {
  return 0;
}

int Platform::getMinorVersion() {
  return 1;
}

int Platform::getBugFixVersion() {
  return 0;
}

std::string Platform::getVersionStr() {
  std::ostringstream oss;
  oss << getMajorVersion() << "." 
      << getMinorVersion() << "."
      << getBugFixVersion();
  return oss.str();
}

const char* Platform::getCommitDateTime() {
  return "";
}

const char* Platform::getBugReportUrl() {
  return "";
}

const char* Platform::getBuildType() {
  return "RelWithDebInfo";
}

const char* Platform::getCompilerName() {
  return COMPILER_NAME;
}

int Platform::getCompilerVersion() {
  return COMPILER_VERSION;
}

const char* Platform::getCompileDate() {
  return __DATE__;
}

int64_t Platform::getBuildTime() {
  return BUILD_TIMESTAMP;
}

const char* Platform::getSite() {
  return "https://github.com/cherba29/slp-poly";
}
const char* Platform::getLicenseInfo() {
  return 
    "Polynomial Interpolator (slp-poly) Copyright (C) 2016 Arthur D. Cherba\n"
    "Licensed under the Apache License, Version 2.0 (the \"License\"); "
    "you may not use this file except in compliance with the License."
    "You may obtain a copy of the License at "
    "http://www.apache.org/licenses/LICENSE-2.0";
}

const char* Platform::getCompilerFlags() {
  return "-O2 -g -DNDEBUG";
}

const char* Platform::getCommitHash(void) {
  return BUILD_SCM_REVISION;
}
