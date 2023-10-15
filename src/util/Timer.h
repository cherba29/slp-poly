#ifndef NTRP_UTIL_TIMER_H
#define NTRP_UTIL_TIMER_H

/**
 * @file Timer.h interface for the Timer class.
 */

#include <cmath>
#include <ctime>
#include <sstream>
#include <string>

namespace util {
/**
 * @brief Returns a time elapsed in days, hours, minutes, seconds
 */
std::string formatSecs(long double secs);

/**
 * @brief Returns the current date and time
 */
std::string getCurrentTimeString();

/**
 * @brief A basic stopwatch.
 */
class Timera {
  clock_t _start; /**< Start of the time being measured     */
  double _lapse;  /**< Most recently measured time interval */

 public:
  /**
   * @brief Constructs a timer and starts it
   */
  Timera() {
    _start = clock();  // Returns the number of clock ticks elapsed
                       // since the program was launched.
    _lapse = 0.0;
  }
  /**
   * @brief Sets _lapse to seconds elapsed since the last call to this method
   *        (or since construction, if the current call is the first)
   */
  void measure() {
    clock_t finish = clock();
    _lapse = 1000.0 * (finish - _start) / CLOCKS_PER_SEC;
    _start = finish;
  }

  /**
   * @brief Returns last value measured.
   */
  double lapse() const { return _lapse; }
};

}  // namespace util

#endif  // NTRP_UTIL_TIMER_H
