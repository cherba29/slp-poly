/** 
 * @file parser.yy Contains the Maple Bison parser source 
 */

/* Require bison 2.3 or later, will fail on earlier generators */
%require "2.3"

%code top { /*** C/C++ Declarations ***/

#include "util/log.h"

#include <vector>
#include <string>
#include <cstdlib>
#include <limits>

#if defined _WIN32
// warning C4800: 'int' : forcing value to bool 'true' or 'false'
// (performance warning)
#pragma warning(disable : 4800)
#endif

}

%code requires {
#include "context/context.h"
}

/* add debug output code to generated parser. disable this for release
 * versions. */
/* %debug */

/* write out a header file containing the token defines */
%defines

/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* namespace to enclose parser in */
%name-prefix="maple"

/* set the parser's class identifier */
%define parser_class_name "Parser"

/* keep track of the current position within the input */
%locations
%initial-action
{
  // initialize the initial location object
  // Parser should be declaring these as const in position.hh
  @$.begin.filename = @$.end.filename = const_cast<std::string*>(
      &driver.getStreamName());
};

/* The driver is passed by reference to the parser and to the scanner. This
 * provides a simple but effective pure interface, not relying on global
 * variables. */
%parse-param { class Driver& driver }
%lex-param { class Driver& driver }

/* verbose error messages */
%error-verbose

 /*** BEGIN Maple Parser ***/
%union {
  std::string* strVal_;
  bool boolVal_;
  double realVal_;
  std::string* intVal_;
  context::Value* value_;
  context::Sum* sum_;
  context::Product* product_;
  context::ValueList* values_;
  context::Matrix* matrix_;
  context::Table* table_;
}

%type <value_> value argument 
%type <sum_> summation 
%type <product_> product
%type <values_> values arguments
%type <table_> table_entries
%type <matrix_> matrix_entries

%token ASSIGN 
%token TABLE
%token DET
%token ARRAY
%token UPTO
%token END 0
%token BAD_TOKEN
%token <boolVal_> BOOL
%token <strVal_> ID 
%token <strVal_> STRING
%token <strVal_> BQSTRING
%token <intVal_> INTEGER
%token <realVal_> REAL

%destructor { delete $$; } ID STRING BQSTRING INTEGER
%destructor { delete $$; } value values
%destructor { delete $$; } argument arguments
%destructor { delete $$; } table_entries
%destructor { delete $$; } matrix_entries

%left '+' '-'
%left '*' '/'
%left UMINUS
%right '^'

%start input_file

%{
#include "context/maple/Driver.h"
#include "context/maple/Scanner.h"
#include "context/context.h"
#include <sstream>

#define LOG_MODULE ::logging::Module::PARSER

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex
%}

%% /*** Grammar Rules *********************************************************/


input_file: assignments END {
    driver.getContext()->finalize();
  }
  ;
  
assignments
  : assignment ';' { // nothing
  }
  | assignments assignment ';' {
  }
  ;
  
