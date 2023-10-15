#ifndef NTRP_UTIL_SMART_ASSERT
#define NTRP_UTIL_SMART_ASSERT

/**
 * @file SmartAssert.h Simple SMART_ASSERT
 *       described by Andrei Alexandrescu and John Torjo.
 * http://www.cuj.com/documents/s=8464/cujcexp0308alexandr/cujcexp0308alexandr.html
 */

#include <boost/current_function.hpp>
#include <boost/lexical_cast.hpp>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

//#include "Enumeration.h"
#undef WARN
#undef DEBUG
#undef ERROR
#undef FATAL
#include "util/LevelEnum.h"

namespace smart_assert {
// ENUMERATION(Level, WARN, DEBUG, ERROR, FATAL);

/**
 * @brief Smart Assert context, encapsulates location, level and description of
 * the assert statement
 */
class Context {
 public:
  Context() : level_(LevelEnum::DEBUG), lineNumber_(0) {}
  Context(const std::string& expression)
      : level_(LevelEnum::DEBUG), lineNumber_(0), expression_(expression) {}

  void setLevel(LevelEnum level) { level_ = level; }
  LevelEnum getLevel() const { return level_; }

  void setFileName(const std::string& fileName) { fileName_ = fileName; }
  const std::string& getFileName() const;

  void setFunctionName(const std::string& functionName) {
    functionName_ = functionName;
  }
  const std::string& getFunctionName() const { return functionName_; }

  void setLineNumber(unsigned int lineNumber) { lineNumber_ = lineNumber; }
  unsigned int getLineNumber() const { return lineNumber_; }

  void setDescription(const std::string& description) {
    description_ = description;
  }
  const std::string& getDescription() const { return description_; }

  void setExpression(const std::string& expression) {
    expression_ = expression;
  }
  const std::string& getExpression() const { return expression_; }

  typedef std::pair<std::string, std::string> NamedValue;

  void addValue(const std::string& name, const std::string& value) {
    namedValues_.push_back(std::make_pair(name, value));
  }
  const std::vector<NamedValue>& getValues() const { return namedValues_; }

  // return user-friendly assert message
  std::string getMessage() const;

 protected:
  std::string fileName_;
  LevelEnum level_;
  std::string functionName_;
  unsigned int lineNumber_;
  std::string description_;
  std::string expression_;
  std::vector<NamedValue> namedValues_;
};  // class Context

class AssertException : public std::runtime_error {
 public:
  AssertException(const Context& context)
      : runtime_error(context.getMessage()) {}
  virtual ~AssertException() throw() {}
};

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4355)  // warning C4355: 'this' : used in base member
                                 // initializer list
#endif

class SmartAssert {
 public:
  SmartAssert(const char* expression)
      : SMART_ASSERT_A(*this), SMART_ASSERT_B(*this), context_(expression){};

  ~SmartAssert();

  SmartAssert& setContext(const char* fileName, const char* functionName,
                          const unsigned int lineNumber);

  SmartAssert& setExpression(const char* expression);

  template <typename V>
  SmartAssert& addValue(const char* name, const V& value) {
    try {
      context_.addValue(name, boost::lexical_cast<std::string>(value));
    } catch (const boost::bad_lexical_cast&) {
      context_.addValue(name, "value could not be converted to string.");
    }
    return *this;
  }

  SmartAssert& msg(const std::string& message) {
    context_.setDescription(message);
    return *this;
  }

  SmartAssert& warn(const std::string&);

  SmartAssert& debug(const std::string&);

  SmartAssert& error(const std::string&);

  SmartAssert& fatal(const std::string&);

  // NULL as callback will disable handling specified level of assertions
  typedef void (*HandlerCallback)(const Context&);
  static void setHandler(LevelEnum, HandlerCallback);

  SmartAssert& SMART_ASSERT_A;
  SmartAssert& SMART_ASSERT_B;

 private:
  static void defaultWarnHandler(const Context&);
  static void defaultDebugHandler(const Context&);
  static void defaultErrorHandler(const Context&);
  static void defaultFatalHandler(const Context&);

  Context context_;
  static HandlerCallback callbacks_[LevelEnum::NUM_ENUMS];

  SmartAssert(const SmartAssert&);
  SmartAssert& operator=(const SmartAssert&);
  SmartAssert* operator*(const SmartAssert&);
};

#define SMART_ASSERT_A(x) SMART_ASSERT_OP(x, B)
#define SMART_ASSERT_B(x) SMART_ASSERT_OP(x, A)

#define SMART_ASSERT_OP(x, next) \
  SMART_ASSERT_A.addValue(#x, (x)).SMART_ASSERT_##next

#define ASSERT0(expr)                                           \
  if (expr) {                                                   \
    ;                                                           \
  } else                                                        \
    smart_assert::SmartAssert(#expr)                            \
        .setContext(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__) \
        .SMART_ASSERT_A

#ifndef NDEBUG
#define ASSERT1(expr)                                           \
  if (expr) {                                                   \
    ;                                                           \
  } else                                                        \
    smart_assert::SmartAssert(#expr)                            \
        .setContext(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__) \
        .SMART_ASSERT_A

#define ASSERT2(expr)                                           \
  if (expr) {                                                   \
    ;                                                           \
  } else                                                        \
    smart_assert::SmartAssert(#expr)                            \
        .setContext(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__) \
        .SMART_ASSERT_A

/*
#define SMART_ASSERT(expr) \
   if (expr){;} \
   else \
    smart_assert::SmartAssert(#expr).setContext( \
        __FILE__, BOOST_CURRENT_FUNCTION, __LINE__).SMART_ASSERT_A

#define SMART_VERIFY(expr) \
   if ( expr ) \
     { ; } \
   else \
    smart_assert::SmartAssert(#expr).setContext( \
        __FILE__, BOOST_CURRENT_FUNCTION, __LINE__ ).SMART_ASSERT_A

#define SMART_FAILED() \
  smart_assert::SmartAssert("").setContext(
      __FILE__, BOOST_CURRENT_FUNCTION, __LINE__).SMART_ASSERT_A
*/
#else
#define ASSERT1(expr) \
  if (true) {         \
    ;                 \
  } else              \
    smart_assert::SmartAssert("").SMART_ASSERT_A

#define ASSERT2(expr) \
  if (true) {         \
    ;                 \
  } else              \
    smart_assert::SmartAssert("").SMART_ASSERT_A
/*
#define SMART_VERIFY(expr) \
   if ( true ) \
     { (void)(expr); } \
   else \
   smart_assert::SmartAssert("").SMART_ASSERT_A

#define SMART_FAILED() \
   if ( true ) \
     { ; } \
   else \
   smart_assert::SmartAssert( "" ).SMART_ASSERT_A
*/
#endif  // ifndef NDEBUG

}  // namespace smart_assert

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  // NTRP_UTIL_SMART_ASSERT
