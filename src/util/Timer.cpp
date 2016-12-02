/**
 * @brief Timer.cpp: implementation of the Timer class.
 */

#include "Timer.h"
#include <math.h>

namespace util {

std::string formatSecs(long double secs) {
	long double mins = floor(secs / 60.0L);
	secs = floor(fmod(secs, 60.0L) * 10) / 10;
	long double hrs = floor(mins / 60.0L);
	mins = fmod(mins, 60.0L);
	long double days = floor(hrs / 24.0L);
	hrs = fmod(hrs, 24.0L);
  long double years = floor(days / 365.0L);
  days = fmod(days, 365.0L);
	std::stringstream out;
  out << std::fixed;
  if (years > 0) {
    out << years << "y "
        << days << "d " << hrs << "h " << mins << "m " << secs << "s";
  } else if (days > 0) {
    out << days << "d " << hrs << "h " << mins << "m " << secs << "s";
  } else if (hrs > 0) {
    out << hrs << "h " << mins << "m " << secs << "s";
  } else if (mins > 0) {
    out << mins << "m " << secs << "s";
  } else {
    out << secs << "s";
  }
	return out.str();
}

std::string getCurrentTimeString() {
  // %H	Hour in 24h format (00-23)
  // %M	Minute (00-59)
  // %S	Second (00-61)
  // %b	Abbreviated month name e.g Aug (locale dependent)
  // %m	Month as a decimal number (01-12)
  // %d	Day of the month (01-31)
  // %Y	Year four digit
  // %c	Date and time representation (locale dependent)
  //     e.g. Thu Aug 23 14:55:02 2001

	const unsigned int buflen = 128;
	char timebuf[buflen];
	time_t ltime;
	time(&ltime);
	strftime(timebuf, buflen, "%Y-%m-%dT%H:%M:%S", localtime(&ltime));
	return timebuf;
}

}  // namespace util
