#ifndef NTRP_EVAL_INSTRUCTION_MAXDEGREE_H
#define NTRP_EVAL_INSTRUCTION_MAXDEGREE_H

/**
 * @file InstructionMaxDegree.h Definition of parsed eval::InstructionMaxDegree
 */

#include "eval/Instruction.h"
#include "util/SmartAssert.h"

#include <algorithm>
#include <ostream>
#include <iomanip>

#define LOG_MODULE ::logging::LogModuleEnum::INSTRUCTIONMAXDEGREE

namespace eval {

#ifdef max
#undef max
#endif

class Exponent {
  std::vector<unsigned int> v_;
public:
  Exponent (int n) : v_(n) { }

  void setDegree(int idx, int deg) { v_[idx] = deg; }

  size_t getSize() const { return v_.size(); }

  unsigned int getDegree(size_t idx) const { return v_[idx]; }

  const unsigned int* getDegrees() const { return &v_[0]; }

  void initZero() {
    std::fill(v_.begin(),v_.end(),0);
  }

  static void add(Exponent& d, const Exponent& s1, const Exponent& s2) {
    std::vector<unsigned int>::const_iterator s1It = s1.v_.begin();
    std::vector<unsigned int>::const_iterator s1End = s1.v_.end();
    std::vector<unsigned int>::const_iterator s2It = s2.v_.begin();
    ASSERT0(s1.v_.size() == s2.v_.size())(s1.v_.size())(s2.v_.size())
      .msg("Cannot add exponents of different sizes");
    ASSERT0(d.v_.size() <= s1.v_.size())(d.v_.size())(s1.v_.size())
      .msg("Destination size is too large");
    d.v_.resize(s1.v_.size());
    std::vector<unsigned int>::iterator dIt = d.v_.begin();
    for (; s1It != s1End; ++s1It, ++s2It, ++dIt) *dIt = *s1It + *s2It;
  }

  static void sub(Exponent& d, const Exponent& s1, const Exponent& s2) {
    std::vector<unsigned int>::const_iterator s1It = s1.v_.begin();
    std::vector<unsigned int>::const_iterator s1End = s1.v_.end();
    std::vector<unsigned int>::const_iterator s2It = s2.v_.begin();
    ASSERT0(s1.v_.size() == s2.v_.size())(s1.v_.size())(s2.v_.size())
      .msg("Cannot add exponents of different sizes");
    ASSERT0(d.v_.size() <= s1.v_.size())(d.v_.size())(s1.v_.size())
      .msg("Destination size is too large");
    d.v_.resize(s1.v_.size());
    std::vector<unsigned int>::iterator dIt = d.v_.begin();
    for (; s1It != s1End; ++s1It, ++s2It, ++dIt) *dIt = *s1It - *s2It;
  }

  static void mul(Exponent& d, const Exponent& s1, unsigned int e) {
    std::vector<unsigned int>::const_iterator s1It = s1.v_.begin();
    std::vector<unsigned int>::const_iterator s1End = s1.v_.end();
    ASSERT0(d.v_.size() <= s1.v_.size())(d.v_.size())(s1.v_.size())
      .msg("Destination size is too large");
    d.v_.resize(s1.v_.size());
    std::vector<unsigned int>::iterator dIt = d.v_.begin();
    for (; s1It != s1End; ++s1It, ++dIt) *dIt = *s1It * e;
  }

