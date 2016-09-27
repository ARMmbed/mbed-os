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
#include "Callback.h"
#include <stdio.h>

using namespace utest::v1;

static EventLoop loop(16);
static osThreadId main_thread_id;
static osThreadId other_thread_id;
static volatile uint32_t cb_count;

static void cb1() {
    if (cb_count == 0) {
        // Check that we don't run in the main thread context
        TEST_ASSERT_NOT_EQUAL((uint32_t)Thread::gettid(), (uint32_t)main_thread_id);
        // And save current running context
        other_thread_id = Thread::gettid();
    } else {
        // Check that we're running in the same context as before
        TEST_ASSERT_EQUAL((uint32_t)Thread::gettid(), (uint32_t)other_thread_id);
    }
    cb_count ++;
}

static void test_event_loop_callbacks() {
    // Post 10 callbacks
    for (unsigned i = 0; i < 10; i ++) {
        loop.post(callback(cb1));
    }
    // Wait for all callbacks to finish
    while (cb_count < 10) {
        wait_ms(10);
    }
    // Wait a bit more and check count again to check that no more events are executed
    wait_ms(200);
    TEST_ASSERT_EQUAL_UINT32(cb_count, 10);
}

static Case cases[] = {
    Case("Test event loop callbacks", test_event_loop_callbacks)
};

static status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

static Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    // Start event thread
    main_thread_id = Thread::gettid();
    loop.start();
    // Run tests
    Harness::run(specification);
}
