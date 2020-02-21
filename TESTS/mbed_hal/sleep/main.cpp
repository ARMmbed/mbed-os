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
#if !DEVICE_SLEEP
#error [NOT_SUPPORTED] sleep not supported for this target
#else

#include "mbed.h"

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "mbed_lp_ticker_wrapper.h"
#include "hal/us_ticker_api.h"

#include "sleep_test_utils.h"
#include "sleep_api_tests.h"

using namespace utest::v1;

static char info[512] = {0};

/* The following ticker frequencies are possible:
 * high frequency ticker: 250 KHz (1 tick per 4 us) - 8 Mhz (1 tick per 1/8 us)
 * low power ticker: 8 KHz (1 tick per 125 us) - 64 KHz (1 tick per ~15.6 us)
 */

/* Used for regular sleep mode, a target should be awake within 10 us. Define us delta value as follows:
 * delta = default 10 us + worst ticker resolution + extra time for code execution */
#if defined(MBED_CPU_STATS_ENABLED)
/* extra 25us for stats computation (for more details see MBED_CPU_STATS_ENABLED) */
static const uint32_t sleep_mode_delta_us = (10 + 4 + 5 + 25);
#else
static const uint32_t sleep_mode_delta_us = (10 + 4 + 5);
#endif

/* Used for deep-sleep mode, a target should be awake within 10 ms. Define us delta value as follows:
 * delta = default 10 ms + worst ticker resolution + extra time for code execution */
static const uint32_t deepsleep_mode_delta_us = (10000 + 125 + 5);

/* Test that wake-up time from sleep should be less than 10 us and
 * high frequency ticker interrupt can wake-up target from sleep. */
void sleep_usticker_test()
{
    const ticker_data_t *ticker = get_us_ticker_data();
    const unsigned int ticker_freq = ticker->interface->get_info()->frequency;
    const unsigned int ticker_width = ticker->interface->get_info()->bits;

    const ticker_irq_handler_type us_ticker_irq_handler_org = set_us_ticker_irq_handler(us_ticker_isr);

    /* Give some time Green Tea to finish UART transmission before entering
     * sleep mode.
     */
    busy_wait_ms(SERIAL_FLUSH_TIME_MS);

    /* Test only sleep functionality. */
    sleep_manager_lock_deep_sleep();
    TEST_ASSERT_FALSE_MESSAGE(sleep_manager_can_deep_sleep(), "deep sleep should be locked");

    /* Testing wake-up time 10 us. */
    for (timestamp_t i = 100; i < 1000; i += 100) {
        /* note: us_ticker_read() operates on ticks. */
        const timestamp_t start_timestamp = us_ticker_read();
        const timestamp_t next_match_timestamp = overflow_protect(start_timestamp + us_to_ticks(i, ticker_freq),
                                                                  ticker_width);

        us_ticker_set_interrupt(next_match_timestamp);

        sleep();

        const unsigned int wakeup_timestamp = us_ticker_read();

        sprintf(info, "Delta ticks: %u, Ticker width: %u, Expected wake up tick: %d, Actual wake up tick: %d, delay ticks: %d, wake up after ticks: %d",
                us_to_ticks(sleep_mode_delta_us, ticker_freq), ticker_width, next_match_timestamp, wakeup_timestamp, us_to_ticks(i, ticker_freq), wakeup_timestamp - start_timestamp);

        TEST_ASSERT_MESSAGE(compare_timestamps(us_to_ticks(sleep_mode_delta_us, ticker_freq),
                                               ticker_width, next_match_timestamp, wakeup_timestamp), info);
    }

    set_us_ticker_irq_handler(us_ticker_irq_handler_org);

    sleep_manager_unlock_deep_sleep();
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());
}

#if DEVICE_LPTICKER

/* Test that wake-up time from sleep should be less than 10 ms and
 * low power ticker interrupt can wake-up target from sleep. */
void deepsleep_lpticker_test()
{
    const ticker_data_t *ticker = get_lp_ticker_data();
    const unsigned int ticker_freq = ticker->interface->get_info()->frequency;
    const unsigned int ticker_width = ticker->interface->get_info()->bits;

    const ticker_irq_handler_type lp_ticker_irq_handler_org = set_lp_ticker_irq_handler(lp_ticker_isr);

    /* Give some time Green Tea to finish UART transmission before entering
     * deep-sleep mode.
     */
    busy_wait_ms(SERIAL_FLUSH_TIME_MS);

    TEST_ASSERT_TRUE_MESSAGE(sleep_manager_can_deep_sleep(), "deep sleep should not be locked");

    /* Testing wake-up time 10 ms. */
    for (timestamp_t i = 20000; i < 200000; i += 20000) {
        /* note: lp_ticker_read() operates on ticks. */
        const timestamp_t start_timestamp = lp_ticker_read();
        const timestamp_t next_match_timestamp = overflow_protect(start_timestamp + us_to_ticks(i, ticker_freq), ticker_width);

        lp_ticker_set_interrupt(next_match_timestamp);

        /* On some targets like STM family boards with LPTIM enabled there is a required delay (~100 us) before we are able to
           reprogram LPTIM_COMPARE register back to back. This is handled by the low level lp ticker wrapper which uses LPTIM_CMPOK interrupt.
           CMPOK fires when LPTIM_COMPARE register can be safely reprogrammed again. During this period deep-sleep is locked.
           This means that on these platforms we have additional interrupt (CMPOK) fired always ~100 us after programming lp ticker.
           Since this interrupt wakes-up the board from the sleep we need to go to sleep after CMPOK is handled. */
        TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep_test_check());

        sleep();

        /* On some targets like STM family boards with LPTIM enabled an interrupt is triggered on counter rollover.
           We need special handling for cases when next_match_timestamp < start_timestamp (interrupt is to be fired after rollover).
           In such case after first wake-up we need to reset interrupt and go back to sleep waiting for the valid one.
           NOTE: Above comment (CMPOK) applies also here.*/
#if MBED_CONF_TARGET_LPTICKER_LPTIM
        if ((next_match_timestamp < start_timestamp) && lp_ticker_read() < next_match_timestamp) {
            lp_ticker_set_interrupt(next_match_timestamp);
            wait_ns(200000);
            sleep();
        }
#endif

        const timestamp_t wakeup_timestamp = lp_ticker_read();

        sprintf(info, "Delta ticks: %u, Ticker width: %u, Expected wake up tick: %d, Actual wake up tick: %d, delay ticks: %d, wake up after ticks: %d",
                us_to_ticks(deepsleep_mode_delta_us, ticker_freq), ticker_width, next_match_timestamp, wakeup_timestamp, us_to_ticks(i, ticker_freq),  wakeup_timestamp - start_timestamp);

        TEST_ASSERT_MESSAGE(compare_timestamps(us_to_ticks(deepsleep_mode_delta_us, ticker_freq), ticker_width,
                                               next_match_timestamp, wakeup_timestamp), info);
    }

    set_lp_ticker_irq_handler(lp_ticker_irq_handler_org);

}

