
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

/*
 * Compute if a year is a leap year or not.
 * year 0 is translated into year 1900 CE.
 *
 * @note - For use by the HAL only
 */
bool _rtc_is_leap_year(int year);

/*
 * Thread safe (partial) replacement for mktime.
 * This function is tailored around the RTC specification and is not a full
 * replacement for mktime.
 * The fields from tm used are:
 *   - tm_sec
 *   - tm_min
 *   - tm_hour
 *   - tm_mday
 *   - tm_mon
 *   - tm_year
 * Other fields are ignored and won't be normalized by the call.
 * If the time in input is less than UNIX epoch (1st january of 1970 at 00:00:00),
 * then this function consider the input as invalid and will return time_t(-1).
 * Values in output range from 0 to INT_MAX.
 * Leap seconds are not supported.
 *
 * @note - For use by the HAL only
 */
time_t _rtc_mktime(const struct tm* time);

/*
 * Thread safe (partial) replacement for localtime.
 * This function is tailored around the RTC specification and is not a full
 * replacement for localtime.
 * The tm fields filled by this function are:
 *   - tm_sec
 *   - tm_min
 *   - tm_hour
 *   - tm_mday
 *   - tm_mon
 *   - tm_year
 * The time in input shall be in the range [0, INT32_MAX] otherwise the function
 * will return false and the structure time_info in input will remain untouch.
 *
 * @note - For use by the HAL only
 */
bool _rtc_localtime(time_t timestamp, struct tm* time_info);

#ifdef __cplusplus
}
#endif

#endif /* MBED_MKTIME_H */

/** @}*/
