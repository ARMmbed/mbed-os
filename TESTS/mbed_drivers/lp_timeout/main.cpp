/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#if !DEVICE_LOWPOWERTIMER
    #error [NOT_SUPPORTED] Low power timer not supported for this target
#endif

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"
#include "us_ticker_api.h"

using namespace utest::v1;

volatile static bool complete;
static LowPowerTimeout lpt;

/* Timeouts are quite arbitrary due to large number of boards with varying level of accuracy */
#define LONG_TIMEOUT (100000)
#define SHORT_TIMEOUT (600)

void cb_done() {
    complete = true;
}

#if DEVICE_SLEEP
void lp_timeout_1s_deepsleep(void)
{
    complete = false;

    /*
     * Since deepsleep() may shut down the UART peripheral, we wait for 10ms
     * to allow for hardware serial buffers to completely flush.

     * This should be replaced with a better function that checks if the
     * hardware buffers are empty. However, such an API does not exist now,
     * so we'll use the wait_ms() function for now.
     */
    wait_ms(10);

    /*
     * We use here lp_ticker_read() instead of us_ticker_read() for start and
     * end because the microseconds timer might be disable during deepsleep.
     */
    timestamp_t start = lp_ticker_read();
    lpt.attach(&cb_done, 1);
    deepsleep();
    while (!complete);
    timestamp_t end = lp_ticker_read();

    /* It takes longer to wake up from deep sleep */
    TEST_ASSERT_UINT32_WITHIN(LONG_TIMEOUT, 1000000, end - start);
    TEST_ASSERT_TRUE(complete);
}

void lp_timeout_1s_sleep(void)
{
    complete = false;

    timestamp_t start = us_ticker_read();
    lpt.attach(&cb_done, 1);
    sleep();
    while (!complete);
    timestamp_t end = us_ticker_read();

    TEST_ASSERT_UINT32_WITHIN(LONG_TIMEOUT, 1000000, end - start);
    TEST_ASSERT_TRUE(complete);
}
#endif /* DEVICE_SLEEP */

void lp_timeout_us(uint32_t delay_us, uint32_t tolerance)
{
    complete = false;

    timestamp_t start = us_ticker_read();
    lpt.attach_us(&cb_done, delay_us);
    while (!complete);
    timestamp_t end = us_ticker_read();

    /* Using RTC which is less accurate */
    TEST_ASSERT_UINT32_WITHIN(tolerance, delay_us, end - start);
    TEST_ASSERT_TRUE(complete);
}

void lp_timeout_5s(void)
{
    lp_timeout_us(5000000, LONG_TIMEOUT);
}

void lp_timeout_1s(void)
{
    lp_timeout_us(1000000, LONG_TIMEOUT);
}

void lp_timeout_1ms(void)
{
    lp_timeout_us(1000, SHORT_TIMEOUT);
}

void lp_timeout_500us(void)
{
    lp_timeout_us(500, SHORT_TIMEOUT);

}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("500us LowPowerTimeout", lp_timeout_500us, greentea_failure_handler),
    Case("1ms LowPowerTimeout", lp_timeout_1ms, greentea_failure_handler),
    Case("1sec LowPowerTimeout", lp_timeout_1s, greentea_failure_handler),
    Case("5sec LowPowerTimeout", lp_timeout_5s, greentea_failure_handler),
#if DEVICE_SLEEP
    Case("1sec LowPowerTimeout from sleep", lp_timeout_1s_sleep, greentea_failure_handler),
    Case("1sec LowPowerTimeout from deepsleep", lp_timeout_1s_deepsleep, greentea_failure_handler),
#endif /* DEVICE_SLEEP */
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
