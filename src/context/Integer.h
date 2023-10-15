#ifndef NTRP_CONTEXT_INTEGER_H
#define NTRP_CONTEXT_INTEGER_H

/**
 * @file Integer.h Definition of parsed context::Integer class
 */

#include "Value.h"

#include <string>

namespace context {

/**
 * @brief Integer is represented by string and is converted during compilation
 * to intermediate form as only then the underlying field is known.
 */
class Integer : public Value {
 public:
  Integer(const std::string& val) : val_(val) {}

  const std::string& getInt() const { return val_; }

  virtual void accept(Processor& proc) const { proc.process(*this); }

 private:
  std::string val_;
};

}  // namespace context

#endif  // NTRP_CONTEXT_INTEGER_H
