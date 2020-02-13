/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

#include "rtos/Kernel.h"
#include "mbed.h"

using namespace std::chrono_literals;

using utest::v1::Case;

#define TEST_REPEAT_COUNT   1000
#define NUM_WAIT_TICKS      rtos::Kernel::Clock::duration(1s)

#define ONE_SECOND          Timer::duration(1s)
#define SMALL_DELTA         Timer::duration(1500us)        // 0.15%
#define BIG_DELTA           Timer::duration(15000us)       // 1.5%

/** Test if kernel ticker frequency is 1kHz

    Given a RTOS kernel ticker
    When check it frequency
    Then the the frequency is 1kHz
 */
void test_frequency()
{
#if defined(MBED_CONF_RTOS_PRESENT)
    uint32_t freq = osKernelGetTickFreq();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1000, freq, "Expected SysTick frequency is 1kHz");
#endif
    TEST_ASSERT_TRUE_MESSAGE((std::ratio_equal<rtos::Kernel::Clock::period, std::milli>::value), "Expected Kernel::Clock frequency is 1kHz");
}

/** Test if kernel ticker increments by one

    Given a RTOS kernel ticker
    When perform subsequent calls of @a rtos::Kernel::Clock::now
    Then subsequent reads should not differ by more than one
 */
void test_increment(void)
{
    for (uint32_t i = 0; i < TEST_REPEAT_COUNT; i++) {
        auto start = rtos::Kernel::Clock::now();
        while (true) {
            rtos::Kernel::Clock::duration diff = rtos::Kernel::Clock::now() - start;
            if (diff.count() != 0) {
                TEST_ASSERT_EQUAL_INT64(1, diff.count());
                break;
            }
        }
    }
}

/** Test if kernel ticker interval is 1ms

    Given a RTOS kernel ticker
    When perform subsequent calls of @a rtos::Kernel::Clock::now
    Then the ticker interval should be 1ms
 */
void test_interval()
{
    Kernel::Clock::time_point start, stop;
    Timer timer;

    start = rtos::Kernel::Clock::now();
    // wait for tick
    do {
        stop = rtos::Kernel::Clock::now();
    } while (stop == start);
    timer.start();
    start = stop;

    // wait for NUM_WAIT_TICKS ticks
    do {
        stop = rtos::Kernel::Clock::now();
    } while ((stop - start) != NUM_WAIT_TICKS);
    timer.stop();
    TEST_ASSERT_EQUAL_INT64(NUM_WAIT_TICKS.count(), (stop - start).count());

#if defined(NO_SYSTICK) || defined(MBED_TICKLESS)
    // On targets with NO_SYSTICK/MBED_TICKLESS enabled, systick is emulated by lp_ticker what makes it less accurate
    // for more details https://os.mbed.com/docs/latest/reference/tickless.html
    TEST_ASSERT_INT64_WITHIN(BIG_DELTA.count(), ONE_SECOND.count(), timer.read_duration().count());
#else
    TEST_ASSERT_INT64_WITHIN(SMALL_DELTA.count(), ONE_SECOND.count(), timer.read_duration().count());
#endif
}

// Test cases
Case cases[] = {
    Case("Test kernel ticker frequency", test_frequency),
    Case("Test if kernel ticker increments by one", test_increment),
    Case("Test if kernel ticker interval is 1ms", test_interval)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

utest::v1::Specification specification(greentea_test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
