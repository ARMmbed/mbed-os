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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "mbed_stats.h"
#include <stdlib.h>
#include <stdio.h>

#if !defined(MBED_THREAD_STATS_ENABLED)
  #warning [NOT_SUPPORTED] test not supported
#endif

using namespace utest::v1;

static EventFlags ef;
static int32_t counter = 0;

#define TEST_STACK_SIZE     320
#define FLAG_SIGNAL_DEC     0x2
#define MAX_THREAD_STATS    0x8

void decrement_on_event()
{
    uint32_t ret = ef.wait_all(FLAG_SIGNAL_DEC);
    TEST_ASSERT_FALSE(ret & osFlagsError);
    TEST_ASSERT_EQUAL(FLAG_SIGNAL_DEC, ret);
    counter--;
}

void increment_with_delay()
{
    while (1) {
        counter++;
        Thread::wait(500);
    }
}

void test_case_single_thread_stats()
{
    Thread t1(osPriorityNormal, TEST_STACK_SIZE, NULL, "Th1");
    t1.start(increment_with_delay);

    // Read stats
    mbed_stats_thread_t *stats = new mbed_stats_thread_t[MAX_THREAD_STATS];
    int count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);
    TEST_ASSERT_EQUAL(4, count);

    for (int i = 0; i < count; i++) {
        if (0 == strcmp (stats[i].thread_name, "Th1")) {
            TEST_ASSERT_EQUAL(TEST_STACK_SIZE, stats[i].thread_stack_size);
            TEST_ASSERT_EQUAL(osPriorityNormal, stats[i].thread_priority);
            break;
        }
    }
    delete[] stats;
    t1.terminate();
}

void test_case_less_count()
{
    // Default Mbed OS has 3 threads
    mbed_stats_thread_t *stats = new mbed_stats_thread_t[2];
    int count = mbed_stats_thread_get_each(stats, 2);
    TEST_ASSERT_EQUAL(2, count);
    delete[] stats;
}

void test_case_multi_threads_blocked()
{
    Thread t1(osPriorityNormal, TEST_STACK_SIZE, NULL, "Th1");
    Thread t2(osPriorityNormal1, TEST_STACK_SIZE, NULL, "Th2");
    t1.start(increment_with_delay);
    t2.start(decrement_on_event);

    // Read stats
    mbed_stats_thread_t *stats = new mbed_stats_thread_t[MAX_THREAD_STATS];
    int count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);
    TEST_ASSERT_EQUAL(5, count);

    for (int i = 0; i < count; i++) {
        if (0 == strcmp (stats[i].thread_name, "Th2")) {
            TEST_ASSERT_EQUAL(TEST_STACK_SIZE, stats[i].thread_stack_size);
            TEST_ASSERT_EQUAL(osPriorityNormal1, stats[i].thread_priority);
            TEST_ASSERT_EQUAL(osThreadBlocked, stats[i].thread_state);
        } else if (0 == strcmp (stats[i].thread_name, "Th1")) {
            TEST_ASSERT_EQUAL(TEST_STACK_SIZE, stats[i].thread_stack_size);
            TEST_ASSERT_EQUAL(osPriorityNormal, stats[i].thread_priority);
        }
    }

    // Signal blocked thread
    uint32_t ret = ef.set(FLAG_SIGNAL_DEC);
    TEST_ASSERT_FALSE(ret & osFlagsError);

    Thread::wait(100);

    count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);
    TEST_ASSERT_EQUAL(4, count);

    delete[] stats;
    t1.terminate();
}

void test_case_multi_threads_terminate()
{
    Thread t1(osPriorityNormal1, TEST_STACK_SIZE, NULL, "Th1");
    Thread t2(osPriorityNormal2, TEST_STACK_SIZE, NULL, "Th2");
    t2.start(increment_with_delay);
    t1.start(decrement_on_event);

    // Read stats
    mbed_stats_thread_t *stats = new mbed_stats_thread_t[MAX_THREAD_STATS];
    int count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);
    TEST_ASSERT_EQUAL(5, count);

    for (int i = 0; i < count; i++) {
        if (0 == strcmp (stats[i].thread_name, "Th2")) {
            TEST_ASSERT_EQUAL(TEST_STACK_SIZE, stats[i].thread_stack_size);
            TEST_ASSERT_EQUAL(osPriorityNormal2, stats[i].thread_priority);
        } else if (0 == strcmp (stats[i].thread_name, "Th1")) {
            TEST_ASSERT_EQUAL(TEST_STACK_SIZE, stats[i].thread_stack_size);
            TEST_ASSERT_EQUAL(osPriorityNormal1, stats[i].thread_priority);
            TEST_ASSERT_EQUAL(osThreadBlocked, stats[i].thread_state);
        }
    }

    t1.terminate();
    t2.terminate();

    count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);
    TEST_ASSERT_EQUAL(3, count);

    delete[] stats;
    t1.terminate();
}

Case cases[] = {
    Case("Single Thread Stats", test_case_single_thread_stats),
    Case("Less count value", test_case_less_count),
    Case("Multiple Threads blocked", test_case_multi_threads_blocked),
    Case("Multiple Threads terminate", test_case_multi_threads_terminate),
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
