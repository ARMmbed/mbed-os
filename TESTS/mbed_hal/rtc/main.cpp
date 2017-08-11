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

#if !DEVICE_RTC
    #error [NOT_SUPPORTED] RTC API not supported for this target
#endif

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "rtc_test.h"

#include "mbed.h"
#include "rtc_api.h"


using namespace utest::v1;

static const uint32_t WAIT_TIME = 4;
static const uint32_t WAIT_TOLERANCE = 1;


void rtc_init_test()
{
    for (int i = 0; i < 10; i++) {
        rtc_init();
    }
}

void rtc_sleep_test()
{
    const uint32_t start = 100;
    rtc_init();

    rtc_write(start);
    wait(WAIT_TIME);
    const uint32_t stop = rtc_read();

    rtc_free();

    TEST_ASSERT_UINT32_WITHIN(WAIT_TOLERANCE, WAIT_TIME, stop - start);
}

void rtc_persist_test()
{
    const uint32_t start = 100;
    rtc_init();
    rtc_write(start);
    rtc_free();

    wait(WAIT_TIME);

    rtc_init();
    const uint32_t stop = rtc_read();
    const int enabled = rtc_isenabled();
    rtc_free();

    TEST_ASSERT_TRUE(enabled);
    TEST_ASSERT_UINT32_WITHIN(WAIT_TOLERANCE, WAIT_TIME, stop - start);
}

void rtc_glitch_test()
{
    const uint32_t start = 0xffffe;
    rtc_init();

    rtc_write(start);
    uint32_t last = start;
    while (last < start + 4) {
        const uint32_t cur = rtc_read();
        TEST_ASSERT(cur >= last);
        last = cur;
    }

    rtc_free();
}

void rtc_range_test()
{
    static const uint32_t starts[] = {
        0x00000000,
        0xEFFFFFFF,
        0x00001000,
    };

    rtc_init();
    for (uint32_t i = 0; i < sizeof(starts) / sizeof(starts[0]); i++) {
        const uint32_t start = starts[i];
        rtc_write(start);
        wait(WAIT_TIME);
        const uint32_t stop = rtc_read();
        TEST_ASSERT_UINT32_WITHIN(WAIT_TOLERANCE, WAIT_TIME, stop - start);
    }
    rtc_free();
}

Case cases[] = {
    Case("RTC - init", rtc_init_test),
    Case("RTC - sleep", rtc_sleep_test),
    Case("RTC - persist", rtc_persist_test),
    Case("RTC - glitch", rtc_glitch_test),
    Case("RTC - range", rtc_range_test),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(30, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
