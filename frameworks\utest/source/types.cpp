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

 #include "utest/types.h"

const char* utest::v1::stringify(utest::v1::failure_t failure)
{
    const char *string;
    switch(failure & ~FAILURE_IGNORE)
    {
        case FAILURE_NONE:
            string = "Ignored: No Failure";
            break;
        case FAILURE:
            string = "Ignored: Unspecified Failure";
            break;
        case FAILURE_CASES:
            string = "Ignored: Test Cases Failed";
            break;
        case FAILURE_EMPTY_CASE:
            string = "Ignored: Test Case is Empty";
            break;
        case FAILURE_SETUP:
            string = "Ignored: Setup Failed";
            break;
        case FAILURE_TEARDOWN:
            string = "Ignored: Teardown Failed";
            break;
        case FAILURE_TIMEOUT:
            string = "Ignored: Timed Out";
            break;
        case FAILURE_ASSERTION:
            string = "Ignored: Assertion Failed";
            break;
        default:
            string = "Ignored: Unknown Failure";
            break;
    }
    if (!(failure & FAILURE_IGNORE)) string += 9;
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
