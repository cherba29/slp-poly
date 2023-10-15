#ifndef NTRP_CONTEXT_SUM_H
#define NTRP_CONTEXT_SUM_H

/**
 * @file Sum.h Definition of parsed context::Addition class
 */

#include "Value.h"
#include "util/SmartAssert.h"

#include <boost/shared_ptr.hpp>

namespace context {

/**
 * @brief Sum node, encapsulating a sequence of sub expressions.
 */
class Sum : public Value {
 public:
  Sum() {}

  // Add value to this sum. Sum takes ownership.
  void add(Value* node) {
    ASSERT1(node != NULL).msg("Cannot add NULL node to sum");
    nodes_.push_back(boost::shared_ptr<Value>(node));
    isPos_.push_back(true);
  }

  // Add value to this sum as negative. Sum takes ownership.
  void subtract(Value* node) {
    ASSERT1(node != NULL).msg("Cannot add NULL node to sum");
    nodes_.push_back(boost::shared_ptr<Value>(node));
    isPos_.push_back(false);
  }

  const Value& getNode(size_t idx) const { return *nodes_[idx]; }

  bool isPos(size_t idx) const { return isPos_[idx]; }

  size_t getNumberOfNodes() const { return nodes_.size(); }

  virtual void accept(Processor& proc) const { proc.process(*this); }

 private:
  std::vector<boost::shared_ptr<Value> > nodes_;
  std::vector<bool> isPos_;
};

}  // namespace context

#endif  // NTRP_CONTEXT_SUM_H
