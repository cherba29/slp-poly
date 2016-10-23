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

#include "util/log.h"
#include "platform/Platform.h"

int main(int argc, char* argv[]) {
  logging::init();

  LOGGER(main);

  std::cout << Platform::getApplicationName()
            << " - " << Platform::getApplicationDescription();
  std::cout << " Version " << Platform::getVersionStr()
            << " Commit " << Platform::getCommitHash()
            << " " << Platform::getBuildType()
            << std::endl << std::endl;

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

  return 0;
}
