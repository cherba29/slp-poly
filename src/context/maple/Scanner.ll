/** 
 * @file Scanner.ll Define the maple Flex lexical scanner 
 */

%{ /*** C/C++ Declarations ***/

//#include <algebra/BigUnsigned.h>

#include "context/context.h"

#include "context/maple/Driver.h"
#include "context/maple/Scanner.h"

#include "util/log.h"

#include <vector>
#include <string>
#include <ostream>

#define LOG_MODULE ::logging::LogModuleEnum::PARSER

/* import the parser's token type into a local typedef */
typedef maple::Parser::token token;
typedef maple::Parser::token_type token_type;

/* By default yylex returns int, we use token_type. 
 * Unfortunately yyterminate by default returns 0, 
 * which is not of token_type. */
#define yyterminate() return token::END

/* This disables inclusion of unistd.h, which is not available under Visual C++
 * on Win32. The C++ scanner uses STL streams instead. */
#define YY_NO_UNISTD_H

%}


/*** Flex Declarations and Options ***/

/* enable c++ scanner class generation */
%option c++

/* change the name of the scanner class. results in "MapleFlexLexer" */
%option prefix="Maple"

/* the manual says "somewhat more optimized" */
%option batch

/* enable scanner to generate debug output. disable this for release
 * versions. */
/* %option debug */


/* no support for include files is planned */
%option yywrap nounput 
/* %option noyywrap yylineno */

/* enables the use of start condition stacks */
%option stack

/* The following paragraph to track locations accurately. Each time
 * yylex is invoked, the begin position is moved onto the end position. 
 * YY_USER_ACTION is executed prior to matched rules action.
 * yylloc is of type maple::location*
 * yyleng is length of current match.
 * yylloc->columns advances end position by given length
 */
%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}

%{

std::ostringstream buffer;	//!< Stores characters read from input

%}

%x bqstring_state string_state comment_state

%% /*******************************************************************/

%{ /* code to place at the beginning of yylex() */
    // Reset current initial location to current final location.
    yylloc->step();
%}

#[^\n]*		{	/* Strip comments */
    yylloc->step();   // Set start at end
}

[ \t\r]+ { /* Strip whitespace */
    yylloc->step();    // Set start at end
}
\n	{ 
   yylloc->lines(1);  // Advance line count
   yylloc->step();    // Set start at end
}

table		{ return token::TABLE; }
array		{ return token::ARRAY; }
det	        { return token::DET; }


[\[\]\(\)\-\+\*/^,;=!] { return maple::Parser::token_type(yytext[0]); } 
"\.\."	{ return token::UPTO; }
":="	{ return token::ASSIGN; }

[[:digit:]]+\.[[:digit:]]+	{
    char * endConversion;	//!< Argument for strto*() functions
	yylval->realVal_ = strtod(yytext, &endConversion); 
	return token::REAL;
}
[[:digit:]]+	{
    //char * endConversion;	//!< Argument for strto*() functions
	//yylval->intVal_ = strtoul(yytext, &endConversion, 10);
	/*
	long long val = 0;  // Bigest integer supported
	long long prevVal = 0;
	for (int i = 0; yytext[i]; ++i) {
	  val*=10;
	  val+=(yytext[i] - '0');
	  if (prevVal > val) { // Overflow occured
		driver.error(*yylloc, "integer is out of range");
		return token::BAD_TOKEN;
	  }
	}
	yylval->intVal_ = val;
	return token::INTEGER;
	*/
	/*
	BigUnsigned val = 0;  
	for (int i = 0; yytext[i]; ++i) {
	  val*=10;
	  val+=(yytext[i] - '0');
	}
	yylval->intVal_ = new BigUnsigned(val);
	*/
	yylval->intVal_ = new std::string(yytext); 
	return token::INTEGER;

}

_[[:alnum:]_]+_	{ BEGIN(comment_state); }
<comment_state>{
;	{ BEGIN(INITIAL); }
\n	|
.	/* Skip all characters but semicolons */
}


[[:alpha:]_][[:alnum:]_]*	{
	yylval->strVal_ = new std::string(yytext); 
	return token::ID;
}

`	{
	buffer.str("");
	BEGIN(bqstring_state);
}
<bqstring_state>{
` {
	BEGIN(INITIAL); 
	yylval->strVal_ = new std::string(buffer.str().c_str());
	return token::BQSTRING;
}
\n { 
	BEGIN(INITIAL);
	yylval->strVal_ = new std::string(buffer.str().c_str());
	driver.error(*yylloc, "Unterminated option");
	//input_error( annotate("Unterminated option", buffer.str()) );
	return token::BQSTRING;
}
<<EOF>> { 
	BEGIN(INITIAL);
	yylval->strVal_ = new std::string(buffer.str().c_str());
	driver.error(*yylloc, "Unterminated option");
	//input_error( annotate("Unterminated option", buffer.str()) );
	return token::BQSTRING;
}

[[:alpha:]_][[:alnum:]_]* { buffer << yytext; }

. { 
	BEGIN(INITIAL);
	yylval->strVal_ = new std::string(buffer.str());
	buffer << yytext;
	driver.error(*yylloc, "Illegal character in symbol");
	return token::BQSTRING;
}
}

\"	{
	buffer.str("");
	BEGIN(string_state);
}
<string_state>{
\" {
	BEGIN(INITIAL); 
	yylval->strVal_ = new std::string(buffer.str());
	return token::STRING;
}
\n { 
	BEGIN(INITIAL);
	yylloc->lines(1);
	yylval->strVal_ = new std::string(buffer.str().c_str());
	std::ostringstream oss;
	oss << "Unterminated string: " << buffer.str();
	driver.error(*yylloc, oss.str());
	return token::STRING;
}
<<EOF>> { 
	BEGIN(INITIAL);
	yylval->strVal_ = new std::string(buffer.str().c_str());
	std::ostringstream oss;
	oss << "Unterminated string: " << buffer.str();
	driver.error(*yylloc, oss.str());
	return token::STRING;
}
\\n { buffer << '\n'; }
\\t { buffer << '\t'; }
\\r { buffer << '\r'; }
\\b { buffer << '\b'; }
\\f { buffer << '\f'; }

\\(.|\n) { buffer << yytext[1]; }

[^\\\n"]+ { buffer << yytext; }
}

.	{ 
  std::ostringstream oss;
  oss << "Unrecognized character '" << yytext << "'";
  driver.error(*yylloc, oss.str()); 
}

%% /***************************************************************************/

/* This implementation of MapleFlexLexer::yylex() is required to fill the
 * vtable of the class MapleFlexLexer. We define the scanner's main yylex
 * function via YY_DECL to reside in the Scanner class instead. */

#ifdef yylex
#undef yylex
#endif

int MapleFlexLexer::yylex()
{
    LERR_ << "in MapleFlexLexer::yylex() !";
    return 0;
}

/* When the scanner receives an end-of-file indication from YY_INPUT, it then
 * checks the yywrap() function. If yywrap() returns false (zero), then it is
 * assumed that the function has gone ahead and set up `yyin' to point to
 * another input file, and scanning continues. If it returns true (non-zero),
 * then the scanner terminates, returning 0 to its caller. */

int MapleFlexLexer::yywrap() { return 1; }
