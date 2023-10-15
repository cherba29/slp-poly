#ifndef SLP_POLY_UTIL_LOG_H
#define SLP_POLY_UTIL_LOG_H

/*
 * @file log.h Defines various logging primitives.
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

#include "util/LogLevelEnum.h"
#include "util/LogModuleEnum.h"
#include "util/LogTagEnum.h"

#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <vector>

#define LOGGER_(module, sv)                                   \
  BOOST_LOG_CHANNEL_SEV(slp_global_logger::get(), module, sv) \
      << boost::log::add_value("Line", __LINE__)              \
      << boost::log::add_value("File", __FILE__)              \
      << boost::log::add_value("Function", BOOST_CURRENT_FUNCTION)

#define LTRC_ LOGGER_(LOG_MODULE, logging::LogLevelEnum::TRACE)
#define LDBG_ LOGGER_(LOG_MODULE, logging::LogLevelEnum::DEBUG)
#define LINF_ LOGGER_(LOG_MODULE, logging::LogLevelEnum::INFO)
#define LINF1_ LOGGER_(LOG_MODULE, logging::LogLevelEnum::INFO1)
#define LINF2_ LOGGER_(LOG_MODULE, logging::LogLevelEnum::INFO2)
#define LAPP_ \
  LOGGER_(::logging::LogModuleEnum::APPLICATION, logging::LogLevelEnum::INFO)
#define LAPP1_ \
  LOGGER_(::logging::LogModuleEnum::APPLICATION, logging::LogLevelEnum::INFO1)
#define LAPP2_ \
  LOGGER_(::logging::LogModuleEnum::APPLICATION, logging::LogLevelEnum::INFO2)
#define LWRN_ LOGGER_(LOG_MODULE, logging::LogLevelEnum::WARNING)
#define LERR_ LOGGER_(LOG_MODULE, logging::LogLevelEnum::ERROR)
#define LFTL_ LOGGER_(LOG_MODULE, logging::LogLevelEnum::FATAL)

namespace logging {

typedef boost::log::sources::severity_channel_logger<LogLevelEnum,
                                                     LogModuleEnum>
    logger_type;

typedef boost::log::sinks::synchronous_sink<
    boost::log::sinks::text_ostream_backend>
    sink_t;

boost::shared_ptr<sink_t> init();

/**
 * @brief Set log level for each module in the list
 */
void setModuleLogLevels(
    boost::shared_ptr<sink_t>& sink,
    const std::vector<std::pair<LogModuleEnum, LogLevelEnum> >& moduleLevels);

/**
 * @brief Specify which tags to show on each log line
 * @see LogTagEnum
 */
void setLogTags(boost::shared_ptr<sink_t>& sink,
                const std::vector<LogTagEnum>& tags);

}  // namespace logging

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(slp_global_logger, logging::logger_type)

#endif  // SLP_POLY_UTIL_LOG_H
