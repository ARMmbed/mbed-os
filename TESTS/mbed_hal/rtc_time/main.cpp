/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"
#include "mbed_mktime.h"

#define LAST_VALID_YEAR 206

using namespace utest::v1;

/*  Regular is_leap_year, see platform/mbed_mktime.c for the optimised version. */
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

/* Test the optimised version of _rtc_is_leap_year() against the generic version.
 *
 * Note: This test case is designed for both types of RTC devices:
 *       - RTC devices which handle correctly leap years in whole range (1970 - 2106).
 *       - RTC devices which does not handle correctly leap years in whole range (1970 - 2106).
 *         This RTC devices uses simpler leap year detection and incorrectly treat 2100 as a leap year.
 *       rtc_leap_year_support variable specifies which device is tested.
 *
 * Given is year in valid range.
 * When _rtc_is_leap_year() function is called.
 * Then _rtc_is_leap_year() returns true if given year is a leap year; false otherwise.
 */
template <rtc_leap_year_support_t rtc_leap_year_support>
void test_is_leap_year()
{
    for (int i = 70; i <= LAST_VALID_YEAR; ++i) {
        bool expected = is_leap_year(i);

        /* Add exception for year 2100. */
        if (rtc_leap_year_support == RTC_4_YEAR_LEAP_YEAR_SUPPORT && i == 200) {
            expected = true;
        }

        bool actual_value = _rtc_is_leap_year(i, rtc_leap_year_support);

        if (expected != actual_value) {
            printf("Leap year failed with i = %d\r\n", i);
        }
        TEST_ASSERT_EQUAL(expected, actual_value);
    }
}

/* Structure to test border values for _rtc_maketime(). */
typedef struct {
    struct tm timeinfo;
    time_t exp_seconds;  // if result is false then exp_seconds is irrelevant
    bool result;
} test_mk_time_struct;

/* Test boundary values for _rtc_maketime().
 *
 * Note: This test case is designed for both types of RTC devices:
 *       - RTC devices which handle correctly leap years in whole range (1970 - 2106).
 *       - RTC devices which does not handle correctly leap years in whole range (1970 - 2106).
 *         This RTC devices uses simpler leap year detection and incorrectly treat 2100 as a leap year.
 *       rtc_leap_year_support variable specifies which device is tested.
 *
 * Given is boundary calendar time.
 * When _rtc_maketime() function is called to convert the calendar time into timestamp.
 * Then if given calendar time is valid function returns true and conversion result, otherwise returns false.
 */
template <rtc_leap_year_support_t rtc_leap_year_support>
void test_mk_time_boundary()
{
    test_mk_time_struct *pTestCases;

    /* Array which contains data to test boundary values for the RTC devices which handles correctly leap years in
     * whole range (1970 - 2106).
     * Expected range: the 1st of January 1970 at 00:00:00 (seconds: 0) to the 7th of February 2106 at 06:28:15 (seconds: UINT_MAX).
     */
    test_mk_time_struct test_mk_time_arr_full[] = {
        {{ 0, 0, 0, 1, 0, 70, 0, 0, 0 }, (time_t) 0, true},               // valid lower bound - the 1st of January 1970 at 00:00:00
        {{ 59, 59, 23, 31, 11, 59, 0, 0, 0 }, (time_t) 0, false },        // invalid lower bound - the 31st of December 1969 at 23:59:59

        {{ 15, 28, 6, 7, 1, 206, 0, 0, 0 }, (time_t)(UINT_MAX), true },   // valid upper bound - the 7th of February 2106 at 06:28:15
        {{ 16, 28, 6, 7, 1, 206, 0, 0, 0 }, (time_t) 0, false },          // invalid upper bound - the 7th of February 2106 at 06:28:16
    };

    /* Array which contains data to test boundary values for the RTC devices which does not handle correctly leap years in
     * whole range (1970 - 2106). On this platforms we will be one day off after 28.02.2100 since 2100 year will be
     * incorrectly treated as a leap year.
     * Expected range: the 1st of January 1970 at 00:00:00 (seconds: 0) to the 6th of February 2106 at 06:28:15 (seconds: UINT_MAX).
     */
    test_mk_time_struct test_mk_time_arr_partial[] = {
        {{ 0, 0, 0, 1, 0, 70, 0, 0, 0 }, (time_t) 0, true},               // valid lower bound - the 1st of January 1970 at 00:00:00
        {{ 59, 59, 23, 31, 11, 59, 0, 0, 0 }, (time_t) 0, false },        // invalid lower bound - the 31st of December 1969 at 23:59:59

        {{ 15, 28, 6, 6, 1, 206, 0, 0, 0 }, (time_t)(UINT_MAX), true },   // valid upper bound - the 6th of February 2106 at 06:28:15
        {{ 16, 28, 6, 6, 1, 206, 0, 0, 0 }, (time_t) 0, false },          // invalid upper bound - the 6th of February 2106 at 06:28:16
    };

    /* Select array with test cases. */
    if (rtc_leap_year_support == RTC_FULL_LEAP_YEAR_SUPPORT) {
        pTestCases = test_mk_time_arr_full;
    } else {
        pTestCases = test_mk_time_arr_partial;
    }

    for (int i = 0; i < (sizeof(test_mk_time_arr_full) / (sizeof(test_mk_time_struct))); i++) {
        time_t seconds;
        bool result = _rtc_maketime(&pTestCases[i].timeinfo, &seconds, rtc_leap_year_support);

        TEST_ASSERT_EQUAL(pTestCases[i].result, result);

        /* If the result is false, then we have conversion error - skip checking seconds. */
        if (pTestCases[i].result) {
            TEST_ASSERT_EQUAL_UINT32(pTestCases[i].exp_seconds, seconds);
        }
    }
}

