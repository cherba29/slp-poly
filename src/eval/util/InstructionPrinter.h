#ifndef NTRP_EVAL_INSTRUCTION_PRINTER_H
#define NTRP_EVAL_INSTRUCTION_PRINTER_H

/**
 * @file InstructionPrinter.h Definition of parsed eval::InstructionPrinter
 */

#include "CodeAnnotation.h"

#include "math/field/util/Field-Util.h"

#include <ostream>
#include <iomanip>


namespace eval {

template <typename F>
std::ostream& operator<<(std::ostream& os, const eval::Instruction<F>& instr) {
  os << std::setw(10) << instr.getType().toString() << std::setw(4) << instr.getDestination();
  switch (instr.getType().getId()) {
    case InstructionEnum::ADD:
    case InstructionEnum::SUB:
    case InstructionEnum::MUL:
      os << std::setw(4) << instr.getSource1() ;
      os << ", " << std::setw(4) <<instr.getSource2();
      break;
    case InstructionEnum::DET:
    case InstructionEnum::POW:
      os << std::setw(4) << instr.getSource1();
      os << ", " << std::setw(4) << instr.getSource2();
      break;
    case InstructionEnum::ADDC:
    case InstructionEnum::SUBC:
    case InstructionEnum::MULC:
      os << std::setw(4) << instr.getSource1();
      os << ", " << std::setw(4) << "F(" << instr.getConstant() << ")";
      break;
    case InstructionEnum::SETM:
    case InstructionEnum::SET:
      os << std::setw(4) << "F(" << instr.getConstant() << ")";
      break;
    case InstructionEnum::SUBFROM:
    case InstructionEnum::ADDTO:
    case InstructionEnum::MULBY:
    case InstructionEnum::COPY:
    case InstructionEnum::NEG:
      os << std::setw(4) << instr.getSource1();
      break;
    default:
      BOOST_ASSERT(false);
      os << std::setw(4) << "<ERR>";
      break;
  }
  return os;
}



class InstructionPrinter {
  std::ostream& os_;
  const eval::CodeAnnotation& ann_;
  int count_;
public:
  InstructionPrinter(std::ostream& os, const eval::CodeAnnotation& ann)
     : os_(os), ann_(ann), count_(0) {}

  template <typename F>
  void operator()(const eval::Instruction<F>& instr) {
    const std::string& ann = ann_.getAnnotation(count_);
    if (ann.length() > 0) {
      if (ann.length() > 300) { os_ << ann.substr(0,300) << "..." << std::endl; }
      else { os_ << ann << std::endl; }
    }
    os_ << std::setw(6) << count_++ << std::setw(10) << instr.getType().toString()
        << std::setw(10) << ann_.getDataName(instr.getDestination())
        << " (" << instr.getDestination() << "), ";

    switch (instr.getType().getId()) {
      case InstructionEnum::ADD:
      case InstructionEnum::SUB:
      case InstructionEnum::MUL:
        os_ << std::setw(4) << ann_.getDataName(instr.getSource1())
            << " (" << instr.getSource1() << ")";
        os_ << ", " << std::setw(4) << ann_.getDataName(instr.getSource2())
            << " (" << instr.getSource2() << ")";
        break;
      case InstructionEnum::DET2P:
      case InstructionEnum::DET:
      case InstructionEnum::POW:
        os_ << std::setw(4) << ann_.getDataName(instr.getSource1()) << " (" << instr.getSource1() << ")";
        os_ << ", " << std::setw(4) << instr.getSource2();
        break;
      case InstructionEnum::ADDC:
      case InstructionEnum::SUBC:
      case InstructionEnum::MULC:
        os_ << std::setw(4) << ann_.getDataName(instr.getSource1()) << " (" << instr.getSource1() << ")";
        os_ << ", " << std::setw(4) << "F(" << instr.getConstant() << ")";
        break;
      case InstructionEnum::SETM:
      case InstructionEnum::SET:
        os_ << std::setw(4) << "F(" << instr.getConstant() << "), " << instr.getSource1();
        break;
      case InstructionEnum::SUBFROM:
      case InstructionEnum::ADDTO:
      case InstructionEnum::MULBY:
      case InstructionEnum::COPY:
      case InstructionEnum::NEG:
        os_ << std::setw(4) << ann_.getDataName(instr.getSource1()) << " (" << instr.getSource1() << ")";
        break;
      default:
        os_ << std::setw(4) << "<ERR>";
        break;
    }
    os_ << std::endl;
  }
};

}  // namespace eval

#endif // NTRP_EVAL_INSTRUCTION_PRINTER_H
