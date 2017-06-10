/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"
#include "mbed_mktime.h"

// Limit the test range to 1935 for IAR only. From the IAR C/C++ Development Guide:
// "The 32-bit interface supports years from 1900 up to 2035 and uses a 32-bit integer
// for time_t."
#ifdef __ICCARM__
#define LOCALTIME_MAX       2082758400      // 1st of january 2036 at 00:00:00
#define MKTIME_YR_MAX       136
#else
#define LOCALTIME_MAX       INT_MAX
#define MKTIME_YR_MAX       137
#endif

using namespace utest::v1;

/* 
 * regular is_leap_year, see platform/mbed_mktime.c for the optimized version
 */
bool is_leap_year(int year) {
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

/*
 * Test the optimized version of _rtc_is_leap_year against the generic version.
 */
void test_is_leap_year() { 
    for (int i = 70; i < 138; ++i) { 
        bool expected = is_leap_year(i);
        bool actual_value = _rtc_is_leap_year(i);

        if (expected != actual_value) { 
            printf ("leap year failed with i = %d\r\n", i);
        }
        TEST_ASSERT_EQUAL(expected, actual_value);
    }
}

struct tm make_time_info(int year, int month, int day, int hours, int minutes, int seconds) { 
    struct tm timeinfo = {
            seconds,    // tm_sec
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

/*
 * test out of range values for _rtc_mktime.
 * The function operates from the 1st of january 1970 at 00:00:00 to the 19th 
 * of january 2038 at 03:14:07.
 */
void test_mk_time_out_of_range() { 
    tm invalid_lower_bound = make_time_info(
        69,
        11,
        31,
        23,
        59,
        59
    );    

    tm valid_lower_bound = make_time_info(
        70,
        0,
        1,
        0,
        0,
        0
    );

    tm valid_upper_bound = make_time_info(
        138,
        0,
        19,
        3,
        14,
        7
    );

    tm invalid_upper_bound = make_time_info(
        138,
        0,
        19,
        3,
        14,
        8
    );

    TEST_ASSERT_EQUAL_INT(((time_t) -1), _rtc_mktime(&invalid_lower_bound));
    TEST_ASSERT_EQUAL_INT(((time_t) 0), _rtc_mktime(&valid_lower_bound));
    TEST_ASSERT_EQUAL_INT(((time_t) INT_MAX), _rtc_mktime(&valid_upper_bound));
    TEST_ASSERT_EQUAL_INT(((time_t) -1), _rtc_mktime(&invalid_upper_bound));
}

/* 
 * test mktime over a large set of values 
 */
void test_mk_time() { 
    for (size_t year = 70; year < MKTIME_YR_MAX; ++year) {
        for (size_t month = 0; month < 12; ++month) { 
            for (size_t day = 1; day < 32; ++day) {
                if (month == 1 && is_leap_year(year) && day == 29) { 
                    break;
                } else if(month == 1 && !is_leap_year(year) && day == 28) {
                    break;
                } else if (
                    day == 31 && 
                    (month == 3 || month == 5 || month == 8 || month == 10)
                ) {
                    break;
                }

                for (size_t hour = 0; hour < 24; ++hour) {  
                    tm time_info = make_time_info(
                        year,
                        month,
                        day,
                        hour,
                        hour % 2 ? 59 : 0,
                        hour % 2 ? 59 : 0
                    );

                    time_t expected = mktime(&time_info);
                    time_t actual_value = _rtc_mktime(&time_info);

                    char msg[128] = "";
                    if (expected != actual_value) { 
                        snprintf(
                            msg, sizeof(msg), 
                            "year = %d, month = %d, day = %d, diff = %ld", 
                            year, month, day, expected - actual_value
                        );
                    }

                    TEST_ASSERT_EQUAL_UINT32_MESSAGE(expected, actual_value, msg);
                }
            }
        }
    }
}

/* 
 * test value out of range for localtime
 */
void test_local_time_limit() {
    struct tm dummy_value; 
    TEST_ASSERT_FALSE(_rtc_localtime((time_t) -1, &dummy_value));
    TEST_ASSERT_FALSE(_rtc_localtime((time_t) INT_MIN, &dummy_value));
}

/* 
 * test _rtc_localtime over a large set of values.
 */
void test_local_time() { 
    for (uint32_t i = 0; i < LOCALTIME_MAX; i += 3451) {
        time_t copy = (time_t) i;
        struct tm* expected = localtime(&copy);
        struct tm actual_value; 
        bool result = _rtc_localtime((time_t) i, &actual_value);

        if (
            expected->tm_sec != actual_value.tm_sec || 
            expected->tm_min != actual_value.tm_min ||
            expected->tm_hour != actual_value.tm_hour || 
            expected->tm_mday != actual_value.tm_mday ||
            expected->tm_mon != actual_value.tm_mon || 
            expected->tm_year != actual_value.tm_year || 
            expected->tm_wday != actual_value.tm_wday || 
            expected->tm_yday != actual_value.tm_yday ||
            result == false
        ) { 
            printf("error: i = %lu\r\n", i);
        }

        TEST_ASSERT_TRUE(result);
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(
            expected->tm_sec, actual_value.tm_sec, "invalid seconds"
        );
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(
            expected->tm_min, actual_value.tm_min, "invalid minutes"
        );
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(
            expected->tm_hour, actual_value.tm_hour, "invalid hours"
        );
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(
            expected->tm_mday, actual_value.tm_mday, "invalid day"
        );
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(
            expected->tm_mon, actual_value.tm_mon, "invalid month"
        );
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(
            expected->tm_year, actual_value.tm_year, "invalid year"
        );
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(
            expected->tm_wday, actual_value.tm_wday, "invalid weekday"
        );
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(
            expected->tm_yday, actual_value.tm_yday, "invalid year day"
        );
    }
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("test is leap year", test_is_leap_year, greentea_failure_handler),
    Case("test mk time out of range values", test_mk_time_out_of_range, greentea_failure_handler),
    Case("mk time", test_mk_time, greentea_failure_handler),
    Case("test local time", test_local_time, greentea_failure_handler),
    Case("test local time limits", test_local_time_limit, greentea_failure_handler),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(1200, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    return Harness::run(specification);
}
