
/** \addtogroup platform */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

/* Time range across the whole 32-bit range should be supported which means that years in range 1970 - 2106 can be
 * encoded. We have two types of RTC devices:
 * a) RTCs which handles all leap years in the mentioned year range correctly. Leap year is determined by checking if
 *    the year counter value is divisible by 400, 100, and 4. No problem here.
 * b) RTCs which handles leap years correctly up to 2100. The RTC does a simple bit comparison to see if the two
 *    lowest order bits of the year counter are zero. In this case 2100 year will be considered
 *    incorrectly as a leap year, so the last valid point in time will be 28.02.2100 23:59:59 and next day will be
 *    29.02.2100 (invalid). So after 28.02.2100 the day counter will be off by a day.
 */
typedef enum {
    RTC_FULL_LEAP_YEAR_SUPPORT,
    RTC_4_YEAR_LEAP_YEAR_SUPPORT
} rtc_leap_year_support_t;

/** Compute if a year is a leap year or not.
 *
 * @param year The year to test it shall be in the range [70:206]. Year 0 is
 * translated into year 1900 CE.
 * @param leap_year_support use RTC_FULL_LEAP_YEAR_SUPPORT if RTC device is able
 * to correctly detect all leap years in range [70:206] otherwise use RTC_4_YEAR_LEAP_YEAR_SUPPORT.
 *
 * @return true if the year in input is a leap year and false otherwise.
 *
 * @note For use by the HAL only
 * @note Year 2100 is treated differently for devices with full leap year support and devices with
 * partial leap year support. Devices with partial leap year support treats 2100 as a leap year.
 */
bool _rtc_is_leap_year(int year, rtc_leap_year_support_t leap_year_support);

/* Convert a calendar time into time since UNIX epoch as a time_t.
 *
 * This function is a thread safe (partial) replacement for mktime. It is
 * tailored around RTC peripherals needs and is not by any mean a complete
 * replacement of mktime.
 *
 * @param time The calendar time to convert into a time_t since epoch.
 * The fields from tm used for the computation are:
 *   - tm_sec
 *   - tm_min
 *   - tm_hour
 *   - tm_mday
 *   - tm_mon
 *   - tm_year
 * Other fields are ignored and won't be renormalized by a call to this function.
 * A valid calendar time is comprised between:
 * the 1st of January 1970 at 00:00:00 to the 7th of February 2106 at 06:28:15.
 * @param leap_year_support use RTC_FULL_LEAP_YEAR_SUPPORT if RTC device is able
 * to correctly detect all leap years in range [70:206] otherwise use RTC_4_YEAR_LEAP_YEAR_SUPPORT.
 * @param seconds holder for the result - calendar time as seconds since UNIX epoch.
 *
 * @return true on success, false if conversion error occurred.
 *
 * @note Leap seconds are not supported.
 * @note Values in output range from 0 to UINT_MAX.
 * @note Full and partial leap years support.
 * @note For use by the HAL only
 */
bool _rtc_maketime(const struct tm *time, time_t *seconds, rtc_leap_year_support_t leap_year_support);

/* Convert a given time in seconds since epoch into calendar time.
 *
 * This function is a thread safe (partial) replacement for localtime. It is
 * tailored around RTC peripherals specification and is not by any means a
 * complete of localtime.
 *
 * @param timestamp The time (in seconds) to convert into calendar time. Valid
 * input are in the range [0 : UINT32_MAX].
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
 * @param leap_year_support use RTC_FULL_LEAP_YEAR_SUPPORT if RTC device is able
 * to correctly detect all leap years in range [70:206] otherwise use RTC_4_YEAR_LEAP_YEAR_SUPPORT.
 * @return true if the conversion was successful, false otherwise.
 *
 * @note For use by the HAL only.
 * @note Full and partial leap years support.
 */
bool _rtc_localtime(time_t timestamp, struct tm *time_info, rtc_leap_year_support_t leap_year_support);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif /* MBED_MKTIME_H */

/** @}*/
