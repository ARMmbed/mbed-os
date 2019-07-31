/*
 * Copyright (c) 2013-2017, ARM Limited, All Rights Reserved
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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "MbedTester.h"

#if !COMPONENT_FPGA_CI_TEST_SHIELD
#error [NOT_SUPPORTED] FPGA CI Test Shield is needed to run this test
#endif

#if defined(MBED_RTOS_SINGLE_THREAD)
#error [NOT_SUPPORTED] test not supported
#endif

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] test not supported
#endif

using utest::v1::Case;

#if defined(__CORTEX_M23) || defined(__CORTEX_M33)
#define TEST_STACK_SIZE 512
#else
#define TEST_STACK_SIZE 256
#endif
#define ONE_MILLI_SEC 1000
#define DELAY_MS 1000 // 1 sec

volatile uint32_t elapsed_time_ms = 0;
static const int test_timeout = 40;

const PinList *form_factor = pinmap_ff_default_pins();
const PinList *restricted = pinmap_restricted_pins();
MbedTester tester(form_factor, restricted);

void update_tick_thread(Mutex *mutex)
{
    while (true) {
        ThisThread::sleep_for(1);
        mutex->lock();
        ++elapsed_time_ms;
        mutex->unlock();
    }
}


/** Tests is to measure the accuracy of ThisThread::sleep_for() over a period of time

    Given
        a thread updating elapsed_time_ms every milli sec
        and host script for time measurement accuracy check (More details on tests can be found in timing_drift_auto.py)
    When host query what is current count base_time
    Then Device responds by the elapsed_time_ms
    When host query what is current count final_time
    Then Device responds by the elapsed_time_ms
    When host computes the drift considering base_time, final_time, transport delay and measurement stretch
    Then host send the results back to device pass/fail based on tolerance
 */
void test(void)
{
    char _key[11] = { };
    char _value[128] = { };
    int expected_key = 1;
    Mutex mutex;
    uint32_t elapsed_time;
    uint32_t start, stop;

    tester.reset();

    tester.timer_init();

    tester.peripherals_reset();

    tester.select_peripheral(MbedTester::PeripheralTimer);

    tester.timer_set_mode(MbedTester::TimerModeCountDownTimer);

    tester.timer_set_delay_ms(DELAY_MS);

    tester.timer_reset();

    Thread tick_thread(osPriorityHigh, TEST_STACK_SIZE);
    tick_thread.start(callback(update_tick_thread, &mutex));

    mutex.lock();
    start = elapsed_time_ms;
    mutex.unlock();

    tester.timer_delay();

    mutex.lock();
    stop = elapsed_time_ms;
    mutex.unlock();

    TEST_ASSERT_UINT32_WITHIN(1, DELAY_MS, stop - start);
}

Case cases[] = {
    Case("Test ThisThread::sleep_for accuracy", test)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(test_timeout, "default_auto");
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

utest::v1::Specification specification(greentea_test_setup, cases);

int main()
{
    utest::v1::Harness::run(specification);
}
