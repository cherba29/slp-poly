#ifndef NTRP_EVAL_CODEGENERATOR_H
#define NTRP_EVAL_CODEGENERATOR_H

/**
 * @file CodeGenerator.h Definition of eval::CodeGenerator class
 */

#include "context/context.h"
#include "context/InterpContext.h"
#include "context/Printer.h"
//#include <interp/Interpolator.h>

#include "eval/util/DataAllocator.h"
#include "eval/util/CodeAnnotation.h"
#include "Evaluator.h"
#include "UnivEvaluator.h"
#include "Instruction.h"

#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

#include <assert.h>

namespace eval {

#define LOG_MODULE ::logging::LogModuleEnum::CODEGENERATOR

/**
 * @brief Main responsibility of this class to construct executable instructions from
 *        context which is usually parsed from given input file.
 *        CodeGenerator is a context::Value visitor implementing context::Processor.
 *        Given context::InterpContext it produces eval::Instruction stream, from which
 *        eval::Evaluator can be constucted. It also produces code and data anotation
 *        for debugging and code tracing.
 * @code
 *   // Create context
 *   boost::shared_ptr<context::InterpContext> ctxtPtr(new context::InterpContext());
 *   // Fill it in from file
 *   maple::Driver parseDriver;
 *   if (!parseDriver.parse_file(profile.getInputFileName(), ctxtPtr)) {
 *      LERR_ << "Could not parse input file: " << profile.getInputFileName();
 *      return PARSE_ERROR;
 *   }
 *   // Initialize generator with it. Passed in as const
 *   eval::CodeGenerator cg(ctxtPtr);
 *   boost::shared_ptr<eval::Evaluator<Field> > evalPtr = cg.getEvaluator<Field>();
 *   if (!evalPtr) {
 *      LERR_ << "Not able to build evaluator from parsed in context";
 *   }
 * @endcode
 * @see eval::Evaluator, context::InterpContext
 */

template <typename F>
class CodeGenerator : private context::Processor { // not publicly convertable to processor

  boost::shared_ptr<std::vector<eval::Instruction<F> > > codePtr_;
  boost::shared_ptr<const context::InterpContext> cntxPtr_;

  DataAllocator alloc;

  boost::shared_ptr<CodeAnnotation> annPtr_;

  /** Last operation would store result here, or -1 no value stored */
  int resultLocation_;
  /** Value which should be used whenever resultLocation_ == -1 */
  F resultValue_;

  // Implement privately context::Processor interface
  virtual void process(const context::Assignment& v);
  virtual void process(const context::Determinant& v);
  virtual void process(const context::Function& v);
  virtual void process(const context::Id& v);
  virtual void process(const context::Integer& v);
  virtual void process(const context::Matrix& v);
  virtual void process(const context::NamedValue& v);
  virtual void process(const context::Negation& v);
  virtual void process(const context::Power& v);
  virtual void process(const context::Product& v);
  virtual void process(const context::RealNumber& v);
  virtual void process(const context::String& v);
  virtual void process(const context::Sum& v);
  virtual void process(const context::Table& v);
  virtual void process(const context::ValueList& v);


  int getLastDestination() const {
    if (this->codePtr_->size() < 1) return -1;
    return this->codePtr_->back().getDestination();
  }

  bool setLastDestination(int dest) {
    if (codePtr_->size() < 1) return false;
    eval::Instruction<F>& last = codePtr_->back();
    int lastDest = last.getDestination();
    if (this->alloc.isTemporaryLocation(lastDest)) {
      this->alloc.releaseLocation(lastDest);
      last.setDestination(dest);
      return true;
    }
    return false;
  }

public:
  CodeGenerator(boost::shared_ptr<const context::InterpContext> cntx)
    : codePtr_(new std::vector<eval::Instruction<F> >()),
      cntxPtr_(cntx),
      //currDataSize_(0),
      annPtr_(new CodeAnnotation()) {

    // Reserve space for identifiers, temporaries will have higher index
    int nVars = cntxPtr_->getNumberOfIdentifiers();
    // currDataSize_ = nVars;

    // Annotate memory locations with variable names
    for (int i = 0; i < nVars; i++) {
      annPtr_->setDataName(i, cntxPtr_->getVarName(i));
    }
    int loc = this->alloc.reserveLocation(nVars, false);
    assert(loc == 0); // This block must start at this position

    // Generate code statement by statement.
    int nStmts = cntxPtr_->getNumberOfStatements();
    for (int i = 0; i < nStmts; i++) {
      // Get string representation for statement i
      std::ostringstream ss;
      context::Printer strPrinter(ss);
      // Invoke specific process functions.
      cntxPtr_->getStatement(i).accept(strPrinter);
      LDBG_ << ss.str().substr(0,100);
      // Record it as annotation.
      annPtr_->setAnnotation(codePtr_->size(), ss.str());
      // Now process (Code generate) it, by invoking specific process functions.
      cntxPtr_->getStatement(i).accept(*this);
    }
  }

  /**
   * @brief Builds evaluator from given context::InterpContext
   * @see CodeGenerator::getAnnotation
   */
  boost::shared_ptr<Evaluator<F> > getEvaluator() const {
    // Build evaluator
    int nVars = cntxPtr_->getNumberOfVariables();
    return boost::shared_ptr<Evaluator<F> >(
      new Evaluator<F>(nVars, this->alloc.getMaxAllocSize(), codePtr_, annPtr_)
    );
  }
};

}  // namespace eval

#include "CodeGenerator-Impl.h"

#undef LOG_MODULE
#endif // NTRP_EVAL_CODEGENERATOR_H
