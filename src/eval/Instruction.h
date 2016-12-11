#ifndef NTRP_EVAL_INSTRUCTION_H
#define NTRP_EVAL_INSTRUCTION_H

/**
 * @file Instruction.h Definition eval::Instruction
 */

#include "eval/InstructionEnum.h"
//#include <util/Enumeration.h>

namespace eval {

/**
 * @brief Encapsulates concept of instruction. Its only reposibility
 *        to caintain type, sources and detination of an instruction.
 *  An instruction contains one/two operands and destination.
 *  Operand can be 'memory' (data) location or a constant.
 */

template <class F>
class Instruction {
public:
  //ENUMERATION(Type, ADD, ADDC, ADDTO, NEG, SUB, SUBC, SUBFROM, MUL, MULC, MULBY, DIV, DIVC, DIVBY, POW, DET, DET2P, SET, SETM, SETM2P, COPY );
  // DET2P determinant, determinant of matrix allocated for 2^intlog(size)
  // SETM2P set with constant, set 2D array, set 2D array of size 2^intlog(size)
  Instruction(InstructionEnum t, int src1, int src2, int dest)
    : type_(t), src1_(src1), src2_(src2), dest_(dest) {}

  Instruction(InstructionEnum t, int src1, F src2, int dest)
    : type_(t), src1_(src1), const_(src2), dest_(dest) {}

  InstructionEnum getType() const { return type_; }

  int getSource1() const { return src1_; }
  int getSource2() const { return src2_; }
  F getConstant() const { return const_; }

  int getDestination() const { return dest_; }
  void setDestination(int dest) { dest_ = dest; }

private:
  InstructionEnum type_;
  int src1_;  // Memory location
  int src2_;  // Memory location
  F const_;   // Value of set Constant
  int dest_;  // Memory location
};

//template <typename F>
//ENUM_OSTREAM(typename Instruction<F>::Type)
//
//template <typename F>
//ENUM_CHOICE_OSTREAM(typename Instruction<F>::Type)
//
//template <typename F>
//ENUM_VALUES_NO_CHECK(Instruction<F>::Type,
//            "ADD", "ADDC", "ADDTO",
//            "NEG",
//            "SUB", "SUBC", "SUBFROM",
//            "MUL", "MULC", "MULBY",
//            "DIV", "DIVC", "DIVBY",
//            "POW",
//            "DET", "DET2P",
//            "SET", "SETM", "SETM2P",
//            "COPY")
} // namespace eval


//#undef LOG_MODULE
#endif  // NTRP_EVAL_INSTRUCTION_H