  static void max(Exponent& d, const Exponent& s1, const Exponent& s2) {
    std::vector<unsigned int>::const_iterator s1It = s1.v_.begin();
    std::vector<unsigned int>::const_iterator s1End = s1.v_.end();
    std::vector<unsigned int>::const_iterator s2It = s2.v_.begin();
    ASSERT0(s1.v_.size() == s2.v_.size())(s1.v_.size())(s2.v_.size())
      .msg("Cannot add exponents of different sizes");
    ASSERT0(d.v_.size() <= s1.v_.size())(d.v_.size())(s1.v_.size())
      .msg("Destination size is too large");
    d.v_.resize(s1.v_.size());
    std::vector<unsigned int>::iterator dIt = d.v_.begin();
    for (; s1It != s1End; ++s1It, ++s2It, ++dIt) *dIt = std::max(*s1It, *s2It);
  }
};

inline std::ostream& operator << (std::ostream& os, const Exponent& e) {
  size_t n = e.getSize();
  os << "[";
  for (size_t i = 0; i < n; ++i) {
    if (i) os << ",";
    os << e.getDegree(i);
  }
  os << "]";
  return os;
}

class InstructionMaxDegree {
  const int nVars_;
  std::vector<Exponent> locDegrees_;
  unsigned int resultLoc_;
  unsigned int instructionCounter_;
  Exponent& getExponent(unsigned int idx) {
    //if (idx >= locDegrees_.size()) { // Cant be resizing and returning references
    //  locDegrees_.resize(idx+1, Exponent(nVars_));
    //}
    return locDegrees_[idx];
  }
  // Prevent copying and assignment
  InstructionMaxDegree& operator=(InstructionMaxDegree& src);
  InstructionMaxDegree(const InstructionMaxDegree& orig);
public:
  InstructionMaxDegree(int nVars, int memLocs)
     : nVars_(nVars), locDegrees_(memLocs, Exponent(nVars_)), instructionCounter_(0)  {
       ASSERT0(nVars <= memLocs)(nVars)(memLocs)
         .msg("Number of memory locations must be larger than number of variables");
       // Assuming that location i is of degree 1 in variable i, i = 0..nVars-1
       for (int i = 0; i < nVars_; i++) {
          Exponent& degs = this->getExponent(i);
          degs.setDegree(i, 1);
       }
  }

  ~InstructionMaxDegree() {  }

