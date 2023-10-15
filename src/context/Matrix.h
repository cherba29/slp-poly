#ifndef NTRP_CONTEXT_MATRIX_H
#define NTRP_CONTEXT_MATRIX_H

/**
 * @file Matrix.h Definition of parsed assignment class
 */

#include "Integer.h"
#include "Value.h"

#include <map>

namespace context {

class Matrix : public Value {
  std::string name_;
  int minRow_;
  int maxRow_;
  int minCol_;
  int maxCol_;
  typedef std::pair<int, int> Coord;
  std::map<Coord, boost::shared_ptr<Value> > entries_;
  const Integer zero_;

 public:
  Matrix() : minRow_(0), maxRow_(0), minCol_(0), maxCol_(0), zero_("0") {}

  void setSize(int minRow, int maxRow, int minCol, int maxCol) {
    minRow_ = minRow;
    maxRow_ = maxRow;
    minCol_ = minCol;
    maxCol_ = maxCol;
  }

  void setName(const std::string& name) { name_ = name; }

  const std::string& getName() const { return name_; }

  void setEntry(int i, int j, Value* val) {
    entries_.insert(std::pair<Coord, boost::shared_ptr<Value> >(
        Coord(i, j), boost::shared_ptr<Value>(val)));
  }

  /**
   *  Return true if i,j th entry was set before, false otherwise.
   */
  bool isEntrySet(int i, int j) {
    return entries_.find(Coord(minRow_ + i, minCol_ + j)) != entries_.end();
  }

  /**
   * Return previousely set value at i,jth location.
   * If no value was set before, Integer(0) is returned.
   */
  const Value& getEntry(int i, int j) const {
    std::map<Coord, boost::shared_ptr<Value> >::const_iterator it =
        entries_.find(Coord(minRow_ + i, minCol_ + j));
    if (it != entries_.end()) {
      return *(it->second);
    } else {
      return zero_;
    }
  }

  /**
   * Return number of rows used by this matrix.
   */
  int getNRows() const { return maxRow_ - minRow_ + 1; }

  /**
   * Return number of columns used by this matrix.
   */
  int getNCols() const { return maxCol_ - minCol_ + 1; }

  virtual void accept(Processor& proc) const { proc.process(*this); }
};

}  // namespace context

#endif  // NTRP_CONTEXT_MATRIX_H
