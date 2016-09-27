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
 * WITHOUT WARRANTIES Ovoid CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "EventLoop.h"
#include "Event.h"
#include <stdio.h>

using namespace utest::v1;

static EventLoop queue(16);
static osThreadId main_thread_id;
static osThreadId expected_thread_id;
static volatile uint32_t call_count;

static void test_func() {
    if (expected_thread_id == NULL) {
        // We're testing the event queue, so save the queue thread ID for subsequent
        // tests (and make sure that we're not running in the main context).
        TEST_ASSERT_NOT_EQUAL((uint32_t)expected_thread_id, (uint32_t)main_thread_id);
        expected_thread_id = Thread::gettid();
    } else {
        TEST_ASSERT_EQUAL((uint32_t)expected_thread_id, (uint32_t)Thread::gettid());
    }
    call_count ++;
}

static void test_immediate_events() {
    expected_thread_id = main_thread_id;
    call_count = 0;
    // Post 10 "regular" events (they'll execute immediately in
    // the context of the main thread)
    for (unsigned i = 0; i < 10; i ++) {
        event(test_func, NULL).call();
        TEST_ASSERT_EQUAL_UINT32(call_count, i + 1);
    }
}

static void test_queued_events() {
    // We don't know the event thread ID. it'll be set by the first call to test_func
    expected_thread_id = NULL;
    call_count = 0;
    // Start event thread now
    queue.start();
    // Post 10 events that will be queued
    for (unsigned i = 0; i < 10; i ++) {
        event(test_func, &queue).call();
    }
    // Wait for them to finish
    while (call_count < 10) {
        wait_ms(10);
    }
    // Wait a bit more and check count again to check that no more events are executed
    wait_ms(200);
    TEST_ASSERT_EQUAL_UINT32(call_count, 10);
}

static Case cases[] = {
    Case("Test immediate events", test_immediate_events),
    Case("Test queued events", test_queued_events)
};

static status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

static Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    main_thread_id = Thread::gettid();
    // Run tests
    Harness::run(specification);
}
