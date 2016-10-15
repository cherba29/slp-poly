/**
 * @file main.cpp Entry point for the application.
 */

#include <iostream>

#include <boost/log/trivial.hpp>


int main(int argc, char* argv[]) {
  BOOST_LOG_TRIVIAL(info) << "Hello World." << std::endl;
  return 0;
}
