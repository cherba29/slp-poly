/**
 * @file VandSolveTypeEnum.h
 * @brief Implementation of VandSolveTypeEnum Enumeration class
 *
 * Copyright (C) 2014  Arthur D. Cherba
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// DO NOT EDIT CHANGES WILL OVERWITEN WHEN REGENERATED
// Generated from template "tools/enumgen/cpp.tpl".

#include "interp/VandSolveTypeEnum.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>

namespace interp {

namespace { // Local helper data and functions

/**
 * @brief Structure to map enum name to id
 */
struct entry {
  const char* name;
  int size;
  VandSolveTypeEnum::Choice id;
};

// Enum string name to ID map sorted by name
entry const entries[VandSolveTypeEnum::NUM_ENUMS] = {
  { "BEST",    sizeof("BEST")-1,    VandSolveTypeEnum::BEST    },
  { "FFT",     sizeof("FFT")-1,     VandSolveTypeEnum::FFT     },
  { "INVALID", sizeof("INVALID")-1, VandSolveTypeEnum::INVALID },
  { "NA",      sizeof("NA")-1,      VandSolveTypeEnum::NA      },
  { "QUAD",    sizeof("QUAD")-1,    VandSolveTypeEnum::QUAD    },
  { "UNKNOWN", sizeof("UNKNOWN")-1, VandSolveTypeEnum::UNKNOWN },
};


VandSolveTypeEnum::Choice const idByOrigOrder[VandSolveTypeEnum::NUM_ENUMS] = {
  VandSolveTypeEnum::INVALID,
  VandSolveTypeEnum::QUAD,
  VandSolveTypeEnum::FFT,
  VandSolveTypeEnum::BEST,
  VandSolveTypeEnum::UNKNOWN,
  VandSolveTypeEnum::NA,
};


/**
 * @brief Structure to map enum value to id
 */
struct Val2Id {
  const char* value;
  VandSolveTypeEnum::Choice id;
};

// Value to ID map, sorted by value
Val2Id const values[VandSolveTypeEnum::NUM_ENUMS] = {
 // Value            Enum id
  { "*INVALID*",      VandSolveTypeEnum::INVALID },
  { "Not Applicable", VandSolveTypeEnum::NA      },
  { "Unknown",        VandSolveTypeEnum::UNKNOWN },
  { "adapt",          VandSolveTypeEnum::BEST    },
  { "fft",            VandSolveTypeEnum::FFT     },
  { "quad",           VandSolveTypeEnum::QUAD    },
};


inline int compare(const char* a, const char* b) { return ::strcmp(a,b); }

template <typename T>
inline int compare(T a, T b) { if (a<b) return -1; else return (a==b)?0:1; }

} // anonymous namespace

const char* VandSolveTypeEnum::VAL_OF_INVALID = "*INVALID*";      /**< Additional value, set to when object cannot be initialized */
const char* VandSolveTypeEnum::VAL_OF_QUAD    = "quad";           /**< Quadratic solver */
const char* VandSolveTypeEnum::VAL_OF_FFT     = "fft";            /**< FFT solver */
const char* VandSolveTypeEnum::VAL_OF_BEST    = "adapt";          /**< Predict which one is better solver */
const char* VandSolveTypeEnum::VAL_OF_UNKNOWN = "Unknown";        /**< Default non-selection option */
const char* VandSolveTypeEnum::VAL_OF_NA      = "Not Applicable"; /**< Non applicable option */

const char* VandSolveTypeEnum::NAME_OF_INVALID = "INVALID";
const char* VandSolveTypeEnum::NAME_OF_QUAD    = "QUAD";
const char* VandSolveTypeEnum::NAME_OF_FFT     = "FFT";
const char* VandSolveTypeEnum::NAME_OF_BEST    = "BEST";
const char* VandSolveTypeEnum::NAME_OF_UNKNOWN = "UNKNOWN";
const char* VandSolveTypeEnum::NAME_OF_NA      = "NA";


// get name for current enum value
char const* VandSolveTypeEnum::getName() const {
  switch (val_) {
    case INVALID: return "INVALID";
    case QUAD:    return "QUAD";
    case FFT:     return "FFT";
    case BEST:    return "BEST";
    case UNKNOWN: return "UNKNOWN";
    case NA:      return "NA";
    default:      return "INVALID";
  }
}

