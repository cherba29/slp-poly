#ifndef NTRP_UTIL_MULTIINDEXMAP_H
#define NTRP_UTIL_MULTIINDEXMAP_H

/**
 * @file MultiIndexMap.h
 * @brief Implementation of multiindex class
 */

#include <map>
//#include <vector>
#include <boost/variant.hpp>
#include <sstream>
#include <string>

namespace util {

/**
 * @brief Scoreboard serializable to yml file
 * @code
 *   MultiIndexMap m;
 *   m["Test"] = "Hello";
 *   std::string idx2 = "main";
 *   std::string data = "main-data";
 *   m["Test2"][idx] = data;
 *   m["Test2"] = "overwrite";
 *   std::cout << m["Test2"];
 *   std::cout << m;
 * @code
 */

class MultiIndexMap {
  typedef std::map<std::string, MultiIndexMap> Mim;
  typedef std::map<int, MultiIndexMap> Mil;
  enum Choice { STRING, LIST, MAP };

  boost::variant<std::string,
                 boost::recursive_wrapper<Mil>,  // List
                 boost::recursive_wrapper<Mim>   // Map
                 >
      value_;

  static MultiIndexMap error_;

  friend class stream_value;

 public:
  const MultiIndexMap& operator[](const std::string& idx) const;
  const MultiIndexMap& operator[](int idx) const;

  MultiIndexMap& operator[](const std::string& idx);
  MultiIndexMap& operator[](int idx);

  MultiIndexMap& operator=(const std::string& val) {
    this->value_ = val;
    return *this;
  }
  MultiIndexMap& operator=(const MultiIndexMap& val) {
    this->value_ = val.value_;
    return *this;
  }

  template <typename T>
  MultiIndexMap& operator=(const T& val) {
    std::ostringstream oss;
    oss << val;
    *this = oss.str();
    return *this;
  }

  MultiIndexMap& operator+=(const std::string& val);
  MultiIndexMap& operator+=(const MultiIndexMap& val);

  bool operator==(const std::string& val) const {
    if (value_.which() != STRING) return false;
    return boost::get<std::string>(this->value_) == val;
  }

  bool operator==(const MultiIndexMap& val) const {
    return value_ == val.value_;
  }

  template <typename T>
  bool operator==(const T& val) {
    std::ostringstream oss;
    oss << val;
    return *this == oss.str();
  }

  friend std::ostream& operator<<(std::ostream& os, const MultiIndexMap& val);
};

}  // namespace util

#endif  // NTRP_UTIL_MULTIINDEXMAP_H
