
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

#define ASSERT_CONTROL(value, expected_repeat, expected_timeout) { \
    c = value; \
    TEST_ASSERT_EQUAL_MESSAGE(expected_repeat, c.get_repeat(), "Repeat"); \
    TEST_ASSERT_EQUAL_MESSAGE(expected_timeout, c.get_timeout(), "Timeout"); }


using namespace utest::v1;

void test_constructors()
{
    control_t c;
    TEST_ASSERT_EQUAL(REPEAT_UNDECLR, c.get_repeat());
    TEST_ASSERT_EQUAL(TIMEOUT_UNDECLR, c.get_timeout());

    ASSERT_CONTROL(control_t(REPEAT_ALL), REPEAT_ALL, TIMEOUT_UNDECLR);

    ASSERT_CONTROL(control_t(200), REPEAT_UNDECLR, 200);
}

void test_constants()
{
    control_t c;

    ASSERT_CONTROL(CaseNext, REPEAT_NONE, TIMEOUT_NONE);

    ASSERT_CONTROL(CaseNoRepeat, REPEAT_NONE, TIMEOUT_UNDECLR);

    ASSERT_CONTROL(CaseRepeatAll, REPEAT_ALL, TIMEOUT_UNDECLR);

    ASSERT_CONTROL(CaseRepeatHandler, REPEAT_HANDLER, TIMEOUT_UNDECLR);

    ASSERT_CONTROL(CaseAwait, REPEAT_UNDECLR, TIMEOUT_FOREVER);

    ASSERT_CONTROL(CaseTimeout(0),      REPEAT_UNDECLR,      0);
    ASSERT_CONTROL(CaseTimeout(100000), REPEAT_UNDECLR, 100000);

    ASSERT_CONTROL(CaseRepeatAllOnTimeout(0),      REPEAT_ALL_ON_TIMEOUT,      0);
    ASSERT_CONTROL(CaseRepeatAllOnTimeout(100000), REPEAT_ALL_ON_TIMEOUT, 100000);

    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(0),      REPEAT_HANDLER_ON_TIMEOUT,      0);
    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(100000), REPEAT_HANDLER_ON_TIMEOUT, 100000);
}

void test_same_group_combinations()
{
    control_t c;
    // arbitration within same group should not change anything
    ASSERT_CONTROL(CaseNext + CaseNext, REPEAT_NONE, TIMEOUT_NONE);

    ASSERT_CONTROL(CaseNoRepeat + CaseNoRepeat, REPEAT_NONE, TIMEOUT_UNDECLR);

    ASSERT_CONTROL(CaseRepeatAll + CaseRepeatAll, REPEAT_ALL, TIMEOUT_UNDECLR);

    ASSERT_CONTROL(CaseRepeatHandler + CaseRepeatHandler, REPEAT_HANDLER, TIMEOUT_UNDECLR);

    ASSERT_CONTROL(CaseAwait + CaseAwait, REPEAT_UNDECLR, TIMEOUT_FOREVER);

    // same group _and_ same values
    ASSERT_CONTROL(CaseTimeout(     0) + CaseTimeout(     0), REPEAT_UNDECLR,      0);
    ASSERT_CONTROL(CaseTimeout(100000) + CaseTimeout(100000), REPEAT_UNDECLR, 100000);

    ASSERT_CONTROL(CaseRepeatAllOnTimeout(     0) + CaseRepeatAllOnTimeout(     0), REPEAT_ALL_ON_TIMEOUT,      0);
    ASSERT_CONTROL(CaseRepeatAllOnTimeout(100000) + CaseRepeatAllOnTimeout(100000), REPEAT_ALL_ON_TIMEOUT, 100000);

    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(     0) + CaseRepeatHandlerOnTimeout(     0), REPEAT_HANDLER_ON_TIMEOUT,      0);
    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(100000) + CaseRepeatHandlerOnTimeout(100000), REPEAT_HANDLER_ON_TIMEOUT, 100000);

    // same group but _different_ values
    // stricter value should win
    ASSERT_CONTROL(CaseTimeout(100) + CaseTimeout(   12), REPEAT_UNDECLR,  12);
    ASSERT_CONTROL(CaseTimeout(267) + CaseTimeout(36756), REPEAT_UNDECLR, 267);

    ASSERT_CONTROL(CaseRepeatAllOnTimeout(12145) + CaseRepeatAllOnTimeout(   592), REPEAT_ALL_ON_TIMEOUT,  592);
    ASSERT_CONTROL(CaseRepeatAllOnTimeout( 3593) + CaseRepeatAllOnTimeout(294523), REPEAT_ALL_ON_TIMEOUT, 3593);

    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(124001) + CaseRepeatHandlerOnTimeout(49610), REPEAT_HANDLER_ON_TIMEOUT, 49610);
    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(    47) + CaseRepeatHandlerOnTimeout(  209), REPEAT_HANDLER_ON_TIMEOUT,    47);
}