// Get display string for current enum value
char const* VandSolveTypeEnum::toString() const {
  switch (val_) {
    case INVALID: return "Invalid";
    case QUAD:    return "QuadVandermonde";
    case FFT:     return "FFTVandermonde";
    case BEST:    return "BestVandermonde";
    case UNKNOWN: return "Unknown";
    case NA:      return "Not Applicable";
    default:      return "Invalid";
  }
}

// Get value corresponding to given enum value
const char* VandSolveTypeEnum::getValue() const {
  switch (val_) {
    case INVALID: return "*INVALID*";
    case QUAD:    return "quad";
    case FFT:     return "fft";
    case BEST:    return "adapt";
    case UNKNOWN: return "Unknown";
    case NA:      return "Not Applicable";
    default:      return "*INVALID*";
  }
}

// Given value find enum id for it
VandSolveTypeEnum& VandSolveTypeEnum::setToValue(const char* val) {
  // Perform binary search for value 
  int start = 0;
  int end = VandSolveTypeEnum::NUM_ENUMS-1; // Index of last valid entry
  do {
    int mid = (end + start) >> 1;
    Val2Id const& e = values[mid];
    int cmp = compare(val,e.value);
    if (cmp < 0) { // val < e.value
      end = mid-1; 
    } else if (cmp > 0) { // val > e.value
      start = mid+1;
    } else {
      val_ = e.id; 
      return *this; 
    }
  } while (start <= end);
  val_ = INVALID;
  return *this;  
}


VandSolveTypeEnum& VandSolveTypeEnum::fromName(char const* str) {
  // Perform binary search for string
  int start = 0;
  int end = VandSolveTypeEnum::NUM_ENUMS-1;  // Index of last valid entry
  do {
    int mid = (end + start) >> 1;
    entry const& e = entries[mid];
    int cmp = ::strcmp(str,e.name);
    if (cmp < 0) { 
      end = mid-1; 
    } else if (cmp > 0) { 
      start = mid+1; 
    } else { 
      val_ = e.id; // found it
      return *this; 
    } 
  } while (start <= end);
  val_ = INVALID;
  return *this;
}

VandSolveTypeEnum& VandSolveTypeEnum::fromName(char const* str, int len) {
  // Perform binary search for string
  int start = 0;
  int end = VandSolveTypeEnum::NUM_ENUMS-1; // Index of last valid entry
  do {
    int mid = (end + start) >> 1;
    entry const& e = entries[mid];
    int cmp = ::strncmp(str,e.name,std::max(e.size,len));
    if (cmp < 0) { 
      end = mid-1; 
    } else if (cmp > 0) { 
      start = mid+1; 
    } else { 
      val_ = e.id; // found it
      return *this; 
    }
  } while (start <= end);
  val_ = INVALID;
  return *this;
}

VandSolveTypeEnum& VandSolveTypeEnum::setToIndex(int idx) {
  if (idx < 0 || idx >= VandSolveTypeEnum::NUM_ENUMS) { // Index out of bounds?
    val_ = INVALID;
  } else {
    val_ = idByOrigOrder[idx];
  }
  return *this;
}

int VandSolveTypeEnum::getIndex() const {
  switch (val_) {
    case INVALID: return 0
;
    case QUAD:    return 0
        + 1
;
    case FFT:     return 0
        + 1
        + 1
;
    case BEST:    return 0
        + 1
        + 1
        + 1
;
    case UNKNOWN: return 0
        + 1
        + 1
        + 1
        + 1
;
    case NA:      return 0
        + 1
        + 1
        + 1
        + 1
        + 1
;
    default: // Should not happen
      // ASSERT
      break;
  }
  return -1;
}

std::ostream& operator<<(std::ostream& os, VandSolveTypeEnum const& enm) {
  os << enm.getName();
  return os;           
}

std::ostream& operator<<(std::ostream& os, VandSolveTypeEnum::Choice const& enm) { 
  os << VandSolveTypeEnum(enm);
  return os;
}

std::istream& operator>>(std::istream& is, VandSolveTypeEnum& enm) {
  std::string val;
  is >> val;
  enm.fromName(val.c_str(),val.size());
  return is;
}



} // namespace interp
