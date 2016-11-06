
/* mbed Microcontroller Library
 * Copyright (c) 2013-2015 ARM Limited
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

static bool failure_is_in_setup = false;

void never_call_case()
{
    TEST_FAIL_MESSAGE("Case handler should have never been called!");
}
Case cases[] =
{
    Case("dummy test", never_call_case)
};

// this setup handler fails
utest::v1::status_t failing_setup_handler(const size_t number_of_cases)
{
    GREENTEA_SETUP(5, "default_auto");

    utest::v1::status_t status = greentea_test_setup_handler(number_of_cases);

    failure_is_in_setup = true;
    TEST_FAIL_MESSAGE("Explicit assertion failure in test setup handler!");
    return status;
};

void test_failure_handler(const failure_t failure)
{
    if (failure_is_in_setup) {
        failure_is_in_setup = false;
        TEST_ASSERT_EQUAL(REASON_ASSERTION, failure.reason);
        TEST_ASSERT_EQUAL(LOCATION_TEST_SETUP, failure.location);
        verbose_test_failure_handler(failure);

        // pretend to greentea that we actally executed one test case
        greentea_case_setup_handler(cases, 0);
        greentea_case_teardown_handler(cases, 1, 0, REASON_NONE);

        // pretend to greentea that the test was successful
        greentea_test_teardown_handler(1, 0, REASON_NONE);
        while(1) ;
    }
    else {
        selftest_handlers.test_failure(failure);
    }
}

const handlers_t custom_handlers = {
    greentea_abort_handlers.test_setup,
    greentea_abort_handlers.test_teardown,
    test_failure_handler,
    greentea_abort_handlers.case_setup,
    greentea_abort_handlers.case_teardown,
    greentea_abort_handlers.case_failure
};

Specification specification(failing_setup_handler, cases, custom_handlers);

int main()
{
    Harness::run(specification);
}