void deepsleep_high_speed_clocks_turned_off_test()
{
    const ticker_data_t *us_ticker = get_us_ticker_data();
    const ticker_data_t *lp_ticker = get_lp_ticker_data();
    const unsigned int us_ticker_freq = us_ticker->interface->get_info()->frequency;
    const unsigned int lp_ticker_freq = lp_ticker->interface->get_info()->frequency;
    const unsigned int us_ticker_width = us_ticker->interface->get_info()->bits;
    const unsigned int lp_ticker_width = lp_ticker->interface->get_info()->bits;
    const unsigned int us_ticker_mask = ((1 << us_ticker_width) - 1);

    /* Give some time Green Tea to finish UART transmission before entering
     * deep-sleep mode.
     */
    busy_wait_ms(SERIAL_FLUSH_TIME_MS);

    TEST_ASSERT_TRUE_MESSAGE(sleep_manager_can_deep_sleep(), "deep sleep should not be locked");

    const timestamp_t wakeup_time = lp_ticker_read() + us_to_ticks(20000, lp_ticker_freq);
    lp_ticker_set_interrupt(wakeup_time);

    /* Wait for CMPOK */
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep_test_check());

    const unsigned int us_ticks_before_sleep = us_ticker_read();

    sleep();

    const unsigned int us_ticks_after_sleep = us_ticker_read();
    const unsigned int lp_ticks_after_sleep = lp_ticker_read();

    /* High freqency ticker should be disabled in deep-sleep mode. We expect that time difference between
     * ticker reads before and after the sleep represents only code execution time between calls.
     * Since we went to sleep for about 20 ms check if time counted by high frequency timer does not
     * exceed 1 ms.
     */
    const unsigned int us_ticks_diff = (us_ticks_before_sleep <= us_ticks_after_sleep) ? (us_ticks_after_sleep - us_ticks_before_sleep) : (us_ticker_mask - us_ticks_before_sleep + us_ticks_after_sleep + 1);

    TEST_ASSERT_UINT32_WITHIN(1000, 0, ticks_to_us(us_ticks_diff, us_ticker_freq));

    sprintf(info, "Delta ticks: %u, Ticker width: %u, Expected wake up tick: %d, Actual wake up tick: %d",
            us_to_ticks(deepsleep_mode_delta_us, lp_ticker_freq), lp_ticker_width, wakeup_time, lp_ticks_after_sleep);

    /* Check if we have woken-up after expected time. */
    TEST_ASSERT_MESSAGE(compare_timestamps(us_to_ticks(deepsleep_mode_delta_us, lp_ticker_freq), lp_ticker_width,
                                           wakeup_time, lp_ticks_after_sleep), info);
}

#endif

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    /* Suspend RTOS Kernel to enable sleep modes. */
#if defined(MBED_CONF_RTOS_PRESENT)
    osKernelSuspend();
#endif
#if DEVICE_LPTICKER
    ticker_suspend(get_lp_ticker_data());
#if DEVICE_LPTICKER && (LPTICKER_DELAY_TICKS > 0)
    lp_ticker_wrapper_suspend();
#endif
#endif
    ticker_suspend(get_us_ticker_data());

    us_ticker_init();
#if DEVICE_LPTICKER
    lp_ticker_init();
#endif

    return greentea_test_setup_handler(number_of_cases);
}

void greentea_test_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    ticker_resume(get_us_ticker_data());
#if DEVICE_LPTICKER
#if DEVICE_LPTICKER && (LPTICKER_DELAY_TICKS > 0)
    lp_ticker_wrapper_resume();
#endif
    ticker_resume(get_lp_ticker_data());
#endif
#if defined(MBED_CONF_RTOS_PRESENT)
    osKernelResume(0);
#endif
    greentea_test_teardown_handler(passed, failed, failure);
}

Case cases[] = {
    Case("sleep - source of wake-up - us ticker", sleep_usticker_test, greentea_failure_handler),
#if DEVICE_LPTICKER
    Case("deep-sleep - source of wake-up - lp ticker", deepsleep_lpticker_test, greentea_failure_handler),
    Case("deep-sleep - high-speed clocks are turned off", deepsleep_high_speed_clocks_turned_off_test, greentea_failure_handler),
#endif
};

Specification specification(greentea_test_setup, cases, greentea_test_teardown);

int main()
{
    Harness::run(specification);
}

#endif // !DEVICE_SLEEP
