
/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 ARM Limited
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
#include "utest/utest_stack_trace.h"

#include "ticker_api.h"
#include "us_ticker_api.h"

using namespace utest::v1;

// only one callback is active at any given time
volatile utest_v1_harness_callback_t minimal_callback;
volatile utest_v1_harness_callback_t ticker_callback;
static Timeout utest_minimal_object;

// Scheduler ----------------------------------------------------------------------------------------------------------
static void ticker_handler()
{
    UTEST_LOG_FUNCTION();
    minimal_callback = ticker_callback;
}

static int32_t utest_minimal_init()
{
    UTEST_LOG_FUNCTION();
    minimal_callback = NULL;
    ticker_callback = NULL;
    return 0;
}
static void *utest_minimal_post(const utest_v1_harness_callback_t callback,  timestamp_t delay_ms)
{
    UTEST_LOG_FUNCTION();
    timestamp_t delay_us = delay_ms *1000;

    if (delay_ms) {
        ticker_callback = callback;
        // fire the interrupt in 1000us * delay_ms
        utest_minimal_object.attach_us(ticker_handler, delay_us);
    } else {
        minimal_callback = callback;
    }

    // return a bogus handle
    return (void*)1;
}
static int32_t utest_minimal_cancel(void *handle)
{
    UTEST_LOG_FUNCTION();
    (void) handle;
    utest_minimal_object.detach();
    return 0;
}
static int32_t utest_minimal_run()
{
    UTEST_LOG_FUNCTION();
    /* This is the amazing minimal scheduler.
     * This is just a busy loop that calls the callbacks in this context.
     * THIS LOOP IS BLOCKING.
     */
    while(1)
    {
        // check if a new callback has been set
        if (minimal_callback)
        {
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
    UTEST_LOG_FUNCTION();
    static int counter(0);
    TEST_ASSERT_EQUAL(counter++, call_counter++);
    return CaseNext;
}

// Async Test Case Failure --------------------------------------------------------------------------------------------
control_t test_case_async()
{
    UTEST_LOG_FUNCTION();
    static int counter(3);
    TEST_ASSERT_EQUAL(counter++, call_counter++);
    return CaseTimeout(200);
}
utest::v1::status_t test_case_async_failure(const Case *const source, const failure_t reason)
{
    UTEST_LOG_FUNCTION();
    // ignore the timeout, since this is a test
    return greentea_case_failure_continue_handler(source, reason.ignored());
}

// Cases --------------------------------------------------------------------------------------------------------------
Case cases[] = {
    Case("Minimal Scheduler: Case 1", test_case),
    Case("Minimal Scheduler: Case 2", test_case),
    Case("Minimal Scheduler: Case 3", test_case),
    Case("Minimal Scheduler: Async Case 4 (Failure)", test_case_async, test_case_async_failure)
};

// Specification: Setup & Teardown ------------------------------------------------------------------------------------
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}
void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(4, call_counter++);
    TEST_ASSERT_EQUAL(4, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    UTEST_DUMP_TRACE
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
