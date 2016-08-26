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

 #include "utest/utest_types.h"

const char* utest::v1::stringify(utest::v1::failure_reason_t reason)
{
    const char *string;
    switch(reason & ~REASON_IGNORE)
    {
        case REASON_NONE:
            string = "Ignored: No Failure";
            break;
        case REASON_CASES:
            string = "Ignored: Test Cases Failed";
            break;
        case REASON_EMPTY_CASE:
            string = "Ignored: Test Case is Empty";
            break;
        case REASON_TIMEOUT:
            string = "Ignored: Timed Out";
            break;
        case REASON_ASSERTION:
            string = "Ignored: Assertion Failed";
            break;
        case REASON_TEST_SETUP:
            string = "Ignored: Test Setup Failed";
            break;
        case REASON_TEST_TEARDOWN:
            string = "Ignored: Test Teardown Failed";
            break;
        case REASON_CASE_SETUP:
            string = "Ignored: Case Setup Failed";
            break;
        case REASON_CASE_HANDLER:
            string = "Ignored: Case Handler Failed";
            break;
        case REASON_CASE_TEARDOWN:
            string = "Ignored: Case Teardown Failed";
            break;
        case REASON_CASE_INDEX:
            string = "Ignored: Case Index Invalid";
            break;
        case REASON_SCHEDULER:
            string = "Ignored: Scheduling Asynchronous Callback Failed";
            break;
        default:
        case REASON_UNKNOWN:
            string = "Ignored: Unknown Failure";
            break;
    }
    if (!(reason & REASON_IGNORE)) string += 9;
    return string;
}

const char* utest::v1::stringify(utest::v1::failure_t failure)
{
    return stringify(failure.reason);
}

const char* utest::v1::stringify(utest::v1::location_t location)
{
    const char *string;
    switch(location)
    {
        case LOCATION_TEST_SETUP:
            string = "Test Setup Handler";
            break;
        case LOCATION_TEST_TEARDOWN:
            string = "Test Teardown Handler";
            break;
        case LOCATION_CASE_SETUP:
            string = "Case Setup Handler";
            break;
        case LOCATION_CASE_HANDLER:
            string = "Case Handler";
            break;
        case LOCATION_CASE_TEARDOWN:
            string = "Case Teardown Handler";
            break;
        default:
        case LOCATION_UNKNOWN:
            string = "Unknown Location";
            break;
    }
    return string;
}

const char* utest::v1::stringify(utest::v1::status_t status)
{
    switch(status)
    {
        case STATUS_CONTINUE:
            return "Continue";
        case STATUS_IGNORE:
            return "Ignore";
        case STATUS_ABORT:
            return "Abort";
    }
    return "Unknown Status";
}


const utest::v1::base_control_t utest::v1::CaseNext = { REPEAT_NONE, TIMEOUT_NONE };

const utest::v1::base_control_t utest::v1::CaseNoRepeat = { REPEAT_NONE, TIMEOUT_UNDECLR };

const utest::v1::base_control_t utest::v1::CaseRepeatAll = { REPEAT_ALL, TIMEOUT_UNDECLR };

const utest::v1::base_control_t utest::v1::CaseRepeatHandler = { REPEAT_HANDLER, TIMEOUT_UNDECLR };

const utest::v1::base_control_t utest::v1::CaseNoTimeout = { REPEAT_UNDECLR, TIMEOUT_NONE };

const utest::v1::base_control_t utest::v1::CaseAwait = { REPEAT_UNDECLR, TIMEOUT_FOREVER };

// equal to CaeReapeatAll
const utest::v1::base_control_t utest::v1::CaseRepeat = { REPEAT_ALL, TIMEOUT_UNDECLR };

// equal to CaseRepeatHandler
const utest::v1::base_control_t utest::v1::CaseRepeatHandlerOnly = { REPEAT_HANDLER, TIMEOUT_UNDECLR };
