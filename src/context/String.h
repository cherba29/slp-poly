#ifndef NTRP_CONTEXT_STRING_H
#define NTRP_CONTEXT_STRING_H

/**
 * @file String.h Definition of parsed context::String Value
 */

#include "Processor.h"
#include "Value.h"

#include <string>

namespace context {

/**
 * @brief This class represents pased string value.
 */
class String : public Value {
 public:
  String(const std::string& val) : val_(val) {}

  virtual void accept(Processor& proc) const { proc.process(*this); }

 private:
  std::string val_;
};

}  // namespace context

#endif  // NTRP_CONTEXT_STRING_H
