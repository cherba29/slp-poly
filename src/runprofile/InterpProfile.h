#ifndef NTRP_INTERPPROFILE_H
#define NTRP_INTERPPROFILE_H

/**
 * @file InterpProfile.h Definition of InterpProfile classes.
 */

#include "interp/PrunningEnum.h"
#include "interp/UnivInterpEnum.h"
#include "interp/VandSolveTypeEnum.h"
#include "interp/VarOrderEnum.h"
#include "math/Base.h"
#include "runprofile/FieldOptionEnum.h"

#include <boost/optional.hpp>
#include <string>
#include <vector>

namespace runprofile {

/**
 * @brief Encapsulates parameters to interpolation.
 */
class InterpProfile {
 public:
  InterpProfile()
      : prunningChoice_(interp::PrunningEnum::UNKNOWN),
        univInterpAlg_(interp::UnivInterpEnum::UNKNOWN),
        vandSolver_(interp::VandSolveTypeEnum::UNKNOWN),
        varInterpOrder_(interp::VarOrderEnum::UNKNOWN),
        fieldOption_(FieldOptionEnum::UNKNOWN),
        verifyStage_(0),
        verifyFinal_(0) {}

  void setVerifyStage(unsigned int val) { verifyStage_ = val; }
  unsigned int getVerifyStage() const { return verifyStage_; }
  void setVerifyFinal(unsigned int val) { verifyFinal_ = val; }
  unsigned int getVerifyFinal() const { return verifyFinal_; }

  const interp::VarOrderEnum& getVarOrder() const { return varInterpOrder_; }
  void setVarOrder(const interp::VarOrderEnum& val) { varInterpOrder_ = val; }

  const FieldOptionEnum& getFieldOption() const { return fieldOption_; }
  const std::vector<uint64_t>& getFieldPrimes() const { return primes_; }

  void setFieldOption(const std::string& val);

  void setUnivInterpAlg(const interp::UnivInterpEnum& val) {
    univInterpAlg_ = val;
  }
  const interp::UnivInterpEnum& getUnivInterpAlg() const {
    return univInterpAlg_;
  }

  const interp::VandSolveTypeEnum& getVandermondeSolver() const {
    return vandSolver_;
  }
  void setVandermondeSolver(const interp::VandSolveTypeEnum& val) {
    vandSolver_ = val;
  }

  const interp::PrunningEnum& getPrunningChoice() const {
    return prunningChoice_;
  }
  void setPrunningChoice(const interp::PrunningEnum& val) {
    prunningChoice_ = val;
  }

  const boost::optional<std::string> getSaveFileName() const {
    return saveFileName_;
  }
  void setSaveFileName(const std::string& val) { saveFileName_ = val; }

  void setAsmFileName(const std::string& filename) { asmFileName_ = filename; }
  const boost::optional<std::string>& getAsmFileName() const {
    return asmFileName_;
  }

  void setRandomSeed(int val) { randomSeed_ = val; }
  const boost::optional<int>& getRandomSeed() const { return randomSeed_; }

 private:
  interp::PrunningEnum prunningChoice_;
  interp::UnivInterpEnum univInterpAlg_;
  interp::VandSolveTypeEnum vandSolver_;
  interp::VarOrderEnum varInterpOrder_;
  FieldOptionEnum fieldOption_;
  /** Primes to use for dynamic fields */
  std::vector<uint64_t> primes_;
  /** Verify intermediate polynomial at this many random points */
  unsigned int verifyStage_;
  /** Verify final polynomial at this many random points */
  unsigned int verifyFinal_;
  boost::optional<std::string> asmFileName_;
  boost::optional<std::string> saveFileName_;
  boost::optional<int> randomSeed_;
};

}  // namespace runprofile

#endif  // NTRP_INTERPPROFILE_H
