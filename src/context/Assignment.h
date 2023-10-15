#ifndef NTRP_CONTEXT_ASSIGNMENT_H
#define NTRP_CONTEXT_ASSIGNMENT_H

/**
 * @file Assignment.h Definition of parsed context::Assignment class
 */

#include "Statement.h"
#include "Value.h"

#include <boost/shared_ptr.hpp>
#include <string>

namespace context {

/**
 * @brief Assignment statement, encapsulating id=value
 */
class Assignment : public Statement {
 public:
  Assignment(const std::string& id, Value* val) : id_(id), val_(val) {}

  const std::string& getId() const { return id_; }

  const Value& getValue() const { return *val_; }

  virtual void accept(Processor& proc) const { proc.process(*this); }

 private:
  std::string id_;
  boost::shared_ptr<Value> val_;
};

}  // namespace context

#endif  // NTRP_CONTEXT_ASSIGNMENT_H
