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
#include <limits.h>
#include "mbed.h"
#include "mbed_lp_ticker_wrapper.h"
#include "hal/us_ticker_api.h"
#include "../sleep/sleep_test_utils.h"
#include "sleep_manager_api_tests.h"

#if !DEVICE_SLEEP
#error [NOT_SUPPORTED] test not supported
#else

#define SLEEP_DURATION_US 20000ULL
#define DEEP_SLEEP_TEST_CHECK_WAIT_US 2000
// As sleep_manager_can_deep_sleep_test_check() is based on wait_ns
// and wait_ns can be up to 40% slower, use a 50% delta here.
#define DEEP_SLEEP_TEST_CHECK_WAIT_DELTA_US 1000

using utest::v1::Case;
using utest::v1::Specification;
using utest::v1::Harness;

#if DEVICE_LPTICKER
/* Make sure there are enough ticks to cope with more than SLEEP_DURATION_US sleep
 * without hitting the wrap-around.
 */
void wraparound_lp_protect(void)
{
    const uint32_t ticks_now = get_lp_ticker_data()->interface->read();
    const ticker_info_t *p_ticker_info = get_lp_ticker_data()->interface->get_info();

    const uint32_t max_count = ((1 << p_ticker_info->bits) - 1);
    const uint32_t delta_ticks = us_to_ticks(SLEEP_DURATION_US * 1.5, p_ticker_info->frequency);

    if (ticks_now < (max_count - delta_ticks)) {
        return;
    }

    while (get_lp_ticker_data()->interface->read() > (max_count - delta_ticks));
}
#endif

void test_lock_unlock()
{
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());

    sleep_manager_lock_deep_sleep();
    TEST_ASSERT_FALSE(sleep_manager_can_deep_sleep());

    sleep_manager_unlock_deep_sleep();
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

utest::v1::status_t testcase_setup(const Case *const source, const size_t index_of_case)
{
    // Suspend the RTOS kernel scheduler to prevent interference with duration of sleep.
#if defined(MBED_CONF_RTOS_PRESENT)
    osKernelSuspend();
#endif
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

utest::v1::status_t testcase_teardown(const Case *const source, const size_t passed, const size_t failed,
                                      const utest::v1::failure_t failure)
{
    ticker_resume(get_us_ticker_data());
#if DEVICE_LPTICKER
#if (LPTICKER_DELAY_TICKS > 0)
    lp_ticker_wrapper_resume();
#endif
    ticker_resume(get_lp_ticker_data());
#endif
#if defined(MBED_CONF_RTOS_PRESENT)
    osKernelResume(0);
#endif
    return utest::v1::greentea_case_teardown_handler(source, passed, failed, failure);
}

#if DEVICE_LPTICKER
#if DEVICE_USTICKER
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
    uint32_t us_ts1, us_ts2, lp_ts1, lp_ts2, us_diff1, us_diff2, lp_diff1, lp_diff2;

    /*  Let's avoid the Lp ticker wrap-around case */
    wraparound_lp_protect();
    uint32_t lp_wakeup_ts_raw = lp_ticker_read() + us_to_ticks(SLEEP_DURATION_US, lp_ticker_info->frequency);
    timestamp_t lp_wakeup_ts = overflow_protect(lp_wakeup_ts_raw, lp_ticker_info->bits);
    lp_ticker_set_interrupt(lp_wakeup_ts);

    /* Some targets may need an interrupt short time after LPTIM interrupt is
     * set and forbid deep_sleep during that period. Let this period pass  */
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep_test_check());

    sleep_manager_lock_deep_sleep();

    us_ts1 = us_ticker_read();
    lp_ts1 = lp_ticker_read();

    sleep_manager_sleep_auto();

    us_ts2 = us_ticker_read();
    lp_ts2 = lp_ticker_read();

    us_diff1 = ticks_to_us((us_ts1 <= us_ts2) ? (us_ts2 - us_ts1) : (us_ticker_mask - us_ts1 + us_ts2 + 1), us_ticker_info->frequency);
    lp_diff1 = ticks_to_us((lp_ts1 <= lp_ts2) ? (lp_ts2 - lp_ts1) : (lp_ticker_mask - lp_ts1 + lp_ts2 + 1), lp_ticker_info->frequency);

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

    /*  Let's avoid the Lp ticker wrap-around case */
    wraparound_lp_protect();
    lp_wakeup_ts_raw = lp_ticker_read() + us_to_ticks(SLEEP_DURATION_US, lp_ticker_info->frequency);
    lp_wakeup_ts = overflow_protect(lp_wakeup_ts_raw, lp_ticker_info->bits);
    lp_ticker_set_interrupt(lp_wakeup_ts);

    /* Some targets may need an interrupt short time after LPTIM interrupt is
     * set and forbid deep_sleep during that period. Let this period pass  */
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep_test_check());

    us_ts1 = us_ticker_read();
    lp_ts1 = lp_ticker_read();

    sleep_manager_sleep_auto();

    us_ts2 = us_ticker_read();
    lp_ts2 = lp_ticker_read();

    us_diff2 = ticks_to_us((us_ts1 <= us_ts2) ? (us_ts2 - us_ts1) : (us_ticker_mask - us_ts1 + us_ts2 + 1), us_ticker_info->frequency);
    lp_diff2 = ticks_to_us((lp_ts1 <= lp_ts2) ? (lp_ts2 - lp_ts1) : (lp_ticker_mask - lp_ts1 + lp_ts2 + 1), lp_ticker_info->frequency);

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

