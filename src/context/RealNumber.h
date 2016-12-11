#ifndef NTRP_CONTEXT_REALNUMBER_H
#define NTRP_CONTEXT_REALNUMBER_H

/**
 * @file RealNumber.h Definition of parsed context::RealNumber class.
 */

#include "Value.h"

namespace context {

class RealNumber : public Value {
public:
  RealNumber(double val) : val_(val) { }
  virtual void accept(Processor& proc) const { proc.process(*this); }
private:
  double val_;
};

}  // namespace context

#endif  // NTRP_CONTEXT_REALNUMBER_H
