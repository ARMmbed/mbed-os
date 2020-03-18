/** @file
 * Copyright (c) 2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "val_interfaces.h"
#include "val_target.h"
#include "test_s001.h"
#ifdef ITS_TEST
#include "test_its_data.h"
#elif PS_TEST
#include "test_ps_data.h"
#endif

#define TEST_BUFF_SIZE 16

client_test_t test_s001_sst_list[] = {
    NULL,
    psa_sst_uid_not_found,
    NULL,
};

static uint8_t write_buff[TEST_BUFF_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                             0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
static uint8_t read_buff[TEST_BUFF_SIZE]  = {0};

static int32_t sst_calls_without_set_call(psa_sst_uid_t p_uid)
{
    uint32_t status;

    /* get() without using set() before */
    val->print(PRINT_TEST, "[Check 1] Call get API for UID %d which is not set\n", p_uid);
    status = SST_FUNCTION(s001_data[1].api, p_uid, 0, TEST_BUFF_SIZE, read_buff);
    TEST_ASSERT_EQUAL(status,s001_data[1].status,TEST_CHECKPOINT_NUM(1));

    /*  get_info() without using set() before */
    val->print(PRINT_TEST, "[Check 2] Call get_info API for UID %d which is not set\n", p_uid);
    status = SST_FUNCTION(s001_data[2].api, p_uid, &info);
    TEST_ASSERT_EQUAL(status, s001_data[2].status, TEST_CHECKPOINT_NUM(2));

    /* remove() without using set() before */
    val->print(PRINT_TEST, "[Check 3] Call remove API for UID %d which is not set\n", p_uid);
    status = SST_FUNCTION(s001_data[3].api, p_uid);
    TEST_ASSERT_EQUAL(status, s001_data[3].status, TEST_CHECKPOINT_NUM(3));

    return VAL_STATUS_SUCCESS;
}

static int32_t sst_set_and_remove(psa_sst_uid_t p_uid)
{
    uint32_t status;

    /* set() a UID1 */
    status = SST_FUNCTION(s001_data[4].api, p_uid, TEST_BUFF_SIZE, write_buff, 0);
    TEST_ASSERT_EQUAL(status, s001_data[4].status, TEST_CHECKPOINT_NUM(4));

    /* Also set() with a different UID */
    status = SST_FUNCTION(s001_data[5].api, p_uid + 1, TEST_BUFF_SIZE, write_buff, 0);
    TEST_ASSERT_EQUAL(status, s001_data[5].status, TEST_CHECKPOINT_NUM(5));

    /* remove() UID1 */
    status = SST_FUNCTION(s001_data[6].api, p_uid);
    TEST_ASSERT_EQUAL(status, s001_data[6].status, TEST_CHECKPOINT_NUM(6));

    return VAL_STATUS_SUCCESS;
}

static int32_t sst_calls_after_uid_remove(psa_sst_uid_t p_uid)
{
    uint32_t status;

    /* get() for UID which is removed */
    val->print(PRINT_TEST, "[Check 4] Call get API for UID %d which is removed\n", p_uid);
    status = SST_FUNCTION(s001_data[7].api, p_uid, 0, TEST_BUFF_SIZE, read_buff);
    TEST_ASSERT_EQUAL(status, s001_data[7].status, TEST_CHECKPOINT_NUM(7));

    /* get_info() for UID which is removed */
    val->print(PRINT_TEST, "[Check 5] Call get_info API for UID %d which is removed\n", p_uid);
    status = SST_FUNCTION(s001_data[8].api, p_uid, &info);
    TEST_ASSERT_EQUAL(status, s001_data[8].status, TEST_CHECKPOINT_NUM(8));

    /* remove() for UID which is removed */
    val->print(PRINT_TEST, "[Check 6] Call remove API for UID %d which is removed\n", p_uid);
    status = SST_FUNCTION(s001_data[9].api, p_uid);
    TEST_ASSERT_EQUAL(status, s001_data[9].status, TEST_CHECKPOINT_NUM(9));

    return VAL_STATUS_SUCCESS;
}

static int32_t sst_calls_with_different_uid(psa_sst_uid_t p_uid)
{
    uint32_t status;

    /* set() a UID */
    val->print(PRINT_TEST, "Set storage for UID %d\n", p_uid);
    status = SST_FUNCTION(s001_data[10].api, p_uid, TEST_BUFF_SIZE, write_buff, 0);
    TEST_ASSERT_EQUAL(status, s001_data[10].status, TEST_CHECKPOINT_NUM(10));

    /* get() for different UID then set UID */
    val->print(PRINT_TEST, "[Check 7] Call get API for different UID %d\n", p_uid);
    status = SST_FUNCTION(s001_data[11].api, p_uid-1, 0, TEST_BUFF_SIZE - 1, read_buff);
    TEST_ASSERT_EQUAL(status, s001_data[11].status, TEST_CHECKPOINT_NUM(11));

    /* get_info() for different UID then set UID */
    val->print(PRINT_TEST, "[Check 8] Call get_info API for different UID %d\n", p_uid);
    status = SST_FUNCTION(s001_data[12].api, p_uid-1, &info);
    TEST_ASSERT_EQUAL(status, s001_data[12].status, TEST_CHECKPOINT_NUM(12));

    /* remove() for different UID then set UID */
    val->print(PRINT_TEST, "[Check 9] Call remove API for different UID %d\n", p_uid);
    status = SST_FUNCTION(s001_data[13].api, p_uid-1);
    TEST_ASSERT_EQUAL(status, s001_data[13].status, TEST_CHECKPOINT_NUM(13));

    /* remove() the set UID */
    status = SST_FUNCTION(s001_data[14].api, p_uid);
    TEST_ASSERT_EQUAL(status, s001_data[14].status, TEST_CHECKPOINT_NUM(14));

    return VAL_STATUS_SUCCESS;
}

static int32_t sst_remove_stray_uid(psa_sst_uid_t p_uid)
{
    uint32_t status;

    /* Remove UID + 1 */
    status = SST_FUNCTION(s001_data[15].api, p_uid);
    TEST_ASSERT_EQUAL(status, s001_data[15].status, TEST_CHECKPOINT_NUM(15));

    return VAL_STATUS_SUCCESS;
}

int32_t psa_sst_uid_not_found(caller_security_t caller)
{
    int32_t test_status;
    psa_sst_uid_t uid = UID_BASE_VALUE + 6;

    test_status = sst_calls_without_set_call(uid);
    if (test_status != VAL_STATUS_SUCCESS)
        return test_status;

    test_status = sst_set_and_remove(uid);
    if (test_status != VAL_STATUS_SUCCESS)
        return test_status;

    test_status = sst_calls_after_uid_remove(uid);
    if (test_status != VAL_STATUS_SUCCESS)
        return test_status;

    test_status = sst_calls_with_different_uid(uid);
    if (test_status != VAL_STATUS_SUCCESS)
        return test_status;

    test_status = sst_remove_stray_uid(uid + 1);
    if (test_status != VAL_STATUS_SUCCESS)
        return test_status;

    return VAL_STATUS_SUCCESS;
}
