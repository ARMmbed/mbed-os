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

#include "utest/harness.h"
#include "minar/minar.h"
#include "core-util/CriticalSectionLock.h"
#include <stdlib.h>

using namespace utest::v1;

namespace
{
    const Case *test_cases = NULL;
    size_t test_length = 0;

    size_t test_index_of_case = 0;

    size_t test_passed = 0;
    size_t test_failed = 0;

    const Case *case_current = NULL;
    control_t case_control = control_t(REPEAT_SETUP_TEARDOWN);
    size_t case_repeat_count = 1;

    minar::callback_handle_t case_timeout_handle = NULL;
    size_t case_validation_count = 0;
    bool case_timeout_occurred = false;

    size_t case_passed = 0;
    size_t case_failed = 0;
    size_t case_failed_before = 0;

    handlers_t defaults = default_handlers;
    handlers_t handlers = defaults;

    location_t location = LOCATION_UNKNOWN;
}

static void die() {
    while(1) ;
}

bool Harness::run(const Specification& specification)
{
    return run(specification, 0);
}

bool Harness::run(const Specification& specification, std::size_t start_case)
{
    // ignore any invalid start index
    if (start_case >= specification.length)
        return false;

    // check if a specification is currently running
    if (is_busy())
        return false;

    test_cases  = specification.cases;
    test_length = specification.length;
    defaults    = specification.defaults;
    handlers.test_setup    = defaults.get_handler(specification.setup_handler);
    handlers.test_teardown = defaults.get_handler(specification.teardown_handler);
    handlers.test_failure  = defaults.get_handler(specification.failure_handler);

    test_index_of_case = 0;
    test_passed = 0;
    test_failed = 0;

    case_passed = 0;
    case_failed = 0;
    case_failed_before = 0;
    case_current = &test_cases[start_case];
    location = LOCATION_TEST_SETUP;

    if (handlers.test_setup && (handlers.test_setup(test_length) != STATUS_CONTINUE)) {
        if (handlers.test_failure) handlers.test_failure(failure_t(REASON_TEST_SETUP, location));
        if (handlers.test_teardown) handlers.test_teardown(0, 0, failure_t(REASON_TEST_SETUP, location));
        test_cases = NULL;
        exit(1);
        return true;
    }

    minar::Scheduler::postCallback(run_next_case);
    return true;
}

void Harness::raise_failure(const failure_reason_t reason)
{
    // ignore a failure, if the Harness has not been initialized.
    // this allows using unity assertion macros without setting up utest.
    if (test_cases == NULL) return;

    status_t fail_status = STATUS_ABORT;
    {
        mbed::util::CriticalSectionLock lock;

        if (handlers.test_failure) handlers.test_failure(failure_t(reason, location));
        if (handlers.case_failure) fail_status = handlers.case_failure(case_current, failure_t(reason, location));
        if (!(fail_status & STATUS_IGNORE)) case_failed++;

        if ((fail_status & STATUS_ABORT) && case_timeout_handle)
        {
            minar::Scheduler::cancelCallback(case_timeout_handle);
            case_timeout_handle = NULL;
        }
    }

    if (fail_status & STATUS_ABORT || reason & REASON_CASE_SETUP) {
        if (handlers.case_teardown && location != LOCATION_CASE_TEARDOWN) {
            location_t fail_loc(location);
            location = LOCATION_CASE_TEARDOWN;
            status_t teardown_status = handlers.case_teardown(case_current, case_passed, case_failed, failure_t(reason, fail_loc));
            if (teardown_status != STATUS_CONTINUE) {
                raise_failure(REASON_CASE_TEARDOWN);
            }
            else handlers.case_teardown = NULL;
        }
    }
    if (fail_status & STATUS_ABORT) {
        test_failed++;
        failure_t fail(reason, location);
        location = LOCATION_TEST_TEARDOWN;
        if (handlers.test_teardown) handlers.test_teardown(test_passed, test_failed, fail);
        exit(test_failed);
        die();
    }
}

