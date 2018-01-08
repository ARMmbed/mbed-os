/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This is the mbed device part of the test to verify if:
 * - _rtc_maketime() function converts a calendar time into time since UNIX epoch as a time_t,
 * - _rtc_localtime() function converts a given time in seconds since epoch into calendar time.
 */

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "mbed_mktime.h"

#define LAST_VALID_YEAR 206

using namespace utest::v1;

static rtc_leap_year_support_t rtc_leap_year_support;

/*
 * regular is_leap_year, see platform/mbed_mktime.c for the optimised version
 */
bool is_leap_year(int year)
{
    year = 1900 + year;
    if (year % 4) {
        return false;
    } else if (year % 100) {
        return true;
    } else if (year % 400) {
        return false;
    }
    return true;
}

struct tm make_time_info(int year, int month, int day, int hours, int minutes, int seconds)
{
    struct tm timeinfo =
    { seconds,    // tm_sec
        minutes,    // tm_min
        hours,      // tm_hour
        day,        // tm_mday
        month,      // tm_mon
        year,       // tm_year
        0,          // tm_wday
        0,          // tm_yday
        0,          // tm_isdst
            };
    return timeinfo;
}

/* Test _rtc_maketime() and _rtc_localtime() across wide range
 *
 * Note: This test functions handles both types of RTC devices:
 * - devices which supports full leap years support in range 1970 - 2106.
 * - devices which supports parial leap years support and incorrectly treats 2100 year as a leap year.
 *
 * Given is valid calendar time.
 * When _rtc_maketime() is used to generate timestamp from calendar time and _rtc_localtime() is used to convert
 * timestamp to calendar time.
 * Then both operations gives valid results.
 */
void test_case_mktime_localtime()
{
    char _key[11] =
    { };
    char _value[128] =
    { };

    size_t years[] = {70, 71, 100, 196, 200, 205};

    /* Inform host part of the test about tested RTC type. */
    greentea_send_kv("leap_year_setup",  rtc_leap_year_support);

    /* Check the first and last last day of each month. */
    for (size_t year_id = 0; year_id < (sizeof(years) /sizeof(size_t)) ; ++year_id) {
        for (size_t month = 0; month < 12; ++month) {
            for (size_t dayid = 0; dayid < 2; ++dayid) {

                size_t year = years[year_id];

                size_t day = 0;
                /* Test the first and the last day of each month:
                 * day 0 - first,
                 * day 1 - last
                 * */
                switch (dayid)
                {
                    case 0:
                        day = 1;
                        break;

                    case 1:
                        day = 31;

                        if (month == 3 || month == 5 || month == 8 || month == 10) {
                            day = 30;
                        }

                        if (month == 1) {
                            day = 28;
                        }

                        if (month == 1 && is_leap_year(year)) {
                            day = 29;
                        }

                        /* Additional conditions for RTCs with partial leap year support. */
                        if(month == 1 && year == 200 && rtc_leap_year_support == RTC_4_YEAR_LEAP_YEAR_SUPPORT) {
                            day = 29;
                        }

                        break;

                    default:
                        break;
                }

                tm time_info = make_time_info(year, month, day, 23, dayid ? 59 : 0, dayid ? 59 : 0);

                time_t actual_timestamp;

                TEST_ASSERT_TRUE(_rtc_maketime(&time_info, &actual_timestamp, rtc_leap_year_support));

                greentea_send_kv("timestamp", (int) actual_timestamp);

                greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

                TEST_ASSERT_EQUAL_STRING("passed", _key);

                /* Response which indicates success contains encoded week day
                 * and year day needed to verify _rtc_localtime().
                 * Use validated timestamp to generate and validate calendar time.
                 */

                unsigned int buf = (unsigned int) strtol(_value, NULL, 10);

                time_info.tm_wday = ((buf >> 16) & 0x0000FFFF);
                time_info.tm_yday = (buf & 0x0000FFFF);

                tm actual_time_info;

                bool result = _rtc_localtime((time_t) actual_timestamp, &actual_time_info, rtc_leap_year_support);

                TEST_ASSERT_TRUE(result);
                TEST_ASSERT_EQUAL_UINT32_MESSAGE(time_info.tm_sec, actual_time_info.tm_sec, "invalid seconds");
                TEST_ASSERT_EQUAL_UINT32_MESSAGE(time_info.tm_min, actual_time_info.tm_min, "invalid minutes");
                TEST_ASSERT_EQUAL_UINT32_MESSAGE(time_info.tm_hour, actual_time_info.tm_hour, "invalid hours");
                TEST_ASSERT_EQUAL_UINT32_MESSAGE(time_info.tm_mday, actual_time_info.tm_mday, "invalid day");
                TEST_ASSERT_EQUAL_UINT32_MESSAGE(time_info.tm_mon, actual_time_info.tm_mon, "invalid month");
                TEST_ASSERT_EQUAL_UINT32_MESSAGE(time_info.tm_year, actual_time_info.tm_year, "invalid year");
                TEST_ASSERT_EQUAL_UINT32_MESSAGE(time_info.tm_wday, actual_time_info.tm_wday, "invalid weekday");
                TEST_ASSERT_EQUAL_UINT32_MESSAGE(time_info.tm_yday, actual_time_info.tm_yday, "invalid year day");
            }
        }
    }
}

utest::v1::status_t full_leap_year_case_setup_handler_t(const Case * const source, const size_t index_of_case)
{
    rtc_leap_year_support = RTC_FULL_LEAP_YEAR_SUPPORT;

    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t partial_leap_year_case_setup_handler_t(const Case * const source, const size_t index_of_case)
{
    rtc_leap_year_support = RTC_4_YEAR_LEAP_YEAR_SUPPORT;

    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t teardown_handler_t(const Case * const source, const size_t passed, const size_t failed,
        const failure_t reason)
{
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

// Test cases
Case cases[] ={
   Case("test make time and local time - RTC leap years full support", full_leap_year_case_setup_handler_t, test_case_mktime_localtime, teardown_handler_t),
   Case("test make time and local time - RTC leap years partial support", partial_leap_year_case_setup_handler_t, test_case_mktime_localtime, teardown_handler_t),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(300, "rtc_calc_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
