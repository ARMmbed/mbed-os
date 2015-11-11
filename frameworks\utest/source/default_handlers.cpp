/****************************************************************************
 * Copyright (c) 2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ****************************************************************************
 */

 #include "mbed-test-async/default_handlers.h"
 #include "mbed-test-async/case.h"

using namespace mbed::test::v0;

const char*
failureToString(failure_t failure)
{
    switch(failure)
    {
        case FAILURE_NONE:
            return "No Failure";
        case FAILURE:
            return "Unspecified Failure";
        case FAILURE_SETUP:
            return "Setup Failed";
        case FAILURE_TEARDOWN:
            return "Teardown Failed";
        case FAILURE_TIMEOUT:
            return "Timed Out";
        case FAILURE_ASSERTION:
            return "Assertion Failed";
    }
    return "Unknown Failure";
}

mbed::test::v0::status_t mbed::test::v0::default_test_set_up_handler(const size_t number_of_cases)
{
    printf("\n>>> Running %u test cases...\n", number_of_cases);
    return STATUS_CONTINUE;
}

void mbed::test::v0::default_test_tear_down_handler(const size_t passed, const size_t failed, const failure_t failure)
{
    printf("\n>>> Test Cases: %u passed, %u failed", passed, failed);
    if (failure == FAILURE_NONE) {
        printf("\n");
    } else  {
        printf(" with reason '%s'\n", failureToString(failure));
    }
    if (failed) printf(">>> TESTS FAILED!\n");
}

mbed::test::v0::status_t mbed::test::v0::default_case_set_up_handler(const mbed::test::v0::Case *const source, const size_t index_of_case)
{
    printf("\n>>> Running #%u: '%s'...\n", index_of_case, source->get_description());
    return STATUS_CONTINUE;
}

mbed::test::v0::status_t mbed::test::v0::default_case_tear_down_handler(const mbed::test::v0::Case *const source, const size_t passed, const size_t failed)
{
    printf(">>> '%s': %u passed, %u failed\n", source->get_description(), passed, failed);
    return STATUS_CONTINUE;
}

status_t mbed::test::v0::default_case_failure_handler(const mbed::test::v0::Case *const source, const mbed::test::v0::failure_t reason)
{
    if (reason == FAILURE_ASSERTION) {
        printf("\n");
    } else {
        printf(">>> '%s' failed with reason '%s'\n", source->get_description(), failureToString(reason));
    }
    return STATUS_CONTINUE;
}
