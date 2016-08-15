
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

static int call_counter(0);

// Continue Teardown Handler ------------------------------------------------------------------------------------------
void continue_case()
{
    TEST_ASSERT_EQUAL(0, call_counter++);
}
utest::v1::status_t continue_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(1, call_counter++);
    return greentea_case_teardown_handler(source, passed, failed, failure);
}
utest::v1::status_t continue_failure(const Case *const, const failure_t)
{
    TEST_FAIL_MESSAGE("Failure handler should have never been called!");
    return STATUS_CONTINUE;
}

// Ignoring Teardown Handler ------------------------------------------------------------------------------------------
void ignore_case()
{
    TEST_ASSERT_EQUAL(2, call_counter++);
}
utest::v1::status_t ignore_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(3, call_counter++);
    greentea_case_teardown_handler(source, passed, failed, failure);
    return STATUS_ABORT;
}
utest::v1::status_t ignore_failure(const Case *const source, const failure_t failure)
{
    TEST_ASSERT_EQUAL(4, call_counter++);
    TEST_ASSERT_EQUAL(REASON_CASE_TEARDOWN, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_CASE_TEARDOWN, failure.location);
    verbose_case_failure_handler(source, failure.ignored());
    return STATUS_IGNORE;
}

// Aborting Teardown Handler ------------------------------------------------------------------------------------------
void abort_case()
{
    TEST_ASSERT_EQUAL(5, call_counter++);
}
utest::v1::status_t abort_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(6, call_counter++);
    greentea_case_teardown_handler(source, passed, failed, failure);
    return STATUS_ABORT;
}
utest::v1::status_t abort_failure(const Case *const source, const failure_t failure)
{
    TEST_ASSERT_EQUAL(7, call_counter++);
    TEST_ASSERT_EQUAL(REASON_CASE_TEARDOWN, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_CASE_TEARDOWN, failure.location);
    return verbose_case_failure_handler(source, failure);
}

// Cases --------------------------------------------------------------------------------------------------------------
Case cases[] = {
    Case("Teardown handler returns CONTINUE", continue_case, continue_case_teardown, continue_failure),
    Case("Teardown handler returns ABORT but is IGNORED", ignore_case, ignore_case_teardown, ignore_failure),
    Case("Teardown handler returns ABORT", abort_case, abort_case_teardown, abort_failure)
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
    TEST_ASSERT_EQUAL(2, passed);
    TEST_ASSERT_EQUAL(1, failed);
    TEST_ASSERT_EQUAL(REASON_CASE_TEARDOWN, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_CASE_TEARDOWN, failure.location);

    // pretend to greentea that the test was successful
    greentea_test_teardown_handler(3, 0, REASON_NONE);
}

Specification specification(greentea_setup, cases, greentea_teardown, selftest_handlers);

int main()
{
    // Run the specification only AFTER setting the custom scheduler(if required).
    Harness::run(specification);
}
