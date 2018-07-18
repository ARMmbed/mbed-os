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
#include <limits.h>
#include "mbed.h"
#include "mbed_lp_ticker_wrapper.h"
#include "sleep_manager_api_tests.h"

#if !DEVICE_SLEEP
#error [NOT_SUPPORTED] test not supported
#endif

#define SLEEP_DURATION_US 100000ULL
#define SERIAL_FLUSH_TIME_MS 20

using utest::v1::Case;
using utest::v1::Specification;
using utest::v1::Harness;

static uint32_t num_test_errors = 0UL;

mbed_error_status_t mbed_error(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value,
                               const char *filename, int line_number)
{
    (void) error_status;
    (void) error_msg;
    (void) error_value;
    (void) filename;
    (void) line_number;

    num_test_errors++;
    return MBED_SUCCESS;
}

void test_lock_unlock()
{
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());

    sleep_manager_lock_deep_sleep();
    TEST_ASSERT_FALSE(sleep_manager_can_deep_sleep());

    sleep_manager_unlock_deep_sleep();
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());
}

void test_lone_unlock()
{
    uint32_t expected_err_count = num_test_errors + 1;
    sleep_manager_unlock_deep_sleep();
    TEST_ASSERT_EQUAL_UINT32(expected_err_count, num_test_errors);

    // Make sure upcoming tests won't be broken.
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());
}

void test_lock_eq_ushrt_max()
{
    uint32_t lock_count = 0;
    while (lock_count < USHRT_MAX) {
        sleep_manager_lock_deep_sleep();
        lock_count++;
        TEST_ASSERT_FALSE(sleep_manager_can_deep_sleep());
    }
    while (lock_count > 1) {
        sleep_manager_unlock_deep_sleep();
        lock_count--;
        TEST_ASSERT_FALSE(sleep_manager_can_deep_sleep());
    }
    sleep_manager_unlock_deep_sleep();
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());
}

void test_lock_gt_ushrt_max()
{
    uint32_t lock_count = 0;
    while (lock_count < USHRT_MAX) {
        sleep_manager_lock_deep_sleep();
        lock_count++;
        TEST_ASSERT_FALSE(sleep_manager_can_deep_sleep());
    }

    uint32_t expected_err_count = num_test_errors + 1;
    sleep_manager_lock_deep_sleep();
    TEST_ASSERT_EQUAL_UINT32(expected_err_count, num_test_errors);

    // Make sure upcoming tests won't be broken.
    while (lock_count > 0) {
        sleep_manager_unlock_deep_sleep();
        lock_count--;
    }
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());
}

#if DEVICE_LPTICKER && DEVICE_USTICKER
void wakeup_callback(volatile int *wakeup_flag)
{
    (*wakeup_flag)++;
}

/* This test is based on the fact that the high-speed clocks are turned off
 * in deep sleep mode but remain on in the ordinary sleep mode. Low-speed
 * clocks stay on for both sleep and deep sleep modes.
 *
 * The type of sleep that was actually used by sleep_manager_sleep_auto()
 * can be detected by comparing times measured by us and lp tickers.
 */
void test_sleep_auto()
{
    const ticker_data_t *const us_ticker = get_us_ticker_data();
    const ticker_data_t *const lp_ticker = get_lp_ticker_data();
    us_timestamp_t us_ts, lp_ts, us_diff1, us_diff2, lp_diff1, lp_diff2;
    LowPowerTimeout lp_timeout;

    sleep_manager_lock_deep_sleep();
    volatile int wakeup_flag = 0;
    lp_timeout.attach_us(mbed::callback(wakeup_callback, &wakeup_flag), SLEEP_DURATION_US);
    us_ts = ticker_read_us(us_ticker);
    lp_ts = ticker_read_us(lp_ticker);

    while (wakeup_flag == 0) {
        sleep_manager_sleep_auto();
    }
    us_diff1 = ticker_read_us(us_ticker) - us_ts;
    lp_diff1 = ticker_read_us(lp_ticker) - lp_ts;

    // Deep sleep locked -- ordinary sleep mode used:
    // * us_ticker powered ON,
    // * lp_ticker powered ON,
    // so both should increment equally.

    // Verify us and lp tickers incremented equally, with 10% tolerance.
    TEST_ASSERT_UINT64_WITHIN_MESSAGE(
        SLEEP_DURATION_US / 10ULL, lp_diff1, us_diff1,
        "Deep sleep mode locked, but still used");

    sleep_manager_unlock_deep_sleep();
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());

    // Wait for hardware serial buffers to flush.
    wait_ms(SERIAL_FLUSH_TIME_MS);

    wakeup_flag = 0;
    lp_timeout.attach_us(mbed::callback(wakeup_callback, &wakeup_flag), SLEEP_DURATION_US);
    us_ts = ticker_read_us(us_ticker);
    lp_ts = ticker_read_us(lp_ticker);

    while (wakeup_flag == 0) {
        sleep_manager_sleep_auto();
    }
    us_diff2 = ticker_read_us(us_ticker) - us_ts;
    lp_diff2 = ticker_read_us(lp_ticker) - lp_ts;

    // Deep sleep unlocked -- deep sleep mode used:
    // * us_ticker powered OFF,
    // * lp_ticker powered ON.
    // The us_ticker increment represents only the code execution time
    // and should be much shorter than both:
    // 1. current lp_ticker increment,
    // 2. previous us_ticker increment (locked sleep test above)

    // Verify that the current us_ticker increment:
    // 1. is at most 10% of lp_ticker increment
    // 2. is at most 10% of previous us_ticker increment.
    TEST_ASSERT_MESSAGE(us_diff2 < lp_diff2 / 10ULL, "Deep sleep mode unlocked, but not used");
    TEST_ASSERT_MESSAGE(us_diff2 < us_diff1 / 10ULL, "Deep sleep mode unlocked, but not used");
}
#endif

utest::v1::status_t testsuite_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    // Suspend the RTOS kernel scheduler to prevent interference with duration of sleep.
    osKernelSuspend();
#if DEVICE_LPTICKER && (LPTICKER_DELAY_TICKS > 0)
    // Suspend the low power ticker wrapper to prevent interference with deep sleep lock.
    lp_ticker_wrapper_suspend();
#endif
#if DEVICE_LPTICKER && DEVICE_USTICKER
    // Make sure HAL tickers are initialized.
    us_ticker_init();
    lp_ticker_init();
#endif
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

void testsuite_teardown(const size_t passed, const size_t failed, const utest::v1::failure_t failure)
{
#if DEVICE_LPTICKER && (LPTICKER_DELAY_TICKS > 0)
    lp_ticker_wrapper_resume();
#endif
    osKernelResume(0);
    utest::v1::greentea_test_teardown_handler(passed, failed, failure);
}

Case cases[] = {
    Case("deep sleep lock/unlock", test_lock_unlock),
    Case("deep sleep unbalanced unlock", test_lone_unlock),
    Case("deep sleep locked USHRT_MAX times", test_lock_eq_ushrt_max),
    Case("deep sleep locked more than USHRT_MAX times", test_lock_gt_ushrt_max),
#if DEVICE_LPTICKER && DEVICE_USTICKER
    Case("sleep_auto calls sleep/deep sleep based on lock", test_sleep_auto),
#endif
};

Specification specification(testsuite_setup, cases, testsuite_teardown);

int main()
{
    return !Harness::run(specification);
}
