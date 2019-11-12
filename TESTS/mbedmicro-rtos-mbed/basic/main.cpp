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
#if defined(MBED_RTOS_SINGLE_THREAD) || !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] RTOS basic test cases require RTOS with multithread to run
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

#if defined(SKIP_TIME_DRIFT_TESTS) || !DEVICE_USTICKER
#error [NOT_SUPPORTED] UsTicker need to be enabled for this test.
#else

using utest::v1::Case;

#if defined(__CORTEX_M23) || defined(__CORTEX_M33)
#define TEST_STACK_SIZE 512
#else
#define TEST_STACK_SIZE 256
#endif
#define ONE_MILLI_SEC 1000

volatile uint32_t elapsed_time_ms = 0;
static const int test_timeout = 40;


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

    Thread tick_thread(osPriorityHigh, TEST_STACK_SIZE);
    tick_thread.start(callback(update_tick_thread, &mutex));

    greentea_send_kv("timing_drift_check_start", 0);

    // wait for 1st signal from host
    do {
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        expected_key = strcmp(_key, "base_time");
    } while (expected_key);

    mutex.lock();
    elapsed_time = elapsed_time_ms;
    mutex.unlock();
    // send base_time
    greentea_send_kv(_key, elapsed_time * ONE_MILLI_SEC);

    // wait for 2nd signal from host
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

    mutex.lock();
    elapsed_time = elapsed_time_ms;
    mutex.unlock();
    // send final_time
    greentea_send_kv(_key, elapsed_time * ONE_MILLI_SEC);

    //get the results from host
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

    TEST_ASSERT_EQUAL_STRING_MESSAGE("pass", _key, "Host side script reported a fail...");
}

Case cases[] = {
    Case("Test ThisThread::sleep_for accuracy", test)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(test_timeout, "timing_drift_auto");
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

utest::v1::Specification specification(greentea_test_setup, cases);

int main()
{
    utest::v1::Harness::run(specification);
}

#endif // defined(SKIP_TIME_DRIFT_TESTS) || !DEVICE_USTICKER
#endif // defined(MBED_RTOS_SINGLE_THREAD) || !defined(MBED_CONF_RTOS_PRESENT)
