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

#include "mbed_mktime.h"

/*
 * time constants 
 */
#define SECONDS_BY_MINUTES 60
#define MINUTES_BY_HOUR 60
#define SECONDS_BY_HOUR (SECONDS_BY_MINUTES * MINUTES_BY_HOUR)
#define HOURS_BY_DAY 24 
#define SECONDS_BY_DAY (SECONDS_BY_HOUR * HOURS_BY_DAY)

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
        (31 + 28) * SECONDS_BY_DAY,
        (31 + 28 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) * SECONDS_BY_DAY,
    },
    {
        0,
        31 * SECONDS_BY_DAY,
        (31 + 29) * SECONDS_BY_DAY,
        (31 + 29 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) * SECONDS_BY_DAY,
    }
};

bool _rtc_is_leap_year(int year) {
    /* 
     * since in practice, the value manipulated by this algorithm lie in the 
     * range [70 : 138], the algorith can be reduced to: year % 4.
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
    return (year) % 4 ? false : true;
}

time_t _rtc_mktime(const struct tm* time) {
    // partial check for the upper bound of the range
    // normalization might happen at the end of the function 
    // this solution is faster than checking if the input is after the 19th of 
    // january 2038 at 03:14:07.  
    if ((time->tm_year < 70) || (time->tm_year > 138)) { 
        return ((time_t) -1);
    }

    uint32_t result = time->tm_sec;
    result += time->tm_min * SECONDS_BY_MINUTES;
    result += time->tm_hour * SECONDS_BY_HOUR;
    result += (time->tm_mday - 1) * SECONDS_BY_DAY;
    result += seconds_before_month[_rtc_is_leap_year(time->tm_year)][time->tm_mon];

    if (time->tm_year > 70) { 
        // valid in the range [70:138] 
        uint32_t count_of_leap_days = ((time->tm_year - 1) / 4) - (70 / 4);
        result += (((time->tm_year - 70) * 365) + count_of_leap_days) * SECONDS_BY_DAY;
    }

    if (result > INT32_MAX) { 
        return (time_t) -1;
    }

    return result;
}

bool _rtc_localtime(time_t timestamp, struct tm* time_info) {
    if (((int32_t) timestamp) < 0) { 
        return false;
    } 

    time_info->tm_sec = timestamp % 60;
    timestamp = timestamp / 60;   // timestamp in minutes
    time_info->tm_min = timestamp % 60;
    timestamp = timestamp / 60;  // timestamp in hours
    time_info->tm_hour = timestamp % 24;
    timestamp = timestamp / 24;  // timestamp in days;

    // compute the weekday
    // The 1st of January 1970 was a Thursday which is equal to 4 in the weekday
    // representation ranging from [0:6]
    time_info->tm_wday = (timestamp + 4) % 7;

    // years start at 70
    time_info->tm_year = 70;
    while (true) { 
        if (_rtc_is_leap_year(time_info->tm_year) && timestamp >= 366) {
            ++time_info->tm_year;
            timestamp -= 366;
        } else if (!_rtc_is_leap_year(time_info->tm_year) && timestamp >= 365) {
            ++time_info->tm_year;
            timestamp -= 365;
        } else {
            // the remaining days are less than a years
            break;
        }
    }

    time_info->tm_yday = timestamp;

    // convert days into seconds and find the current month
    timestamp *= SECONDS_BY_DAY;
    time_info->tm_mon = 11;
    bool leap = _rtc_is_leap_year(time_info->tm_year);
    for (uint32_t i = 0; i < 12; ++i) {
        if ((uint32_t) timestamp < seconds_before_month[leap][i]) {
            time_info->tm_mon = i - 1;
            break;
        }
    }

    // remove month from timestamp and compute the number of days.
    // note: unlike other fields, days are not 0 indexed.
    timestamp -= seconds_before_month[leap][time_info->tm_mon];
    time_info->tm_mday = (timestamp / SECONDS_BY_DAY) + 1;

    return true;
}
