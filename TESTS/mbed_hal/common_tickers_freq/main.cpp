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
 * This is the mbed device part of the test to verify if mbed board ticker
 * freqency is valid.
 */

#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] common tickers frequency test cases require a RTOS to run.
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "ticker_api_test_freq.h"
#include "hal/us_ticker_api.h"
#include "hal/lp_ticker_api.h"
#include "hal/mbed_lp_ticker_wrapper.h"

#if defined(SKIP_TIME_DRIFT_TESTS) || !DEVICE_USTICKER
#error [NOT_SUPPORTED] test not supported
#else

#define US_PER_S 1000000

using namespace utest::v1;

const ticker_interface_t *intf;
uint32_t intf_mask;
uint32_t intf_last_tick;
uint32_t intf_elapsed_ticks;
ticker_irq_handler_type prev_handler;

uint32_t ticks_to_us(uint32_t ticks, uint32_t freq)
{
    return (uint32_t)((uint64_t)ticks * US_PER_S / freq);
}

void elapsed_ticks_reset()
{
    core_util_critical_section_enter();

    const uint32_t ticker_bits = intf->get_info()->bits;

    intf_mask = (1 << ticker_bits) - 1;
    intf_last_tick = intf->read();
    intf_elapsed_ticks = 0;

    core_util_critical_section_exit();
}

uint32_t elapsed_ticks_update()
{
    core_util_critical_section_enter();

    const uint32_t current_tick = intf->read();
    intf_elapsed_ticks += (current_tick - intf_last_tick) & intf_mask;
    intf_last_tick = current_tick;

    /* Schedule next interrupt half way to overflow */
    uint32_t next = (current_tick + intf_mask / 2) & intf_mask;
    intf->set_interrupt(next);

    uint32_t elapsed = intf_elapsed_ticks;

    core_util_critical_section_exit();
    return elapsed;
}

void ticker_event_handler_stub(const ticker_data_t *const ticker)
{
    intf->clear_interrupt();
    elapsed_ticks_update();
}

/* Test that the ticker is operating at the frequency it specifies. */
void ticker_frequency_test()
{
    char _key[11] = { };
    char _value[128] = { };
    int expected_key = 1;
    const uint32_t ticker_freq = intf->get_info()->frequency;

    intf->init();

    elapsed_ticks_reset();

    /* Detect overflow for tickers with lower counters width. */
    elapsed_ticks_update();

    greentea_send_kv("timing_drift_check_start", 0);

    /* Wait for 1st signal from host. */
    do {
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        expected_key = strcmp(_key, "base_time");
    } while (expected_key);

    const uint32_t begin_ticks = elapsed_ticks_update();

    /* Assume that there was no overflow at this point - we are just after init. */
    greentea_send_kv(_key, ticks_to_us(begin_ticks, ticker_freq));

    /* Wait for 2nd signal from host. */
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

    const uint32_t end_ticks = elapsed_ticks_update();

    greentea_send_kv(_key, ticks_to_us(end_ticks, ticker_freq));

    /* Get the results from host. */
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

    TEST_ASSERT_EQUAL_STRING_MESSAGE("pass", _key, "Host side script reported a fail...");

    intf->disable_interrupt();
}

utest::v1::status_t us_ticker_case_setup_handler_t(const Case *const source, const size_t index_of_case)
{
#if DEVICE_LPTICKER && (LPTICKER_DELAY_TICKS > 0)
    /* Suspend the lp ticker wrapper since it makes use of the us ticker */
    ticker_suspend(get_lp_ticker_data());
    lp_ticker_wrapper_suspend();
#endif
    ticker_suspend(get_us_ticker_data());
    intf = get_us_ticker_data()->interface;
    prev_handler = set_us_ticker_irq_handler(ticker_event_handler_stub);
    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t us_ticker_case_teardown_handler_t(const Case *const source, const size_t passed, const size_t failed,
                                                      const failure_t reason)
{
    set_us_ticker_irq_handler(prev_handler);
    ticker_resume(get_us_ticker_data());
#if DEVICE_LPTICKER && (LPTICKER_DELAY_TICKS > 0)
    lp_ticker_wrapper_resume();
    ticker_resume(get_lp_ticker_data());
#endif
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

#if DEVICE_LPTICKER
utest::v1::status_t lp_ticker_case_setup_handler_t(const Case *const source, const size_t index_of_case)
{
    ticker_suspend(get_lp_ticker_data());
    intf = get_lp_ticker_data()->interface;
    prev_handler = set_lp_ticker_irq_handler(ticker_event_handler_stub);
    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t lp_ticker_case_teardown_handler_t(const Case *const source, const size_t passed, const size_t failed,
                                                      const failure_t reason)
{
    set_lp_ticker_irq_handler(prev_handler);
    ticker_resume(get_lp_ticker_data());
    return greentea_case_teardown_handler(source, passed, failed, reason);
}
#endif

// Test cases
Case cases[] = {
    Case("Microsecond ticker frequency test", us_ticker_case_setup_handler_t, ticker_frequency_test,
         us_ticker_case_teardown_handler_t),
#if DEVICE_LPTICKER
    Case("Low power ticker frequency test", lp_ticker_case_setup_handler_t, ticker_frequency_test,
         lp_ticker_case_teardown_handler_t),
#endif
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    /* Suspend RTOS Kernel so the timers are not in use. */
    osKernelSuspend();

    GREENTEA_SETUP(120, "timing_drift_auto");
    return greentea_test_setup_handler(number_of_cases);
}

void greentea_test_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    osKernelResume(0);

    greentea_test_teardown_handler(passed, failed, failure);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown);

int main()
{
    Harness::run(specification);
}

#endif // defined(SKIP_TIME_DRIFT_TESTS) || !DEVICE_USTICKER
#endif // !defined(MBED_CONF_RTOS_PRESENT)
