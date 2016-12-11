/**
 * @file Driver.cpp Implementation of the maple::Driver class.
 */

#include "Driver.h"
#include "Scanner.h"

#include "util/log.h"

#include <fstream>
#include <sstream>

namespace maple {

#define LOG_MODULE ::logging::LogModuleEnum::PARSER


Driver::Driver()
    : trace_scanning_(false),
      trace_parsing_(false)
{
}

bool Driver::parse_stream(std::istream& in,
                          boost::shared_ptr<context::InterpContext> context,
                          const std::string& sname) {
  streamname_ = sname;
  context_ = context;

  Scanner scanner(&in);
  scanner.set_debug(trace_scanning_);
  this->lexer = &scanner;

  Parser parser(*this);
#if YYDEBUG
  parser.set_debug_level(trace_parsing_);
#endif
  return parser.parse() == 0;
}

bool Driver::parse_file(const std::string &filename,
                        boost::shared_ptr<context::InterpContext> context) {
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    std::stringstream ss;
    ss << "Unable to open file: " << filename;
    error(ss.str());
    return false;
  }
  return parse_stream(in, context, filename);
}

bool Driver::parse_string(const std::string &input,
                          boost::shared_ptr<context::InterpContext> context,
                          const std::string& sname) {
  std::istringstream iss(input);
  return parse_stream(iss, context, sname);
}

void Driver::error(const class location& l, const std::string& m) {
  LERR_ << l << ": " << m;
}

void Driver::error(const std::string& m) {
  LERR_ << m;
}

} // namespace maple
