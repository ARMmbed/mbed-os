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
#include "us_ticker_api.h"
#include "ticker_api.h"

using namespace utest::v1;

namespace {
    volatile bool complete;
    const ticker_interface_t* intf;
}

/* Ticker init should be run just once, thus read should always return a value that 
 * increases (no reset).
 */
void test_ticker_init(void)
{
    intf->init();
    uint32_t previous = intf->read();

    intf->init();
    uint32_t current = intf->read();
    TEST_ASSERT_TRUE_MESSAGE(previous <= current, "init() changed the counter");

    previous = intf->read();
    intf->init();
    current = intf->read();
    TEST_ASSERT_TRUE_MESSAGE(previous <= current, "init() changed the counter");
}

/* Read multiple times, each returned time should be bigger than the previous one
 * Counter up.
 */
void test_ticker_read(void)
{
    // this test assumes that to wrap around we would need to run >60 minutes
    // therefore wrapping should not happen - previous <= current
    const uint32_t test_loop = 15000;
    uint32_t previous = intf->read();
    uint32_t current;
    for (uint32_t i = 0UL; i < test_loop; i++) {
        current = intf->read();
        TEST_ASSERT_TRUE_MESSAGE(previous <= current, "us ticker counter wrapped around");
    }
}

/* Implement simple read while loop to check if time is increasing (counter up)
 */
void test_ticker_read_loop()
{
    uint32_t future_time = intf->read() + 10000UL;
    while (intf->read() < future_time);
    TEST_ASSERT_TRUE_MESSAGE(future_time <= intf->read(), "Future time is in the past");
}

utest::v1::status_t us_ticker_setup(const Case *const source, const size_t index_of_case)
{
    intf = get_us_ticker_data()->interface;
    return greentea_case_setup_handler(source, index_of_case);
}

#if DEVICE_LOWPOWERTIMER
utest::v1::status_t lp_ticker_setup(const Case *const source, const size_t index_of_case)
{
    intf = get_lp_ticker_data()->interface;
    return greentea_case_setup_handler(source, index_of_case);
}
#endif

Case cases[] = {
    Case("us ticker HAL - Init", us_ticker_setup, test_ticker_init),
    Case("us ticker HAL - Read", us_ticker_setup, test_ticker_read),
    Case("us ticker HAL - Read in the loop", us_ticker_setup, test_ticker_read_loop),
#if DEVICE_LOWPOWERTIMER
    Case("lp ticker HAL - Init", lp_ticker_setup, test_ticker_init),
    Case("lp ticker HAL - Read", lp_ticker_setup, test_ticker_read),
    Case("lp ticker HAL - Read in the loop", lp_ticker_setup, test_ticker_read_loop),
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
    Harness::run(specification);
}
