/* Copyright (c) 2017-2018 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef COMPONENT_PSA_SRV_IPC
#error [NOT_SUPPORTED] SPM tests can run only on SPM-enabled targets
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "psa/client.h"
#include "server_tests.h"
#include "psa_manifest/sid.h"

#if defined(TARGET_TFM)
#define PSA_MAX_IOVEC 4
#endif

using namespace utest::v1;

#define TEST_ROT_SRV_MINOR   12
#define OUT_BUFFER_SIZE 60

psa_handle_t control_handle = 0;
char test_str[] = "abcdefghijklmnopqrstuvwxyz";
char cross_part_buf[] = "Hello and welcome SPM";


PSA_TEST_CLIENT(identity_during_connect)
{
    psa_handle_t test_handle = psa_connect(SERVER_TESTS_PART1_TEST, TEST_ROT_SRV_MINOR);
    TEST_ASSERT(test_handle > 0);

    psa_close(test_handle);
}


PSA_TEST_CLIENT(identity_during_call)
{
    psa_status_t status = PSA_SUCCESS;
    psa_handle_t test_handle = psa_connect(SERVER_TESTS_PART1_TEST, TEST_ROT_SRV_MINOR);
    TEST_ASSERT(test_handle > 0);

    status = psa_call(test_handle, NULL, 0, NULL, 0);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    psa_close(test_handle);
}

PSA_TEST_CLIENT(msg_size_assertion)
{
    psa_status_t status = PSA_SUCCESS;
    psa_invec data[PSA_MAX_IOVEC] = {
        {test_str, 4},
        {test_str + 5, 6},
        {test_str + 13, 1},
        {NULL, 0}
    };
    psa_handle_t test_handle = psa_connect(SERVER_TESTS_PART1_TEST, TEST_ROT_SRV_MINOR);
    TEST_ASSERT(test_handle > 0);

    status = psa_call(test_handle, data, PSA_MAX_IOVEC, NULL, 0);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    psa_close(test_handle);
}

PSA_TEST_CLIENT(reject_connection)
{
    psa_handle_t test_handle = psa_connect(SERVER_TESTS_PART1_TEST, TEST_ROT_SRV_MINOR);
    TEST_ASSERT_EQUAL(PSA_CONNECTION_REFUSED, test_handle);
}

PSA_TEST_CLIENT(read_at_outofboud_offset)
{
    psa_status_t status = PSA_SUCCESS;
    psa_invec data = { test_str, sizeof(test_str) };
    psa_handle_t test_handle = psa_connect(SERVER_TESTS_PART1_TEST, TEST_ROT_SRV_MINOR);
    TEST_ASSERT(test_handle > 0);

    status = psa_call(test_handle, &data, 1, NULL, 0);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    psa_close(test_handle);
}

PSA_TEST_CLIENT(msg_read_truncation)
{
    psa_status_t status = PSA_SUCCESS;
    psa_invec data[3] = {
        {test_str, 4},
        {test_str + 5, 6},
        {test_str + 13, 1}
    };
    psa_handle_t test_handle = psa_connect(SERVER_TESTS_PART1_TEST, TEST_ROT_SRV_MINOR);
    TEST_ASSERT(test_handle > 0);

    status = psa_call(test_handle, data, 3, NULL, 0);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    psa_close(test_handle);
}

PSA_TEST_CLIENT(skip_zero)
{
    psa_status_t status = PSA_SUCCESS;
    psa_invec data = { test_str, sizeof(test_str) };
    psa_handle_t test_handle = psa_connect(SERVER_TESTS_PART1_TEST, TEST_ROT_SRV_MINOR);
    TEST_ASSERT(test_handle > 0);

    status = psa_call(test_handle, &data, 1, NULL, 0);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    psa_close(test_handle);
}

PSA_TEST_CLIENT(skip_some)
{
    psa_status_t status = PSA_SUCCESS;
    psa_invec data = { test_str, sizeof(test_str) };
    psa_handle_t test_handle = psa_connect(SERVER_TESTS_PART1_TEST, TEST_ROT_SRV_MINOR);
    TEST_ASSERT(test_handle > 0);

    status = psa_call(test_handle, &data, 1, NULL, 0);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    psa_close(test_handle);
}

PSA_TEST_CLIENT(skip_more_than_left)
{
    psa_status_t status = PSA_SUCCESS;
    psa_invec data = { test_str, 8 };
    psa_handle_t test_handle = psa_connect(SERVER_TESTS_PART1_TEST, TEST_ROT_SRV_MINOR);
    TEST_ASSERT(test_handle > 0);

    status = psa_call(test_handle, &data, 1, NULL, 0);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    psa_close(test_handle);
}

PSA_TEST_CLIENT(rhandle_factorial)
{
    uint32_t secure_value = 0;
    uint32_t value = 1;
    psa_status_t status = PSA_SUCCESS;
    psa_outvec resp = { &secure_value, sizeof(secure_value) };
    psa_handle_t test_handle = psa_connect(SERVER_TESTS_PART1_TEST, TEST_ROT_SRV_MINOR);
    TEST_ASSERT(test_handle > 0);

    for (uint32_t i = 1; i <= 5; i++) {
        value *= i;
        status = psa_call(test_handle, NULL, 0, &resp, 1);
        TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
        TEST_ASSERT_EQUAL(value, secure_value);
    }

    psa_close(test_handle);
}

PSA_TEST_CLIENT(cross_partition_call)
{
    psa_handle_t test_handle = psa_connect(SERVER_TESTS_PART1_TEST, TEST_ROT_SRV_MINOR);
    size_t in_len = strlen(cross_part_buf);
    TEST_ASSERT_MESSAGE(test_handle > 0, "psa_connect() failed");

    psa_invec iovec = { cross_part_buf, in_len };
    uint8_t *response_buf = (uint8_t *)malloc(sizeof(uint8_t) * OUT_BUFFER_SIZE);
    memset(response_buf, 0, OUT_BUFFER_SIZE);
    psa_outvec resp = { response_buf, OUT_BUFFER_SIZE };

    psa_status_t status = psa_call(test_handle, &iovec, 1, &resp, 1);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL_STRING_LEN("MPS emoclew dna olleHMPS emoclew dna olleH", response_buf, in_len * 2);
    free(response_buf);

    psa_close(test_handle);
}

// Test a common DOORBELL scenario
PSA_TEST_CLIENT(doorbell_test)
{
    psa_handle_t test_handle = psa_connect(SERVER_TESTS_PART1_TEST, TEST_ROT_SRV_MINOR);
    TEST_ASSERT_MESSAGE(test_handle > 0, "psa_connect() failed");

    psa_status_t status = psa_call(test_handle, NULL, 0, NULL, 0);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    psa_close(test_handle);
}


utest::v1::status_t spm_setup(const size_t number_of_cases)
{
    control_handle = psa_connect(SERVER_TESTS_PART1_CONTROL, 0);
    if (control_handle < 0) {
        error("Could not open a connection with SERVER_TESTS_PART1_CONTROL ROT_SRV");
    }

    GREENTEA_SETUP(60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

void spm_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    psa_close(control_handle);
    greentea_test_teardown_handler(passed, failed, failure);
}

utest::v1::status_t spm_case_setup(const Case *const source, const size_t index_of_case)
{
    psa_status_t status = PSA_SUCCESS;
    test_action_t action = START_TEST;
    psa_invec data = {&action, sizeof(action)};

    status = psa_call(control_handle, &data, 1, NULL, 0);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t spm_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)
{
    psa_status_t status = PSA_SUCCESS;
    psa_status_t test_status = PSA_SUCCESS;
    test_action_t action = GET_TEST_RESULT;
    psa_invec data = {&action, sizeof(action)};
    psa_outvec resp = {&test_status, sizeof(test_status)};

    status = psa_call(control_handle, &data, 1, &resp, 1);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_status);
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

#define SPM_UTEST_CASE(desc, test) Case(desc, spm_case_setup, PSA_TEST_CLIENT_NAME(test), spm_case_teardown)

Case cases[] = {
    SPM_UTEST_CASE("Get identity during connect", identity_during_connect),
    SPM_UTEST_CASE("Get identity during call", identity_during_call),
    SPM_UTEST_CASE("Assert msg size", msg_size_assertion),
    SPM_UTEST_CASE("Reject on connect", reject_connection),
    SPM_UTEST_CASE("Read at an out of bound offset", read_at_outofboud_offset),
    SPM_UTEST_CASE("Read msg with size bigger than message", msg_read_truncation),
    SPM_UTEST_CASE("Make sure skip with 0 byte number skips nothing", skip_zero),
    SPM_UTEST_CASE("Skip a few bytes while reading a message", skip_some),
    SPM_UTEST_CASE("Try to skip more bytes than left while reading", skip_more_than_left),
    SPM_UTEST_CASE("Test rhandle implementation by calculating the factorial function", rhandle_factorial),
    SPM_UTEST_CASE("Test a call flow between 2 secure partitions", cross_partition_call),
};

//Declare your test specification with a custom setup handler
Specification specification(spm_setup, cases, spm_teardown);

int main(int, char **)
{
    Harness::run(specification);
    return 0;
}

#endif // COMPONENT_PSA_SRV_IPC
