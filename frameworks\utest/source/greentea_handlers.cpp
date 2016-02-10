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
#include "mbed-drivers/test_env.h"

using namespace utest::v1;

/**

extern const char* TEST_ENV_TESTCASE_COUNT;
extern const char* TEST_ENV_TESTCASE_START;
extern const char* TEST_ENV_TESTCASE_FINISH;

void greentea_send_kv(const char *, const char *);
void greentea_send_kv(const char *, const int);
int greentea_parse_kv(char *, char *, const int, const int);


 */

// --- GREENTEA HANDLERS ---
status_t utest::v1::greentea_test_setup_handler(const size_t number_of_cases)
{
    greentea_send_kv(TEST_ENV_TESTCASE_COUNT, number_of_cases);
    return verbose_test_setup_handler(number_of_cases);
}

void utest::v1::greentea_test_teardown_handler(const size_t passed, const size_t failed, const failure_t failure)
{
    verbose_test_teardown_handler(passed, failed, failure);
    int result = !(failed || (failure.reason && !(failure.reason & REASON_IGNORE)));
    GREENTEA_TESTSUITE_RESULT(result);
}

void utest::v1::greentea_test_failure_handler(const failure_t)
{
    // does nothing here
}

// --- GREENTEA CASE HANDLERS ---
status_t utest::v1::greentea_case_setup_handler(const Case *const source, const size_t index_of_case)
{
    greentea_send_kv(TEST_ENV_TESTCASE_START, source->get_description());
    return verbose_case_setup_handler(source, index_of_case);
}

status_t utest::v1::greentea_case_teardown_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    greentea_send_kv(TEST_ENV_TESTCASE_FINISH, source->get_description(), passed, failed);
    return verbose_case_teardown_handler(source, passed, failed, failure);
}

status_t utest::v1::greentea_case_failure_abort_handler(const Case *const source, const failure_t failure)
{
    status_t status = verbose_case_failure_handler(source, failure);
    return (status & STATUS_IGNORE) ? STATUS_IGNORE : STATUS_ABORT;
}

status_t utest::v1::greentea_case_failure_continue_handler(const Case *const source, const failure_t failure)
{
    return verbose_case_failure_handler(source, failure);
}
