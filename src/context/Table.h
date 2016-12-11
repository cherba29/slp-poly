#ifndef NTRP_CONTEXT_TABLE_H
#define NTRP_CONTEXT_TABLE_H

/**
 * @file Table.h Definition of parsed context::Table class
 */

#include "Processor.h"
#include "Value.h"
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

namespace context {

class Table : public Value {
public:
  void setEntry(const std::string& name, Value* val) {
    entries_.insert(
        std::pair<std::string, boost::shared_ptr<Value> >(
            name, boost::shared_ptr<Value>(val)));
  }

  virtual void accept(Processor& proc) const { proc.process(*this); }
private:
  std::map<std::string, boost::shared_ptr<Value> > entries_;
};

}  // namespace context

#endif  // NTRP_CONTEXT_TABLE_H
