/*
* Copyright (c) 2018 ARM Limited. All rights reserved.
* SPDX-License-Identifier: Apache-2.0
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

using namespace utest::v1;

int skipped_at_right_place = 0;

utest::v1::status_t test_skip_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, skipped_at_right_place);
    greentea_case_teardown_handler(source, passed, failed, failure);
    return STATUS_CONTINUE;
}

// Aborting Teardown Handler ------------------------------------------------------------------------------------------
void unconditional_test_skip_test()
{
    TEST_SKIP();
    // Should not get here
    TEST_ASSERT(0);
}

void conditional_test_skip_test()
{
    skipped_at_right_place = 0;
    TEST_SKIP_UNLESS_MESSAGE(1, "Test skipped at the wrong place!");

    skipped_at_right_place = 1;
    TEST_SKIP_UNLESS_MESSAGE(0, "Test skipped at the right place.");

    // Should not get here
    skipped_at_right_place = 0;
    TEST_ASSERT(0);
}

// Cases --------------------------------------------------------------------------------------------------------------
Case cases[] = {
        Case("Unconditional test skip macro test", unconditional_test_skip_test),
        Case("Conditional test skip macro test",   conditional_test_skip_test, test_skip_case_teardown),
};

// Specification: Setup & Teardown ------------------------------------------------------------------------------------
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");

    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_setup, cases);

int main()
{
    // Run the specification only AFTER setting the custom scheduler(if required).
    Harness::run(specification);
}
