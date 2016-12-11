#ifndef NTRP_EVAL_CODEGENERATOR_H
#error Do not include this file. This is used internaly by CodeGenerator.h
#endif

/**
 * @file CodeGenerator-Impl.h Implementation of eval::CodeGenerator
 */

#include "eval/util/InstructionPrinter.h"
#include "util/log.h"

#include <boost/log/attributes/named_scope.hpp>

//#include <algebra/BigInteger.h>
//#include <algebra/BigIntegerUtils.h>

namespace eval {
/*
template <typename F, int T> class Operation;
enum {SUM, PRODUCT};

template <typename F>
class Operation<F, SUM> {
public:
  static F identity() { return 0; }
  static bool isIdentity(const F& val) { return F.isZero(); }
  static typename eval::Instruction<F>::Type getLongInstruction(bool isPositive) {
    return isPositive?eval::Instruction<F>::Type::ADD
                     :eval::Instruction<F>::Type::SUB;
  }
  static typename eval::Instruction<F>::Type getAccumulateConstantInstruction() {
    return eval::Instruction<F>::Type::ADDC;
  }
  static void accumulate(F& sum, bool isPositive, F& val) {
    sum += isPositive?val:-val;
  }
};

template <typename F, typename Op, typename T>
class Processor {
public:
void operator()(CodeGenerator<F>& cg, T v, int& resultLocation_, F& resultValue_) {
  int nTerms = v.getNumberOfNodes();
  BOOST_SCOPED_LOG_CTX(LTRC_) << "Generating code for Sum " << nTerms << " terms...";
  int sumLocation = -1; // undefined, location where this sum will be stored
  int firstTermLocation = -1; // Location of first sum term
  F sumValue = Op::identity();  // Accumulation of constants
  eval::Instruction<F>::Type instrType;

  for (int i = 0; i < nTerms; ++i) {
    v.getNode(i).accept(cg); // generate code for this term
    // If result was a constant, just accumulate
    if (resultLocation_ < 0) {
      Op::accumulate(sumValue, v.isPos(i), this->resultValue_)
      continue;
    }
    // Establish sum location
    if (sumLocation < 0) { // If sum location is not set
      if (firstTermLocation >= 0) {
        if (cg.alloc.isTemporaryLocation(firstTermLocation)) {
          sumLocation = firstTermLocation;
        } else {
          sumLocation = this->alloc.reserveLocation();
          std::stringstream oss; oss << "_t" << sumLocation;
          annPtr_->setDataName(sumLocation, oss.str());

          instrType = Op::getLongInstruction(v.isPos(i));
          codePtr_->push_back(eval::Instruction<F>(
            instrType, firstTermLocation, this->resultLocation_, sumLocation));
          LDBG_ << codePtr_->back();
          continue;
        }
      } else {
        firstTermLocation = this->resultLocation_;
        continue;
      }
    }
    // It is not a constant and sum location is set
    if (i < nTerms-1) { // If this is not the last term
      instrType = Op::getSortInstruction(v.isPos(i));
      codePtr_->push_back(eval::Instruction<F>(
        instrType, this->resultLocation_, 0, sumLocation));
    } else {
      instrType = Op::getLongInstruction(v.isPos(i));
      codePtr_->push_back(eval::Instruction<F>(
        instrType, sumLocation, this->resultLocation_, sumLocation));
    }
    LDBG_ << codePtr_->back();
    // Release previous result location if it is temporary as it is no longer needed
    if (this->alloc.isTemporaryLocation(this->resultLocation_)) {
      this->alloc.releaseLocation(this->resultLocation_);
    }
  }
  // Once we passed though all sum terms, we have 3 cases
  //  1. only contant(s) have been encountered (sumlocation < 0 && firstTermLocation < 0)
  //     - nothing todo just propogate the constant
  //  2. there was only one non-constant with possibly a accumulated constant
  //     (sumlocation < 0 && firstTermLocation >=0)
  //     - add constant if exist otherwise propogate
  //  3. there was at least few terms in the sum (sumlocation >= 0)
  //     - return sumLocation (after adding in a constant)

  if (sumLocation < 0) {
    if (firstTermLocation < 0) { // case 1
      this->resultValue_ = sumValue;  // Only set resultValue as constant
      this->resultLocation_ = -1;
    } else { // case 2
      if (!Op::isIdentity(sumValue)) {
        if (this->alloc.isTemporaryLocation(firstTermLocation)) {
          sumLocation = firstTermLocation;
        } else {
          sumLocation = this->alloc.reserveLocation();
          std::stringstream oss; oss << "_t" << sumLocation;
          annPtr_->setDataName(sumLocation, oss.str());
        }
        codePtr_->push_back(eval::Instruction<F>(
          Op::getAccumulateConstantInstruction(), firstTermLocation, sumValue, sumLocation));
        LDBG_ << codePtr_->back();
      } else {
        sumLocation = firstTermLocation;
      }
      this->resultLocation_ = sumLocation;
    }
  } else { // case 3, nothing to do except perhaps add in accumulated constant if any
    if (!sumValue.isZero()) {
      codePtr_->push_back(eval::Instruction<F>(
        Op::getAccumulateConstantInstruction(), sumLocation, sumValue, sumLocation));
      LDBG_ << codePtr_->back();
    }
    resultLocation_ = sumLocation;
  }
}
};

template <typename F>
void CodeGenerator<F>::process(const context::Sum& v) {
  eval::Processor<typename F, Operation<F, SUM>, const context::Sum&> process;
  process(*this, v, this->resultLocation_, this->resultValue_);
}

template <typename F>
void CodeGenerator<F>::process(const context::Product& v) {
  eval::Processor<F, Operation<F, PRODUCT>, const context::Product&> process;
  process(*this, v, this->resultLocation_, this->resultValue_);
}
*/

template <typename F>
void CodeGenerator<F>::process(const context::Sum& v) {
  int nTerms = v.getNumberOfNodes();
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for Sum " << nTerms << " terms...";
  int sumLocation = -1; // undefined, location where this sum will be stored
  int firstTermLocation = -1; // Location of first sum term
  F sumValue(0);  // Accumulation of constants
  eval::InstructionEnum instrType;

  for (int i = 0; i < nTerms; ++i) {
    v.getNode(i).accept(*this); // generate code for this term
    // If result was a constant, just accumulate
    if (resultLocation_ < 0) {
      sumValue += v.isPos(i)?this->resultValue_:-this->resultValue_;
      continue;
    }
    // Establish sum location
    if (sumLocation < 0) { // If sum location is not set
      if (firstTermLocation >= 0) {
        if (this->alloc.isTemporaryLocation(firstTermLocation)) {
          sumLocation = firstTermLocation;
        } else {
          sumLocation = this->alloc.reserveLocation();
          std::stringstream oss; oss << "_t" << sumLocation;
          annPtr_->setDataName(sumLocation, oss.str());
          instrType = v.isPos(i)?eval::InstructionEnum::ADD
                                :eval::InstructionEnum::SUB;
          codePtr_->push_back(eval::Instruction<F>(
            instrType, firstTermLocation, this->resultLocation_, sumLocation));
          LDBG_ << codePtr_->back();
          continue;
        }
      } else {
        firstTermLocation = this->resultLocation_;
        continue;
      }
    }
    // It is not a constant and sum location is set
    if (i < nTerms-1) { // If this is not the last term
      instrType = v.isPos(i)?eval::InstructionEnum::ADDTO
                            :eval::InstructionEnum::SUBFROM;
      codePtr_->push_back(eval::Instruction<F>(
        instrType, this->resultLocation_, 0, sumLocation));
    } else {
      instrType = v.isPos(i)?eval::InstructionEnum::ADD
                            :eval::InstructionEnum::SUB;
      codePtr_->push_back(eval::Instruction<F>(
        instrType, sumLocation, this->resultLocation_, sumLocation));
    }
    LDBG_ << codePtr_->back();
    // Release previous result location if it is temporary as it is no longer needed
    if (this->alloc.isTemporaryLocation(this->resultLocation_)) {
      this->alloc.releaseLocation(this->resultLocation_);
    }
  }
  // Once we passed though all sum terms, we have 3 cases
  //  1. only contant(s) have been encountered (sumlocation < 0 && firstTermLocation < 0)
  //     - nothing todo just propogate the constant
  //  2. there was only one non-constant with possibly a accumulated constant
  //     (sumlocation < 0 && firstTermLocation >=0)
  //     - add constant if exist otherwise propogate
  //  3. there was at least few terms in the sum (sumlocation >= 0)
  //     - return sumLocation (after adding in a constant)

  if (sumLocation < 0) {
    if (firstTermLocation < 0) { // case 1
      this->resultValue_ = sumValue;  // Only set resultValue as constant
      this->resultLocation_ = -1;
    } else { // case 2
      if (!sumValue.template is<0>()) {
        if (this->alloc.isTemporaryLocation(firstTermLocation)) {
          sumLocation = firstTermLocation;
        } else {
          sumLocation = this->alloc.reserveLocation();
          std::stringstream oss; oss << "_t" << sumLocation;
          annPtr_->setDataName(sumLocation, oss.str());
        }
        codePtr_->push_back(eval::Instruction<F>(
          eval::InstructionEnum::ADDC, firstTermLocation, sumValue, sumLocation));
        LDBG_ << codePtr_->back();
      } else {
        sumLocation = firstTermLocation;
      }
      this->resultLocation_ = sumLocation;
    }
  } else { // case 3, nothing to do except perhaps add in accumulated constant if any
    if (!sumValue.template is<0>()) {
      codePtr_->push_back(eval::Instruction<F>(
        eval::InstructionEnum::ADDC, sumLocation, sumValue, sumLocation));
      LDBG_ << codePtr_->back();
    }
    resultLocation_ = sumLocation;
  }
}

template <typename F>
void CodeGenerator<F>::process(const context::Product& v) {
  int nTerms = v.getNumberOfNodes();
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for Product " << nTerms << " terms...";
  int sumLocation = -1; // undefined, location where this sum will be stored
  int firstTermLocation = -1; // Location of first sum term
  F sumValue(1);  // Accumulation of constants
  eval::InstructionEnum instrType;

  for (int i = 0; i < nTerms; ++i) {
    v.getNode(i).accept(*this); // generate code for this term
    // If result was a constant, just accumulate
    if (resultLocation_ < 0) {
      sumValue *= v.isPos(i)?this->resultValue_:this->resultValue_.getInverse();
      continue;
    }
    // Establish sum location
    if (sumLocation < 0) { // If sum location is not set
      if (firstTermLocation >= 0) {
        if (this->alloc.isTemporaryLocation(firstTermLocation)) {
          sumLocation = firstTermLocation;
        } else {
          sumLocation = this->alloc.reserveLocation();
          std::stringstream oss; oss << "_t" << sumLocation;
          annPtr_->setDataName(sumLocation, oss.str());
          instrType = v.isPos(i)?eval::InstructionEnum::MUL
                                :eval::InstructionEnum::DIV;
          codePtr_->push_back(eval::Instruction<F>(
            instrType, firstTermLocation, this->resultLocation_, sumLocation));
          LDBG_ << codePtr_->back();
          continue;
        }
      } else {
        firstTermLocation = this->resultLocation_;
        continue;
      }
    }
    // It is not a constant and sum location is set
    if (i < nTerms-1) { // If this is not the last term
      instrType = v.isPos(i)?eval::InstructionEnum::MULBY
                            :eval::InstructionEnum::DIVBY;
      codePtr_->push_back(eval::Instruction<F>(
        instrType, this->resultLocation_, 0, sumLocation));
    } else {
      instrType = v.isPos(i)?eval::InstructionEnum::MUL
                            :eval::InstructionEnum::DIV;
      codePtr_->push_back(eval::Instruction<F>(
        instrType, sumLocation, this->resultLocation_, sumLocation));
    }
    LDBG_ << codePtr_->back();
    // Release previous result location if it is temporary as it is no longer needed
    if (this->alloc.isTemporaryLocation(this->resultLocation_)) {
      this->alloc.releaseLocation(this->resultLocation_);
    }
  }
  // Once we passed though all sum terms, we have 3 cases
  //  1. only contant(s) have been encountered (sumlocation < 0 && firstTermLocation < 0)
  //     - nothing todo just propogate the constant
  //  2. there was only one non-constant with possibly a accumulated constant
  //     (sumlocation < 0 && firstTermLocation >=0)
  //     - add constant if exist otherwise propogate
  //  3. there was at least few terms in the sum (sumlocation >= 0)
  //     - return sumLocation (after adding in a constant)

  if (sumLocation < 0) {
    if (firstTermLocation < 0) { // case 1
      this->resultValue_ = sumValue;  // Only set resultValue as constant
      this->resultLocation_ = -1;
    } else { // case 2
      if (!sumValue.template is<1>()) {
        if (this->alloc.isTemporaryLocation(firstTermLocation)) {
          sumLocation = firstTermLocation;
        } else {
          sumLocation = this->alloc.reserveLocation();
          std::stringstream oss; oss << "_t" << sumLocation;
          annPtr_->setDataName(sumLocation, oss.str());
        }
        codePtr_->push_back(eval::Instruction<F>(
          eval::InstructionEnum::MULC, firstTermLocation, sumValue, sumLocation));
        LDBG_ << codePtr_->back();
      } else {
        sumLocation = firstTermLocation;
      }
      this->resultLocation_ = sumLocation;
    }
  } else { // case 3, nothing to do except perhaps add in accumulated constant if any
    if (!sumValue.template is<1>()) {
      codePtr_->push_back(eval::Instruction<F>(
        eval::InstructionEnum::MULC, sumLocation, sumValue, sumLocation));
      LDBG_ << codePtr_->back();
    }
    resultLocation_ = sumLocation;
  }
}

/*

template <typename F>
void CodeGenerator<F>::process(const context::Sum& v) {
  // TODO: need to be refactored
  int nTerms = v.getNumberOfNodes();
  BOOST_SCOPED_LOG_CTX(LTRC_) << "Generating code for Sum " << nTerms << " terms...";

  int sumLocation = -1; // undefined, location where this sum will be stored
  int firstTermLocation = -1; // Location of first sum term
  F sumValue = 0;  // Accumulation of constants
  for (int i = 0; i < nTerms; ++i) {
    v.getNode(i).accept(*this); // generate code for this term
    if (resultLocation_ < 0) { // If result was a constant
      if (v.isPos(i)) {
        sumValue += resultValue_;
      } else {
        sumValue -= resultValue_;
      }
    } else { // Result is not a constant
      if (sumLocation >= 0) { // If sum location already determined
        if (i < nTerms-1) { // If this is not the last term
          if (v.isPos(i)) {
            LDBG_ << "ADDTO   " << sumLocation << ", "  << this->resultLocation_;
            codePtr_->push_back(eval::Instruction<F>(
              eval::Instruction<F>::Type::ADDTO, this->resultLocation_, 0, sumLocation));
          } else {
            LDBG_ << "SUBFROM " << sumLocation << ", "  << this->resultLocation_;
            codePtr_->push_back(eval::Instruction<F>(
              eval::Instruction<F>::Type::SUBFROM, this->resultLocation_, 0, sumLocation));
          }
        } else { // This is last term of the sum
          if (v.isPos(i)) {
            LDBG_ << "ADD   " << sumLocation << ", "  << sumLocation << ", " << resultLocation_;
            codePtr_->push_back(eval::Instruction<F>(
              eval::Instruction<F>::Type::ADD, this->resultLocation_, sumLocation, sumLocation));
          } else {
            LDBG_ << "SUB   " << sumLocation << ", "  << sumLocation << ", " << resultLocation_;
            codePtr_->push_back(eval::Instruction<F>(
              eval::Instruction<F>::Type::SUB, sumLocation, this->resultLocation_, sumLocation));
          }
        }
        if (this->alloc.isTemporaryLocation(this->resultLocation_)) {
          this->alloc.releaseLocation(this->resultLocation_);
        }
      } else { // very first non constant term
        if (firstTermLocation >= 0) {
          if (this->alloc.isTemporaryLocation(firstTermLocation)) {
            sumLocation = firstTermLocation;
            if (i < nTerms-1) { // If this is not the last term
              if (v.isPos(i)) {
                LDBG_ << "ADDTO  " << sumLocation << ", "  << this->resultLocation_;
                codePtr_->push_back(eval::Instruction<F>(
                  eval::Instruction<F>::Type::ADDTO, this->resultLocation_, 0, sumLocation));
              } else {
                LDBG_ << "SUBFROM " << sumLocation << ", "  << this->resultLocation_;
                codePtr_->push_back(eval::Instruction<F>(
                  eval::Instruction<F>::Type::SUBFROM, this->resultLocation_, 0, sumLocation));
              }
            } else {
              if (v.isPos(i)) {
                LDBG_ << "ADD   " << sumLocation << ", "  << sumLocation << ", " << resultLocation_;
                codePtr_->push_back(eval::Instruction<F>(
                  eval::Instruction<F>::Type::ADD, this->resultLocation_, sumLocation, sumLocation));
              } else {
                LDBG_ << "SUB   " << sumLocation << ", "  << sumLocation << ", " << resultLocation_;
                codePtr_->push_back(eval::Instruction<F>(
                  eval::Instruction<F>::Type::SUB, sumLocation, this->resultLocation_, sumLocation));
              }
            }
          } else {
            sumLocation = this->alloc.reserveLocation();
            std::stringstream oss; oss << "_t" << sumLocation;
            annPtr_->setDataName(sumLocation, oss.str());
            if (v.isPos(i)) {
              LDBG_ << "ADD  " << sumLocation << ", "  << firstTermLocation << ", " << this->resultLocation_;
              codePtr_->push_back(eval::Instruction<F>(
                eval::Instruction<F>::Type::ADD, this->resultLocation_, firstTermLocation, sumLocation));
            } else {
              LDBG_ << "SUB   " << sumLocation << ", "  << sumLocation << ", " << resultLocation_;
              codePtr_->push_back(eval::Instruction<F>(
                eval::Instruction<F>::Type::SUB, sumLocation, this->resultLocation_, sumLocation));
            }
          }
          if (this->alloc.isTemporaryLocation(this->resultLocation_)) {
            this->alloc.releaseLocation(this->resultLocation_);
          }
        } else {
          firstTermLocation = this->resultLocation_;
        }
      }
    }
  }
  if (sumLocation >= 0) { // and there was non-constant term
      if (!sumValue.isZero()) {
        LDBG_ << "ADDC  " << sumLocation << ", " << sumLocation << ", F(" << sumValue << ")";
        codePtr_->push_back(eval::Instruction<F>(eval::Instruction<F>::Type::ADDC, sumLocation, sumValue, sumLocation));
      }
  }  else {
    if (firstTermLocation >= 0) {  // But there was non-const term
      if (!sumValue.isZero()) {
          if (this->alloc.isTemporaryLocation(firstTermLocation)) {
            sumLocation = firstTermLocation;
            LDBG_ << "ADDC  " << sumLocation << ", " << sumLocation << ", F(" << sumValue << ")";
            codePtr_->push_back(eval::Instruction<F>(eval::Instruction<F>::Type::ADDC, sumLocation, sumValue, sumLocation));
          } else {
            sumLocation = this->alloc.reserveLocation();
            std::stringstream oss; oss << "_t" << sumLocation;
            annPtr_->setDataName(sumLocation, oss.str());

            LDBG_ << "ADDC  " << sumLocation << ", "  << firstTermLocation << ", F(" << sumValue << ")";
            codePtr_->push_back(eval::Instruction<F>(eval::Instruction<F>::Type::ADDC, firstTermLocation, sumValue, sumLocation));
          }
      } else {
        sumLocation = firstTermLocation;
      }
    }
  }
  resultLocation_ = sumLocation;
  if (resultLocation_ > 0) { // New temporary location was used
  } else {
    resultValue_ = sumValue;
  }
}
*/

template <typename F>
void CodeGenerator<F>::process(const context::Assignment& v) {
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for Assignment to '"
    << v.getId() << "' (" << cntxPtr_->getIdIndex(v.getId()) << ")";

  v.getValue().accept(*this);  // Process right hand side first.

  // Get last instruction produced by rhs.
  if (!codePtr_ || (codePtr_->size() == 0)) {
    const char* err = "Unexpected null or empty code array while generating code for assignment";
    LERR_ << err;
    throw std::runtime_error(err);
  }
  // Get index of a variable to assign to.
  int idxDest = this->cntxPtr_->getIdIndex(v.getId());
  if (resultLocation_ < 0) {  // Result is just a constant.
    codePtr_->push_back(eval::Instruction<F>(
        eval::InstructionEnum::SET, 0, resultValue_,idxDest));
  } else if (!this->alloc.isTemporaryLocation(resultLocation_)) {
      //resultLocation_ != currDataSize_-1) {
    codePtr_->push_back(eval::Instruction<F>(
        eval::InstructionEnum::COPY, resultLocation_, 0, idxDest));
  } else { // Temporary location, modify last instruction
    eval::Instruction<F>& last = codePtr_->back();
    // Change destination of last instruction to it.
    last.setDestination(idxDest);
    // Anotate destination location with a name.
    annPtr_->setDataName(idxDest, v.getId());
  }
}

template <typename F>
void CodeGenerator<F>::process(const context::Determinant& v) {
  const context::Matrix& mtx = v.getMatrix();

  int nRows = mtx.getNRows();
  int nCols = mtx.getNCols();

  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for Determinant of matrix "
    << mtx.getName() << " " << nRows << "x" << nCols;
  //int size2 = math::intlog(nRows);

  // Reserve memmory locations for this matrix
  int matrixStart = this->alloc.reserveLocation(
      nRows * nCols /* 1 << (size2 << 1) */, false /* non-temporary */);


  //std::vector<std::pair<int,int> > zeroes;
  //int prevZeroLocation = -1;
  codePtr_->push_back(eval::Instruction<F>(
    eval::InstructionEnum::SETM, nRows*nCols, F(0), matrixStart));

  LDBG_ << this->codePtr_->back();

  for (int i = 0; i < nRows; i++) {
    for (int j = 0; j < nCols; j++) {
      //int loc = matrixStart+(i<<size2)|j;
      int loc = matrixStart+i*nRows+j;

      const context::Value& val = mtx.getEntry(i,j);

      // Get string representation of matrix entry assignment
      std::ostringstream ss;
      ss << "(" << i << "," << j << ") = ";
      context::Printer strPrinter(ss);
      val.accept(strPrinter);
      int startInstr = this->codePtr_->size();

      // Compile right side of matrix entry assignment
      val.accept(*this);

      // Set matrix entry

      if (resultLocation_ < 0) {
        if (!this->resultValue_.template is<0>()) {
          this->annPtr_->addAnnotation(startInstr, ss.str());
          codePtr_->push_back(eval::Instruction<F>(
            eval::InstructionEnum::SET, 1, this->resultValue_, loc));
          LDBG_ << this->codePtr_->back();
        }
      } else {
        this->annPtr_->addAnnotation(startInstr, ss.str());
        if (!this->alloc.isTemporaryLocation(resultLocation_)) {
          //resultLocation_ != currDataSize_-1) {
          codePtr_->push_back(eval::Instruction<F>(
             eval::InstructionEnum::COPY, resultLocation_, 0, loc));
          LDBG_ << this->codePtr_->back();
        } else {
          // No need to issue instruction, just change last destination.
          this->setLastDestination(loc);
        }
      }
      // Annotate matrix entry memory location
      std::stringstream oss;
      oss << mtx.getName() << "(" << i << "," << j << ")";
      annPtr_->setDataName(loc, oss.str());
    }
  }
  // Save result of the determinant into temporary location
  int loc = this->alloc.reserveLocation();
  codePtr_->push_back(eval::Instruction<F>(
      eval::InstructionEnum::DET, matrixStart, nRows, loc));
  LDBG_ << this->codePtr_->back();
  resultLocation_ = loc;
  // Anotate memory location
  std::stringstream oss; oss << "_t" << resultLocation_;
  annPtr_->setDataName(resultLocation_, oss.str());
}

template <typename F>
void CodeGenerator<F>::process(const context::Function& f) {
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for Function: " << f.getName().toString();
  switch (f.getName().getId()) {
    case context::FunctionEnum::DET: {
    } break;
    default:
      LERR_ << "Unimplemented function";
  }
}

template <typename F>
void CodeGenerator<F>::process(const context::Id& v) {
  int varIndex = cntxPtr_->getIdIndex(v.getName());
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for Id '"
      << v.getName() << "' (" << varIndex << ")";

  this->resultLocation_ = this->cntxPtr_->getIdIndex(v.getName());
}

template <typename F>
void CodeGenerator<F>::process(const context::Integer& v) {
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for Integer '"
      << v.getInt() << "'";

  // Convert integer represented as string into Field value this way unlimited
  // size integers are supported and only limited by field size.
  const std::string& val = v.getInt();

  this->resultValue_.template setTo<0>();
  const F radix(10);
  for (size_t i = 0; i < val.size(); ++i) {
    this->resultValue_ *= radix;
    this->resultValue_ += F(val[i] - '0');
  }
  this->resultLocation_ = -1;
}

template <typename F>
void CodeGenerator<F>::process(const context::Matrix& /* v */) {
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for Matrix...";
  throw std::logic_error("Cant generate code for Matrix");
}

/*
template <typename F>
void CodeGenerator<F>::process(const context::Product& v) {
  int nTerms = v.getNumberOfNodes();
  BOOST_SCOPED_LOG_CTX(LTRC_) << "Generating code for Product of " << nTerms << " terms...";

  int productLocation = -1; // undefined
  int firstTermLocation = -1; // undefined
  F productValue = 1; // occumulation of constants
  for (int i = 0; i < nTerms; ++i) {
    v.getNode(i).accept(*this); // generate code for this term
    if (resultLocation_ < 0) { // If result was a constant
      LDBG_ << "Got constant, accumulating";
      if (v.isPos(i)) {
        LDBG_ << productValue << " * " << resultValue_;
        productValue *= resultValue_;
      } else {
        LDBG_ << productValue << " / " << resultValue_;
        productValue /= resultValue_;
      }
    } else { // Result is not a constant
      if (productLocation >= 0) { // If we have a place to accumulate product
         if (i < nTerms-1) { // If not last term
           LDBG_ << "MULBY  " << productLocation << ", " << resultLocation_;
           codePtr_->push_back(eval::Instruction<F>(eval::Instruction<F>::Type::MULBY, resultLocation_, 0,  productLocation));
         } else {
           LDBG_ << "MUL  " << productLocation << ", " << productLocation << ", " << resultLocation_;
           codePtr_->push_back(eval::Instruction<F>(eval::Instruction<F>::Type::MUL, resultLocation_, productLocation,  productLocation));
         }
      } else { // No product location has not been established yet
        if (firstTermLocation >= 0) { // See if there was already a non constant term
          if (this->alloc.isTemporaryLocation(firstTermLocation)) {
            productLocation = firstTermLocation;
            LDBG_ << "MULTO  " << productLocation << ", "  << this->resultLocation_;
            codePtr_->push_back(eval::Instruction<F>(eval::Instruction<F>::Type::MULBY, this->resultLocation_, 0, productLocation));
          } else {
            productLocation = this->alloc.reserveLocation();
            std::stringstream oss; oss << "_t" << productLocation;
            annPtr_->setDataName(productLocation, oss.str());

            LDBG_ << "MUL  " << productLocation << ", "  << firstTermLocation << ", " << this->resultLocation_;
            codePtr_->push_back(eval::Instruction<F>(eval::Instruction<F>::Type::MUL, this->resultLocation_, firstTermLocation, productLocation));
          }
        } else { // This is first non-const term
          firstTermLocation = this->resultLocation_;
        }
      }
    }
  }
  if (productLocation >= 0) { // There was product accumulation location established
     if (!productValue.isOne()) {
       LDBG_ << "MULC  " << productLocation << ", " << productLocation << ", F(" << productValue << ")";
       codePtr_->push_back(eval::Instruction<F>(eval::Instruction<F>::Type::MULC, productLocation, productValue, productLocation));
     }
  } else {
    if (firstTermLocation >= 0) {  // But there was non-const term
      if (!productValue.isOne()) {
          if (this->alloc.isTemporaryLocation(firstTermLocation)) {
            productLocation = firstTermLocation;
            LDBG_ << "MULC  " << productLocation << ", " << productLocation << ", F(" << productValue << ")";
            codePtr_->push_back(eval::Instruction<F>(eval::Instruction<F>::Type::MULC, productLocation, productValue, productLocation));
          } else {
            productLocation = this->alloc.reserveLocation();
            std::stringstream oss; oss << "_t" << productLocation;
            annPtr_->setDataName(productLocation, oss.str());

            LDBG_ << "MULC  " << productLocation << ", "  << firstTermLocation << ", F(" << productValue << ")";
            codePtr_->push_back(eval::Instruction<F>(eval::Instruction<F>::Type::MULC, firstTermLocation, productValue, productLocation));
          }
      } else {
        productLocation = firstTermLocation;
      }
    }
  }
  resultLocation_ = productLocation;
  if (resultLocation_ > 0) { // New temporary location was used
  } else {
    resultValue_ = productValue;
  }
}
*/

template <typename F>
void CodeGenerator<F>::process(const context::NamedValue&) {
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for NamedValue...";
  throw std::logic_error("Cant generate code for NamedValue");
}

template <typename F>
void CodeGenerator<F>::process(const context::Negation& v) {
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for Negation...";
  v.getValue().accept(*this);
  if (this->resultLocation_ < 0) {
    this->resultLocation_ = -1;
    this->resultValue_.negate();
  } else {
    int loc = this->alloc.reserveLocation();
    std::stringstream oss; oss << "_t" << loc;
    annPtr_->setDataName(loc, oss.str());

    this->codePtr_->push_back(eval::Instruction<F>(
      eval::InstructionEnum::NEG, this->resultLocation_, 0, loc));
    LDBG_ << this->codePtr_->back();
    this->resultLocation_ = loc;
  }
}

template <typename F>
void CodeGenerator<F>::process(const context::Power& v) {
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for Power...";
  v.getLeftValue().accept(*this);
  long exponent = v.getExponent();
  if (this->resultLocation_ < 0) {
    this->resultLocation_ = -1;
    F val = this->resultValue_;
    for (int i = 0; i < exponent; i++) { this->resultValue_ *= val; }
  } else {
    int loc = this->alloc.reserveLocation();
    std::stringstream oss; oss << "_t" << loc;
    annPtr_->setDataName(loc, oss.str());

    codePtr_->push_back(eval::Instruction<F>(
      eval::InstructionEnum::POW, resultLocation_, exponent, loc));
    LDBG_ << this->codePtr_->back();
    this->resultLocation_ = loc;
  }
}

template <typename F>
void CodeGenerator<F>::process(const context::RealNumber&) {
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for RealNumber...";
  throw std::logic_error("Cant generate code for Real Number");
}

template <typename F>
void CodeGenerator<F>::process(const context::String&) {
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for String...";
  throw std::logic_error("Cant generate code for String");
}

template <typename F>
void CodeGenerator<F>::process(const context::Table&) {
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for Table...";
  LERR_ << "Cant generate code for table";
  throw std::logic_error("Cant generate code for table");
}

template <typename F>
void CodeGenerator<F>::process(const context::ValueList& /* v */) {
  BOOST_LOG_FUNCTION();
  LTRC_ << "Generating code for ValueList...";
  LERR_ << "Cant generate code for Value list";
  throw std::logic_error("Cant generate code for Value list");
}

}  // namespace eval
