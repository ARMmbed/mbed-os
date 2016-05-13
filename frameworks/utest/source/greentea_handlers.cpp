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

#include "utest/default_handlers.h"
#include "utest/case.h"
#include "greentea-client/test_env.h"
#include "utest/stack_trace.h"

using namespace utest::v1;

static status_t unknown_test_setup_handler(const size_t);
static void selftest_failure_handler(const failure_t);
static void test_failure_handler(const failure_t);


const handlers_t utest::v1::greentea_abort_handlers = {
    unknown_test_setup_handler,
    greentea_test_teardown_handler,
    test_failure_handler,
    greentea_case_setup_handler,
    greentea_case_teardown_handler,
    greentea_case_failure_abort_handler
};

const handlers_t utest::v1::greentea_continue_handlers = {
    unknown_test_setup_handler,
    greentea_test_teardown_handler,
    test_failure_handler,
    greentea_case_setup_handler,
    greentea_case_teardown_handler,
    greentea_case_failure_continue_handler
};

const handlers_t utest::v1::selftest_handlers = {
    unknown_test_setup_handler,
    greentea_test_teardown_handler,
    selftest_failure_handler,
    greentea_case_setup_handler,
    greentea_case_teardown_handler,
    greentea_case_failure_continue_handler
};


// --- SPECIAL HANDLERS ---
static status_t unknown_test_setup_handler(const size_t) {
    UTEST_LOG_FUNCTION
    printf(">>> I do not know how to tell greentea that the test started, since\n");
    printf(">>> you forgot to override the `test_setup_handler` in your specification.\n");

    return STATUS_ABORT;
}

static void selftest_failure_handler(const failure_t failure) {
    UTEST_LOG_FUNCTION
    if (failure.location == LOCATION_TEST_SETUP || failure.location == LOCATION_TEST_TEARDOWN || failure.reason == REASON_ASSERTION) {
        verbose_test_failure_handler(failure);
    }
    if (failure.reason == REASON_ASSERTION) {
        UTEST_DUMP_TRACE
        GREENTEA_TESTSUITE_RESULT(false);
        while(1) ;
    }
}

static void test_failure_handler(const failure_t failure) {
    UTEST_LOG_FUNCTION
    if (failure.location == LOCATION_TEST_SETUP || failure.location == LOCATION_TEST_TEARDOWN) {
        verbose_test_failure_handler(failure);
        GREENTEA_TESTSUITE_RESULT(false);
        while(1) ;
    }
}

// --- GREENTEA HANDLERS ---
status_t utest::v1::greentea_test_setup_handler(const size_t number_of_cases)
{
    UTEST_LOG_FUNCTION
    greentea_send_kv(TEST_ENV_TESTCASE_COUNT, number_of_cases);
    return verbose_test_setup_handler(number_of_cases);
}

void utest::v1::greentea_test_teardown_handler(const size_t passed, const size_t failed, const failure_t failure)
{
    UTEST_LOG_FUNCTION
    verbose_test_teardown_handler(passed, failed, failure);
    greentea_send_kv(TEST_ENV_TESTCASE_SUMMARY, passed, failed);
    int result = !(failed || (failure.reason && !(failure.reason & REASON_IGNORE)));
    GREENTEA_TESTSUITE_RESULT(result);
}

void utest::v1::greentea_test_failure_handler(const failure_t failure)
{
    UTEST_LOG_FUNCTION
    verbose_test_failure_handler(failure);
}

// --- GREENTEA CASE HANDLERS ---
status_t utest::v1::greentea_case_setup_handler(const Case *const source, const size_t index_of_case)
{
    UTEST_LOG_FUNCTION
    status_t status = verbose_case_setup_handler(source, index_of_case);
    greentea_send_kv(TEST_ENV_TESTCASE_START, source->get_description());
    return status;
}

status_t utest::v1::greentea_case_teardown_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    UTEST_LOG_FUNCTION
    greentea_send_kv(TEST_ENV_TESTCASE_FINISH, source->get_description(), passed, failed);
    return verbose_case_teardown_handler(source, passed, failed, failure);
}

status_t utest::v1::greentea_case_failure_abort_handler(const Case *const source, const failure_t failure)
{
    UTEST_LOG_FUNCTION
    status_t status = verbose_case_failure_handler(source, failure);
    return (status == STATUS_IGNORE) ? STATUS_IGNORE : STATUS_ABORT;
}

status_t utest::v1::greentea_case_failure_continue_handler(const Case *const source, const failure_t failure)
{
    UTEST_LOG_FUNCTION
    return verbose_case_failure_handler(source, failure);
}
