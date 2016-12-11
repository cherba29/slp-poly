#ifndef NTRP_EVAL_INSTRUCTION_COUNTER_H
#define NTRP_EVAL_INSTRUCTION_COUNTER_H

/**
 * @file InstructionCounter.h Definition of parsed eval::InstructionCounter
 */

#include "eval/Instruction.h"
#include "eval/InstructionEnum.h"

#include <vector>

namespace eval {

template <typename F>
class InstructionCounter {
  std::vector<unsigned int> count;
public:
  InstructionCounter()
    : count(InstructionEnum::NUM_ENUMS) {}

  void operator()(const eval::Instruction<F>& instr) {
    count[instr.getType().getIndex()]++;
  }

  unsigned int getCount(InstructionEnum t) const {
    return count[t.getIndex()];
  }
};

}  // namespace eval
#endif  // NTRP_EVAL_INSTRUCTION_COUNTER_H
