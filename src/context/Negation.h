#ifndef NTRP_CONTEXT_NEGATION_H
#define NTRP_CONTEXT_NEGATION_H

/**
 * @file Negation.h Definition of parsed context::Negation class
 */

#include "Value.h"
#include "util/SmartAssert.h"

#include <boost/shared_ptr.hpp>

namespace context {

class Negation : public Value {
  boost::shared_ptr<Value> val_;

 public:
  Negation(Value* val) : val_(val) {
    ASSERT1(val)(val).msg("Negation cannot be created out of NULL");
  }

  virtual void accept(Processor& proc) const { proc.process(*this); }

  const Value& getValue() const { return *val_; }
};

}  // namespace context

#endif  // NTRP_CONTEXT_NEGATION_H
