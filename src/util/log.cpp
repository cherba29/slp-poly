/*
 * @file log.cpp Implements setup of logging.
 *
 * Copyright 2016 Arthur D. Cherba
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "log.h"

#include <ostream>
#include <fstream>

#include <boost/core/null_deleter.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", logging::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "Timestamp", boost::posix_time::ptime)

namespace logging {

std::ostream& operator<< (std::ostream& strm, severity_level level) {
  static const char* strings[] = {
      "T ",  // trace
      "D ",  // debug
      "I2",  // info
      "I1",  // info
      "I ",  // info
      "A2",  // app
      "A1",  // app
      "A ",  // app
      "W ",  // warning
      "E ",  // error
      "F "   // fatal 
  };

  if (static_cast<std::size_t>(level) < sizeof(strings) / sizeof(*strings)) {
    strm << strings[level];
  } else {
    strm << static_cast<int>(level);
  }
  return strm;
}


void log_formatter(boost::log::record_view const& rec,
                  boost::log::formatting_ostream& strm) {
    strm << rec[line_id] << " ";
    strm << rec[severity] << " ";
    strm << rec[timestamp] << " ";
    strm << rec[channel] << " ";

    boost::log::value_ref<std::string> fullpath
        = boost::log::extract<std::string>("File", rec);
    strm << boost::filesystem::path(fullpath.get()).filename().string() << ":";

    strm << boost::log::extract<int>("Line", rec) << " ";

    strm << rec[boost::log::expressions::smessage];
}


void init() {
  typedef boost::log::sinks::synchronous_sink<
      boost::log::sinks::text_ostream_backend> text_sink;
  boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();

  boost::shared_ptr<std::ostream> stream(&std::clog, boost::null_deleter());
  sink->locked_backend()->add_stream(stream);

  sink->set_formatter(&log_formatter);

  boost::shared_ptr<boost::log::core> core = boost::log::core::get();
  core->add_sink(sink);

  core->add_global_attribute(
      line_id.get_name(), boost::log::attributes::counter<unsigned int>(1));
  core->add_global_attribute(
      timestamp.get_name(), boost::log::attributes::local_clock());
}

}  // namespace logging