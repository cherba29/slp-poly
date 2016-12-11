#ifndef NTRP_CONTEXT_FUNCTION_H
#define NTRP_CONTEXT_FUNCTION_H

/**
 * @file Function.h Definition of parsed context::Function class
 */

#include "ValueList.h"

#include "context/FunctionEnum.h"

#include <boost/shared_ptr.hpp>

namespace context {

class Function : public Value {
public:
  Function(FunctionEnum name, ValueList* valueList)
      : name_(name), valList_(valueList) { }

  const FunctionEnum& getName() const { return name_; }

  const ValueList& getArguments() const { return *valList_; }

  virtual void accept(Processor& proc) const { proc.process(*this); }
private:
  FunctionEnum name_;
  boost::shared_ptr<ValueList> valList_;
};

}  // namespace context

#endif  // NTRP_CONTEXT_FUNCTION_H