  template <typename F>
  void operator()(const eval::Instruction<F>& instr) {
    unsigned int dest = instr.getDestination();
    resultLoc_ = dest;
    Exponent& destDegs = getExponent(dest);
    switch (instr.getType().getId()) {
      case InstructionEnum::ADD:
      case InstructionEnum::SUB: {
        const Exponent& src1Degs = getExponent(instr.getSource1());
        const Exponent& src2Degs = getExponent(instr.getSource2());
        Exponent::max(destDegs, src1Degs, src2Degs);
        LDBG_ << instructionCounter_ << " " << instr.getType().toString() << " "
              << destDegs << "(" << instr.getDestination() << ") <-- "
              << src1Degs << "(" << instr.getSource1() << ") "
              << src2Degs << "(" << instr.getSource2() << ") " ;

      } break;
      case InstructionEnum::SUBFROM:
      case InstructionEnum::ADDTO: {
        const Exponent& src1Degs = getExponent(instr.getSource1());
        Exponent::max(destDegs, destDegs, src1Degs);
        LDBG_ << instructionCounter_ << " " << instr.getType().toString() << " "
              << destDegs << "(" << instr.getDestination() << ") <-- "
              << src1Degs << "(" << instr.getSource1() << ") " ;

      } break;

      case InstructionEnum::MUL: {
        const Exponent& src1Degs = getExponent(instr.getSource1());
        const Exponent& src2Degs = getExponent(instr.getSource2());
        Exponent::add(destDegs, src1Degs, src2Degs);
        LDBG_ << instructionCounter_ << " " << instr.getType().toString() << " "
              << destDegs << "(" << instr.getDestination() << ") <-- "
              << src1Degs << "(" << instr.getSource1() << ") "
              << src2Degs << "(" << instr.getSource2() << ") " ;

        break;
      }
      case InstructionEnum::MULBY: {
        const Exponent& src1Degs = getExponent(instr.getSource1());
        Exponent::add(destDegs, destDegs, src1Degs);
        LDBG_ << instructionCounter_ << " " << instr.getType().toString() << " "
              << destDegs << "(" << instr.getDestination() << ") <-- "
              << src1Degs << "(" << instr.getSource1() << ") ";

        break;
      }

      case InstructionEnum::DIV: {
        const Exponent& src1Degs = getExponent(instr.getSource1());
        const Exponent& src2Degs = getExponent(instr.getSource2());
        Exponent::sub(destDegs, src1Degs, src2Degs);
        LDBG_ << instructionCounter_ << " " << instr.getType().toString() << " "
              << destDegs << "(" << instr.getDestination() << ") <-- "
              << src1Degs << "(" << instr.getSource1() << ") "
              << src2Degs << "(" << instr.getSource2() << ") " ;

        break;
      }
      case InstructionEnum::DIVBY: {
        const Exponent& src1Degs = getExponent(instr.getSource1());
        Exponent::sub(destDegs, destDegs, src1Degs);
        LDBG_ << instructionCounter_ << " " << instr.getType().toString() << " "
              << destDegs << "(" << instr.getDestination() << ") <-- "
              << src1Degs << "(" << instr.getSource1() << ") ";

        break;
      }

      case InstructionEnum::POW: {
        const Exponent& src1Degs = getExponent(instr.getSource1());
        int src2Val = instr.getSource2();
        Exponent::mul(destDegs, src1Degs, src2Val);
        LDBG_ << instructionCounter_ << " " << instr.getType().toString() << " "
              << destDegs << "(" << instr.getDestination() << ") <-- "
              << src1Degs << "(" << instr.getSource1() << ") "
              << "(" << instr.getSource2() << ") " ;
        break;
      }
      case InstructionEnum::NEG:
      case InstructionEnum::COPY:
      case InstructionEnum::DIVC:
      case InstructionEnum::MULC:
      case InstructionEnum::ADDC: {
        destDegs = getExponent(instr.getSource1());
        LDBG_ << instructionCounter_ << " " << instr.getType().toString() << " "
              << destDegs << "(" << instr.getDestination() << ") <-- "
              << "(" << instr.getSource1() << ") "
              << "(" << instr.getSource2() << ") " ;

        break;
      }
      case InstructionEnum::SETM: {
        int size = instr.getSource1();
        for (int i = 0; i < size; ++i) {
          getExponent(dest+i).initZero();
        }
        LDBG_ << instructionCounter_ << " " << instr.getType().toString() << " "
              << destDegs << "(" << instr.getDestination() << ") " << size;
        break;
      }
      case InstructionEnum::SET: {
        destDegs.initZero();
        LDBG_ << instructionCounter_ << " " << instr.getType().toString() << " "
              << destDegs << "(" << instr.getDestination() << ") " ;

        break;
      }
      case InstructionEnum::DET: {
        destDegs.initZero();
        Exponent maxExp(nVars_);
        int firstLoc = instr.getSource1();
        int size = instr.getSource2();
        for (int i = 0; i < size; i++) {  // For each row
          maxExp.initZero();
          for (int j = 0; j < size; j++) {
            const Exponent& cellDegs = getExponent(firstLoc+i*size+j);
            Exponent::max(maxExp,maxExp,cellDegs);
          }
          Exponent::add(destDegs, destDegs, maxExp);
        }
        LDBG_ << instructionCounter_ << " " << instr.getType().toString() << " "
              << destDegs << "(" << instr.getDestination() << ") <-- "
              << "(" << instr.getSource1() << ") "
              << "(" << instr.getSource2() << ") " ;
        break;
      }

      default: {
        ASSERT0(false)(instructionCounter_)(instr.getType().toString())
          .error("Unknown instruction");
        break;
      }
    }
    ++instructionCounter_;
  }
  const unsigned int* get() const {
    if (locDegrees_.size() > resultLoc_) {
      return locDegrees_[resultLoc_].getDegrees();
    }
    return NULL;
  }
};

} // namespace eval

#undef LOG_MODULE
#endif  // NTRP_EVAL_INSTRUCTION_MAXDEGREE_H
