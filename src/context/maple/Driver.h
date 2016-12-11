#ifndef NTRP_MAPLE_DRIVER_H
#define NTRP_MAPLE_DRIVER_H

/**
 * @file Driver.h Declaration of the maple::Driver class.
 */

#include "Scanner.h"

#include "context/context.h"
#include "context/InterpContext.h"

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

/**
 * @brief The maple namespace is used to encapsulate the three parser classes
 * maple::Parser, maple::Scanner and maple::Driver
 */
namespace maple {

/** The Driver class brings together all components. It creates an instance of
 * the Parser and Scanner classes and connects them. Then the input stream is
 * fed into the scanner object and the parser gets it's token
 * sequence. Furthermore the driver object is available in the grammar rules as
 * a parameter. Therefore the driver class contains a reference to the
 * structure into which the parsed data is saved.
 */

class Driver {

  /** Reference to the calculator context filled during parsing of the
   * expressions. */
  boost::shared_ptr<context::InterpContext> context_;

  /// enable debug output in the flex scanner
  bool trace_scanning_;

  /// enable debug output in the bison parser
  bool trace_parsing_;

  /// stream name (file or input stream) used for error messages.
  std::string streamname_;

public:
  boost::shared_ptr<context::InterpContext> getContext() { return context_; }
  /// construct a new parser driver context
  Driver();

  const std::string& getStreamName() { return streamname_; }

  /** Invoke the scanner and parser for a stream.
   * @param  in	input stream
   * @param context the input will be parsed into this context
   * @param  sname	stream name for error messages
   * @return true if successfully parsed
   */
  bool parse_stream(std::istream& in,
       boost::shared_ptr<context::InterpContext> context,
	     const std::string& sname = "stream input");

  /** Invoke the scanner and parser on an input string.
   * @param input	input string
   * @param sname	stream name for error messages
   * @param context the input will be parsed into this context
   * @return		true if successfully parsed
   */
  bool parse_string(const std::string& input,
        boost::shared_ptr<context::InterpContext> context,
		    const std::string& sname = "string stream");

  /** Invoke the scanner and parser on a file. Use parse_stream with a
   * std::ifstream if detection of file reading errors is required.
   * @param filename	input file name
   * @param context the input will be parsed into this context
   * @return		true if successfully parsed
   */
  bool parse_file(const std::string& filename,
    boost::shared_ptr<context::InterpContext> context);

  void setDebugScanner(bool val) { trace_scanning_ = val;}
  void setDebugParser(bool val) { trace_parsing_ = val; }
  // To demonstrate pure handling of parse errors, instead of
  // simply dumping them on the standard error output, we will pass
  // them to the driver using the following two member functions.

  /** Error handling with associated line number. This can be modified to
   * output the error e.g. to a dialog box. */
  void error(const class location& l, const std::string& m);

  /** General error handling. This can be modified to output the error
   * e.g. to a dialog box. */
  void error(const std::string& m);

  /** Pointer to the current lexer instance, this is used to connect the
   *  parser to the scanner. It is used in the yylex macro in parser.yy.
   *  \#define yylex driver.lexer->lex
   */
  Scanner* lexer;

};

}  // namespace maple

#endif  // NTRP_MAPLE_DRIVER_H
