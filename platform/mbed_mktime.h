
/** \addtogroup platform */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_MKTIME_H
#define MBED_MKTIME_H

#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup platform_mktime mktime functions
 * @{
 */

/** Compute if a year is a leap year or not.
 *
 * @param year The year to test it shall be in the range [70:138]. Year 0 is
 * translated into year 1900 CE.
 * @return true if the year in input is a leap year and false otherwise.
 * @note - For use by the HAL only
 */
bool _rtc_is_leap_year(int year);

/* Convert a calendar time into time since UNIX epoch as a time_t.
 *
 * This function is a thread safe (partial) replacement for mktime. It is
 * tailored around RTC peripherals needs and is not by any mean a complete
 * replacement of mktime.
 *
 * @param calendar_time The calendar time to convert into a time_t since epoch.
 * The fields from tm used for the computation are:
 *   - tm_sec
 *   - tm_min
 *   - tm_hour
 *   - tm_mday
 *   - tm_mon
 *   - tm_year
 * Other fields are ignored and won't be renormalized by a call to this function.
 * A valid calendar time is comprised between the 1st january of 1970 at
 * 00:00:00 and the 19th of january 2038 at 03:14:07.
 *
 * @return The calendar time as seconds since UNIX epoch if the input is in the
 * valid range. Otherwise ((time_t) -1).
 *
 * @note Leap seconds are not supported.
 * @note Values in output range from 0 to INT_MAX.
 * @note - For use by the HAL only
 */
time_t _rtc_mktime(const struct tm* calendar_time);

/* Convert a given time in seconds since epoch into calendar time.
 *
 * This function is a thread safe (partial) replacement for localtime. It is
 * tailored around RTC peripherals specification and is not by any means a
 * complete of localtime.
 *
 * @param timestamp The time (in seconds) to convert into calendar time. Valid
 * input are in the range [0 : INT32_MAX].
 * @param calendar_time Pointer to the object which will contain the result of
 * the conversion. The tm fields filled by this function are:
 *   - tm_sec
 *   - tm_min
 *   - tm_hour
 *   - tm_mday
 *   - tm_mon
 *   - tm_year
 *   - tm_wday
 *   - tm_yday
 * The object remains untouched if the time in input is invalid.
 * @return true if the conversion was successful, false otherwise.
 *
 * @note - For use by the HAL only
 */
bool _rtc_localtime(time_t timestamp, struct tm* calendar_time);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif /* MBED_MKTIME_H */

/** @}*/
