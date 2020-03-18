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
#include "test_s007.h"
#ifdef ITS_TEST
#include "test_its_data.h"
#elif PS_TEST
#include "test_ps_data.h"
#endif

#define TEST_BUFF_SIZE 30

client_test_t test_s007_sst_list[] = {
    NULL,
    psa_sst_get_incorrect_size,
    NULL,
};

static uint8_t write_buff[TEST_BUFF_SIZE] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x23, 0xF6, 0x07, 0x08, 0x0D, 0x0A, 0x1B, 0x0C, 0x5D, 0x0E,\
  0x70, 0xA1, 0xFF, 0xFF, 0x14, 0x73, 0x46, 0x97, 0xE8, 0xDD, 0xCA, 0x0B, 0x3C, 0x0D, 0x2E};
static uint8_t read_buff[TEST_BUFF_SIZE];

int32_t psa_sst_get_incorrect_size(caller_security_t caller)
{
    psa_sst_uid_t uid = UID_BASE_VALUE + 5;
    uint32_t status = VAL_STATUS_SUCCESS;

    /* Set the UID with the data_len and data_buff */
    val->print(PRINT_TEST, "Create a valid Storage\n", 0);
    status = SST_FUNCTION(s007_data[1].api, uid, TEST_BUFF_SIZE/2, write_buff, 0);
    TEST_ASSERT_EQUAL(status, s007_data[1].status, TEST_CHECKPOINT_NUM(1));

    /* Call set for same UID and increase the length */
    val->print(PRINT_TEST, "Increase the length of storage\n", 0);
    status = SST_FUNCTION(s007_data[2].api, uid, TEST_BUFF_SIZE, write_buff, 0);
    TEST_ASSERT_EQUAL(status, s007_data[2].status, TEST_CHECKPOINT_NUM(2));

    /* Access data using get API and old length */
    val->print(PRINT_TEST, "[Check 1] Call get API with old length\n", 0);
    status = SST_FUNCTION(s007_data[3].api, uid, 0, TEST_BUFF_SIZE/2, read_buff);
    TEST_ASSERT_EQUAL(status, s007_data[3].status, TEST_CHECKPOINT_NUM(3));

    /* Access data using get API and valid length */
    status = SST_FUNCTION(s007_data[4].api, uid, 0, TEST_BUFF_SIZE/4, read_buff);
    TEST_ASSERT_EQUAL(status, s007_data[4].status, TEST_CHECKPOINT_NUM(4));
    TEST_ASSERT_MEMCMP(read_buff, write_buff, TEST_BUFF_SIZE/4, TEST_CHECKPOINT_NUM(5));

    /* Decrease the length again */
    val->print(PRINT_TEST, "Decrease the length of storage\n", 0);
    status = SST_FUNCTION(s007_data[6].api, uid, TEST_BUFF_SIZE/4, write_buff, 0);
    TEST_ASSERT_EQUAL(status, s007_data[6].status, TEST_CHECKPOINT_NUM(6));

    /* Access data using get API and old length */
    status = SST_FUNCTION(s007_data[7].api, uid, 0, TEST_BUFF_SIZE/2, read_buff);
    TEST_ASSERT_EQUAL(status, s007_data[7].status, TEST_CHECKPOINT_NUM(7));

    /* Access data using get API and old length */
    val->print(PRINT_TEST, "[Check 2] Call get API with old length\n", 0);
    status = SST_FUNCTION(s007_data[8].api, uid, 0, TEST_BUFF_SIZE, read_buff);
    TEST_ASSERT_EQUAL(status, s007_data[8].status, TEST_CHECKPOINT_NUM(8));

    /* Access data using correct length */
    val->print(PRINT_TEST, "[Check 3] Call get API with valid length\n", 0);
    status = SST_FUNCTION(s007_data[9].api, uid, 0, TEST_BUFF_SIZE/4, read_buff);
    TEST_ASSERT_EQUAL(status, s007_data[9].status, TEST_CHECKPOINT_NUM(9));

    /* Remove the UID  */
    status = SST_FUNCTION(s007_data[10].api, uid);
    TEST_ASSERT_EQUAL(status, s007_data[10].status, TEST_CHECKPOINT_NUM(10));

    return status;
}
