#ifndef NTRP_CONTEXT_PRODUCT_H
#define NTRP_CONTEXT_PRODUCT_H

/**
 * @file Product.h Definition of parsed context::Product class.
 */

#include "Value.h"
#include "util/SmartAssert.h"

#include <boost/shared_ptr.hpp>

namespace context {

/**
 * @brief Product node, encapsulating a sequence of sub expressions.
 */
class Product : public Value {
 public:
  Product() {}

  void multiply(Value* node) {
    ASSERT1(node != NULL).msg("Cannot add NULL node to product");
    nodes_.push_back(boost::shared_ptr<Value>(node));
    isPos_.push_back(true);
  }

  void divide(Value* node) {
    ASSERT1(node != NULL).msg("Cannot add NULL node to product");
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

#endif  // NTRP_CONTEXT_PRODUCT_H
