#ifndef NTRP_CONTEXT_NAMEDVALUE_H
#define NTRP_CONTEXT_NAMEDVALUE_H

/**
 * @file NamedValue.h Definition of parsed context::NamedValue class
 */

#include <string>
#include "Value.h"
namespace context {

/**
 * @brief Named value ID = val, e.g. in maple table
 */
class NamedValue : public Value  {
  std::string name_;
  Value* val_;
public:
  NamedValue(const std::string& name, Value* val)
    : name_(name), val_(val) {}

  virtual ~NamedValue() { delete val_; }

  virtual void accept(Processor& proc) const { proc.process(*this); }
};

}  // namespace context

#endif  // NTRP_CONTEXT_NAMEDVALUE_H
