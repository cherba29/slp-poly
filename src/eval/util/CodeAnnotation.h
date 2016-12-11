#ifndef NTRP_EVAL_UTIL_CODEANNOTATION_H
#define NTRP_EVAL_UTIL_CODEANNOTATION_H

/**
 * @file CodeAnnotation.h Definition of eval::CodeAnnotation class
 */

#include <string>
#include <vector>

#include "util/log.h"

namespace eval {

#define LOG_MODULE logging::LogModuleEnum::CODEANNOTATION

class CodeAnnotation {
  std::vector<std::string> dataNames_;
  std::vector<std::string> codeAnnotation_;
  const std::string NOTSET_;
public:
  CodeAnnotation() : NOTSET_("") { }
  void setDataName(unsigned int idx, const std::string& name) {
    LTRC_ << "setDataName: " << idx << " " << name;
    if (dataNames_.size() < idx + 1) {
      dataNames_.resize(idx + 1);
    }
    dataNames_[idx] = name;
  }

	/**
	 * @brief
	 * Add annotation to instruction at given location.
	 *
	 * @param idx instruction index to annotate
	 * @param desc description string for given location of instruction
	 * @throws none
	 *
	 * Write detailed description for addAnnotation here.
	 *
	 * @see eval::Instruction | CodeAnnotation::getAnnotation
	 */
  void setAnnotation(unsigned int idx, const std::string& desc) {
    LTRC_ << "setAnnotation: " << idx << " " << desc.substr(0,80) << " ...";
    if (codeAnnotation_.size() < idx+1) { codeAnnotation_.resize(idx+1); }
    codeAnnotation_[idx] = desc;
  }

  void addAnnotation(unsigned int idx, const std::string& desc) {
    LTRC_ << "addAnnotation " << idx << " " <<desc;
    if (codeAnnotation_.size() < idx+1) { codeAnnotation_.resize(idx+1); }
    codeAnnotation_[idx] += desc;
  }

  const std::string& getDataName(unsigned int idx) const {
    if (idx < dataNames_.size())
      return dataNames_[idx];
    else
      return NOTSET_;
  }

  const std::string& getAnnotation(unsigned int idx) const {
    if (idx < codeAnnotation_.size()) {
      return codeAnnotation_[idx];
    } else {
      return NOTSET_;
    }
  }
};

}  // namespace eval
#undef LOG_MODULE
#endif  // NTRP_EVAL_UTIL_CODEANNOTATION_H
