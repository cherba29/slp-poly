#ifndef NTRP_OPERATION_FIELD_FIELDOPERATIONS_H_
#define NTRP_OPERATION_FIELD_FIELDOPERATIONS_H_

/**
 * @file FieldOperations.h Enumeration for field operations
 *
 */

#include <stdexcept>
#include "Operation.h"

namespace operation {
namespace field {

class FieldOperation {
public:
  typedef enum {
    ADD,
    SUBTRACT,
    NEGATE,
    MULTIPLY_EQ,
    DIVIDE,
    INVERSE,
    POWER,
    NUMOPS
  } OperatorEnum_type;
  FieldOperation() : op_(ADD) {}
  FieldOperation(int i) {
    if (i < 0 || i >= NUMOPS) {
      throw std::range_error("Index out of range, no such field operation");
    }
    op_ = static_cast<OperatorEnum_type>(i);
  }
  operator OperatorEnum_type() const {
    return op_;
  }
  operator int() const {
    return static_cast<int>(op_);
  }
private:
  OperatorEnum_type op_;
};

typedef OperationCount<FieldOperation> FieldOperations;

}  // namespace field
}  // namespace operation

#endif // NTRP_OPERATION_FIELD_FIELDOPERATIONS_H_

