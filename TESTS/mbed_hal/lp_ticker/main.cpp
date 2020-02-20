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

#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] Low power timer test cases require a RTOS to run.
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"
#include "lp_ticker_api_tests.h"
#include "hal/lp_ticker_api.h"
#include "hal/mbed_lp_ticker_wrapper.h"
#include "hal/us_ticker_api.h"

#if !DEVICE_LPTICKER
#error [NOT_SUPPORTED] Low power timer not supported for this target
#else

using namespace utest::v1;

volatile int intFlag = 0;

ticker_irq_handler_type prev_handler;

#define US_PER_MS 1000

#define TICKER_GLITCH_TEST_TICKS 1000

#define TICKER_INT_VAL 500
#define TICKER_DELTA 10

#define LP_TICKER_OV_LIMIT 4000

/* To prevent a loss of Greentea data, the serial buffers have to be flushed
 * before the UART peripheral shutdown. The UART shutdown happens when the
 * device is entering the deepsleep mode or performing a reset.
 *
 * With the current API, it is not possible to check if the hardware buffers
 * are empty. However, it is possible to determine the time required for the
 * buffers to flush.
 *
 * Assuming the biggest Tx FIFO of 128 bytes (as for CY8CPROTO_062_4343W)
 * and a default UART config (9600, 8N1), flushing the Tx FIFO wold take:
 * (1 start_bit + 8 data_bits + 1 stop_bit) * 128 * 1000 / 9600 = 133.3 ms.
 * To be on the safe side, set the wait time to 150 ms.
 */
#define SERIAL_FLUSH_TIME_MS 150

void busy_wait_ms(int ms)
{
    const ticker_data_t *const ticker = get_us_ticker_data();
    uint32_t start = ticker_read(ticker);
    while ((ticker_read(ticker) - start) < (uint32_t)(ms * US_PER_MS));
}

/* Since according to the ticker requirements min acceptable counter size is
 * - 12 bits for low power timer - max count = 4095,
 * then all test cases must be executed in this time windows.
 * HAL ticker layer handles counter overflow and it is not handled in the target
 * ticker drivers. Ensure we have enough time to execute test case without overflow.
 */
void overflow_protect()
{
    uint32_t time_window;

    time_window = LP_TICKER_OV_LIMIT;

    const uint32_t ticks_now = lp_ticker_read();
    const ticker_info_t *p_ticker_info = lp_ticker_get_info();

    const uint32_t max_count = ((1 << p_ticker_info->bits) - 1);

    if ((max_count - ticks_now) > time_window) {
        return;
    }

    while (lp_ticker_read() >= ticks_now);
}

void ticker_event_handler_stub(const ticker_data_t *const ticker)
{
    /* Indicate that ISR has been executed in interrupt context. */
    if (core_util_is_isr_active()) {
        intFlag++;
    }

    /* Clear and disable ticker interrupt. */
    lp_ticker_clear_interrupt();
    lp_ticker_disable_interrupt();
}

/* Test that the ticker has the correct frequency and number of bits. */
void lp_ticker_info_test()
{
    const ticker_info_t *p_ticker_info = lp_ticker_get_info();

    TEST_ASSERT(p_ticker_info->frequency >= 4000);
    TEST_ASSERT(p_ticker_info->frequency <= 64000);
    TEST_ASSERT(p_ticker_info->bits >= 12);
}

#if DEVICE_SLEEP
/* Test that the ticker continues operating in deep sleep mode. */
void lp_ticker_deepsleep_test()
{
    intFlag = 0;

    lp_ticker_init();

    /* Give some time Green Tea to finish UART transmission before entering
     * deep-sleep mode.
     */
    busy_wait_ms(SERIAL_FLUSH_TIME_MS);

    overflow_protect();

    const uint32_t tick_count = lp_ticker_read();

    /* Set interrupt. Interrupt should be fired when tick count is equal to:
     * tick_count + TICKER_INT_VAL. */
    lp_ticker_set_interrupt(tick_count + TICKER_INT_VAL);

    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep_test_check());

    while (!intFlag) {
        sleep();
    }

    TEST_ASSERT_EQUAL(1, intFlag);
}
#endif

/* Test that the ticker does not glitch backwards due to an incorrectly implemented ripple counter driver. */
void lp_ticker_glitch_test()
{
    lp_ticker_init();

    overflow_protect();

    uint32_t last = lp_ticker_read();
    const uint32_t start = last;

    while (last < (start + TICKER_GLITCH_TEST_TICKS)) {
        const uint32_t cur = lp_ticker_read();
        TEST_ASSERT(cur >= last);
        last = cur;
    }
}

#if DEVICE_LPTICKER
utest::v1::status_t lp_ticker_deepsleep_test_setup_handler(const Case *const source, const size_t index_of_case)
{
    /* disable everything using the lp ticker for this test */
    osKernelSuspend();
    ticker_suspend(get_lp_ticker_data());
#if DEVICE_LPTICKER && (LPTICKER_DELAY_TICKS > 0)
    lp_ticker_wrapper_suspend();
#endif
    prev_handler = set_lp_ticker_irq_handler(ticker_event_handler_stub);
    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t lp_ticker_deepsleep_test_teardown_handler(const Case *const source, const size_t passed, const size_t failed,
                                                              const failure_t reason)
{
    set_lp_ticker_irq_handler(prev_handler);
#if DEVICE_LPTICKER && (LPTICKER_DELAY_TICKS > 0)
    lp_ticker_wrapper_resume();
#endif
    ticker_resume(get_lp_ticker_data());
    osKernelResume(0);
    return greentea_case_teardown_handler(source, passed, failed, reason);
}
#endif

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("lp ticker info test", lp_ticker_info_test),
#if DEVICE_SLEEP
    Case("lp ticker sleep test", lp_ticker_deepsleep_test_setup_handler, lp_ticker_deepsleep_test, lp_ticker_deepsleep_test_teardown_handler),
#endif
    Case("lp ticker glitch test", lp_ticker_glitch_test)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !DEVICE_LPTICKER
#endif // !defined(MBED_CONF_RTOS_PRESENT)
