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

#include "util/LogModuleEnum.h"
#include "util/LogTagEnum.h"

#include <boost/core/null_deleter.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <fstream>
#include <ostream>

BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", logging::LogLevelEnum)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", logging::LogModuleEnum)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "Timestamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(
    thread_id, "ThreadID",
    boost::log::attributes::current_thread_id::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope",
                            boost::log::attributes::named_scope::value_type)

namespace logging {

void log_formatter(const boost::log::record_view& rec,
                   boost::log::formatting_ostream& strm) {
  strm << rec[severity] << " ";
  strm << rec[boost::log::expressions::smessage];
}

bool log_filter(const boost::log::attribute_value_set& attr_set) {
  return attr_set[severity] > LogLevelEnum::DEBUG;
}

boost::shared_ptr<sink_t> init() {
  boost::shared_ptr<sink_t> sink = boost::make_shared<sink_t>();

  // Log to stderr.
  boost::shared_ptr<std::ostream> stream(&std::clog, boost::null_deleter());
  sink->locked_backend()->add_stream(stream);

  boost::log::register_simple_formatter_factory<LogLevelEnum, char>(
      severity.get_name());
  boost::log::register_simple_formatter_factory<LogModuleEnum, char>(
      channel.get_name());

  sink->set_formatter(&log_formatter);
  sink->set_filter(&log_filter);

  boost::shared_ptr<boost::log::core> core = boost::log::core::get();
  core->add_sink(sink);

  return sink;
}

void setModuleLogLevels(
    boost::shared_ptr<sink_t>& sink,
    const std::vector<std::pair<logging::LogModuleEnum,
                                logging::LogLevelEnum> >& moduleLevels) {
  typedef boost::log::expressions::channel_severity_filter_actor<
      logging::LogModuleEnum, logging::LogLevelEnum>
      min_severity_filter;
  min_severity_filter min_severity =
      boost::log::expressions::channel_severity_filter(channel, severity);

  for (unsigned int i = 0; i < moduleLevels.size(); ++i) {
    min_severity[moduleLevels[i].first] = moduleLevels[i].second;
  }

  sink->set_filter(min_severity);
}

void setLogTags(boost::shared_ptr<sink_t>& sink,
                const std::vector<logging::LogTagEnum>& tags) {
  boost::shared_ptr<boost::log::core> core = boost::log::core::get();

  std::ostringstream oss;
  for (size_t i = 0; i < tags.size(); ++i) {
    switch (tags[i].getId()) {
      case LogTagEnum::INDEX: {
        if (i && tags[i - 1] == logging::LogTagEnum::THREAD_ID) {
          oss << ".";
        }
        oss << "[%" << line_id.get_name() << "%] ";
      } break;
      case LogTagEnum::THREAD_ID: {
        oss << "T%" << thread_id.get_name() << "% ";
      } break;
      case LogTagEnum::TIME: {
        oss << "%" << timestamp.get_name() << "% ";
      } break;
      case LogTagEnum::MODULE: {
        oss << "%" << channel.get_name() << "%";
      } break;
      case LogTagEnum::LEVEL: {
        if (i && tags[i - 1] == logging::LogTagEnum::MODULE) {
          oss << "-";
        }
        oss << "%" << severity.get_name() << "% ";
      } break;
      case LogTagEnum::FUNCTION: {
        oss << "%"
            << "File"
            << "% ";
      } break;
      case LogTagEnum::FILELINE: {
        oss << "%"
            << "Line"
            << "% ";
      } break;
      default:
        std::cerr << "Bad tag specification " << tags[i] << std::endl;
        break;
    }
  }
  oss << " %Message%";

  boost::log::basic_formatter<char> formatter =
      boost::log::parse_formatter(oss.str());
  sink->set_formatter(formatter);

  // Select attributes based on specified tags.
  for (size_t i = 0; i < tags.size(); ++i) {
    switch (tags[i].getId()) {
      case LogTagEnum::INDEX: {
        core->add_global_attribute(
            line_id.get_name(),
            boost::log::attributes::counter<unsigned int>(1));
      } break;
      case LogTagEnum::THREAD_ID: {
        core->add_global_attribute(thread_id.get_name(),
                                   boost::log::attributes::current_thread_id());

      } break;
      case LogTagEnum::TIME: {
        core->add_global_attribute(timestamp.get_name(),
                                   boost::log::attributes::local_clock());
      } break;
      case LogTagEnum::MODULE: {
      } break;
      case LogTagEnum::LEVEL: {
      } break;
      case LogTagEnum::FUNCTION: {
        core->add_global_attribute(scope.get_name(),
                                   boost::log::attributes::named_scope());
      } break;
      case LogTagEnum::FILELINE: {
      } break;
      default:
        std::cerr << "Bad tag specification " << tags[i] << std::endl;
        break;
    }
  }
}

}  // namespace logging
