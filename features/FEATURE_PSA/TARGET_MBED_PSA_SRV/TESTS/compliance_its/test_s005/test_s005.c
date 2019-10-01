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
#include "test_s005.h"
#ifdef ITS_TEST
#include "test_its_data.h"
#elif PS_TEST
#include "test_ps_data.h"
#endif

#define TEST_BUFF_SIZE 30

client_test_t test_s005_sst_list[] = {
    NULL,
    psa_sst_apis_check_success_case,
    NULL,
};

static uint8_t read_buff[TEST_BUFF_SIZE];
static uint8_t write_buff[TEST_BUFF_SIZE] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x23, 0xF6, 0x07, 0x08, 0x0D, 0x0A, 0x1B, 0x0C, 0x5D, 0x0E,\
  0x70, 0xA1, 0xFF, 0xFF, 0x14, 0x73, 0x46, 0x97, 0xE8, 0xDD, 0xCA, 0x0B, 0x3C, 0x0D, 0x2E};

static int32_t psa_sst_apis_check(psa_sst_uid_t uid, uint32_t data_len,
                                  uint8_t *data_buff, psa_sst_create_flags_t create_flag)
{
    uint32_t status;

    /* Set the UID with the data_len and data_buff */
    status = SST_FUNCTION(s005_data[1].api, uid, data_len, data_buff, create_flag);
    TEST_ASSERT_EQUAL(status, s005_data[1].status, TEST_CHECKPOINT_NUM(1));

    /* Call the get function to get the data buffer and match the buffer */
    status = SST_FUNCTION(s005_data[2].api, uid, 0, data_len, read_buff);
    TEST_ASSERT_EQUAL(status, s005_data[2].status, TEST_CHECKPOINT_NUM(2));
    TEST_ASSERT_MEMCMP(read_buff, data_buff, data_len, TEST_CHECKPOINT_NUM(3));

    /* Call the get_info function and match the attributes */
    status = SST_FUNCTION(s005_data[4].api, uid, &info);
    TEST_ASSERT_EQUAL(status, s005_data[4].status, TEST_CHECKPOINT_NUM(4));
    TEST_ASSERT_EQUAL(info.size, data_len, TEST_CHECKPOINT_NUM(5));
    TEST_ASSERT_EQUAL(info.flags, create_flag, TEST_CHECKPOINT_NUM(6));

    /* Remove the UID  */
    status = SST_FUNCTION(s005_data[7].api, uid);
    TEST_ASSERT_EQUAL(status, s005_data[7].status, TEST_CHECKPOINT_NUM(7));

    return VAL_STATUS_SUCCESS;
}

int32_t psa_sst_apis_check_success_case(caller_security_t caller)
{
   psa_sst_uid_t uid = UID_BASE_VALUE + 4;
   uint32_t data_len = 0, status = VAL_STATUS_SUCCESS;

    /* Calling set function with data_len 1 and valid data pointer */
    val->print(PRINT_TEST, "[Check 1] Set UID with data length zero and call storage APIs\n", 0);
    if (psa_sst_apis_check(uid, data_len, write_buff, 0))
    {
        val->print(PRINT_ERROR, "Data Len = %d\n", data_len);
        return VAL_STATUS_ERROR;
    }

    data_len = TEST_BUFF_SIZE/2;
    val->print(PRINT_TEST, "[Check 2] Resetting the length check\n", 0);
    if (psa_sst_apis_check(uid, data_len, write_buff, 0))
    {
       val->print(PRINT_ERROR, "Data Len = %d\n", data_len);
       return VAL_STATUS_ERROR;
    }

    return status;
}
