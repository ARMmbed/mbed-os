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

#if !DEVICE_RTC
#error [NOT_SUPPORTED] RTC API not supported for this target
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "rtc_test.h"

#include "mbed.h"
#include "rtc_api.h"

using namespace utest::v1;

static const uint32_t WAIT_TIME = 4;
static const uint32_t WAIT_TOLERANCE = 1;

#define US_PER_SEC 1000000
#define ACCURACY_FACTOR 10

static const uint32_t DELAY_4S = 4;
static const uint32_t DELAY_10S = 10;
static const uint32_t RTC_TOLERANCE = 1;
static const uint32_t TOLERANCE_ACCURACY_US = (DELAY_10S *US_PER_SEC / ACCURACY_FACTOR);

#if DEVICE_LPTICKER
volatile bool expired;

void set_flag_true(void)
{
    expired = true;
}

/* Auxiliary function to test if RTC continue counting in
 * sleep and deep-sleep modes. */
void rtc_sleep_test_support(bool deepsleep_mode)
{
    LowPowerTimeout timeout;
    const uint32_t start = 100;
    expired = false;

    /*
     * Since deepsleep() may shut down the UART peripheral, we wait for 10ms
     * to allow for hardware serial buffers to completely flush.
     * This should be replaced with a better function that checks if the
     * hardware buffers are empty. However, such an API does not exist now,
     * so we'll use the ThisThread::sleep_for() function for now.
     */
    ThisThread::sleep_for(10);

    rtc_init();

    if (deepsleep_mode == false) {
        sleep_manager_lock_deep_sleep();
    }

    rtc_write(start);

    timeout.attach(set_flag_true, DELAY_4S);

    TEST_ASSERT(sleep_manager_can_deep_sleep_test_check() == deepsleep_mode);

    while (!expired) {
        sleep();
    }

    const uint32_t stop = rtc_read();

    TEST_ASSERT_UINT32_WITHIN(RTC_TOLERANCE, DELAY_4S, stop - start);

    timeout.detach();

    if (deepsleep_mode == false) {
        sleep_manager_unlock_deep_sleep();
    }

    rtc_free();
}
#endif

/* Test that ::rtc_init can be called multiple times. */
void rtc_init_test()
{
    for (int i = 0; i < 10; i++) {
        rtc_init();
    }

    rtc_free();
}

#if DEVICE_LPTICKER
/** Test that the RTC keeps counting in the various sleep modes. */

void rtc_sleep_test()
{
    /* Test sleep mode. */
    rtc_sleep_test_support(false);

    /* Test deep-sleep mode. */
    rtc_sleep_test_support(true);
}
#endif

/* Test that the RTC keeps counting even after ::rtc_free has been called. */
void rtc_persist_test()
{
    const uint32_t start = 100;
    rtc_init();
    rtc_write(start);
    rtc_free();

    ThisThread::sleep_for(WAIT_TIME * 1000);

    rtc_init();
    const uint32_t stop = rtc_read();
    const int enabled = rtc_isenabled();
    rtc_free();

    TEST_ASSERT_TRUE(enabled);
    TEST_ASSERT_UINT32_WITHIN(WAIT_TOLERANCE, WAIT_TIME, stop - start);
}

/* Test time does not glitch backwards due to an incorrectly implemented ripple counter driver. */
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

/* Test that the RTC correctly handles different time values. */
void rtc_range_test()
{
    static const uint32_t starts[] = {
        0x00000000,
        0xEFFFFFFF,
        0x00001000,
        0x00010000,
    };

    rtc_init();
    for (uint32_t i = 0; i < sizeof(starts) / sizeof(starts[0]); i++) {
        const uint32_t start = starts[i];
        rtc_write(start);
        ThisThread::sleep_for(WAIT_TIME * 1000);
        const uint32_t stop = rtc_read();
        TEST_ASSERT_UINT32_WITHIN(WAIT_TOLERANCE, WAIT_TIME, stop - start);
    }
    rtc_free();
}

/* Test that the RTC accuracy is at least 10%. */
void rtc_accuracy_test()
{
    Timer timer1;

    const uint32_t start = 100;
    rtc_init();
    rtc_write(start);

    timer1.start();
    while (rtc_read() < (start + DELAY_10S)) {
        /* Just wait. */
    }
    timer1.stop();

    /* RTC accuracy is at least 10%. */
    TEST_ASSERT_INT32_WITHIN(TOLERANCE_ACCURACY_US, DELAY_10S * US_PER_SEC, timer1.read_us());
}

/* Test that ::rtc_write/::rtc_read functions provides availability to set/get RTC time. */
void rtc_write_read_test()
{
    static const uint32_t rtc_init_val = 100;

    rtc_init();

    for (int i = 0; i < 3; i++) {
        const uint32_t init_val = (rtc_init_val + i * rtc_init_val);

        core_util_critical_section_enter();

        rtc_write(init_val);
        const uint32_t read_val = rtc_read();

        core_util_critical_section_exit();

        /* No tolerance is provided since we should have 1 second to
         * execute this case after the RTC time is set.
         */
        TEST_ASSERT_EQUAL_UINT32(init_val, read_val);
    }

    rtc_free();
}

/* Test that ::is_enabled function returns 1 if the RTC is counting and the time has been set. */
void rtc_enabled_test()
{
    /* Since some platforms use RTC for low power timer RTC may be already enabled.
     * Because of that we will only verify if rtc_isenabled() returns 1 in case when init is done
     * and RTC time is set.
     */

    rtc_init();
    rtc_write(0);
    TEST_ASSERT_EQUAL_INT(1, rtc_isenabled());
    rtc_free();
}

Case cases[] = {
    Case("RTC - init", rtc_init_test),
#if DEVICE_LPTICKER
    Case("RTC - sleep", rtc_sleep_test),
#endif
    Case("RTC - persist", rtc_persist_test),
    Case("RTC - glitch", rtc_glitch_test),
    Case("RTC - range", rtc_range_test),
    Case("RTC - accuracy", rtc_accuracy_test),
    Case("RTC - write/read", rtc_write_read_test),
    Case("RTC - enabled", rtc_enabled_test),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif // !DEVICE_RTC