assignment
  : ID ASSIGN value { // nothing
    driver.getContext()->addStatement(new context::Assignment(*$1, $3));
    if (!driver.getContext()->addIdentifier(*$1, true)) {
      std::ostringstream ss;
      ss << "Was not able to parse assignment for '" << (*$1)
         << "', perhaps one by this name is already defined.";
      error(@$, ss.str());

      YYERROR; // Binding previously free variable
    }
  }
  | ID ASSIGN TABLE '(' '[' table_entries ']' ')' { 
    if (!driver.getContext()->addTable(*$1, $6)) {
      std::ostringstream ss;
      ss << "Was not able to parse table '" << (*$1)
         << "', perhaps one by this name is already defined.";
      error(@$, ss.str());
      YYERROR;
    }
  }
  | ID ASSIGN ARRAY '(' BQSTRING ',' INTEGER UPTO INTEGER ',' INTEGER UPTO INTEGER ',' '[' matrix_entries ']' ')' {
#ifdef max
#undef max
#endif
    if (*$5 == "sparse") {
      try {
        char* endConversion;  //!< Argument for strto*() functions
        unsigned int startRowIndex = std::strtoul(
            $7->c_str(), &endConversion, 10);
        if (errno == ERANGE) { // Overflow
          std::ostringstream ss;
          ss << "Could not parse " << *$7 << " for matrix starting row index ";
          error(@$, ss.str());
          YYERROR;
        }
        unsigned int endRowIndex = std::strtoul(
            $9->c_str(), &endConversion, 10);
        if (errno == ERANGE) { // Overflow
          std::ostringstream ss;
          ss << "Could not parse " << *$9 << " for matrix ending row index ";
          error(@$, ss.str());
          YYERROR;
        }
        if (startRowIndex > endRowIndex) {
          std::ostringstream ss;
          ss << "Staring matrix row index " << startRowIndex 
             << " is larger than ending index " << endRowIndex;
          error(@$, ss.str());
          YYERROR;
        }
        unsigned int startColIndex = std::strtoul(
            $11->c_str(), &endConversion, 10);
        if (errno == ERANGE) { // Overflow
          std::ostringstream ss;
          ss << "Could not parse " << *$11
             << " for matrix starting column index ";
          error(@$, ss.str());
          YYERROR;
        }
        unsigned int endColIndex = std::strtoul(
            $13->c_str(), &endConversion, 10);
        if (errno == ERANGE) { // Overflow
          std::ostringstream ss;
          ss << "Could not parse " << *$13 
             << " for matrix ending column index ";
          error(@$, ss.str());
          YYERROR;
        }
        if (startColIndex > endColIndex) {
          std::ostringstream ss;
          ss << "Staring matrix column index " << startColIndex
             << " is larger than ending index " << endColIndex;
          error(@$, ss.str());
          YYERROR;
        }

        if (startRowIndex > std::numeric_limits<unsigned int>::max()
            || endRowIndex > std::numeric_limits<unsigned int>::max() 
            || startColIndex > std::numeric_limits<unsigned int>::max()
            || endColIndex > std::numeric_limits<unsigned int>::max()) {
          std::ostringstream ss;
          ss << "Matrix indices are exceed max size of int ("
             << startRowIndex << ".." << endRowIndex << ","
             << startColIndex << ".." << endColIndex << ")";
          error(@$, ss.str());
          YYERROR;
        }
        $16->setSize(startRowIndex, endRowIndex, startColIndex, endColIndex);
        $16->setName(*$1);
      } catch(const std::logic_error& e) {
        std::ostringstream ss;
        ss << "Error setting matrix size for '" << (*$1)
           << "'.  Posibly entries outsides it bounds have been declared: " 
           << e.what();
        error(@$, ss.str());
        YYERROR;
      }
    } else {
      std::ostringstream ss;
      ss << "Parse error. Expected 'sparse' got '" << (*$5) << "'.";
      error(@$, ss.str());
      YYERROR;
    }
    if (!driver.getContext()->addMatrix(*$1, $16)) {
      std::ostringstream ss;
      ss << "Was not able to parse matrix '" << (*$1)
         << "', perhaps one by this name is already defined.";
      error(@$, ss.str());
      YYERROR;
    }
  }
  ;
  
value
  : '(' value ')'  { $$ = $2;  }
  | ID             { 
    $$ = new context::Id(*$1); 
    driver.getContext()->addIdentifier(*$1, false);
  }
  | INTEGER        { $$ = new context::Integer(*$1); }
  | REAL           { $$ = new context::RealNumber($1); }
  | STRING         { $$ = new context::String(*$1); }
  | BQSTRING       { $$ = new context::String(*$1); }
  | '[' values ']' { $$ = $2; }
  | '-' value %prec UMINUS  { $$ = new context::Negation($2); }
  | summation      { $$ = $1; }
  | product        { $$ = $1; }
  | value '^' INTEGER          {
    char * endConversion;  //!< Argument for strto*() functions
    unsigned int power = std::strtoul($3->c_str(), &endConversion, 10);
    if (errno == ERANGE) { // Overflow
      std::ostringstream ss;
      ss << "Could not parse " << *$3 << " for power";
      error(@$, ss.str());
      YYERROR;
    }

    if (power > std::numeric_limits<unsigned int>::max()) {
      std::ostringstream ss;
      ss << "Power (" << *$3 << ") exceeds maximum integer";
      error(@$, ss.str());
      YYERROR;
    } else {
      $$ = new context::Power($1, power); 
    }
  }
  | DET '(' ID ')' { 
    context::Matrix* mtx = driver.getContext()->getMatrix(*$3);
    if (mtx->getNRows() == mtx->getNCols()) {
      $$ = new context::Determinant(mtx); 
    } else {
      std::ostringstream ss;
      ss << "Determinant is only valid for square matrices. '" << (*$3) << "' "
         << mtx->getNRows() << "x" << mtx->getNCols() << " is not square";
      error(@$, ss.str());
      YYERROR; // raise error
    }
  } 
  | ID '(' arguments ')'      {
    context::FunctionEnum func;
    func.setToValue($1->c_str());
    if (func) { 
      $$ = new context::Function(func, $3);
    } else {
      std::ostringstream ss;
      ss << "Unknown function " << *$1;
      error(@$, ss.str());
      YYERROR; // raise error
    }
  }
  ;
