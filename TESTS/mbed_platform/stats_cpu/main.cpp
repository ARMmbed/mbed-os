
/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "unity/unity.h"
#include "utest/utest.h"

#include "mbed.h"

#if !defined(MBED_CPU_STATS_ENABLED) || !DEVICE_LPTICKER || !DEVICE_SLEEP
#error [NOT_SUPPORTED] test not supported
#else

using namespace utest::v1;

DigitalOut led1(LED1);

// Targets with these cores have their RAM enough size to create threads with bigger stacks
#if defined(__CORTEX_A9) || defined(__CORTEX_M23) || defined(__CORTEX_M33) || defined(__CORTEX_M7)
#define MAX_THREAD_STACK        512
#else
#define MAX_THREAD_STACK        384
#endif

#define SAMPLE_TIME             1000    // msec
#define LOOP_TIME               2000    // msec

static int32_t wait_time = 5000;

static void busy_thread()
{
    volatile uint64_t i = ~0;

    while (i--) {
        led1 = !led1;
        wait_us(wait_time);
    }
}

void get_cpu_usage()
{
    static uint64_t prev_idle_time = 0;
    mbed_stats_cpu_t stats;

    while (1) {
        mbed_stats_cpu_get(&stats);
        uint64_t diff = (stats.idle_time - prev_idle_time);
        uint8_t usage = 100 - ((diff * 100) / (SAMPLE_TIME * 1000));
        prev_idle_time = stats.idle_time;
        TEST_ASSERT_NOT_EQUAL(0, usage);
        ThisThread::sleep_for(SAMPLE_TIME);
    }
}

void test_cpu_info(void)
{
    mbed_stats_cpu_t stats;
    // Additional read to make sure timer is initialized
    mbed_stats_cpu_get(&stats);
    ThisThread::sleep_for(3);
    mbed_stats_cpu_get(&stats);
    TEST_ASSERT_NOT_EQUAL(0, stats.uptime);
    TEST_ASSERT_NOT_EQUAL(0, stats.idle_time);
    return;
}

void test_cpu_load(void)
{

    Thread thread(osPriorityNormal, MAX_THREAD_STACK);
    Thread thread_stats(osPriorityNormal, MAX_THREAD_STACK);

    thread.start(busy_thread);
    thread_stats.start(get_cpu_usage);

    // Steadily increase the system load
    for (int count = 1; ; count++) {
        ThisThread::sleep_for(LOOP_TIME);
        if (wait_time <= 0) {
            break;
        }
        wait_time -= 1000;  // usec
    }
    thread.terminate();
    thread_stats.terminate();
}

Case cases[] = {
    Case("Test CPU Info", test_cpu_info),
    Case("Test CPU load", test_cpu_load)
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

#endif // !defined(MBED_CPU_STATS_ENABLED) || !DEVICE_LPTICKER || !DEVICE_SLEEP
