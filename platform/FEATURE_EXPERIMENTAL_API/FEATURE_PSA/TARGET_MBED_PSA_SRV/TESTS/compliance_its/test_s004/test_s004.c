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
#include "test_s004.h"
#ifdef ITS_TEST
#include "test_its_data.h"
#elif PS_TEST
#include "test_ps_data.h"
#endif

#define TEST_BUFF_SIZE 20

client_test_t test_s004_sst_list[] = {
    NULL,
    psa_sst_get_data_check,
    NULL,
};

static psa_sst_uid_t uid = UID_BASE_VALUE + 5;
static uint8_t read_buff[TEST_BUFF_SIZE] = {0};
static uint8_t write_buff[TEST_BUFF_SIZE] = {0x99, 0x01, 0x30, 0x50, 0x04, 0x23, 0xF6, 0x07, 0x08, \
                                  0x0D, 0x70, 0xA1, 0xFF, 0xFF, 0x14, 0x73, 0x46, 0x97, 0xE8, 0xDD};

int32_t psa_sst_get_data_check(caller_security_t caller)
{
    uint32_t status,j;

    /* Set data for UID */
    status = SST_FUNCTION(s004_data[1].api, uid, TEST_BUFF_SIZE, write_buff,0);
    TEST_ASSERT_EQUAL(status, s004_data[1].status, TEST_CHECKPOINT_NUM(1));

    /* Call get function and check the data consistency */
    status = SST_FUNCTION(s004_data[2].api, uid, 0, TEST_BUFF_SIZE, read_buff);
    TEST_ASSERT_EQUAL(status, s004_data[2].status, TEST_CHECKPOINT_NUM(2));
    TEST_ASSERT_MEMCMP(read_buff, write_buff, TEST_BUFF_SIZE, TEST_CHECKPOINT_NUM(3));

    /* Call the set again for same uid and set the length as half */
    status = SST_FUNCTION(s004_data[4].api, uid, TEST_BUFF_SIZE/2, write_buff, 0);
    TEST_ASSERT_EQUAL(status, s004_data[4].status, TEST_CHECKPOINT_NUM(4));

    /* Call get function with incorrect buffer length  */
    val->print(PRINT_TEST, "[Check 1] Call get API with incorrect length\n", 0);
    memset(read_buff, 0, TEST_BUFF_SIZE);
    status = SST_FUNCTION(s004_data[5].api, uid, 0, TEST_BUFF_SIZE, read_buff);
    TEST_ASSERT_EQUAL(status, s004_data[5].status, TEST_CHECKPOINT_NUM(5));
    for (j = 0; j < TEST_BUFF_SIZE; j++)
    {
        TEST_ASSERT_EQUAL(read_buff[j], 0, TEST_CHECKPOINT_NUM(6));
    }

    /* Call get function with CORRECT buffer length  */
    status = SST_FUNCTION(s004_data[7].api, uid, 0, TEST_BUFF_SIZE/2, read_buff);
    TEST_ASSERT_EQUAL(status, s004_data[7].status, TEST_CHECKPOINT_NUM(7));
    TEST_ASSERT_MEMCMP(read_buff, write_buff, TEST_BUFF_SIZE/2, TEST_CHECKPOINT_NUM(8));

    /* Check we should not be able to access old set data */
    val->print(PRINT_TEST, "[Check 2] Old buffer invalid after length change\n", 0);
    for (j = TEST_BUFF_SIZE/2; j < TEST_BUFF_SIZE; j++)
    {
        TEST_ASSERT_EQUAL(read_buff[j], 0, TEST_CHECKPOINT_NUM(9));
    }

    /* Remove the UID */
    status = SST_FUNCTION(s004_data[10].api, uid);
    TEST_ASSERT_EQUAL(status, s004_data[10].status, TEST_CHECKPOINT_NUM(10));

    return VAL_STATUS_SUCCESS;
}
