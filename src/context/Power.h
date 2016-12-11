#ifndef NTRP_CONTEXT_POWER_H
#define NTRP_CONTEXT_POWER_H

/**
 * @file Power.h Definition of parsed context::Power class
 */

#include "Value.h"
#include <boost/shared_ptr.hpp>

namespace context {

class Power : public Value {
  boost::shared_ptr<Value> left_;
  long power_;
public:
  Power(Value* left, long power) : left_(left), power_(power) { }
  virtual ~Power() {}

  const Value& getLeftValue() const { return *left_; }
  long getExponent() const { return power_; }

  virtual void accept(Processor& proc) const { proc.process(*this); }
};

}  // namespace context

#endif // NTRP_CONTEXT_POWER_H
