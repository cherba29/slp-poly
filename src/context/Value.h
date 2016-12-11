#ifndef NTRP_CONTEXT_VALUE_H
#define NTRP_CONTEXT_VALUE_H

/**
 * @file Value.h Definition of parsed context::Value class
 */

namespace context{

class Processor;

/**
 * @brief Generic interface to implement visitor pattern
 *        deriving class would implement accept as { proc.process(*this); }
 * @sa Processor
 */
class Value   {
public:
  virtual void accept(Processor& proc) const  = 0;
  virtual ~Value();
};

}  // namespace constext

#endif  // NTRP_CONTEXT_VALUE_H
