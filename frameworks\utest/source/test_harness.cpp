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
 #include "minar/minar.h"

using namespace mbed::test::v0;

namespace
{
    const Test *specification = NULL;
    size_t length = 0;
    size_t number_of_tests = 0;
    size_t index_of_tests = 0;
    const Case *current = NULL;
    set_up_handler_t set_up = NULL;
    tear_down_handler_t tear_down = NULL;
    int32_t repeats = 0;

    size_t passed = 0;
    size_t failed = 0;
    minar::callback_handle_t timeout_handle = NULL;
}

static void die() {
    while(1) ;
}

mbed::test::v0::status_t mbed::test::v0::default_set_up_handler(const size_t number_of_tests) {
    printf("\n>>> Running %u tests...\n", number_of_tests);
    return STATUS_SUCCESS;
}

void mbed::test::v0::default_tear_down_handler(const size_t _passed, const size_t _failed) {
    printf("\n>>> Tear Down: %u passed, %u failed\n", _passed, _failed);
    if (_failed) printf(">>> TESTS FAILED!\n");
}

void TestHarness::run(const Test *const _specification,
                      const size_t _length,
                      const set_up_handler_t _set_up,
                      const tear_down_handler_t _tear_down)
{
    specification = _specification;
    length = _length;
    set_up = _set_up;
    tear_down = _tear_down;
    number_of_tests = 0;
    index_of_tests = 0;

    for (current = specification; current != (specification + length); current++) {
        number_of_tests += 1 + current->repeats;
    }

    current = specification;
    repeats = current->repeats;

    if (set_up && (set_up(number_of_tests) != STATUS_SUCCESS)) {
        printf(">>> Setup failed!\n");
        if (tear_down) tear_down(passed, failed);
        die();
    }

    minar::Scheduler::postCallback(run_next_test);
}

void TestHarness::handle_failure(status_t reason)
{
    failed++;
    status_t fail_status = current->failure_handler(current, reason);
    if (fail_status == STATUS_CONTINUE) {
        current++;
        minar::Scheduler::postCallback(run_next_test);
        return;
    }
    if (tear_down) tear_down(passed, failed);
    die();
}

void TestHarness::handle_timeout()
{
    if (timeout_handle != NULL)
    {
        handle_failure(STATUS_FAILURE_TIMEOUT);
        timeout_handle = NULL;
    }
}

void TestHarness::validateCallback()
{
    if (timeout_handle != NULL)
    {
        minar::Scheduler::cancelCallback(timeout_handle);
        printf(">>> Validated Callback...\n");
        passed++;
        current++;
        timeout_handle = NULL;
        minar::Scheduler::postCallback(run_next_test);
    }
}

void TestHarness::run_next_test()
{
    if(current != (specification + length))
    {
        Unity.CurrentTestFailed = false;

        TEST_PROTECT();
        if (Unity.CurrentTestFailed) {
            if (timeout_handle) {
                minar::Scheduler::cancelCallback(timeout_handle);
                timeout_handle = NULL;
            }
            handle_failure(STATUS_FAILURE_ASSERTION);
            return;
        }

        printf("\n>>> Running #%u: '%s'...\n", ++index_of_tests, current->description);

        status_t status = current->case_handler();

        if (status == STATUS_EXPECT_ASYNC_CALL) {
            timeout_handle = minar::Scheduler::postCallback(handle_timeout)
                            .delay(minar::milliseconds(current->timeout_ms))
                            .tolerance(0)
                            .getHandle();
        }
        else if (status != STATUS_SUCCESS) {
            handle_failure(status);
            return;
        }
        else {
            passed++;
            if (repeats-- <= 0) {
                current++;
                repeats = current->repeats;
            }
            minar::Scheduler::postCallback(run_next_test);
        }
    }
    else if (tear_down) tear_down(passed, failed);
}
