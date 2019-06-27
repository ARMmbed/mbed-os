/* mbed Microcontroller Library
 * Copyright (c) 2017-2019 ARM Limited
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

#include "platform/mbed_mktime.h"

/* Time constants. */
#define SECONDS_BY_MINUTES 60
#define MINUTES_BY_HOUR 60
#define SECONDS_BY_HOUR (SECONDS_BY_MINUTES * MINUTES_BY_HOUR)
#define HOURS_BY_DAY 24
#define SECONDS_BY_DAY (SECONDS_BY_HOUR * HOURS_BY_DAY)
#define LAST_VALID_YEAR 206

/* Macros which will be used to determine if we are within valid range. */
#define EDGE_TIMESTAMP_FULL_LEAP_YEAR_SUPPORT 3220095     // 7th of February 1970 at 06:28:15
#define EDGE_TIMESTAMP_4_YEAR_LEAP_YEAR_SUPPORT 3133695  // 6th of February 1970 at 06:28:15

/*
 * 2 dimensional array containing the number of seconds elapsed before a given
 * month.
 * The second index map to the month while the first map to the type of year:
 *   - 0: non leap year
 *   - 1: leap year
 */
static const uint32_t seconds_before_month[2][12] = {
    {
        0,
        31 * SECONDS_BY_DAY,
        (31 + 28) *SECONDS_BY_DAY,
        (31 + 28 + 31) *SECONDS_BY_DAY,
        (31 + 28 + 31 + 30) *SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31) *SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30) *SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31) *SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31) *SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30) *SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) *SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) *SECONDS_BY_DAY,
    },
    {
        0,
        31 * SECONDS_BY_DAY,
        (31 + 29) *SECONDS_BY_DAY,
        (31 + 29 + 31) *SECONDS_BY_DAY,
        (31 + 29 + 31 + 30) *SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31) *SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30) *SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31) *SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31) *SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30) *SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) *SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) *SECONDS_BY_DAY,
    }
};

bool _rtc_is_leap_year(int year, rtc_leap_year_support_t leap_year_support)
{
    /*
     * since in practice, the value manipulated by this algorithm lie in the
     * range: [70 : 206] the algorithm can be reduced to: year % 4 with exception for 200 (year 2100 is not leap year).
     * The algorithm valid over the full range of value is:

        year = 1900 + year;
        if (year % 4) {
            return false;
        } else if (year % 100) {
            return true;
        } else if (year % 400) {
            return false;
        }
        return true;

     */
    if (leap_year_support == RTC_FULL_LEAP_YEAR_SUPPORT && year == 200) {
        return false; // 2100 is not a leap year
    }

    return (year) % 4 ? false : true;
}

bool _rtc_maketime(const struct tm *time, time_t *seconds, rtc_leap_year_support_t leap_year_support)
{
    if (seconds == NULL || time == NULL) {
        return false;
    }

    /* Partial check for the upper bound of the range - check years only. Full check will be performed after the
     * elapsed time since the beginning of the year is calculated.
     */
    if ((time->tm_year < 70) || (time->tm_year > LAST_VALID_YEAR)) {
        return false;
    }

    uint32_t result = time->tm_sec;
    result += time->tm_min * SECONDS_BY_MINUTES;
    result += time->tm_hour * SECONDS_BY_HOUR;
    result += (time->tm_mday - 1) * SECONDS_BY_DAY;
    result += seconds_before_month[_rtc_is_leap_year(time->tm_year, leap_year_support)][time->tm_mon];

    /* Check if we are within valid range. */
    if (time->tm_year == LAST_VALID_YEAR) {
        if ((leap_year_support == RTC_FULL_LEAP_YEAR_SUPPORT && result > EDGE_TIMESTAMP_FULL_LEAP_YEAR_SUPPORT) ||
                (leap_year_support == RTC_4_YEAR_LEAP_YEAR_SUPPORT && result > EDGE_TIMESTAMP_4_YEAR_LEAP_YEAR_SUPPORT)) {
            return false;
        }
    }

    if (time->tm_year > 70) {
        /* Valid in the range [70:206]. */
        uint32_t count_of_leap_days = ((time->tm_year - 1) / 4) - (70 / 4);
        if (leap_year_support == RTC_FULL_LEAP_YEAR_SUPPORT) {
            if (time->tm_year > 200) {
                count_of_leap_days--; // 2100 is not a leap year
            }
        }

        result += (((time->tm_year - 70) * 365) + count_of_leap_days) * SECONDS_BY_DAY;
    }

    *seconds = result;

    return true;
}

bool _rtc_localtime(time_t timestamp, struct tm *time_info, rtc_leap_year_support_t leap_year_support)
{
    if (time_info == NULL) {
        return false;
    }

    uint32_t seconds = (uint32_t)timestamp;

    time_info->tm_sec = seconds % 60;
    seconds = seconds / 60;   // timestamp in minutes
    time_info->tm_min = seconds % 60;
    seconds = seconds / 60;  // timestamp in hours
    time_info->tm_hour = seconds % 24;
    seconds = seconds / 24;  // timestamp in days;

    /* Compute the weekday.
     * The 1st of January 1970 was a Thursday which is equal to 4 in the weekday representation ranging from [0:6].
     */
    time_info->tm_wday = (seconds + 4) % 7;

    /* Years start at 70. */
    time_info->tm_year = 70;
    while (true) {
        if (_rtc_is_leap_year(time_info->tm_year, leap_year_support) && seconds >= 366) {
            ++time_info->tm_year;
            seconds -= 366;
        } else if (!_rtc_is_leap_year(time_info->tm_year, leap_year_support) && seconds >= 365) {
            ++time_info->tm_year;
            seconds -= 365;
        } else {
            /* The remaining days are less than a years. */
            break;
        }
    }

    time_info->tm_yday = seconds;

    /* Convert days into seconds and find the current month. */
    seconds *= SECONDS_BY_DAY;
    time_info->tm_mon = 11;
    bool leap = _rtc_is_leap_year(time_info->tm_year, leap_year_support);
    for (uint32_t i = 0; i < 12; ++i) {
        if ((uint32_t) seconds < seconds_before_month[leap][i]) {
            time_info->tm_mon = i - 1;
            break;
        }
    }

    /* Remove month from timestamp and compute the number of days.
     * Note: unlike other fields, days are not 0 indexed.
     */
    seconds -= seconds_before_month[leap][time_info->tm_mon];
    time_info->tm_mday = (seconds / SECONDS_BY_DAY) + 1;

    return true;
}
