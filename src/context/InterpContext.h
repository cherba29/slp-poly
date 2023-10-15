#ifndef NTRP_CONTEXT_INTERP_CONTEXT_H
#define NTRP_CONTEXT_INTERP_CONTEXT_H

/**
 * @file InterpContext.h Definition of parsed context class
 */

#include "context.h"
#include "util/log.h"

#include <sstream>
#include <vector>

#ifdef LOG_MODULE
#undef LOG_MODULE
#endif
#define LOG_MODULE ::logging::LogModuleEnum::INTERPCONTEXT

/**
 * @brief Execution context, defines statements and their
 *        structure to execute.
 */
namespace context {

class IdentifierData {
  int index_;  // Also index of the result.
  bool bound_;

 public:
  IdentifierData(int index, bool bound) : index_(index), bound_(bound) {}

  void setBound(bool bound) { bound_ = bound; }

  bool isBound() const { return bound_; }

  int getIndex() const { return index_; }

  void setIndex(int index) { index_ = index; }
};

class InterpContext {
  std::map<std::string, IdentifierData> symTable_;
  std::map<std::string, Table*> tables_;
  std::map<std::string, Matrix*> matrices_;
  std::vector<Statement*> statements_;
  int nVars_;
  int nBoundVars_;

 public:
  InterpContext() : nVars_(0), nBoundVars_(0) {}

  void finalize() {
    std::map<std::string, IdentifierData>::iterator it;
    for (it = symTable_.begin(); it != symTable_.end(); ++it) {
      IdentifierData& id = it->second;
      if (id.isBound()) {
        id.setIndex(nVars_ - id.getIndex());
      }
    }
  }

  bool addIdentifier(const std::string& name, bool asBound) {
    std::map<std::string, IdentifierData>::const_iterator it;
    if ((it = symTable_.find(name)) != symTable_.end()) {
      if (asBound && !it->second.isBound()) {
        return false;  // now bound but previously unbound, should be an error
      }
    } else {
      int index = nVars_;
      if (asBound) {
        index = -nBoundVars_;
        ++nBoundVars_;
      } else {
        index = nVars_;
        ++nVars_;
      }
      symTable_.insert(std::make_pair(name, IdentifierData(index, asBound)));
    }
    return true;
  }

  bool addTable(const std::string& name, Table* t) {
    return tables_.insert(std::make_pair(name, t)).second;
  }

  bool addMatrix(const std::string& name, Matrix* m) {
    return matrices_.insert(std::make_pair(name, m)).second;
  }

  Matrix* getMatrix(const std::string& name) {
    std::map<std::string, Matrix*>::const_iterator it;
    if ((it = matrices_.find(name)) != matrices_.end()) {
      return it->second;
    } else {
      return NULL;
    }
  }
  /*
  int getConstantIndex(long val) const {
    std::map<std::string, IdentifierData>::const_iterator it;
    if ((it=constTable_.find(val) != constTable_.end()) {
       return it->second.getIndex();
    }
    return -1;
  }
  */

  int getIdIndex(const std::string& name) const {
    std::map<std::string, IdentifierData>::const_iterator it;
    if ((it = symTable_.find(name)) != symTable_.end()) {
      return it->second.getIndex();
    }
    return -1;
  }

  const std::string& getVarName(int idx) const {
    std::map<std::string, IdentifierData>::const_iterator it;
    for (it = symTable_.begin(); it != symTable_.end(); ++it) {
      if (it->second.getIndex() == idx) return it->first;
    }
    std::stringstream errMsg;
    errMsg << "Could not find variable in context with index " << idx;
    LERR_ << errMsg.str();
    throw std::logic_error(errMsg.str());
  }

  void addStatement(Statement* stmt) { statements_.push_back(stmt); }

  size_t getNumberOfStatements() const { return statements_.size(); }

  int getNumberOfVariables() const { return nVars_; }

  int getNumberOfIdentifiers() const { return nVars_ + nBoundVars_; }

  const Statement& getStatement(int i) const { return *(statements_[i]); }
};

}  // namespace context

#undef LOG_MODULE
#endif  // NTRP_CONTEXT_INTERP_CONTEXT_H
