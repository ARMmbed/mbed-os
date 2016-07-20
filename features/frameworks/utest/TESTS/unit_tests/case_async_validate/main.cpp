
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

#include <stdio.h>

using namespace utest::v1;

static int call_counter(0);

static Timeout utest_to;

// Validate: Simple Validation ----------------------------------------------------------------------------------------
void simple_validation()
{
    TEST_ASSERT_EQUAL(1, call_counter++);
    Harness::validate_callback();
}

control_t simple_validation_case()
{
    TEST_ASSERT_EQUAL(0, call_counter++);
    utest_to.attach_us(simple_validation, 100); // Fire after 100 us

    return CaseAwait;
}

// Validate: Multiple Validation --------------------------------------------------------------------------------------
void multiple_validation()
{
    // make sure validation is side-effect free
    TEST_ASSERT_EQUAL(3, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(4, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(5, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(6, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(7, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(8, call_counter++);

}

control_t multiple_validation_case()
{
    TEST_ASSERT_EQUAL(2, call_counter++);
    utest_to.attach_us(multiple_validation, 100000); // Fire after 100 ms
    return CaseAwait;
}

utest::v1::status_t multiple_validation_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(9, call_counter++);
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Validate: Premature Validation -------------------------------------------------------------------------------------
control_t premature_validation_case()
{
    TEST_ASSERT_EQUAL(10, call_counter++);
    /* Prematurely validate the callback.
     * This can happen, when you set up a callback that occurs in an interrupt
     * and it fires and validates the callback before this function completes.
     * The harness only knows whether to expect a callback after the case Handler
     * completes (obviously), so the callback validations are logged.
     */
    Harness::validate_callback();
    return CaseAwait;
}

utest::v1::status_t premature_validation_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(11, call_counter++);
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Validate: Multiple Premature Validation ----------------------------------------------------------------------------
control_t multiple_premature_validation_case()
{
    TEST_ASSERT_EQUAL(12, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(13, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(14, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(15, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(16, call_counter++);
    return CaseAwait;
}

utest::v1::status_t multiple_premature_validation_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(17, call_counter++);
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Validate: Attributed Validation: Cancel Repeat ---------------------------------------------------------------------
void attributed_validation_cancel_repeat()
{
    TEST_ASSERT_EQUAL(19, call_counter++);
    // cancel all repeats
    Harness::validate_callback(CaseNoRepeat);
}

control_t attributed_validation_cancel_repeat_case()
{
    TEST_ASSERT_EQUAL(18, call_counter++);

    utest_to.attach_us(attributed_validation_cancel_repeat, 100000); // Fire after 100 ms
    // the RepeatAll will be cancelled during callback validation
    return CaseRepeatAll + CaseAwait;
}

utest::v1::status_t attributed_validation_cancel_repeat_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(20, call_counter++);
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Validate: Attributed Validation: Enable Repeat Handler -------------------------------------------------------------
void attributed_validation_enable_repeat()
{
    TEST_ASSERT_EQUAL(22, call_counter++);
    // cancel all repeats
    Harness::validate_callback(CaseRepeatHandler);
    TEST_ASSERT_EQUAL(23, call_counter++);
    // only the first validation counts
    Harness::validate_callback(CaseNoRepeat);
    TEST_ASSERT_EQUAL(24, call_counter++);
}

control_t attributed_validation_enable_repeat_case(const size_t call_count)
{
    if (call_count == 1) {
        TEST_ASSERT_EQUAL(21, call_counter++);
        utest_to.attach_us(attributed_validation_enable_repeat, 100000); // Fire after 100 ms
        // the RepeatAll will be cancelled during callback validation
        return CaseAwait;
    }
    TEST_ASSERT_EQUAL(25, call_counter++);
    return CaseNext;
}

utest::v1::status_t attributed_validation_enable_repeat_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(2, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(26, call_counter++);
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Cases --------------------------------------------------------------------------------------------------------------
Case cases[] = {
    Case("Validate: Simple Validation", simple_validation_case),
    Case("Validate: Multiple Validation", multiple_validation_case, multiple_validation_case_teardown),
    Case("Validate: Premature Validation", premature_validation_case, premature_validation_case_teardown),
    Case("Validate: Multiple Premature Validation", multiple_premature_validation_case, multiple_premature_validation_case_teardown),
    Case("Validate: Attributed Validation: Cancel Repeat", attributed_validation_cancel_repeat_case, attributed_validation_cancel_repeat_case_teardown),
    Case("Validate: Attributed Validation: Enable Repeat Handler", attributed_validation_enable_repeat_case, attributed_validation_enable_repeat_case_teardown)
};

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");

    return greentea_test_setup_handler(number_of_cases);
}

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(27, call_counter++);
    TEST_ASSERT_EQUAL(6, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    greentea_test_teardown_handler(passed, failed, failure);
}

Specification specification(greentea_setup, cases, greentea_teardown, selftest_handlers);
extern void utest_run(const Specification& specification);

int main()
{
    // Run the specification only AFTER setting the custom scheduler.
    Harness::run(specification);
}
