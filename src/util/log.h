#ifndef SLP_POLY_UTIL_LOG_H
#define SLP_POLY_UTIL_LOG_H

#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>


#define LOGGER_(log_,sv) BOOST_LOG_SEV(log_, sv) \
  << boost::log::add_value("Line", __LINE__)     \
  << boost::log::add_value("File", __FILE__)     \
  << boost::log::add_value("Function", BOOST_CURRENT_FUNCTION)

#define LTRC_  LOGGER_(lg_, logging::trace)
#define LDBG_  LOGGER_(lg_, logging::debug)
#define LINF_  LOGGER_(lg_, logging::info)
#define LINF1_ LOGGER_(lg_, logging::info1)
#define LINF2_ LOGGER_(lg_, logging::info2)
#define LAPP_  LOGGER_(lg_, logging::app)
#define LAPP1_ LOGGER_(lg_, logging::app1)
#define LAPP2_ LOGGER_(lg_, logging::app2)
#define LWRN_  LOGGER_(lg_, logging::warning)
#define LERR_  LOGGER_(lg_, logging::error)
#define LFTL_  LOGGER_(lg_, logging::fatal)

namespace logging {

enum severity_level {
   trace,
   debug,
   info2,
   info1,
   info,
   app2,
   app1,
   app,
   warning,
   error,
   fatal
};

typedef boost::log::sources::severity_channel_logger<
    severity_level, std::string> logger_type;

void init();

}  // namespace logging

#define LOGGER(c) logging::logger_type lg_(boost::log::keywords::channel = #c);

#endif  // SLP_POLY_UTIL_LOG_H 

