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


#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "ticker_api_test_freq.h"
#include "hal/us_ticker_api.h"
#include "hal/lp_ticker_api.h"

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] test not supported
#endif

#define US_PER_S 1000000

using namespace utest::v1;

const ticker_interface_t* intf;

static volatile unsigned int overflowCounter;

uint32_t ticks_to_us(uint32_t ticks, uint32_t freq)
{
    return (uint32_t)((uint64_t)ticks * US_PER_S / freq);
}

void ticker_event_handler_stub(const ticker_data_t * const ticker)
{
    if (ticker == get_us_ticker_data()) {
        us_ticker_clear_interrupt();
    } else {
#if DEVICE_LPTICKER
        lp_ticker_clear_interrupt();
#endif
    }

    overflowCounter++;
}

/* Test that the ticker is operating at the frequency it specifies. */
void ticker_frequency_test()
{
    char _key[11] = { };
    char _value[128] = { };
    int expected_key = 1;
    const uint32_t ticker_freq = intf->get_info()->frequency;
    const uint32_t ticker_bits = intf->get_info()->bits;
    const uint32_t ticker_max = (1 << ticker_bits) - 1;

    intf->init();

    /* Detect overflow for tickers with lower counters width. */
    intf->set_interrupt(0);

    greentea_send_kv("timing_drift_check_start", 0);

    /* Wait for 1st signal from host. */
    do {
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        expected_key = strcmp(_key, "base_time");
    } while (expected_key);

    overflowCounter = 0;

    const uint32_t begin_ticks = intf->read();

    /* Assume that there was no overflow at this point - we are just after init. */
    greentea_send_kv(_key, ticks_to_us(begin_ticks, ticker_freq));

    /* Wait for 2nd signal from host. */
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

    const uint32_t end_ticks = intf->read();

    greentea_send_kv(_key, ticks_to_us(end_ticks + overflowCounter * ticker_max, ticker_freq));

    /* Get the results from host. */
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

    TEST_ASSERT_EQUAL_STRING_MESSAGE("pass", _key,"Host side script reported a fail...");

    intf->disable_interrupt();
}

utest::v1::status_t us_ticker_case_setup_handler_t(const Case * const source, const size_t index_of_case)
{
    intf = get_us_ticker_data()->interface;
    set_us_ticker_irq_handler(ticker_event_handler_stub);
    return greentea_case_setup_handler(source, index_of_case);
}

#if DEVICE_LPTICKER
utest::v1::status_t lp_ticker_case_setup_handler_t(const Case * const source, const size_t index_of_case)
{
    intf = get_lp_ticker_data()->interface;
    set_lp_ticker_irq_handler(ticker_event_handler_stub);
    return greentea_case_setup_handler(source, index_of_case);
}
#endif

utest::v1::status_t ticker_case_teardown_handler_t(const Case * const source, const size_t passed, const size_t failed,
        const failure_t reason)
{
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

// Test cases
Case cases[] = {
    Case("Microsecond ticker frequency test", us_ticker_case_setup_handler_t, ticker_frequency_test,
        ticker_case_teardown_handler_t),
#if DEVICE_LPTICKER
    Case("Low power ticker frequency test", lp_ticker_case_setup_handler_t, ticker_frequency_test,
            ticker_case_teardown_handler_t),
#endif
        };

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(120, "timing_drift_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
