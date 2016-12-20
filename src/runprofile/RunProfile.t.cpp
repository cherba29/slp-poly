/**
 * @file CommandLine.h Unit tests for CommandLine parser
 */

#include "runprofile/factory/CommandLine.h"
#include "runprofile/InterpProfile.h"
#include <numeric>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(command_line)

BOOST_AUTO_TEST_CASE(getRunProfile_showhelp) {
  const char* argv[] = { "interp", "-h" };
  int const argc = sizeof(argv)/sizeof(argv[0]);
  std::unique_ptr<runprofile::RunProfile> prof
      = runprofile::factory::CommandLine::getRunProfile(
          argc, const_cast<char**>(argv));
  BOOST_CHECK_EQUAL(prof->getAction(), runprofile::ActionEnum::HELP);
}

BOOST_AUTO_TEST_CASE( getRunProfile_showhelp_intepolate ) {
  const char* argv[] = { "interp", "-ht" };
  int const argc = sizeof(argv) / sizeof(argv[0]);
  std::unique_ptr<runprofile::RunProfile> prof
      = runprofile::factory::CommandLine::getRunProfile(
          argc, const_cast<char**>(argv));
  BOOST_CHECK_EQUAL(prof->getAction(),runprofile::ActionEnum::HELP);
}

BOOST_AUTO_TEST_CASE(getRunProfile_showversion) {
  const char* argv[] = {"interp", "--version"};
  int const argc = sizeof(argv)/sizeof(argv[0]);
  std::unique_ptr<runprofile::RunProfile> prof
      = runprofile::factory::CommandLine::getRunProfile(
          argc, const_cast<char**>(argv));
  BOOST_CHECK_EQUAL(prof->getAction(),runprofile::ActionEnum::VERSION);
}

BOOST_AUTO_TEST_CASE(getRunProfile_machine) {
  const char* argv[] = { "interp", "-m", "-o", "mach_info_out.txt" };
  int const argc = sizeof(argv)/sizeof(argv[0]);
  std::unique_ptr<runprofile::RunProfile> prof
      = runprofile::factory::CommandLine::getRunProfile(
          argc,  const_cast<char**>(argv));
  BOOST_CHECK_EQUAL(prof->getAction(), runprofile::ActionEnum::MACHINE_INFO);

  BOOST_CHECK_EQUAL(prof->getOutputFileName(), argv[3]);
}

BOOST_AUTO_TEST_CASE(getRunProfile_benchmark) {
  const char* argv[] = { "interp", "-b" };
  int const argc = sizeof(argv) / sizeof(argv[0]);
  try {
    std::unique_ptr<runprofile::RunProfile> prof
        = runprofile::factory::CommandLine::getRunProfile(
            argc, const_cast<char**>(argv));
    BOOST_CHECK_EQUAL(prof->getAction(), runprofile::ActionEnum::BENCHMARK);
    BOOST_CHECK_EQUAL(prof->getOutputFileName(), "");
  } catch (const std::logic_error& e) {
    BOOST_ERROR(e.what());
  }
}

BOOST_AUTO_TEST_CASE(getRunProfile_interpolate_basic) {
  const char* argv[] = { "interp", "-t", "input.mpl", "-o", "output.yaml" };
  int const argc = sizeof(argv) / sizeof(argv[0]);
  std::unique_ptr<runprofile::RunProfile> prof
      = runprofile::factory::CommandLine::getRunProfile(
          argc, const_cast<char**>(argv));
  BOOST_CHECK_EQUAL(prof->getAction(), runprofile::ActionEnum::INTERPOLATE);
  BOOST_CHECK_EQUAL(prof->getInputFileName(), argv[2]);
  BOOST_CHECK_EQUAL(prof->getOutputFileName(), argv[4]);
}

BOOST_AUTO_TEST_CASE(getRunProfile_interpolate_full) {
  const char* argv[] = { "interp", "-tw", "input.mpl", "-o", "output.yaml" };
  int const argc = sizeof(argv) / sizeof(argv[0]);
  std::unique_ptr<runprofile::RunProfile> prof
      = runprofile::factory::CommandLine::getRunProfile(
          argc, const_cast<char**>(argv));
  BOOST_CHECK_EQUAL(prof->getAction(), runprofile::ActionEnum::INTERPOLATE);
  BOOST_CHECK(prof->isOverwrite());
  BOOST_CHECK_EQUAL(prof->getInputFileName(), argv[2]);
  BOOST_CHECK_EQUAL(prof->getOutputFileName(), argv[4]);
}

BOOST_AUTO_TEST_CASE( getRunProfile_interpolate_machine_info ) {
  const char* argv[] = { "interp", "-tm", "-o", "output.yaml" };
  int const argc = sizeof(argv) / sizeof(argv[0]);
  BOOST_CHECK_THROW(
    runprofile::factory::CommandLine::getRunProfile(
        argc,  const_cast<char**>(argv)),
    std::logic_error
  );
}

BOOST_AUTO_TEST_SUITE_END()


bool init_unit_test() {
  return true;
}
