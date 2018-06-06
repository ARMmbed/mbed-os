
/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#if !defined(MBED_THREAD_STATS_ENABLED)
#error [NOT_SUPPORTED] test not supported
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
    mbed_stats_thread_t *stats = new mbed_stats_thread_t[MAX_THREAD_STATS];
    int old_count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);
    Thread t1(osPriorityNormal, TEST_STACK_SIZE, NULL, "Th1");
    t1.start(increment_with_delay);

    // Read stats
    int count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);
    TEST_ASSERT_EQUAL(1, (count - old_count));

    for (int i = 0; i < count; i++) {
        if (0 == strcmp(stats[i].name, "Th1")) {
            TEST_ASSERT_EQUAL(TEST_STACK_SIZE, stats[i].stack_size);
            TEST_ASSERT_EQUAL(osPriorityNormal, stats[i].priority);
            break;
        }
    }

    t1.terminate();
    delete[] stats;
}

#define SINGLE_ELEMENT  1
void test_case_less_count()
{
    // Default Mbed OS has 3 threads
    mbed_stats_thread_t stats;
    int count = mbed_stats_thread_get_each(&stats, SINGLE_ELEMENT);
    TEST_ASSERT_EQUAL(SINGLE_ELEMENT, count);
}

void test_case_multi_threads_blocked()
{
    mbed_stats_thread_t *stats = new mbed_stats_thread_t[MAX_THREAD_STATS];
    int old_count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);

    Thread t1(osPriorityNormal, TEST_STACK_SIZE, NULL, "Th1");
    Thread t2(osPriorityNormal1, TEST_STACK_SIZE, NULL, "Th2");
    t1.start(increment_with_delay);
    t2.start(decrement_on_event);

    // Read stats

    int count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);
    TEST_ASSERT_EQUAL(2, (count - old_count));
    for (int i = 0; i < count; i++) {
        if (0 == strcmp(stats[i].name, "Th2")) {
            TEST_ASSERT_EQUAL(TEST_STACK_SIZE, stats[i].stack_size);
            TEST_ASSERT_EQUAL(osPriorityNormal1, stats[i].priority);
            TEST_ASSERT_EQUAL(osThreadBlocked, stats[i].state);
        } else if (0 == strcmp (stats[i].name, "Th1")) {
            TEST_ASSERT_EQUAL(TEST_STACK_SIZE, stats[i].stack_size);
            TEST_ASSERT_EQUAL(osPriorityNormal, stats[i].priority);
        }
    }

    // Signal blocked thread
    uint32_t ret = ef.set(FLAG_SIGNAL_DEC);
    TEST_ASSERT_FALSE(ret & osFlagsError);

    Thread::wait(100);

    count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);
    TEST_ASSERT_EQUAL(1, (count - old_count));

    t1.terminate();
    delete[] stats;
}

void test_case_multi_threads_terminate()
{
    mbed_stats_thread_t *stats = new mbed_stats_thread_t[MAX_THREAD_STATS];
    int old_count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);

    Thread t1(osPriorityNormal1, TEST_STACK_SIZE, NULL, "Th1");
    Thread t2(osPriorityNormal2, TEST_STACK_SIZE, NULL, "Th2");
    t2.start(increment_with_delay);
    t1.start(decrement_on_event);

    // Read stats

    int count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);
    TEST_ASSERT_EQUAL(2, (count - old_count));

    for (int i = 0; i < count; i++) {
        if (0 == strcmp(stats[i].name, "Th2")) {
            TEST_ASSERT_EQUAL(TEST_STACK_SIZE, stats[i].stack_size);
            TEST_ASSERT_EQUAL(osPriorityNormal2, stats[i].priority);
        } else if (0 == strcmp(stats[i].name, "Th1")) {
            TEST_ASSERT_EQUAL(TEST_STACK_SIZE, stats[i].stack_size);
            TEST_ASSERT_EQUAL(osPriorityNormal1, stats[i].priority);
            TEST_ASSERT_EQUAL(osThreadBlocked, stats[i].state);
        }
    }

    t1.terminate();
    t2.terminate();

    count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);
    TEST_ASSERT_EQUAL(count, old_count);

    delete[] stats;
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
