
/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 ARM Limited
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


#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

using namespace utest::v1;

// only one callback is active at any given time
volatile utest_v1_harness_callback_t minimal_callback;

// Scheduler ----------------------------------------------------------------------------------------------------------
static int32_t utest_minimal_init()
{
    minimal_callback = NULL;
    return 0;
}
static void *utest_minimal_post(const utest_v1_harness_callback_t callback, const uint32_t delay_ms)
{
    minimal_callback = callback;
    // this scheduler does not support scheduling of asynchronous callbacks
    return (delay_ms ? NULL : (void*)1);
}
static int32_t utest_minimal_cancel(void *handle)
{
    (void) handle;
    // this scheduler does not support canceling of asynchronous callbacks
    return -1;
}
static int32_t utest_minimal_run()
{
    /* This is the amazing minimal scheduler.
     * This is just a busy loop that calls the callbacks in this context.
     * THIS LOOP IS BLOCKING.
     */
    while(1)
    {
        // check if a new callback has been set
        if (minimal_callback) {
            // copy the callback
            utest_v1_harness_callback_t callback = minimal_callback;
            // reset the shared callback
            minimal_callback = NULL;
            // execute the copied callback
            callback();
        }
    }
    return 0;
}
static const utest_v1_scheduler_t utest_minimal_scheduler =
{
    utest_minimal_init,
    utest_minimal_post,
    utest_minimal_cancel,
    utest_minimal_run
};

// Tests --------------------------------------------------------------------------------------------------------------
int call_counter(0);

// Basic Test Case ----------------------------------------------------------------------------------------------------
control_t test_case()
{
    static int counter(0);
    TEST_ASSERT_EQUAL(counter++, call_counter++);
    return CaseNext;
}

// Cases --------------------------------------------------------------------------------------------------------------
Case cases[] = {
    Case("Minimal Scheduler: Case 1", test_case),
    Case("Minimal Scheduler: Case 2", test_case),
    Case("Minimal Scheduler: Case 3", test_case),
    Case("Minimal Scheduler: Case 4", test_case),
    Case("Minimal Scheduler: Case 5", test_case),
    Case("Minimal Scheduler: Case 6", test_case),
    Case("Minimal Scheduler: Case 7", test_case),
    Case("Minimal Scheduler: Case 8", test_case)
};

// Specification: Setup & Teardown ------------------------------------------------------------------------------------
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}
void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(8, call_counter++);
    TEST_ASSERT_EQUAL(8, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    greentea_test_teardown_handler(passed, failed, failure);
}

Specification specification(greentea_setup, cases, greentea_teardown, selftest_handlers);

int main()
{
    // You MUST set the custom scheduler before running the specification.
    Harness::set_scheduler(utest_minimal_scheduler);
    // Run the specification only AFTER setting the custom scheduler.
    Harness::run(specification);
}
