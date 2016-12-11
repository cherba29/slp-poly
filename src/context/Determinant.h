#ifndef NTRP_CONTEXT_DETERMINANT_H
#define NTRP_CONTEXT_DETERMINANT_H

/**
 * @file Determinant.h Definition of parsed context::Power class
 */

#include "Value.h"
#include <boost/shared_ptr.hpp>

namespace context {

class Determinant : public Value {
public:
  Determinant(Matrix* mtx) : mtx_(mtx) { }

  virtual ~Determinant() {}

  const Matrix& getMatrix() const { return *mtx_; }

  virtual void accept(Processor& proc) const { proc.process(*this); }
private:
  boost::shared_ptr<Matrix> mtx_;
};

}  // namespace context

#endif  // NTRP_CONTEXT_DETERMINANT_H