#define US_PER_S 1000000

uint32_t diff_us(uint32_t start_ticks, uint32_t stop_ticks, const ticker_info_t *info)
{
    uint32_t counter_mask = ((1 << info->bits) - 1);

    uint32_t diff_ticks = ((stop_ticks - start_ticks) & counter_mask);

    return (uint32_t)((uint64_t) diff_ticks * US_PER_S / info->frequency);
}

volatile bool unlock_deep_sleep = false;

void ticker_event_handler_stub(const ticker_data_t *const ticker)
{
    lp_ticker_clear_interrupt();
    if (unlock_deep_sleep) {
        sleep_manager_unlock_deep_sleep_internal();
        unlock_deep_sleep = false;
    }
}

ticker_irq_handler_type prev_irq_handler;

void test_lock_unlock_test_check()
{
    prev_irq_handler = set_lp_ticker_irq_handler(ticker_event_handler_stub);

    for (int i = 0; i < 1000; i++) {

        wraparound_lp_protect();

        const ticker_info_t *p_ticker_info = get_lp_ticker_data()->interface->get_info();

        // Use LowPowerTimer instead of Timer to prevent deep sleep lock.
        us_timestamp_t exec_time_unlocked, exec_time_locked;
        uint32_t start, stop;

        // Deep sleep unlocked:
        // * sleep_manager_can_deep_sleep() returns true,
        // * sleep_manager_can_deep_sleep_test_check() returns true instantly.
        TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());
        start = lp_ticker_read();
        TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep_test_check());
        stop = lp_ticker_read();
        exec_time_unlocked = diff_us(start, stop, p_ticker_info);

        // Deep sleep locked:
        // * sleep_manager_can_deep_sleep() returns false,
        // * sleep_manager_can_deep_sleep_test_check() returns false with 2 ms delay.
        sleep_manager_lock_deep_sleep();
        TEST_ASSERT_FALSE(sleep_manager_can_deep_sleep());
        start = lp_ticker_read();
        TEST_ASSERT_FALSE(sleep_manager_can_deep_sleep_test_check());
        stop = lp_ticker_read();
        exec_time_locked = diff_us(start, stop, p_ticker_info);
        TEST_ASSERT_UINT64_WITHIN(DEEP_SLEEP_TEST_CHECK_WAIT_DELTA_US, DEEP_SLEEP_TEST_CHECK_WAIT_US,
                                  exec_time_locked - exec_time_unlocked);

        // Deep sleep unlocked with a 1 ms delay:
        // * sleep_manager_can_deep_sleep() returns false,
        // * sleep_manager_can_deep_sleep_test_check() returns true with a 1 ms delay,
        // * sleep_manager_can_deep_sleep() returns true when checked again.
        unlock_deep_sleep = true;
        /*  Let's avoid the Lp ticker wrap-around case */
        wraparound_lp_protect();
        start = lp_ticker_read();
        uint32_t lp_wakeup_ts_raw = start + us_to_ticks(DEEP_SLEEP_TEST_CHECK_WAIT_US / 2, p_ticker_info->frequency);
        timestamp_t lp_wakeup_ts = overflow_protect(lp_wakeup_ts_raw, p_ticker_info->bits);
        lp_ticker_set_interrupt(lp_wakeup_ts);

        // Extra wait after setting interrupt to handle CMPOK
        wait_ns(100000);
        TEST_ASSERT_FALSE(sleep_manager_can_deep_sleep());
        TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep_test_check());
        stop = lp_ticker_read();
        TEST_ASSERT(diff_us(start, stop, p_ticker_info) > 0UL);
        TEST_ASSERT(diff_us(start, stop, p_ticker_info) < DEEP_SLEEP_TEST_CHECK_WAIT_US);
        TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());
    }

    set_lp_ticker_irq_handler(prev_irq_handler);
}
#endif

utest::v1::status_t testsuite_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("deep sleep lock/unlock",
         (utest::v1::case_setup_handler_t) testcase_setup,
         test_lock_unlock,
         (utest::v1::case_teardown_handler_t) testcase_teardown),
    Case("deep sleep locked USHRT_MAX times",
         (utest::v1::case_setup_handler_t) testcase_setup,
         test_lock_eq_ushrt_max,
         (utest::v1::case_teardown_handler_t) testcase_teardown),
#if DEVICE_LPTICKER
#if DEVICE_USTICKER
    Case("sleep_auto calls sleep/deep sleep based on lock",
         (utest::v1::case_setup_handler_t) testcase_setup,
         test_sleep_auto,
         (utest::v1::case_teardown_handler_t) testcase_teardown),
#endif
    Case("deep sleep lock/unlock test_check",
         (utest::v1::case_setup_handler_t) testcase_setup,
         test_lock_unlock_test_check,
         (utest::v1::case_teardown_handler_t) testcase_teardown)
#endif
};

Specification specification(testsuite_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !DEVICE_SLEEP
