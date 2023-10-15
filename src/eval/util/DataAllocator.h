#ifndef NTRP_EVAL_UTIL_DATAALLOCATOR_H
#define NTRP_EVAL_UTIL_DATAALLOCATOR_H

/**
 * @file DataAllocator.h Definition of eval::CodeGenerator class
 */

#include <map>

namespace eval {

class DataAllocator {
  int maxAllocatedLocation;
  std::map<int, bool> locations;  // Location Index, isTemporary
 public:
  DataAllocator() : maxAllocatedLocation(-1) {}

  int getMaxAllocSize() const { return maxAllocatedLocation; }

  int getCurrentAllocSize() const {
    if (this->locations.size() > 0) {
      return (--(this->locations.end()))->first;
    }
    return 0;
  }

  // Allocate continueous new memory/data location block
  int reserveLocation(int size = 1, bool isTemporary = true) {
    int location = -1;
    // Find a gap, in case some location was released
    std::map<int, bool>::const_iterator it = this->locations.begin();
    for (; it != this->locations.end(); ++it) {
      if ((it->first - location) > size) {
        location = it->first - size - 1;  // Next location will allocated
        break;
      }
      location = it->first;
    }
    int startPos = location + 1;
    for (int i = 0; i < size; ++i) {
      this->locations.insert(std::pair<int, bool>(++location, isTemporary));
    }
    if (startPos + size > maxAllocatedLocation) {
      maxAllocatedLocation = startPos + size;
    }
    return startPos;
  }

  void releaseLocation(int locIdx) {
    std::map<int, bool>::iterator it = this->locations.find(locIdx);
    if (it != this->locations.end()) {
      this->locations.erase(it);
    }
  }

  bool isTemporaryLocation(int locIdx) {
    std::map<int, bool>::iterator it = this->locations.find(locIdx);
    if (it != this->locations.end()) {
      return it->second;
    }
    return false;
  }
};

}  // namespace eval
#endif  // NTRP_EVAL_UTIL_DATAALLOCATOR_H
