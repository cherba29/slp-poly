/**
 * @file InterpProfile.cpp Implementation of InterpProfile
 */

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS 1
#include <boost/cstdint.hpp> // for UINT64_C
#endif  // #ifndef __STDC_CONSTANT_MACROS


#include "runprofile/InterpProfile.h"
#include "util/SmartAssert.h"

#include <boost/static_assert.hpp>
#include <boost/algorithm/string.hpp>


namespace runprofile {

void InterpProfile::setFieldOption(const std::string& val) {
  FieldOptionEnum opt(val.c_str());
  switch (opt.getId()) {
    case FieldOptionEnum::INVALID:
    case FieldOptionEnum::UNKNOWN: {
      bool is64 = false;

      std::vector<std::string> tokens;
      boost::split(tokens, val, boost::is_any_of(",;|"));
      primes_.resize(0);
      for(unsigned int i = 0; i < tokens.size(); ++i){
        std::istringstream iss(tokens[i]);
        uint64_t numVal;
        iss >> numVal;
        if (iss.fail()) {
          fieldOption_ = FieldOptionEnum::UNKNOWN;
          break;
        } else {
          is64 |= (numVal >= UINT64_C(0x00000000FFFFFFFF));
          primes_.push_back(numVal);
        }
      }
      if (!is64) {
        fieldOption_ = (primes_.size() > 1)
          ? (
#ifdef FIELD_CRT32
              FieldOptionEnum::CRT32
#else
              FieldOptionEnum::UNKNOWN
#endif
             ):(
#ifdef FIELD_DYNAMIC32
               FieldOptionEnum::DYNAMIC32
#else
               FieldOptionEnum::UNKNOWN
#endif
             );
      }
      if (primes_.size() > 1) {
        fieldOption_ = is64?(
#ifdef FIELD_CRT64
            FieldOptionEnum::CRT64
#else
            FieldOptionEnum::UNKNOWN
#endif
        ):(
#ifdef FIELD_CRT32
            FieldOptionEnum::CRT32
#else
            FieldOptionEnum::UNKNOWN
#endif
        );
      } else {
        fieldOption_ = is64?(
#ifdef FIELD_DYNAMIC64
            FieldOptionEnum::DYNAMIC64
#else
            FieldOptionEnum::UNKNOWN
#endif
        ):(
#ifdef FIELD_DYNAMIC32
            FieldOptionEnum::DYNAMIC32
#else
            FieldOptionEnum::UNKNOWN
#endif
        );
      }
      break;
    }
#ifdef FIELD_DYNAMIC32
    case FieldOptionEnum::DYNAMIC32:
      fieldOption_ = FieldOptionEnum::UNKNOWN;
      break;
#endif
#ifdef FIELD_CRT32
    case FieldOptionEnum::CRT32:
      fieldOption_ = FieldOptionEnum::UNKNOWN;
      break;
#endif
#ifdef FIELD_DYNAMIC64
    case FieldOptionEnum::DYNAMIC64:
      fieldOption_ = FieldOptionEnum::UNKNOWN;
      break;
#endif
#ifdef FIELD_CRT64
    case FieldOptionEnum::CRT64:
      fieldOption_ = FieldOptionEnum::UNKNOWN;
      break;
#endif
    default:
      fieldOption_ = opt;
  }
}

}  // namespace runprofile
