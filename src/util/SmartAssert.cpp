/**
 * @file SmartAssert.cpp implementation of SMART_ASSERT
 */

#include "SmartAssert.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

namespace smart_assert {

//ENUM_VALUES(Level, "warn", "debug", "error", "fatal")

std::string Context::getMessage() const {
  std::ostringstream oss;

  oss << fileName_ << ":" << lineNumber_ << " ASSERT " << level_.toString()
      << " " << description_ << std::endl;
  oss << "In function: " << functionName_ << std::endl;
  if (!expression_.empty()) {
    oss << "expression '" << expression_ << "' has failed." << std::endl;
  }
  std::vector<NamedValue>::const_iterator it = namedValues_.begin();
  if (it != namedValues_.end()) {
    oss << "Values: " << it->first << " = " << it->second << std::endl;
  }
  for (++it; it != namedValues_.end(); ++it ) {
    oss << std::setw(9) << it->first << " = " << it->second << std::endl;
  }
  return oss.str();
}

SmartAssert::HandlerCallback SmartAssert::callbacks_[LevelEnum::NUM_ENUMS] = {
  SmartAssert::defaultWarnHandler,
  SmartAssert::defaultDebugHandler,
  SmartAssert::defaultErrorHandler,
  SmartAssert::defaultFatalHandler
};

SmartAssert::~SmartAssert() {
  const LevelEnum level = context_.getLevel();
  if ((level.getId() >= LevelEnum::WARN)
      && (level.getId() < LevelEnum::FATAL)) {
    if (callbacks_[level.getIndex()]) {
      callbacks_[level.getIndex()](context_);
    }
  } else {
    abort(); // something very bad
  }
}

SmartAssert& SmartAssert::setContext(
    const char* fileName,
    const char* functionName,
    const unsigned int lineNumber) {
#if defined(WIN32)
  static const char* dir_separator = "\\";
#else
  static const char* dir_separator = "/";
#endif // defined(WIN32)

  std::string sfile(fileName);
  const std::string::size_type nPos = sfile.find_last_of( dir_separator );
  context_.setFileName(
      (nPos == std::string::npos) ? fileName : sfile.substr(nPos + 1));

  context_.setFunctionName(functionName);
  context_.setLineNumber(lineNumber);
  return *this;
}

SmartAssert& SmartAssert::setExpression(const char* expression) {
  context_.setExpression(expression);
  return *this;
}

SmartAssert& SmartAssert::warn(const std::string& description) {
  context_.setLevel(LevelEnum::WARN);
  context_.setDescription(description);
  return *this;
}

SmartAssert& SmartAssert::debug(const std::string& description) {
  context_.setLevel(LevelEnum::DEBUG);
  context_.setDescription(description);
  return *this;
}

SmartAssert& SmartAssert::error(const std::string& description) {
  context_.setLevel(LevelEnum::ERROR);
  context_.setDescription(description);
  return *this;
}

SmartAssert& SmartAssert::fatal(const std::string& description) {
  context_.setLevel(LevelEnum::FATAL);
  context_.setDescription(description);
  return *this;
}

void SmartAssert::defaultWarnHandler(const Context& context) {
  std::cerr << context.getMessage();
}

void SmartAssert::defaultDebugHandler(const Context& context) {
  std::cerr << context.getMessage();
  throw AssertException(context);
}

void SmartAssert::defaultErrorHandler(const Context& context) {
  std::cerr << context.getMessage();
  throw AssertException(context);
}

void SmartAssert::defaultFatalHandler(const Context& context) {
  std::cerr << context.getMessage();
  std::abort(); // Hard exit, no clean up
}

void SmartAssert::setHandler(LevelEnum level, HandlerCallback callback) {
  ASSERT0((level.getId() >= LevelEnum::WARN)
      && (level.getIndex() > LevelEnum::NUM_ENUMS))
          .error("Bad assertion level.").addValue("level", level);

  ASSERT0( callback != NULL )
      .warn("Assertion handling for specified level will be disabled")
      .addValue("level", level);

  callbacks_[level.getIndex()] = callback;
}

}  // namespace smart_assert