values
  : value {
    $$ = new context::ValueList();
    $$->addValue($1);
  }
  | values ',' value {
    $$ = $1;
    $$->addValue($3);
  }
  ;

//  | value '-' value      { $$ = new context::Subtraction($1, $3); }
//  | value '*' value      { $$ = new context::Multiplication($1, $3); }
//  | value '/' value      { $$ = new context::Division($1, $3); }

summation : value '+' value      { 
    $$ = new context::Sum(); 
    $$->add($1);
    $$->add($3);
  }
  | value '-' value      { 
    $$ = new context::Sum(); 
    $$->add($1);
    $$->subtract($3);
  }
  | summation '+' value {
    $1->add($3);
    $$ = $1;
  }
  | summation '-' value {
    $1->subtract($3);
    $$ = $1;
  }
  
product : value '*' value      {   
    $$ = new context::Product(); 
    $$->multiply($1);
    $$->multiply($3);
  }
  | value '/' value      {   
    $$ = new context::Product(); 
    $$->multiply($1);
    $$->divide($3);
  }
  | product '*' value {
    $1->multiply($3);
    $$ = $1;
  }
  | product '/' value {
    $1->divide($3);
    $$ = $1;
  }
  
table_entries
  : ID '=' value {
      $$ = new context::Table();
    $$->setEntry(*$1, $3);
  }
  | table_entries ',' ID '=' value {
    $$ = $1;
    $$->setEntry(*$3, $5);
  }
  ;
matrix_entries
  : '(' INTEGER ',' INTEGER ')' '=' value {
    char * endConversion;  //!< Argument for strto*() functions
    unsigned int row = std::strtoul($2->c_str(), &endConversion, 10);
    if (errno == ERANGE) { // Overflow
      std::ostringstream ss;
      ss << "Could not parse " << *$2 << " for row index";
      error(@$, ss.str());
      YYERROR;
    }
    unsigned int column = std::strtoul($4->c_str(), &endConversion, 10);
    if (errno == ERANGE) { // Overflow
      std::ostringstream ss;
      ss << "Could not parse " << *$2 << " for column index";
      error(@$, ss.str());
      YYERROR;
    }

    if (row > std::numeric_limits<unsigned int>::max()
        || column > std::numeric_limits<unsigned int>::max()) {
      std::ostringstream ss;
      ss << "Index (" << row << "," << column << ") exceeds maximum integer";
      error(@$, ss.str());
      YYERROR;
    } else {
      $$ = new context::Matrix();
      $$->setEntry(row,column,$7);
    }
  }
  | matrix_entries ',' '(' INTEGER ',' INTEGER ')' '=' value {
    char * endConversion;  //!< Argument for strto*() functions
    unsigned int row = std::strtoul($4->c_str(), &endConversion, 10);
    if (errno == ERANGE) { // Overflow
      std::ostringstream ss;
      ss << "Could not parse " << *$4 << " for row index";
      error(@$, ss.str());
      YYERROR;
    }
    unsigned int column = std::strtoul($6->c_str(), &endConversion, 10);
    if (errno == ERANGE) { // Overflow
      std::ostringstream ss;
      ss << "Could not parse " << *$6 << " for column index";
      error(@$, ss.str());
      YYERROR;
    }
  
    if (row > std::numeric_limits<unsigned int>::max()
        || column > std::numeric_limits<unsigned int>::max()) {
      std::ostringstream ss;
      ss << "Index (" << row << "," << column << ") exceeds maximum integer";
      error(@$, ss.str());
      YYERROR;
    } else {
      if ($1->isEntrySet(row,column)) {
        std::ostringstream ss;
        ss << "Entry (" << row << "," << column << ") was previously set";
        error(@$, ss.str());
        YYERROR;
      } else {
        $$ = $1;
        $$->setEntry(row,column,$9);
      }
    }
  }
  ;

arguments
  : argument {
    $$ = new context::ValueList();
    $$->addValue($1);
  }
  | arguments ',' argument {
    $$ = $1;
    $$->addValue($3);
  }
  ;

argument
  : value {
    $$ = $1;
  }
  | ID '=' value {
    $$ = new context::NamedValue(*$1, $3);
  }
  ;

%% /*** Additional Code ***/

void maple::Parser::error(const Parser::location_type& l,
                          const std::string& m) {
  driver.error(l, m);
}
