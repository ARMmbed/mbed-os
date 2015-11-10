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

 #include "mbed-test-async/async_test.h"

using namespace mbed::test::v0;

const char*
statusToString(status_t status)
{
    switch(status)
    {
        case STATUS_SUCCESS:
            return "Success/Continue";
        case STATUS_ABORT:
            return "Abort";

        case STATUS_FAILURE:
            return "Failure";
        case STATUS_FAILURE_TIMEOUT:
            return "Failure (Timed Out)";
        case STATUS_FAILURE_ASSERTION:
            return "Failure (Assertion Failed)";
    }
    return "Unknown";
}

status_t mbed::test::v0::default_failure_handler(const mbed::test::v0::Case *const source, const mbed::test::v0::status_t reason)
{
    if (reason == STATUS_FAILURE_ASSERTION) printf("\n");
    printf(">>> '%s' failed with reason '%s'\n", source->getDescription(), statusToString(reason));
    return STATUS_CONTINUE;
}