/* Test _rtc_maketime() function - call with invalid parameters.
 *
 * Given is _rtc_maketime() function.
 * When _rtc_maketime() function is called with invalid parameter.
 * Then _rtc_maketime() function returns false.
 */
void test_mk_time_invalid_param()
{
    time_t seconds;
    struct tm timeinfo;

    TEST_ASSERT_EQUAL(false, _rtc_maketime(NULL, &seconds, RTC_FULL_LEAP_YEAR_SUPPORT));
    TEST_ASSERT_EQUAL(false, _rtc_maketime(NULL, &seconds, RTC_4_YEAR_LEAP_YEAR_SUPPORT));
    TEST_ASSERT_EQUAL(false, _rtc_maketime(&timeinfo, NULL, RTC_FULL_LEAP_YEAR_SUPPORT));
    TEST_ASSERT_EQUAL(false, _rtc_maketime(&timeinfo, NULL, RTC_4_YEAR_LEAP_YEAR_SUPPORT));
}

/* Test _rtc_localtime() function - call with invalid parameters.
 *
 * Given is _rtc_localtime() function.
 * When _rtc_localtime() function is called with invalid parameter.
 * Then _rtc_localtime() function returns false.
 */
void test_local_time_invalid_param()
{
    TEST_ASSERT_EQUAL(false, _rtc_localtime(1, NULL, RTC_FULL_LEAP_YEAR_SUPPORT));
    TEST_ASSERT_EQUAL(false, _rtc_localtime(1, NULL, RTC_4_YEAR_LEAP_YEAR_SUPPORT));
}

/* Test set_time() function called a few seconds apart.
 *
 * Given is set_time() function.
 * When set_time() is used to set the system time two times.
 * Then if the value returned from time() is always correct return true, otherwise return false.
 */
#define NEW_TIME 15
void test_set_time_twice()
{
    time_t current_time;

    /* Set the time to NEW_TIME and check it */
    set_time(NEW_TIME);
    current_time = time(NULL);
    TEST_ASSERT_EQUAL(true, (current_time == NEW_TIME));

    /* Wait 2 seconds */
    ThisThread::sleep_for(2000);

    /* set the time to NEW_TIME again and check it */
    set_time(NEW_TIME);
    current_time = time(NULL);
    TEST_ASSERT_EQUAL(true, (current_time == NEW_TIME));
}

Case cases[] = {
    Case("test is leap year - RTC leap years full support", test_is_leap_year<RTC_FULL_LEAP_YEAR_SUPPORT>),
    Case("test is leap year - RTC leap years partial support", test_is_leap_year<RTC_4_YEAR_LEAP_YEAR_SUPPORT>),
    Case("test make time boundary values - RTC leap years full support", test_mk_time_boundary<RTC_FULL_LEAP_YEAR_SUPPORT>),
    Case("test make time boundary values - RTC leap years partial support", test_mk_time_boundary<RTC_4_YEAR_LEAP_YEAR_SUPPORT>),
    Case("test make time - invalid param", test_mk_time_invalid_param),
    Case("test local time - invalid param", test_local_time_invalid_param),
#if DEVICE_RTC || DEVICE_LPTICKER
    Case("test set_time twice", test_set_time_twice),
#endif
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    return Harness::run(specification);
}
