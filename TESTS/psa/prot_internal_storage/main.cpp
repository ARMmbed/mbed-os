/*
* Copyright (c) 2018 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef TARGET_PSA
#error [NOT_SUPPORTED] ITS tests can run only on PSA-enabled targets.
#endif // TARGET_PSA

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "psa_prot_internal_storage.h"
#include "test_pits.h"

using namespace utest::v1;

#define TEST_BUFF_SIZE 16

static void pits_test()
{
    psa_its_status_t status = PSA_ITS_SUCCESS;
    uint8_t write_buff[TEST_BUFF_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    uint8_t read_buff[TEST_BUFF_SIZE] = {0};
    struct psa_its_info_t info = {0, PSA_ITS_WRITE_ONCE_FLAG};
    memset(read_buff, 0, TEST_BUFF_SIZE);

    status = test_psa_its_reset();
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);

    status = psa_its_get_info(5, &info);
    TEST_ASSERT_EQUAL(PSA_ITS_ERROR_KEY_NOT_FOUND, status);

    status = psa_its_set(5, TEST_BUFF_SIZE, write_buff, 0);
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);

    status = psa_its_get_info(5, &info);
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);
    TEST_ASSERT_EQUAL(TEST_BUFF_SIZE, info.size);
    TEST_ASSERT_EQUAL(0, info.flags);

    status = psa_its_get(5, 0, TEST_BUFF_SIZE, read_buff);
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);
    TEST_ASSERT_EQUAL_MEMORY(write_buff, read_buff, TEST_BUFF_SIZE);

    memset(read_buff, 0, TEST_BUFF_SIZE);
    status = psa_its_get(5, 1, TEST_BUFF_SIZE, read_buff);
    TEST_ASSERT_NOT_EQUAL(PSA_ITS_SUCCESS, status);

    status = psa_its_get(5, 1, TEST_BUFF_SIZE - 1, read_buff);
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);
    TEST_ASSERT_EQUAL_MEMORY(write_buff + 1, read_buff, TEST_BUFF_SIZE - 1);

    status = psa_its_remove(5);
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);

    status = psa_its_get_info(5, &info);
    TEST_ASSERT_EQUAL(PSA_ITS_ERROR_KEY_NOT_FOUND, status);
}

static void pits_write_once_test()
{
    psa_its_status_t status = PSA_ITS_SUCCESS;
    uint8_t write_buff[TEST_BUFF_SIZE] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
    uint8_t read_buff[TEST_BUFF_SIZE] = {0};
    struct psa_its_info_t info = {0, 0};

    status = test_psa_its_reset();
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);

    status = psa_its_get_info(5, &info);
    TEST_ASSERT_EQUAL(PSA_ITS_ERROR_KEY_NOT_FOUND, status);

    status = psa_its_set(5, TEST_BUFF_SIZE, write_buff, PSA_ITS_WRITE_ONCE_FLAG);
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);

    info.size = 0;
    info.flags = 0;
    status = psa_its_get_info(5, &info);
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);
    TEST_ASSERT_EQUAL(TEST_BUFF_SIZE, info.size);
    TEST_ASSERT_EQUAL(PSA_ITS_WRITE_ONCE_FLAG, info.flags);

    status = psa_its_get(5, 0, TEST_BUFF_SIZE, read_buff);
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);
    TEST_ASSERT_EQUAL_MEMORY(write_buff, read_buff, TEST_BUFF_SIZE);

    status = psa_its_set(5, TEST_BUFF_SIZE, write_buff, PSA_ITS_WRITE_ONCE_FLAG);
    TEST_ASSERT_NOT_EQUAL(PSA_ITS_SUCCESS, status);

    status = psa_its_set(5, TEST_BUFF_SIZE, write_buff, 0);
    TEST_ASSERT_NOT_EQUAL(PSA_ITS_SUCCESS, status);

    status = psa_its_remove(5);
    TEST_ASSERT_NOT_EQUAL(PSA_ITS_SUCCESS, status);

    info.size = 0;
    info.flags = 0;
    status = psa_its_get_info(5, &info);
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);
    TEST_ASSERT_EQUAL(TEST_BUFF_SIZE, info.size);
    TEST_ASSERT_EQUAL(PSA_ITS_WRITE_ONCE_FLAG, info.flags);

    status = test_psa_its_reset();
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);
}

Case cases[] = {
    Case("PSA prot internal storage - Basic",  pits_test),
    Case("PSA prot internal storage - Write-once",  pits_write_once_test),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
#ifndef NO_GREENTEA
    GREENTEA_SETUP(60, "default_auto");
#endif
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    return !Harness::run(specification);
}
