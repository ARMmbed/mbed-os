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
#include "../sleep/sleep_test_utils.h"
#include "sleep_manager_api_tests.h"

#if !DEVICE_SLEEP
#error [NOT_SUPPORTED] test not supported
#endif

#define SLEEP_DURATION_US 20000ULL
#define DEEP_SLEEP_TEST_CHECK_WAIT_US 2000
#define DEEP_SLEEP_TEST_CHECK_WAIT_DELTA_US 500

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

#if DEVICE_LPTICKER
#if DEVICE_USTICKER
utest::v1::status_t testcase_setup(const Case * const source, const size_t index_of_case)
{
    // Suspend the RTOS kernel scheduler to prevent interference with duration of sleep.
    osKernelSuspend();
#if DEVICE_LPTICKER
    ticker_suspend(get_lp_ticker_data());
#if (LPTICKER_DELAY_TICKS > 0)
    // Suspend the low power ticker wrapper to prevent interference with deep sleep lock.
    lp_ticker_wrapper_suspend();
#endif
#endif
    ticker_suspend(get_us_ticker_data());
    // Make sure HAL tickers are initialized.
    us_ticker_init();
#if DEVICE_LPTICKER
    lp_ticker_init();
#endif
    return utest::v1::greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t testcase_teardown(const Case * const source, const size_t passed, const size_t failed,
        const utest::v1::failure_t failure)
{
    ticker_resume(get_us_ticker_data());
#if DEVICE_LPTICKER
#if (LPTICKER_DELAY_TICKS > 0)
    lp_ticker_wrapper_resume();
#endif
    ticker_resume(get_lp_ticker_data());
#endif
    osKernelResume(0);
    return utest::v1::greentea_case_teardown_handler(source, passed, failed, failure);
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
    const ticker_info_t *us_ticker_info = get_us_ticker_data()->interface->get_info();
    const unsigned us_ticker_mask = ((1 << us_ticker_info->bits) - 1);
    const ticker_irq_handler_type us_ticker_irq_handler_org = set_us_ticker_irq_handler(us_ticker_isr);
    const ticker_info_t *lp_ticker_info = get_lp_ticker_data()->interface->get_info();
    const unsigned lp_ticker_mask = ((1 << lp_ticker_info->bits) - 1);
    const ticker_irq_handler_type lp_ticker_irq_handler_org = set_lp_ticker_irq_handler(lp_ticker_isr);
    us_timestamp_t us_ts1, us_ts2, lp_ts1, lp_ts2, us_diff1, us_diff2, lp_diff1, lp_diff2;

    sleep_manager_lock_deep_sleep();
    uint32_t lp_wakeup_ts_raw = lp_ticker_read() + us_to_ticks(SLEEP_DURATION_US, lp_ticker_info->frequency);
    timestamp_t lp_wakeup_ts = overflow_protect(lp_wakeup_ts_raw, lp_ticker_info->bits);
    lp_ticker_set_interrupt(lp_wakeup_ts);
    us_ts1 = ticks_to_us(us_ticker_read(), us_ticker_info->frequency);
    lp_ts1 = ticks_to_us(lp_ticker_read(), lp_ticker_info->frequency);

    sleep_manager_sleep_auto();
    us_ts2 = ticks_to_us(us_ticker_read(), us_ticker_info->frequency);
    us_diff1 = (us_ts1 <= us_ts2) ? (us_ts2 - us_ts1) : (us_ticker_mask - us_ts1 + us_ts2 + 1);
    lp_ts2 = ticks_to_us(lp_ticker_read(), lp_ticker_info->frequency);
    lp_diff1 = (lp_ts1 <= lp_ts2) ? (lp_ts2 - lp_ts1) : (lp_ticker_mask - lp_ts1 + lp_ts2 + 1);

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
    busy_wait_ms(SERIAL_FLUSH_TIME_MS);

    lp_wakeup_ts_raw = lp_ticker_read() + us_to_ticks(SLEEP_DURATION_US, lp_ticker_info->frequency);
    lp_wakeup_ts = overflow_protect(lp_wakeup_ts_raw, lp_ticker_info->bits);
    lp_ticker_set_interrupt(lp_wakeup_ts);
    us_ts1 = ticks_to_us(us_ticker_read(), us_ticker_info->frequency);
    lp_ts1 = ticks_to_us(lp_ticker_read(), lp_ticker_info->frequency);

    sleep_manager_sleep_auto();
    us_ts2 = ticks_to_us(us_ticker_read(), us_ticker_info->frequency);
    us_diff2 = (us_ts1 <= us_ts2) ? (us_ts2 - us_ts1) : (us_ticker_mask - us_ts1 + us_ts2 + 1);
    lp_ts2 = ticks_to_us(lp_ticker_read(), lp_ticker_info->frequency);
    lp_diff2 = (lp_ts1 <= lp_ts2) ? (lp_ts2 - lp_ts1) : (lp_ticker_mask - lp_ts1 + lp_ts2 + 1);

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

    set_us_ticker_irq_handler(us_ticker_irq_handler_org);
    set_lp_ticker_irq_handler(lp_ticker_irq_handler_org);
}
#endif

void test_lock_unlock_test_check()
{
    // Make sure HAL tickers are initialized.
    ticker_read(get_us_ticker_data());
    ticker_read(get_lp_ticker_data());

    // Use LowPowerTimer instead of Timer to prevent deep sleep lock.
    LowPowerTimer lp_timer;
    us_timestamp_t exec_time_unlocked, exec_time_locked;
    LowPowerTimeout lp_timeout;

    // Deep sleep unlocked:
    // * sleep_manager_can_deep_sleep() returns true,
    // * sleep_manager_can_deep_sleep_test_check() returns true instantly.
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());
    lp_timer.start();
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep_test_check());
    lp_timer.stop();
    exec_time_unlocked = lp_timer.read_high_resolution_us();

    // Deep sleep locked:
    // * sleep_manager_can_deep_sleep() returns false,
    // * sleep_manager_can_deep_sleep_test_check() returns false with 2 ms delay.
    sleep_manager_lock_deep_sleep();
    TEST_ASSERT_FALSE(sleep_manager_can_deep_sleep());
    lp_timer.reset();
    lp_timer.start();
    TEST_ASSERT_FALSE(sleep_manager_can_deep_sleep_test_check());
    lp_timer.stop();
    exec_time_locked = lp_timer.read_high_resolution_us();
    TEST_ASSERT_UINT64_WITHIN(DEEP_SLEEP_TEST_CHECK_WAIT_DELTA_US, DEEP_SLEEP_TEST_CHECK_WAIT_US,
                              exec_time_locked - exec_time_unlocked);

    // Deep sleep unlocked with a 1 ms delay:
    // * sleep_manager_can_deep_sleep() returns false,
    // * sleep_manager_can_deep_sleep_test_check() returns true with a 1 ms delay,
    // * sleep_manager_can_deep_sleep() returns true when checked again.
    lp_timer.reset();
    lp_timeout.attach_us(mbed::callback(sleep_manager_unlock_deep_sleep_internal),
                         DEEP_SLEEP_TEST_CHECK_WAIT_US / 2);
    lp_timer.start();
    TEST_ASSERT_FALSE(sleep_manager_can_deep_sleep());
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep_test_check());
    lp_timer.stop();
    TEST_ASSERT_UINT64_WITHIN(DEEP_SLEEP_TEST_CHECK_WAIT_DELTA_US, DEEP_SLEEP_TEST_CHECK_WAIT_US / 2,
                              lp_timer.read_high_resolution_us());
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());
}
#endif

utest::v1::status_t testsuite_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("deep sleep lock/unlock", test_lock_unlock),
    Case("deep sleep unbalanced unlock", test_lone_unlock),
    Case("deep sleep locked USHRT_MAX times", test_lock_eq_ushrt_max),
    Case("deep sleep locked more than USHRT_MAX times", test_lock_gt_ushrt_max),
#if DEVICE_LPTICKER
#if DEVICE_USTICKER
    Case("sleep_auto calls sleep/deep sleep based on lock",
         (utest::v1::case_setup_handler_t) testcase_setup,
         test_sleep_auto,
         (utest::v1::case_teardown_handler_t) testcase_teardown),
#endif
    Case("deep sleep lock/unlock test_check", test_lock_unlock_test_check),
#endif
};

Specification specification(testsuite_setup, cases);

int main()
{
    return !Harness::run(specification);
}
