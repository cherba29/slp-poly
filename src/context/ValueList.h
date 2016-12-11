#ifndef NTRP_CONTEXT_VALUELIST_H
#define NTRP_CONTEXT_VALUELIST_H

/**
 * @file ValueList.h Definition of context::ValueList
 */

#include "Value.h"
#include <vector>

namespace context {

class ValueList : public Value {
public:
  void addValue(Value* val) { vals_.push_back(val); }
  size_t getSize() const { return vals_.size(); }

  virtual void accept(Processor& proc) const { proc.process(*this); }
private:
  std::vector<Value*> vals_;
};

}  // namespace context

#endif  // NTRP_CONTEXT_VALUELIST_H
