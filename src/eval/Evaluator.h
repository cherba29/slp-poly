#ifndef NTRP_EVAL_EVALUATOR_H
#define NTRP_EVAL_EVALUATOR_H

/**
 * @file Evaluator.h Definition of parsed eval::Evaluator Value
 */

#include "Instruction.h"

#include "eval/util/CodeAnnotation.h"
#include "math/field/util/Field-Util.h"
#include "util/log.h"

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <vector>

#define LOG_MODULE ::logging::LogModuleEnum::EVALUATOR

namespace eval {

/**
 * @brief This class represents a black box function
 *        in several variables
 */
template <typename F>
class Evaluator {
  int nVars_;
  bool homogenized_;
  std::vector<int> varOrder_; /**< variable order used during operator() call */
  int dataSize_;
  F* data_;
  boost::shared_ptr<std::vector<Instruction<F> > > code_;
  boost::shared_ptr<CodeAnnotation> ann_;
  //F* result_;  /**< Location of the result after each evaluation,
  //                  reference to element of data_ */
public:

  Evaluator(
      int nVars,
      int dataSize,
      boost::shared_ptr<std::vector<Instruction<F> > > code,
      boost::shared_ptr<CodeAnnotation> ann)
    : nVars_(nVars), // Homogenized version effectively has n+1 variables
      homogenized_(false), // Remember that if it is homogenized version
      varOrder_(nVars_),    // Preallocate this array
      dataSize_(dataSize),  // Data block already includes space for homogenizing var
      data_(new F[dataSize_]), // actually allocate data block, code refers to it by index
      code_(code),                 // Array of instructions to execute
      ann_(ann)
      //result_(data_+(code_->back().getDestination())) // Result is produced by last instruction
  {
    // Set default variable ordering
    for (int i = 0; i < nVars_; i++) { varOrder_[i] = i;  }
  }

  F operator()(const F* values);
//  F* operator()(const F* values, int n);

  template <typename T>
  void traceWith(T& f) {
    typename std::vector<Instruction<F> >::const_iterator instr = code_->begin();
    for (int ip = 0; instr != code_->end(); ip++, ++instr) {
      f(*instr);
    }
  }
  int getNumberOfVariables() const { return nVars_; }
  int getDataSize() const { return dataSize_; }
  int getCodeSize() const { return code_->size(); }

  /**
   * @brief Builds code annotation to be used with Evaluator::trace
   * @see CodeGenerator::getEvaluator, Evaluator::trace
   */
  const CodeAnnotation& getAnnotation() const {
    return *ann_;
  }

