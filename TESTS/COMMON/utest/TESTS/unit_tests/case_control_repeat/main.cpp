
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

// Control: RepeatAll -------------------------------------------------------------------------------------------------
utest::v1::status_t repeat_all_case_setup(const Case *const source, const size_t index_of_case)
{
    static int repeat_counter(0);
    TEST_ASSERT_EQUAL(0, index_of_case);
    TEST_ASSERT_EQUAL(repeat_counter*3, call_counter++);
    repeat_counter++;
    return greentea_case_setup_handler(source, index_of_case);
}
control_t repeat_all_case(const size_t call_count)
{
    static int repeat_counter(1);
    TEST_ASSERT_EQUAL(repeat_counter++, call_count);
    TEST_ASSERT_EQUAL((call_count-1)*3 + 1, call_counter++);
    return (call_count < 10) ? CaseRepeatAll : CaseNoRepeat;
}
utest::v1::status_t repeat_all_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    static int repeat_counter(0);
    TEST_ASSERT_EQUAL(repeat_counter*3 + 2, call_counter++);
    TEST_ASSERT_EQUAL(repeat_counter+1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    repeat_counter++;
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Control: RepeatHandler ---------------------------------------------------------------------------------------------
utest::v1::status_t repeat_handler_case_setup(const Case *const source, const size_t index_of_case)
{
    TEST_ASSERT_EQUAL(1, index_of_case);
    TEST_ASSERT_EQUAL(30, call_counter++);
    return greentea_case_setup_handler(source, index_of_case);
}
control_t repeat_handler_case(const size_t call_count)
{
    static int repeat_counter(1);
    TEST_ASSERT_EQUAL(repeat_counter++, call_count);
    TEST_ASSERT_EQUAL((call_count-1) + 31, call_counter++);
    return (call_count < 10) ? CaseRepeatHandler : CaseNoRepeat;
}
utest::v1::status_t repeat_handler_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(41, call_counter++);
    TEST_ASSERT_EQUAL(10, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Control: NoRepeat --------------------------------------------------------------------------------------------------
control_t no_repeat_handler_case(const size_t call_count)
{
    static int repeat_counter(1);
    TEST_ASSERT_EQUAL(repeat_counter++, call_count);
    TEST_ASSERT_EQUAL(1, call_count);
    TEST_ASSERT_EQUAL(42, call_counter++);
    return CaseNoRepeat;
}

// Control: CaseNext --------------------------------------------------------------------------------------------------
control_t next_handler_case(const size_t call_count)
{
    static int repeat_counter(1);
    TEST_ASSERT_EQUAL(repeat_counter++, call_count);
    TEST_ASSERT_EQUAL(1, call_count);
    TEST_ASSERT_EQUAL(43, call_counter++);
    return CaseNext;
}

Case cases[] = {
    Case("Control: RepeatAll", repeat_all_case_setup, repeat_all_case, repeat_all_case_teardown),
    Case("Control: RepeatHandler", repeat_handler_case_setup, repeat_handler_case, repeat_handler_case_teardown),
    Case("Control: NoRepeat", no_repeat_handler_case),
    Case("Control: CaseNext", next_handler_case)
};

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");

    return greentea_test_setup_handler(number_of_cases);
}
void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(44, call_counter);
    TEST_ASSERT_EQUAL(4, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    greentea_test_teardown_handler(passed, failed, failure);
}


Specification specification(greentea_setup, cases, greentea_teardown, selftest_handlers);

int main()
{
    // Run the specification only AFTER setting the custom scheduler(if required).
    Harness::run(specification);
}
