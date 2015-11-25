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
    switch(failure)
    {
        case FAILURE_NONE:
            return "No Failure";
        case FAILURE:
            return "Unspecified Failure";
        case FAILURE_CASES:
            return "Test Cases Failed";
        case FAILURE_EMPTY_CASE:
            return "Test Case is Empty";
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
