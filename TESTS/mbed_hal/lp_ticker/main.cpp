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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"
#include "lp_ticker_api_tests.h"
#include "hal/lp_ticker_api.h"

#if !DEVICE_LPTICKER
    #error [NOT_SUPPORTED] Low power timer not supported for this target
#endif

using namespace utest::v1;

volatile int intFlag = 0;

#define TICKER_GLITCH_TEST_TICKS 1000

#define TICKER_INT_VAL 500
#define TICKER_DELTA 10

#define LP_TICKER_OV_LIMIT 4000

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
    const ticker_info_t* p_ticker_info = lp_ticker_get_info();

    const uint32_t max_count = ((1 << p_ticker_info->bits) - 1);

    if ((max_count - ticks_now) > time_window) {
        return;
    }

    while (lp_ticker_read() > ticks_now);
}

void ticker_event_handler_stub(const ticker_data_t * const ticker)
{
    /* Indicate that ISR has been executed in interrupt context. */
    if (IsIrqMode()) {
        intFlag++;
    }

    /* Clear and disable ticker interrupt. */
    lp_ticker_clear_interrupt();
    lp_ticker_disable_interrupt();
}

void wait_cycles(volatile unsigned int cycles)
{
    while (cycles--);
}

/* Test that the ticker has the correct frequency and number of bits. */
void lp_ticker_info_test()
{
    const ticker_info_t* p_ticker_info = lp_ticker_get_info();

    TEST_ASSERT(p_ticker_info->frequency >= 4000);
    TEST_ASSERT(p_ticker_info->frequency <= 64000);
    TEST_ASSERT(p_ticker_info->bits >= 12);
}

#if DEVICE_SLEEP
/* Test that the ticker continues operating in deep sleep mode. */
void lp_ticker_deepsleep_test()
{
    intFlag = 0;

    set_lp_ticker_irq_handler(ticker_event_handler_stub);

    lp_ticker_init();

    /* Wait for green tea UART transmission before entering deep-sleep mode. */
    wait_cycles(400000);

    overflow_protect();

    const uint32_t tick_count = lp_ticker_read();

    /* Set interrupt. Interrupt should be fired when tick count is equal to:
     * tick_count + TICKER_INT_VAL. */
    lp_ticker_set_interrupt(tick_count + TICKER_INT_VAL);

    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());

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

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("lp ticker info test", lp_ticker_info_test),
#if DEVICE_SLEEP
    Case("lp ticker sleep test", lp_ticker_deepsleep_test),
#endif
    Case("lp ticker glitch test", lp_ticker_glitch_test)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
