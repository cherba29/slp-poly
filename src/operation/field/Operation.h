#ifndef NTRP_OPERATION_FIELD_H_
#define NTRP_OPERATION_FIELD_H_

/**
 * @file Operation.h
 * @brief implementation of Operation
 *
 * Operation should have
 * 1. enumeration {op1, op2, ..., opn, NUMOPS}
 * 2. Should implement int operator
 * 3. (optionally) should also implement
 */
template <typename Operation>
class OperationCount {
public:
  OperationCount() {
    reset();
  }
  void reset() {
    for (int i = 0; i < static_cast<int> (Operation::NUMOPS); ++i) {
      ops_[i] = 0;
    }
  }
  void set(const Operation& op, unsigned int val) {
    ops_[op] = val;
  }
  unsigned int get(const Operation& op) const {
    return ops_[op];
  }
  void add(const Operation& op, unsigned int val) {
    ops_[op] += val;
  }
  OperationCount<Operation>& operator+=(const OperationCount<Operation>& fops) {
    for (int i = 0; i < static_cast<int>(Operation::NUMOPS); ++i) {
      ops_[i] += fops.ops_[i];
    }
	return *this;
  }
private:
  unsigned int ops_[Operation::NUMOPS];
};

#endif  // NTRP_OPERATION_FIELD_H_
