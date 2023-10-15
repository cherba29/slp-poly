#ifndef SLP_POLY_PLATFORM_H
#define SLP_POLY_PLATFORM_H

/*
 * @file Platform.h platform namespace for collection of functions to query
 * system specific information.
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
 */

#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>

/**
 * @brief Set of function to query system/compilation variables
 */

class Platform {
 public:
  static const char* getApplicationName();
  static const char* getApplicationDescription();
  static int getMajorVersion();
  static int getMinorVersion();
  static int getBugFixVersion();
  static std::string getVersionStr();

  /**
   * @brief return current commit hash.
   */
  static const char* getCommitHash(void);

  /**
   * @return Date Time of last committed revision.
   */
  static const char* getCommitDateTime();

  static const char* getBugReportUrl();
  static const char* getSite();
  static const char* getLicenseInfo();

  static const char* getBuildType();
  static const char* getCompilerName();
  static int getCompilerVersion();
  static const char* getCompilerFlags();

  static const char* getCompileDate();
  static boost::posix_time::ptime getBuildTime();
};

#endif  // SLP_POLY_PLATFORM_H_