void Harness::schedule_next_case()
{
    if (!case_timeout_occurred && case_failed_before == case_failed) {
        case_passed++;
    }

    if (case_control.repeat & REPEAT_SETUP_TEARDOWN || !(case_control.repeat & (REPEAT_ON_TIMEOUT | REPEAT_ON_VALIDATE))) {
        location = LOCATION_CASE_TEARDOWN;
        if (handlers.case_teardown &&
            (handlers.case_teardown(case_current, case_passed, case_failed,
                    case_failed ? failure_t(REASON_CASES, LOCATION_UNKNOWN) : failure_t(REASON_NONE)) != STATUS_CONTINUE)) {
            raise_failure(REASON_CASE_TEARDOWN);
        }
    }

    if (!(case_control.repeat & (REPEAT_ON_TIMEOUT | REPEAT_ON_VALIDATE))) {
        if (case_failed > 0) test_failed++;
        else test_passed++;

        case_control = control_t(REPEAT_SETUP_TEARDOWN);
        case_current++;
        case_passed = 0;
        case_failed = 0;
        case_failed_before = 0;
        case_repeat_count = 1;
        test_index_of_case++;
    }
    minar::Scheduler::postCallback(run_next_case);
}

void Harness::handle_timeout()
{
    {
        mbed::util::CriticalSectionLock lock;

        if (case_timeout_handle != NULL) {
            case_timeout_handle = NULL;
            case_timeout_occurred = true;
        }
    }
    if (case_timeout_occurred) {
        raise_failure(failure_reason_t(REASON_TIMEOUT | ((case_control.repeat & REPEAT_ON_TIMEOUT) ? REASON_IGNORE : 0)));
        minar::Scheduler::postCallback(schedule_next_case);
    }
}

void Harness::validate_callback(const control_t control)
{
    mbed::util::CriticalSectionLock lock;
    case_validation_count++;

    if (case_timeout_handle != NULL || case_control.timeout == TIMEOUT_FOREVER)
    {
        minar::Scheduler::cancelCallback(case_timeout_handle);
        case_timeout_handle = NULL;
        control_t merged_control = case_control + control;
        case_control.repeat = repeat_t(merged_control.repeat & ~REPEAT_ON_TIMEOUT);
        case_control.timeout = TIMEOUT_NONE;
        minar::Scheduler::postCallback(schedule_next_case);
    }
}

bool Harness::is_busy()
{
    mbed::util::CriticalSectionLock lock;
    if (!test_cases)   return false;
    if (!case_current) return false;

    return (case_current < (test_cases + test_length));
}

void Harness::run_next_case()
{
    if(case_current < (test_cases + test_length))
    {
        handlers.case_setup    = defaults.get_handler(case_current->setup_handler);
        handlers.case_teardown = defaults.get_handler(case_current->teardown_handler);
        handlers.case_failure  = defaults.get_handler(case_current->failure_handler);

        if (case_current->is_empty()) {
            location = LOCATION_UNKNOWN;
            raise_failure(REASON_EMPTY_CASE);
            schedule_next_case();
            return;
        }

        repeat_t setup_repeat;
        {
            mbed::util::CriticalSectionLock lock;
            case_validation_count = 0;
            case_timeout_occurred = false;
            setup_repeat = case_control.repeat;
            case_control = control_t();
        }

        if (setup_repeat & REPEAT_SETUP_TEARDOWN) {
            location = LOCATION_CASE_SETUP;
            if (handlers.case_setup && (handlers.case_setup(case_current, test_index_of_case) != STATUS_CONTINUE)) {
                raise_failure(REASON_CASE_SETUP);
                schedule_next_case();
                return;
            }
        }

        case_failed_before = case_failed;
        location = LOCATION_CASE_HANDLER;

        if (case_current->handler) {
            case_current->handler();
        } else if (case_current->control_handler) {
            case_control = case_control + case_current->control_handler();
        } else if (case_current->repeat_count_handler) {
            case_control = case_control + case_current->repeat_count_handler(case_repeat_count);
        }
        case_repeat_count++;

        {
            mbed::util::CriticalSectionLock lock;
            if (case_validation_count) case_control.repeat = repeat_t(case_control.repeat & ~REPEAT_ON_TIMEOUT);

            // if timeout valid
            if (case_control.timeout < TIMEOUT_UNDECLR && case_validation_count == 0) {
                // if await validation _with_ timeout
                if (case_control.timeout < TIMEOUT_FOREVER) {
                    case_timeout_handle = minar::Scheduler::postCallback(handle_timeout)
                                                .delay(minar::milliseconds(case_control.timeout))
                                                .getHandle();
                }
            }
            else {
                minar::Scheduler::postCallback(schedule_next_case);
            }
        }
    }
    else if (handlers.test_teardown) {
        location = LOCATION_TEST_TEARDOWN;
        handlers.test_teardown(test_passed, test_failed, test_failed ? failure_t(REASON_CASES, LOCATION_UNKNOWN) : failure_t(REASON_NONE));
        test_cases = NULL;
        exit(test_failed);
    } else {
        exit(test_failed);
    }
}