void test_different_group_combinations()
{
    // different group and different value
    control_t c;

    // Matrix with CaseNext
    // CaseNext + CaseNoRepeat => CaseNext
    ASSERT_CONTROL(CaseNext + CaseNoRepeat, REPEAT_NONE, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseNoRepeat + CaseNext, REPEAT_NONE, TIMEOUT_NONE);

    // CaseNext + CaseRepeatAll => CaseNext
    ASSERT_CONTROL(CaseNext + CaseRepeatAll, REPEAT_NONE, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseRepeatAll + CaseNext, REPEAT_NONE, TIMEOUT_NONE);

    // CaseNext + CaseRepeatHandler => CaseNext
    ASSERT_CONTROL(CaseNext + CaseRepeatHandler, REPEAT_NONE, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseRepeatHandler + CaseNext, REPEAT_NONE, TIMEOUT_NONE);

    // CaseNext + CaseNoTimeout => CaseNext
    ASSERT_CONTROL(CaseNext + CaseNoTimeout, REPEAT_NONE, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseNoTimeout + CaseNext, REPEAT_NONE, TIMEOUT_NONE);

    // CaseNext + CaseAwait => CaseNext
    ASSERT_CONTROL(CaseNext + CaseAwait, REPEAT_NONE, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseAwait + CaseNext, REPEAT_NONE, TIMEOUT_NONE);

    // CaseNext + CaseTimeout => CaseNext
    ASSERT_CONTROL(CaseNext + CaseTimeout(42), REPEAT_NONE, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseTimeout(42) + CaseNext, REPEAT_NONE, TIMEOUT_NONE);

    // CaseNext + CaseRepeatAllOnTimeout => CaseNext
    ASSERT_CONTROL(CaseNext + CaseRepeatAllOnTimeout(42), REPEAT_NONE, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseRepeatAllOnTimeout(42) + CaseNext, REPEAT_NONE, TIMEOUT_NONE);

    // CaseNext + CaseRepeatHandlerOnTimeout => CaseNext
    ASSERT_CONTROL(CaseNext + CaseRepeatHandlerOnTimeout(42), REPEAT_NONE, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(42) + CaseNext, REPEAT_NONE, TIMEOUT_NONE);

    // Matrix with CaseNoRepeat
    // CaseNoRepeat + CaseRepeatHandler => CaseNoRepeat
    ASSERT_CONTROL(CaseNoRepeat + CaseRepeatHandler, REPEAT_NONE, TIMEOUT_UNDECLR);
    ASSERT_CONTROL(CaseRepeatHandler + CaseNoRepeat, REPEAT_NONE, TIMEOUT_UNDECLR);

    // CaseNoRepeat + CaseNoTimeout => CaseNext
    ASSERT_CONTROL(CaseNoRepeat + CaseNoTimeout, REPEAT_NONE, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseNoTimeout + CaseNoRepeat, REPEAT_NONE, TIMEOUT_NONE);

    // CaseNoRepeat + CaseAwait => REPEAT_NONE + TIMEOUT_FOREVER
    ASSERT_CONTROL(CaseNoRepeat + CaseAwait, REPEAT_NONE, TIMEOUT_FOREVER);
    ASSERT_CONTROL(CaseAwait + CaseNoRepeat, REPEAT_NONE, TIMEOUT_FOREVER);

    // CaseNoRepeat + CaseTimeout => REPEAT_NONE + timeout
    ASSERT_CONTROL(CaseNoRepeat + CaseTimeout(42), REPEAT_NONE, 42);
    ASSERT_CONTROL(CaseTimeout(42) + CaseNoRepeat, REPEAT_NONE, 42);

    // CaseNoRepeat + CaseRepeatAllOnTimeout => REPEAT_NONE + timeout
    ASSERT_CONTROL(CaseNoRepeat + CaseRepeatAllOnTimeout(42), REPEAT_NONE, 42);
    ASSERT_CONTROL(CaseRepeatAllOnTimeout(42) + CaseNoRepeat, REPEAT_NONE, 42);

    // CaseNoRepeat + CaseRepeatHandlerOnTimeout => REPEAT_NONE + timeout
    ASSERT_CONTROL(CaseNoRepeat + CaseRepeatHandlerOnTimeout(42), REPEAT_NONE, 42);
    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(42) + CaseNoRepeat, REPEAT_NONE, 42);

    // Matrix with CaseRepeatAll
    // CaseRepeatAll + CaseRepeatHandler => CaseRepeatAll
    ASSERT_CONTROL(CaseRepeatAll + CaseRepeatHandler, REPEAT_ALL, TIMEOUT_UNDECLR);
    ASSERT_CONTROL(CaseRepeatHandler + CaseRepeatAll, REPEAT_ALL, TIMEOUT_UNDECLR);

    // CaseRepeatAll + CaseNoTimeout => REPEAT_ALL + TIMEOUT_NONE
    ASSERT_CONTROL(CaseRepeatAll + CaseNoTimeout, REPEAT_ALL, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseNoTimeout + CaseRepeatAll, REPEAT_ALL, TIMEOUT_NONE);

    // CaseRepeatAll + CaseAwait => REPEAT_ALL + TIMEOUT_FOREVER
    ASSERT_CONTROL(CaseRepeatAll + CaseAwait, REPEAT_ALL, TIMEOUT_FOREVER);
    ASSERT_CONTROL(CaseAwait + CaseRepeatAll, REPEAT_ALL, TIMEOUT_FOREVER);

    // CaseRepeatAll + CaseTimeout => REPEAT_ALL + timeout
    ASSERT_CONTROL(CaseRepeatAll + CaseTimeout(42), REPEAT_ALL, 42);
    ASSERT_CONTROL(CaseTimeout(42) + CaseRepeatAll, REPEAT_ALL, 42);

    // CaseRepeatAll + CaseRepeatAllOnTimeout => (REPEAT_ALL | REPEAT_ON_TIMEOUT) + timeout
    ASSERT_CONTROL(CaseRepeatAll + CaseRepeatAllOnTimeout(42), (REPEAT_ALL | REPEAT_ON_TIMEOUT), 42);
    ASSERT_CONTROL(CaseRepeatAllOnTimeout(42) + CaseRepeatAll, (REPEAT_ALL | REPEAT_ON_TIMEOUT), 42);

    // CaseRepeatAll + CaseRepeatHandlerOnTimeout => (REPEAT_ALL | REPEAT_ON_TIMEOUT) + timeout
    ASSERT_CONTROL(CaseRepeatAll + CaseRepeatHandlerOnTimeout(42), REPEAT_ALL | REPEAT_ON_TIMEOUT, 42);
    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(42) + CaseRepeatAll, REPEAT_ALL | REPEAT_ON_TIMEOUT, 42);

    // Matrix with CaseRepeatHandler
    // CaseRepeatHandler + CaseNoTimeout => REPEAT_HANDLER + TIMEOUT_NONE
    ASSERT_CONTROL(CaseRepeatHandler + CaseNoTimeout, REPEAT_HANDLER, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseNoTimeout + CaseRepeatHandler, REPEAT_HANDLER, TIMEOUT_NONE);

    // CaseRepeatHandler + CaseAwait => REPEAT_HANDLER + TIMEOUT_FOREVER
    ASSERT_CONTROL(CaseRepeatHandler + CaseAwait, REPEAT_HANDLER, TIMEOUT_FOREVER);
    ASSERT_CONTROL(CaseAwait + CaseRepeatHandler, REPEAT_HANDLER, TIMEOUT_FOREVER);

    // CaseRepeatHandler + CaseTimeout => REPEAT_HANDLER + timeout
    ASSERT_CONTROL(CaseRepeatHandler + CaseTimeout(42), REPEAT_HANDLER, 42);
    ASSERT_CONTROL(CaseTimeout(42) + CaseRepeatHandler, REPEAT_HANDLER, 42);

    // CaseRepeatHandler + CaseRepeatAllOnTimeout => (REPEAT_ALL | REPEAT_ON_TIMEOUT) + timeout
    ASSERT_CONTROL(CaseRepeatHandler + CaseRepeatAllOnTimeout(42), REPEAT_ALL | REPEAT_ON_TIMEOUT, 42);
    ASSERT_CONTROL(CaseRepeatAllOnTimeout(42) + CaseRepeatHandler, REPEAT_ALL | REPEAT_ON_TIMEOUT, 42);

    // CaseRepeatHandler + CaseRepeatHandlerOnTimeout => (REPEAT_HANDLER | REPEAT_ON_TIMEOUT) + timeout
    ASSERT_CONTROL(CaseRepeatHandler + CaseRepeatHandlerOnTimeout(42), REPEAT_HANDLER | REPEAT_ON_TIMEOUT, 42);
    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(42) + CaseRepeatHandler, REPEAT_HANDLER | REPEAT_ON_TIMEOUT, 42);

    // Matrix with CaseNoTimeout
    // CaseNoTimeout + CaseAwait => CaseNoTimeout
    ASSERT_CONTROL(CaseNoTimeout + CaseAwait, REPEAT_UNDECLR, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseAwait + CaseNoTimeout, REPEAT_UNDECLR, TIMEOUT_NONE);

    // CaseNoTimeout + CaseTimeout => CaseNoTimeout
    ASSERT_CONTROL(CaseNoTimeout + CaseTimeout(42), REPEAT_UNDECLR, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseTimeout(42) + CaseNoTimeout, REPEAT_UNDECLR, TIMEOUT_NONE);

    // CaseNoTimeout + CaseRepeatAllOnTimeout => REPEAT_SETUP_TEARDOWN + TIMEOUT_NONE
    ASSERT_CONTROL(CaseNoTimeout + CaseRepeatAllOnTimeout(42), REPEAT_SETUP_TEARDOWN, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseRepeatAllOnTimeout(42) + CaseNoTimeout, REPEAT_SETUP_TEARDOWN, TIMEOUT_NONE);

    // CaseNoTimeout + CaseRepeatHandlerOnTimeout => REPEAT_CASE_ONLY + TIMEOUT_NONE
    ASSERT_CONTROL(CaseNoTimeout + CaseRepeatHandlerOnTimeout(42), REPEAT_CASE_ONLY, TIMEOUT_NONE);
    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(42) + CaseNoTimeout, REPEAT_CASE_ONLY, TIMEOUT_NONE);

    // Matrix with CaseAwait
    // CaseAwait + CaseTimeout => CaseTimeout
    ASSERT_CONTROL(CaseAwait + CaseTimeout(42), REPEAT_UNDECLR, 42);
    ASSERT_CONTROL(CaseTimeout(42) + CaseAwait, REPEAT_UNDECLR, 42);

    // CaseAwait + CaseRepeatAllOnTimeout => CaseRepeatAllOnTimeout
    ASSERT_CONTROL(CaseAwait + CaseRepeatAllOnTimeout(42), REPEAT_ALL_ON_TIMEOUT, 42);
    ASSERT_CONTROL(CaseRepeatAllOnTimeout(42) + CaseAwait, REPEAT_ALL_ON_TIMEOUT, 42);

    // CaseAwait + CaseRepeatHandlerOnTimeout => CaseRepeatHandlerOnTimeout
    ASSERT_CONTROL(CaseAwait + CaseRepeatHandlerOnTimeout(42), REPEAT_HANDLER_ON_TIMEOUT, 42);
    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(42) + CaseAwait, REPEAT_HANDLER_ON_TIMEOUT, 42);

    // Matrix with CaseTimeout
    // CaseTimeout + CaseRepeatAllOnTimeout => CaseRepeatAllOnTimeout with lower timeout
    ASSERT_CONTROL(CaseTimeout(21) + CaseRepeatAllOnTimeout(42), REPEAT_ALL_ON_TIMEOUT, 21);
    ASSERT_CONTROL(CaseRepeatAllOnTimeout(42) + CaseTimeout(21), REPEAT_ALL_ON_TIMEOUT, 21);

    // CaseAwait + CaseRepeatHandlerOnTimeout => CaseRepeatHandlerOnTimeout with lower timeout
    ASSERT_CONTROL(CaseTimeout(21) + CaseRepeatHandlerOnTimeout(42), REPEAT_HANDLER_ON_TIMEOUT, 21);
    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(42) + CaseTimeout(21), REPEAT_HANDLER_ON_TIMEOUT, 21);

    // Matrix with CaseRepeatAllOnTimeout
    // CaseRepeatAllOnTimeout + CaseRepeatHandlerOnTimeout => CaseRepeatAllOnTimeout with lower timeout
    ASSERT_CONTROL(CaseRepeatAllOnTimeout(21) + CaseRepeatHandlerOnTimeout(42), REPEAT_ALL_ON_TIMEOUT, 21);
    ASSERT_CONTROL(CaseRepeatHandlerOnTimeout(42) + CaseRepeatAllOnTimeout(21), REPEAT_ALL_ON_TIMEOUT, 21);
}

Case cases[] =
{
    Case("Testing constructors", test_constructors),
    Case("Testing constants", test_constants),
    Case("Testing combinations of same group", test_same_group_combinations),
    Case("Testing combinations of different group", test_different_group_combinations)
};

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");

    return greentea_test_setup_handler(number_of_cases);
};
Specification specification(greentea_setup, cases, greentea_continue_handlers);

int main()
{
    // Run the specification only AFTER setting the custom scheduler(if required).
    Harness::run(specification);
}