  bool swapInVariableOrder(std::vector<int>* order) {
    if (order->size() == nVars_+1) {
      homogenized_ = true;
    } else if (order->size() != nVars_) {
      return false;
    }
    varOrder_.swap(*order);
    return true;
  }
};

template <typename F>
F Evaluator<F>::operator()(const F* values) {
  //const Field& homVal = values[nVars_];
  for (int i = 0; i < nVars_; i++) {
    data_[varOrder_[i]] = values[i];
  }
  typename std::vector<Instruction<F> >::const_iterator instr = code_->begin();
  for (int ip = 0; instr != code_->end(); ip++, ++instr) {
    //LDBG_ << instr->getType().toString() << " "
    //      << instr->getDestination() << " "
    //      << instr->getSource1() << " "
    //      << instr->getSource2();
    F& dest = data_[instr->getDestination()];
    F& src1 = data_[instr->getSource1()];
    switch (instr->getType().getId()) {
      case InstructionEnum::ADD: {
        F& src2 = data_[instr->getSource2()];
        F::add(&dest, src1, src2);
        //LDBG_ << dest << " <-- " << src1 << " + " << src2;
        break;
      }
      case InstructionEnum::ADDC: {
        //F src2(instr->getSource2());
        F::add(&dest, src1, instr->getConstant());
        //LDBG_ << dest << " <-- " << src1 << " + " << src2;
        break;
      }
      case InstructionEnum::ADDTO: {
        dest += src1;
        //LDBG_ << dest << " += " << src1 ;
        break;
      }
      case InstructionEnum::MUL: {
        F& src2 = data_[instr->getSource2()];
        F::multiply(&dest, src1, src2);
        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << src1 << " * " << src2;
        break;
      }
      case InstructionEnum::MULC: {
        //F src2(instr->getSource2());
        F::multiply(&dest, src1, instr->getConstant());
        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << src1 << " * " << src2;
        break;
      }
      case InstructionEnum::MULBY: {
        dest *= src1;
        //LDBG_ << dest << " += " << src1 ;
        break;
      }
      case InstructionEnum::DIV: {
        F& src2 = data_[instr->getSource2()];
        F::divide(&dest, src1, src2);
        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << src1 << " / " << src2;
        break;
      }

      case InstructionEnum::DIVC: {
        //F src2(instr->getSource2());
        F::divide(&dest, src1, instr->getConstant());
        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << src1 << " / " << src2;
        break;
      }

      case InstructionEnum::DIVBY: {
        dest /= src1;
        //LDBG_ << dest << " /= " << src1 ;
        break;
      }

      case InstructionEnum::POW: {
        F::pow(&dest, src1, instr->getSource2());
        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " = " << src1 << "^" << instr->getSource2();
        break;
      }
      case InstructionEnum::NEG: {
        F::neg(&dest, src1);
        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <--  -" << src1;
        break;
      }
      case InstructionEnum::SUB: {
        F& src2 = data_[instr->getSource2()];
        F::subtract(&dest, src1, src2);
        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << src1 << " - " << src2;
        break;
      }
      case InstructionEnum::SUBC: {
        //F src2(instr->getSource2());
        F::subtract(&dest, src1, instr->getConstant());
        //LDBG_ << dest << " <-- " << src1 << " + " << src2;
        break;
      }
      case InstructionEnum::SUBFROM: {
        dest -= src1;
        //LDBG_ << dest << " += " << src1 ;
        break;
      }
      case InstructionEnum::SETM2P: {
        math::field::util::setFill2P(&dest, instr->getConstant(), instr->getSource1());
        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << instr->getSource1();
        break;
      }
      case InstructionEnum::SETM: {
        math::field::util::setFill(&dest, instr->getConstant(), instr->getSource1());
        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << instr->getSource1();
        break;
      }
      case InstructionEnum::SET: {
        dest = instr->getConstant();
                  //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << instr->getSource1();
        break;
      }
      case InstructionEnum::DET2P: {
        math::field::util::det2p<F>(&dest, &src1, instr->getSource2());
        break;
      }
      case InstructionEnum::DET: {
        /*
        int size = instr->getSource2();
        for (int i = 0; i < size; i++) {
          std::ostringstream oss;
          for (int j = 0; j < size; j++) {
            oss << std::setw(6) << *(&src1 + i*size+j);
          }
          LDBG_ << oss.str();
        }
        LDBG_ << "----------------------------";
        */
        math::field::util::det<F>(&dest, &src1, instr->getSource2());
        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest;
        /*
        for (int i = 0; i < size; i++) {
          std::ostringstream oss;
          for (int j = 0; j < size; j++) {
            oss << std::setw(6) << *(&src1 + i*size+j);
          }
          LAPP_ << oss.str();
        }
        */
        break;
      }
      case InstructionEnum::COPY: {
        dest = src1;
        //LDBG_ << dest << " <== " << src1;
        break;
      }
      default: {
        LERR_ << "Unimplemented instruction: " << instr->getType().toString();
        BOOST_ASSERT(false);
      }
    }
  }
  return *(data_+(code_->back().getDestination()));
  //return *result_;
}

//template <typename F>
//F* Evaluator<F>::operator()(const F* values, int n) {
//
//  ASSERT1(n <= maxBlockSize_)(n)(maxBlockSize_)
//    .msg("exceeding maximum block size supported bu this eveluator");
//
//  // Copy n points into data_ rearranging according to var order
//  F* dataTarget = data_;
//  const F* valuesSource = values;
//  for (int b = 0; b < n; ++b, dataTarget+=nVars_, values+=nVars_) {
//    for (int i = 0; i < nVars_; ++i) {
//      dataTarget[varOrder_[i]] = valuesSource[i];
//    }
//  }
//
//  typename std::vector<Instruction<F> >::const_iterator
//    instr = code_->begin();
//  for (int ip = 0; instr != code_->end(); ip++, ++instr) {
//    //LDBG_ << instr->getType().toString() << " "
//    //      << instr->getDestination() << " "
//    //      << instr->getSource1() << " "
//    //      << instr->getSource2();
//    F* dest = data_+(n*instr->getDestination());
//    F* src1 = data_+(n*instr->getSource1());
//    switch (instr->getType()) {
//      case Instruction<F>::Type::ADD: {
//        F* src2 = data_+n*instr->getSource2();
//        F::add(dest, src1, src2, n);
//        //LDBG_ << dest << " <-- " << src1 << " + " << src2;
//        break;
//      }
//      case Instruction<F>::Type::ADDC: {
//        //F src2(instr->getSource2());
//        F::add(dest, src1, instr->getConstant(), n);
//        //LDBG_ << dest << " <-- " << src1 << " + " << src2;
//        break;
//      }
//      case Instruction<F>::Type::ADDTO: {
//        F::add(dest, src1, n);
//        //dest += src1;
//        //LDBG_ << dest << " += " << src1 ;
//        break;
//      }
//      case Instruction<F>::Type::MUL: {
//        F* src2 = data_+(n*instr->getSource2());
//        F::multiply(dest, src1, src2, n);
//        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << src1 << " * " << src2;
//        break;
//      }
//      case Instruction<F>::Type::MULC: {
//        //F src2(instr->getSource2());
//        F::multiply(dest, src1, instr->getConstant(), n);
//        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << src1 << " * " << src2;
//        break;
//      }
//      case Instruction<F>::Type::MULBY: {
//        F::multiply(dest, src1, n);
//        //dest *= src1;
//        //LDBG_ << dest << " += " << src1 ;
//        break;
//      }
//      case Instruction<F>::Type::DIV: {
//        F* src2 = data_+(n*instr->getSource2());
//        F::divide(dest, src1, src2, n);
//        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << src1 << " / " << src2;
//        break;
//      }
//
//      case Instruction<F>::Type::DIVC: {
//        //F src2(instr->getSource2());
//        F::divide(dest, src1, instr->getConstant(), n);
//        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << src1 << " / " << src2;
//        break;
//      }
//
//      case Instruction<F>::Type::DIVBY: {
//        F::divide(dest, src1, n);
//        //dest /= src1;
//        //LDBG_ << dest << " /= " << src1 ;
//        break;
//      }
//
//      case Instruction<F>::Type::POW: {
//        F::pow(dest, src1, instr->getSource2(), n);
//        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " = " << src1 << "^" << instr->getSource2();
//        break;
//      }
//      case Instruction<F>::Type::NEG: {
//        F::neg(dest, src1, n);
//        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <--  -" << src1;
//        break;
//      }
//      case Instruction<F>::Type::SUB: {
//        F* src2 = data_+(n*instr->getSource2());
//        F::subtract(dest, src1, src2, n);
//        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << src1 << " - " << src2;
//        break;
//      }
//      case Instruction<F>::Type::SUBC: {
//        //F src2(instr->getSource2());
//        F::subtract(dest, src1, instr->getConstant(), n);
//        //LDBG_ << dest << " <-- " << src1 << " + " << src2;
//        break;
//      }
//      case Instruction<F>::Type::SUBFROM: {
//        F::subtract(dest, src1, n);
//        //dest -= src1;
//        //LDBG_ << dest << " += " << src1 ;
//        break;
//      }
//      case Instruction<F>::Type::SETM2P: {
//        math::field::util::setFill2P(dest, instr->getConstant(), instr->getSource1(), n);
//        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << instr->getSource1();
//        break;
//      }
//      case Instruction<F>::Type::SETM: {
//        math::field::util::setFill(dest, instr->getConstant(), instr->getSource1(),n);
//        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << instr->getSource1();
//        break;
//      }
//      case Instruction<F>::Type::SET: {
//        math::field::util::setFill(dest, instr->getConstant(), 1, n);
//        //dest = instr->getConstant();
//        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest << " <-- " << instr->getSource1();
//        break;
//      }
//      case Instruction<F>::Type::DET2P: {
//        math::field::util::det2p<F>(
//          dest,               // address of the result
//          src1,               // address of first element
//          instr->getSource2() // size, where layout size is 2^(ceil(log(size)))
//        );
//        break;
//      }
//      case Instruction<F>::Type::DET: {
//        /*
//        int size = instr->getSource2();
//        for (int i = 0; i < size; i++) {
//          std::ostringstream oss;
//          for (int j = 0; j < size; j++) {
//            oss << std::setw(6) << *(&src1 + i*size+j);
//          }
//          LDBG_ << oss.str();
//        }
//        LDBG_ << "----------------------------";
//        */
//        math::field::util::det<F>(
//          dest, // destination address
//          src1, // address of first element
//          instr->getSource2() // size x size
//        );
//        //LDBG_ << "loc[" << instr->getDestination() << "] = " << dest;
//        /*
//        for (int i = 0; i < size; i++) {
//          std::ostringstream oss;
//          for (int j = 0; j < size; j++) {
//            oss << std::setw(6) << *(&src1 + i*size+j);
//          }
//          LAPP_ << oss.str();
//        }
//        */
//        break;
//      }
//      case Instruction<F>::Type::COPY: {
//        F::copy(dest,src1);
//        //dest = src1;
//        //LDBG_ << dest << " <== " << src1;
//        break;
//      }
//      default: {
//        LERR_ << "Unimplemented instruction: " << instr->getType().toString();
//        BOOST_ASSERT(false);
//      }
//    }
//  }
//  return data_+(n*code_->back().getDestination());
//  //return *result_;
//}

} // namespace eval

#undef LOG_MODULE
#endif  // NTRP_EVAL_EVALUATOR_H
