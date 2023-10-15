#ifndef NTRP_CONTEXT_ID_H
#define NTRP_CONTEXT_ID_H

/**
 * @file Id.h Definition of parsed context::Id class
 */

#include "Value.h"

namespace context {

class Id : public Value {
 public:
  Id(const std::string& name) : name_(name) {}

  const std::string& getName() const { return name_; }

  virtual void accept(Processor& proc) const { proc.process(*this); }

 private:
  std::string name_;
};

}  // namespace context

#endif  // NTRP_CONTEXT_ID_H
