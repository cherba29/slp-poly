/**
 * @file MultiIndexMap.cpp Implementation of MultiIndexMap class
 *
 * Copyright (C) 2010  Arthur D. Cherba
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

#include "MultiIndexMap.h"

#include <ostream>
#include <sstream>
#include <string>

namespace {
bool stringContains(const std::string& str, const char* substrings[],
                    int nSubStrings) {
  for (int i = 0; i < nSubStrings; ++i) {
    if (str.find(substrings[i]) != std::string::npos) return true;
  }
  return false;
}

const char* needToQuote[] = {":", "[", "?", "]", "@", "!", "'", "`", "\"",
                             "{", "}", "&", "%", "#", ",", " ", "|", "*"};

const int needToQuoteSize = sizeof(needToQuote) / sizeof(const char*);

std::string ymlEscapeString(const std::string& str) {
  if (stringContains(str, needToQuote, needToQuoteSize)) {
    std::ostringstream oss;
    oss << '"';
    for (std::string::const_iterator i = str.begin(), end = str.end(); i != end;
         ++i) {
      unsigned char c = *i;
      if (' ' <= c and c <= '~' and c != '\\' and c != '"') {
        oss << c;
      } else {
        oss << '\\';
        switch (c) {
          case '"':
            oss << '"';
            break;
          case '\\':
            oss << '\\';
            break;
          case '\t':
            oss << 't';
            break;
          case '\r':
            oss << 'r';
            break;
          case '\n':
            oss << 'n';
            break;
          default:
            char const* const hexdig = "0123456789ABCDEF";
            oss << 'x';
            oss << hexdig[c >> 4];
            oss << hexdig[c & 0xF];
        }
      }
    }
    oss << '"';
    return oss.str();
  }
  return str;
}
}  // namespace

namespace util {

MultiIndexMap MultiIndexMap::error_;

const MultiIndexMap& MultiIndexMap::operator[](const std::string& idx) const {
  if (value_.which() == MultiIndexMap::MAP) {
    const MultiIndexMap::Mim& mim = boost::get<Mim>(this->value_);
    Mim::const_iterator it = mim.find(idx);
    if (it == mim.end()) return error_ = MultiIndexMap();
    return it->second;
  }
  return error_ = MultiIndexMap();
}

const MultiIndexMap& MultiIndexMap::operator[](int idx) const {
  if (value_.which() == MultiIndexMap::LIST) {
    const MultiIndexMap::Mil& mil = boost::get<Mil>(this->value_);
    Mil::const_iterator it = mil.find(idx);
    if (it == mil.end()) return error_ = MultiIndexMap();
    return it->second;
  }
  return error_ = MultiIndexMap();
}

MultiIndexMap& MultiIndexMap::operator[](const std::string& idx) {
  if (value_.which() != MultiIndexMap::MAP) {
    this->value_ = MultiIndexMap::Mim();
  }
  MultiIndexMap::Mim& mim = boost::get<Mim>(this->value_);
  return mim[idx];
}

MultiIndexMap& MultiIndexMap::operator[](int idx) {
  if (value_.which() != MultiIndexMap::LIST) {
    this->value_ = MultiIndexMap::Mil();
  }
  MultiIndexMap::Mil& mil = boost::get<Mil>(this->value_);
  return mil[idx];
}

MultiIndexMap& MultiIndexMap::operator+=(const std::string& val) {
  if (value_.which() != MultiIndexMap::LIST) {
    this->value_ = MultiIndexMap::Mil();
  }
  MultiIndexMap::Mil& mil = boost::get<Mil>(this->value_);
  mil[mil.size()] = val;
  return *this;
}

MultiIndexMap& MultiIndexMap::operator+=(const MultiIndexMap& val) {
  if (value_.which() != MultiIndexMap::LIST) {
    this->value_ = MultiIndexMap::Mil();
  }
  MultiIndexMap::Mil& mil = boost::get<Mil>(this->value_);
  mil[mil.size()] = val;
  return *this;
}

class stream_value : public boost::static_visitor<> {
  std::ostream* os_;
  int indent_;

 public:
  stream_value(std::ostream* os, int indent) : os_(os), indent_(indent) {}
  void operator()(const std::string& str) const {
    *os_ << ymlEscapeString(str);
  }
  void operator()(const MultiIndexMap::Mil& list) const {
    MultiIndexMap::Mil::const_iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
      if (it->second.value_.which() != MultiIndexMap::STRING) break;
    }
    bool isPureList = (it == list.end());
    if (isPureList) {
      *os_ << "[ ";
      for (it = list.begin(); it != list.end(); ++it) {
        if (it != list.begin()) *os_ << ", ";
        *os_ << it->second;
      }
      *os_ << " ]";
    } else {
      for (it = list.begin(); it != list.end(); ++it) {
        *os_ << std::endl;
        for (int i = 0; i < indent_; ++i) *os_ << "  ";
        *os_ << it->first << ": ";
        *os_ << it->second;
      }
    }
  }
  void operator()(const MultiIndexMap::Mim& map) const {
    MultiIndexMap::Mim::const_iterator it;
    for (it = map.begin(); it != map.end(); ++it) {
      *os_ << std::endl;
      for (int i = 0; i < indent_; ++i) *os_ << "  ";
      *os_ << it->first << ": ";
      *os_ << it->second;
    }
  }
};

std::ostream& operator<<(std::ostream& os, const MultiIndexMap& val) {
  static int indent = -1;
  indent += 1;
  boost::apply_visitor(stream_value(&os, indent), val.value_);
  indent -= 1;
  return os;
}

}  // namespace util
