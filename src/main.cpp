/**
 * @file main.cpp Entry point for the application.
 */

#include "util/log.h"

int main(int argc, char* argv[]) {
  logging::init();

  LOGGER(main);

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
