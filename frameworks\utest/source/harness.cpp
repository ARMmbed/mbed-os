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
#include <stdlib.h>
#include "core-util/CriticalSectionLock.h"


#ifndef YOTTA_CFG_UTEST_USE_CUSTOM_SCHEDULER
#include "minar/minar.h"

static void *utest_minar_post(const utest_v1_harness_callback_t callback, const uint32_t delay_ms)
{
    void *handle = minar::Scheduler::postCallback(callback).delay(minar::milliseconds(delay_ms)).getHandle();
    return handle;
}
static int32_t utest_minar_cancel(void *handle)
{
    int32_t ret = minar::Scheduler::cancelCallback(handle);
    return ret;
}
static const utest_v1_scheduler_t utest_minar_scheduler =
{
    utest_minar_post,
    utest_minar_cancel
};
#endif

using namespace utest::v1;

namespace
{
    const Case *test_cases = NULL;
    size_t test_length = 0;

    size_t test_index_of_case = 0;

    size_t test_passed = 0;
    size_t test_failed = 0;

    const Case *case_current = NULL;
    size_t case_index = 0;
    control_t case_control = control_t(REPEAT_SETUP_TEARDOWN);
    size_t case_repeat_count = 1;

    void *case_timeout_handle = NULL;
    size_t case_validation_count = 0;
    bool case_timeout_occurred = false;

    size_t case_passed = 0;
    size_t case_failed = 0;
    size_t case_failed_before = 0;

    handlers_t defaults = default_handlers;
    handlers_t handlers = defaults;

    location_t location = LOCATION_UNKNOWN;

#ifndef YOTTA_CFG_UTEST_USE_CUSTOM_SCHEDULER
    utest_v1_scheduler_t scheduler = utest_minar_scheduler;
#else
    utest_v1_scheduler_t scheduler = {NULL, NULL};
#endif
}

static void die() {
    while(1) ;
}

bool Harness::set_scheduler(const utest_v1_scheduler_t scheduler)
{
    if (!scheduler.init || !scheduler.post || !scheduler.cancel || !scheduler.run)
        return false;

    ::scheduler = scheduler;
    return true;
}

bool Harness::run(const Specification& specification, size_t)
{
    return run(specification);
}

bool Harness::run(const Specification& specification)
{
    // check if a specification is currently running
    if (is_busy())
        return false;

    if (!scheduler.init || !scheduler.post || !scheduler.cancel || !scheduler.run)
        return false;

    if (scheduler.init() != 0)
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

    location = LOCATION_TEST_SETUP;
    int setup_status = 0;
    failure_t failure(REASON_NONE, location);

    if (handlers.test_setup) {
        setup_status = handlers.test_setup(test_length);
        if (setup_status == STATUS_CONTINUE) setup_status = 0;
        else if (setup_status < STATUS_CONTINUE)     failure.reason = REASON_TEST_SETUP;
        else if (setup_status > signed(test_length)) failure.reason = REASON_CASE_INDEX;
    }

    if (failure.reason != REASON_NONE) {
        if (handlers.test_failure) handlers.test_failure(failure);
        if (handlers.test_teardown) handlers.test_teardown(0, 0, failure);
        test_cases = NULL;
        exit(1);
        return true;
    }

    case_index = setup_status;
    case_current = &test_cases[case_index];

    scheduler.post(run_next_case, 0);
    if (scheduler.run() != 0) {
        failure.reason = REASON_SCHEDULER;
        if (handlers.test_failure) handlers.test_failure(failure);
        if (handlers.test_teardown) handlers.test_teardown(0, 0, failure);
        test_cases = NULL;
        exit(1);
        return true;
    }
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
        if (fail_status != STATUS_IGNORE) case_failed++;

        if ((fail_status == STATUS_ABORT) && case_timeout_handle)
        {
            scheduler.cancel(case_timeout_handle);
            case_timeout_handle = NULL;
        }
    }

    if (fail_status == STATUS_ABORT || reason & REASON_CASE_SETUP) {
        if (handlers.case_teardown && location != LOCATION_CASE_TEARDOWN) {
            location_t fail_loc(location);
            location = LOCATION_CASE_TEARDOWN;

            status_t teardown_status = handlers.case_teardown(case_current, case_passed, case_failed, failure_t(reason, fail_loc));
            if (teardown_status < STATUS_CONTINUE) raise_failure(REASON_CASE_TEARDOWN);
            else if (teardown_status > signed(test_length)) raise_failure(REASON_CASE_INDEX);
            else if (teardown_status >= 0) case_index = teardown_status - 1;

            handlers.case_teardown = NULL;
        }
    }
    if (fail_status == STATUS_ABORT) {
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

        if (handlers.case_teardown) {
            status_t status = handlers.case_teardown(case_current, case_passed, case_failed,
                                                     case_failed ? failure_t(REASON_CASES, LOCATION_UNKNOWN) : failure_t(REASON_NONE));
            if (status < STATUS_CONTINUE)          raise_failure(REASON_CASE_TEARDOWN);
            else if (status > signed(test_length)) raise_failure(REASON_CASE_INDEX);
            else if (status >= 0) case_index = status - 1;
        }
    }

    if (!(case_control.repeat & (REPEAT_ON_TIMEOUT | REPEAT_ON_VALIDATE))) {
        if (case_failed > 0) test_failed++;
        else test_passed++;

        case_control = control_t(REPEAT_SETUP_TEARDOWN);
        case_index++;
        case_current = &test_cases[case_index];
        case_passed = 0;
        case_failed = 0;
        case_failed_before = 0;
        case_repeat_count = 1;
        test_index_of_case++;
    }
    scheduler.post(run_next_case, 0);
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
        scheduler.post(schedule_next_case, 0);
    }
}

void Harness::validate_callback(const control_t control)
{
    mbed::util::CriticalSectionLock lock;
    case_validation_count++;

    if (case_timeout_handle != NULL || case_control.timeout == TIMEOUT_FOREVER)
    {
        scheduler.cancel(case_timeout_handle);
        case_timeout_handle = NULL;
        control_t merged_control = case_control + control;
        case_control.repeat = repeat_t(merged_control.repeat & ~REPEAT_ON_TIMEOUT);
        case_control.timeout = TIMEOUT_NONE;
        scheduler.post(schedule_next_case, 0);
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
                    case_timeout_handle = scheduler.post(handle_timeout, case_control.timeout);
                    if (case_timeout_handle == NULL) {
                        raise_failure(REASON_SCHEDULER);
                        schedule_next_case();
                    }
                }
            }
            else {
                scheduler.post(schedule_next_case, 0);
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
