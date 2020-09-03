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
#include "test_s009.h"
#ifdef ITS_TEST
#include "test_its_data.h"
#elif PS_TEST
#include "test_ps_data.h"
#endif

#define TEST_BUFF_SIZE 20

client_test_t test_s009_sst_list[] = {
    NULL,
    psa_sst_zero_length_check,
    NULL,
};

static psa_sst_uid_t uid = UID_BASE_VALUE + 5;
static uint8_t write_buff[TEST_BUFF_SIZE] = {0x99, 0x01, 0x02, 0x03, 0x04, 0x23, 0xF6, 0x07, 0x08, \
                                  0x0D, 0x70, 0xA1, 0xFF, 0xFF, 0x14, 0x73, 0x46, 0x97, 0xE8, 0xDD};

int32_t psa_sst_zero_length_check(caller_security_t caller)
{
    uint32_t status;

    /* Set data for UID with length 0 and NULL pointer */
    val->print(PRINT_TEST, "[Check 1] Call set API with NULL pointer and data length 0\n", 0);
    status = SST_FUNCTION(s009_data[1].api, uid, 0, NULL, 0);
    TEST_ASSERT_EQUAL(status, s009_data[1].status, TEST_CHECKPOINT_NUM(1));

    /* Call the get_info function to verify UID created */
    status = SST_FUNCTION(s009_data[2].api, uid, &info);
    TEST_ASSERT_EQUAL(status, s009_data[2].status, TEST_CHECKPOINT_NUM(2));

    /* Call get API with NULL read buffer */
    val->print(PRINT_TEST, "[Check 2] Call get API with NULL read buffer and data length 0\n", 0);
    status = SST_FUNCTION(s009_data[3].api, uid, 0, 0, NULL);
    TEST_ASSERT_EQUAL(status, s009_data[3].status, TEST_CHECKPOINT_NUM(3));

    /* Remove the UID */
    val->print(PRINT_TEST, "[Check 3] Remove the UID\n", 0);
    status = SST_FUNCTION(s009_data[4].api, uid);
    TEST_ASSERT_EQUAL(status, s009_data[4].status, TEST_CHECKPOINT_NUM(4));

    /* Call the get_info function to verify UID is removed */
    val->print(PRINT_TEST, "[Check 4] Call get_info API to verify UID removed\n", 0);
    status = SST_FUNCTION(s009_data[5].api, uid, &info);
    TEST_ASSERT_EQUAL(status, s009_data[5].status, TEST_CHECKPOINT_NUM(5));

    /* Create UID with length 0 and valid write buffer */
    val->print(PRINT_TEST, "[Check 5] Create UID with zero data_len and valid write buffer\n", 0);
    status = SST_FUNCTION(s009_data[6].api, uid, 0, write_buff, 0);
    TEST_ASSERT_EQUAL(status, s009_data[6].status, TEST_CHECKPOINT_NUM(6));

    /* Call the get_info function and match the attributes */
    status = SST_FUNCTION(s009_data[7].api, uid, &info);
    TEST_ASSERT_EQUAL(status, s009_data[7].status, TEST_CHECKPOINT_NUM(7));
    TEST_ASSERT_EQUAL(info.size, 0, TEST_CHECKPOINT_NUM(8));

    /* Call get API with NULL read buffer and valid UID */
    val->print(PRINT_TEST, "[Check 8] Call get API with NULL read buffer and data length 0\n", 0);
    status = SST_FUNCTION(s009_data[9].api, uid, 0, 0, NULL);
    TEST_ASSERT_EQUAL(status, s009_data[9].status, TEST_CHECKPOINT_NUM(9));

    /* Change the length to test_buff_size */
    val->print(PRINT_TEST, "[Check 9] Increase the length\n", 0);
    status = SST_FUNCTION(s009_data[10].api, uid, TEST_BUFF_SIZE, write_buff, 0);
    TEST_ASSERT_EQUAL(status, s009_data[10].status, TEST_CHECKPOINT_NUM(10));

    /* Remove the UID */
    status = SST_FUNCTION(s009_data[11].api, uid);
    TEST_ASSERT_EQUAL(status, s009_data[11].status, TEST_CHECKPOINT_NUM(11));

    return VAL_STATUS_SUCCESS;
}
