
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
static bool never_call = false;

void never_call_case()
{
      never_call = true;
}

utest::v1::status_t abort_case_setup(const Case *const source, const size_t index_of_case)
{
    call_counter++;
    TEST_ASSERT_EQUAL(0, index_of_case);
    greentea_case_setup_handler(source, index_of_case);
    return STATUS_ABORT;
}

utest::v1::status_t abort_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_FALSE_MESSAGE(never_call, "Case handler should never have been called!");
    TEST_ASSERT_EQUAL(1, call_counter);
    TEST_ASSERT_EQUAL(0, passed);
    TEST_ASSERT_EQUAL(1, failed);
    TEST_ASSERT_EQUAL(REASON_CASE_SETUP, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_CASE_SETUP, failure.location);
    call_counter++;
    return greentea_case_teardown_handler(source, 1, 0, failure);
}

utest::v1::status_t ignore_case_setup(const Case *const source, const size_t index_of_case)
{
    TEST_ASSERT_EQUAL(2, call_counter);
    TEST_ASSERT_EQUAL(1, index_of_case);
    greentea_case_setup_handler(source, index_of_case);
    call_counter++;
    return STATUS_IGNORE;   // this is the same
}

utest::v1::status_t ignore_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(3, call_counter);
    TEST_ASSERT_EQUAL(0, passed);
    TEST_ASSERT_EQUAL(1, failed);
    TEST_ASSERT_EQUAL(REASON_CASE_SETUP, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_CASE_SETUP, failure.location);
    call_counter++;
    return greentea_case_teardown_handler(source, 1, 0, failure);
}

utest::v1::status_t continue_case_setup(const Case *const source, const size_t index_of_case)
{
    TEST_ASSERT_EQUAL(4, call_counter);
    TEST_ASSERT_EQUAL(2, index_of_case);
    greentea_case_setup_handler(source, index_of_case);
    call_counter++;
    return STATUS_CONTINUE;   // this is the same
}

void continue_case_handler()
{
    TEST_ASSERT_EQUAL(5, call_counter);
    call_counter++;
}

utest::v1::status_t continue_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(6, call_counter);
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    call_counter++;
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

Case cases[] = {
    Case("Setup handler returns ABORT", abort_case_setup, never_call_case, abort_case_teardown),
    Case("Setup handler returns IGNORE", ignore_case_setup, never_call_case, ignore_case_teardown),
    Case("Setup handler returns CONTINUE", continue_case_setup, continue_case_handler, continue_case_teardown)
};

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");

    return greentea_test_setup_handler(number_of_cases);
}

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(7, call_counter);
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(2, failed);
    TEST_ASSERT_EQUAL(REASON_CASES, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_UNKNOWN, failure.location);

    // pretend to greentea that the test was successful
    greentea_test_teardown_handler(3, 0, REASON_NONE);
}

Specification specification(greentea_setup, cases, greentea_teardown, selftest_handlers);

int main()
{
    // Run the specification only AFTER setting the custom scheduler(if required).
    Harness::run(specification);
}
