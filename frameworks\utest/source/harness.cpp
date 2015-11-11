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

 #include "mbed-test-async/harness.h"
 #include "minar/minar.h"

using namespace mbed::test::v0;


namespace
{
    const Test *test_specification = NULL;
    size_t test_length = 0;

    size_t test_index_of_case = 0;

    size_t test_passed = 0;
    size_t test_failed = 0;

    const Case *case_current = NULL;
    control_flow_t case_control_flow = CONTROL_FLOW_NEXT;

    minar::callback_handle_t case_timeout_handle = NULL;

    size_t case_passed = 0;
    size_t case_failed = 0;
    size_t case_failed_before = 0;

    handlers_t defaults = verbose_handlers;
    handlers_t handlers = defaults;
}

static void die() {
    while(1) ;
}

void Harness::set_default_handlers(const handlers_t defaults)
{
    ::defaults = defaults;
}

void Harness::run(const Test *const specification,
                      const size_t length,
                      const test_set_up_handler_t set_up_handler,
                      const test_tear_down_handler_t tear_down_handler)
{
    test_specification = specification;
    test_length = length;
    handlers.test_set_up = defaults.get_handler(set_up_handler);
    handlers.test_tear_down = defaults.get_handler(tear_down_handler);

    test_index_of_case = 0;
    test_passed = 0;
    test_failed = 0;

    case_passed = 0;
    case_failed = 0;
    case_failed_before = 0;
    case_current = specification;

    if (handlers.test_set_up && (handlers.test_set_up(test_length) != STATUS_CONTINUE)) {
        if (handlers.test_tear_down) handlers.test_tear_down(0, 0, FAILURE_SETUP);
        die();
    }

    minar::Scheduler::postCallback(run_next_case);
}

void Harness::raise_failure(failure_t reason)
{
    case_failed++;
    if (!handlers.case_failure) return;

    status_t fail_status = handlers.case_failure(case_current, reason);
    if (fail_status != STATUS_CONTINUE) {
        if (handlers.case_tear_down) handlers.case_tear_down(case_current, case_passed, case_failed);
        test_failed++;
        if (handlers.test_tear_down) handlers.test_tear_down(test_passed, test_failed, reason);
        die();
    }
}

void Harness::schedule_next_case()
{
    if (case_failed_before == case_failed) case_passed++;

    if(case_control_flow == CONTROL_FLOW_NEXT) {
        if (handlers.case_tear_down && (handlers.case_tear_down(case_current, case_passed, case_failed) != STATUS_CONTINUE)) {
            raise_failure(FAILURE_TEARDOWN);
        }

        if (case_failed > 0) test_failed++;
        else test_passed++;

        case_current++;
        case_passed = 0;
        case_failed = 0;
        case_failed_before = 0;
    }
    minar::Scheduler::postCallback(run_next_case);
}

void Harness::handle_timeout()
{
    if (case_timeout_handle != NULL)
    {
        raise_failure(FAILURE_TIMEOUT);
        case_timeout_handle = NULL;
        schedule_next_case();
    }
}

void Harness::validate_callback()
{
    if (case_timeout_handle != NULL)
    {
        minar::Scheduler::cancelCallback(case_timeout_handle);
        case_timeout_handle = NULL;
        schedule_next_case();
    }
}

void Harness::run_next_case()
{
    if(case_current != (test_specification + test_length))
    {
        handlers.case_set_up    = defaults.get_handler(case_current->set_up_handler);
        handlers.case_tear_down = defaults.get_handler(case_current->tear_down_handler);
        handlers.case_failure   = defaults.get_handler(case_current->failure_handler);

        if (!case_failed && !case_passed) {
            if (handlers.case_set_up && (handlers.case_set_up(case_current, test_index_of_case) != STATUS_CONTINUE)) {
                raise_failure(FAILURE_SETUP);
            }
            test_index_of_case++;
        }

        case_failed_before = case_failed;

        if (case_current->handler) {
            case_control_flow = CONTROL_FLOW_NEXT;
            case_current->handler();
        } else if (case_current->control_flow_handler) {
            case_control_flow = case_current->control_flow_handler();
        }

        if (case_current->timeout_ms >= 0) {
            case_timeout_handle = minar::Scheduler::postCallback(handle_timeout)
                                            .delay(minar::milliseconds(case_current->timeout_ms))
                                            .tolerance(0)
                                            .getHandle();
        }
        else {
            schedule_next_case();
        }
    }
    else if (handlers.test_tear_down) {
        handlers.test_tear_down(test_passed, test_failed, FAILURE_NONE);
    }
}
