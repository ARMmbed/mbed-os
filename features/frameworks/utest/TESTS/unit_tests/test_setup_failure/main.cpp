
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

void never_call_case()
{
    TEST_FAIL_MESSAGE("Case handler should have never been called!");
}
Case cases[] =
{
    Case("dummy test", never_call_case),
    Case("dummy test 2", never_call_case)
};

// this setup handler fails
utest::v1::status_t failing_setup_handler(const size_t number_of_cases)
{
    GREENTEA_SETUP(5, "default_auto");

    TEST_ASSERT_EQUAL(2, number_of_cases);
    greentea_test_setup_handler(number_of_cases);
    return STATUS_ABORT;    // aborting test
};

// the teardown handler will then be called with the reason `REASON_TEST_SETUP`
void failing_teardown_handler(const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(0, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_TEST_SETUP, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_TEST_SETUP, failure.location);

    verbose_test_teardown_handler(passed, failed, failure);

    // pretend to greentea that we actally executed two test case
    greentea_case_setup_handler(cases, 0);
    greentea_case_teardown_handler(cases, 1, 0, REASON_NONE);

    greentea_case_setup_handler(cases + 1, 0);
    greentea_case_teardown_handler(cases + 1, 1, 0, REASON_NONE);

    greentea_test_teardown_handler(2, 0, REASON_NONE);
};

Specification specification(failing_setup_handler, cases, failing_teardown_handler, selftest_handlers);

int main()
{
    Harness::run(specification);
}
