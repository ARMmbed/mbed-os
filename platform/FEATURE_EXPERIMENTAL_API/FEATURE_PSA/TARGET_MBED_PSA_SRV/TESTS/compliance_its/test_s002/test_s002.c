/** @file
 * Copyright (c) 2019, Arm Limited or sst affiliates. All rights reserved.
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
#include "test_s002.h"
#ifdef ITS_TEST
#include "test_its_data.h"
#elif PS_TEST
#include "test_ps_data.h"
#endif

#define UID_WRITE_ONCE_1 UID_BASE_VALUE + 1
#define UID_WRITE_ONCE_2 UID_BASE_VALUE + 2
#define TEST_BUFF_SIZE 16

client_test_t test_s002_sst_list[] = {
    NULL,
    psa_sst_update_write_once_flag_after_create,
    psa_sst_create_with_write_once_flag,
    NULL,
};

int32_t psa_sst_update_write_once_flag_after_create(caller_security_t caller)
{
    uint32_t status;
    psa_sst_uid_t uid = UID_WRITE_ONCE_1;
    uint8_t  write_buff[TEST_BUFF_SIZE/2]     = {0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE, 0xBA, 0xBE};
    uint8_t  read_buff[TEST_BUFF_SIZE/2]      = {0};
    uint8_t  write_buff_new[TEST_BUFF_SIZE/4] = {0xFF, 0xFF, 0xFF, 0xFF};

    /* set() data without a WRITE_ONCE flag */
    status = SST_FUNCTION(s002_data[1].api, uid, TEST_BUFF_SIZE/2, write_buff, 0);
    TEST_ASSERT_EQUAL(status, s002_data[1].status, TEST_CHECKPOINT_NUM(1));

    /* Check that get_info() returns correct attributes; also store for reference for later */
    status = SST_FUNCTION(s002_data[2].api, uid, &orig_info);
    TEST_ASSERT_EQUAL(status, s002_data[2].status, TEST_CHECKPOINT_NUM(2));
    TEST_ASSERT_EQUAL(orig_info.size, TEST_BUFF_SIZE/2, TEST_CHECKPOINT_NUM(3));
    TEST_ASSERT_EQUAL(orig_info.flags, 0, TEST_CHECKPOINT_NUM(4));

    /* Check for data consistency using get() */
    status = SST_FUNCTION(s002_data[5].api, uid, 0, TEST_BUFF_SIZE/2, read_buff);
    TEST_ASSERT_EQUAL(status, s002_data[5].status, TEST_CHECKPOINT_NUM(5));
    TEST_ASSERT_MEMCMP(write_buff, read_buff, TEST_BUFF_SIZE/2, TEST_CHECKPOINT_NUM(6));

    /* set() with WRITE_ONCE_FLAG */
    val->print(PRINT_TEST, "[Check 1] Update the flag of UID %d with WRITE_ONCE flag\n", uid);
    status = SST_FUNCTION(s002_data[7].api, uid, TEST_BUFF_SIZE/4, write_buff_new,
                          PSA_SST_FLAG_WRITE_ONCE);
    TEST_ASSERT_EQUAL(status, s002_data[7].status, TEST_CHECKPOINT_NUM(7));

    /* Check that info is updated, after new set */
    status = SST_FUNCTION(s002_data[8].api, uid, &new_info);
    TEST_ASSERT_EQUAL(status, s002_data[8].status, TEST_CHECKPOINT_NUM(8));
    TEST_ASSERT_EQUAL(new_info.size, new_info.size, TEST_CHECKPOINT_NUM(9));
    TEST_ASSERT_EQUAL(new_info.flags, new_info.flags, TEST_CHECKPOINT_NUM(10));

    /* Check that data contents are preserved which were written with WRITE_ONCE_FLAG originally */
    status = SST_FUNCTION(s002_data[11].api, uid, 0, TEST_BUFF_SIZE/4, read_buff);
    TEST_ASSERT_EQUAL(status, s002_data[11].status, TEST_CHECKPOINT_NUM(11));
    TEST_ASSERT_MEMCMP(write_buff_new, read_buff, TEST_BUFF_SIZE/4, TEST_CHECKPOINT_NUM(12));

    /* remove() the UID */
    val->print(PRINT_TEST, "[Check 2] Try to remove the UID %d having WRITE_ONCE flag\n", uid);
    status = SST_FUNCTION(s002_data[13].api, uid);
    TEST_ASSERT_EQUAL(status, s002_data[13].status, TEST_CHECKPOINT_NUM(13));

    return VAL_STATUS_SUCCESS;
}


int32_t psa_sst_create_with_write_once_flag(caller_security_t caller)
{
    uint32_t status;
    psa_sst_uid_t uid = UID_WRITE_ONCE_2;
    uint8_t write_buff[TEST_BUFF_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                          0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    uint8_t read_buff[TEST_BUFF_SIZE]  = {0};
    uint8_t write_buff_new[TEST_BUFF_SIZE + 1] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                  0xFF};

    /* Set data for a UID using WRITE_ONCE flag */
    val->print(PRINT_TEST, "[Check 3] Create a new UID %d with WRITE_ONCE flag\n", uid);
    status = SST_FUNCTION(s002_data[14].api, uid, TEST_BUFF_SIZE, write_buff,
                          PSA_SST_FLAG_WRITE_ONCE);
    TEST_ASSERT_EQUAL(status, s002_data[14].status, TEST_CHECKPOINT_NUM(14));

    /* Check that remove() fails with PSA_SST_ERROR_WRITE_ONCE */
    val->print(PRINT_TEST, "[Check 4] Try to remove the UID %d having WRITE_ONCE flag\n", uid);
    status = SST_FUNCTION(s002_data[15].api, uid);
    TEST_ASSERT_EQUAL(status, s002_data[15].status, TEST_CHECKPOINT_NUM(15));

    /* Check data consistency using get()*/
    status = SST_FUNCTION(s002_data[16].api, uid, 0, TEST_BUFF_SIZE, read_buff);
    TEST_ASSERT_EQUAL(status, s002_data[16].status, TEST_CHECKPOINT_NUM(16));
    TEST_ASSERT_MEMCMP(write_buff, read_buff, TEST_BUFF_SIZE, TEST_CHECKPOINT_NUM(17));

    /* Check that info values is as expected */
    status = SST_FUNCTION(s002_data[18].api, uid, &orig_info);
    TEST_ASSERT_EQUAL(status, s002_data[18].status, TEST_CHECKPOINT_NUM(18));
    TEST_ASSERT_EQUAL(orig_info.size, TEST_BUFF_SIZE, TEST_CHECKPOINT_NUM(19));
    TEST_ASSERT_EQUAL(orig_info.flags, PSA_SST_FLAG_WRITE_ONCE, TEST_CHECKPOINT_NUM(20));

    /* Try to overwrite using set() with same UID as used before with WRITE_ONCE_FLAG */
    val->print(PRINT_TEST, "[Check 5] Try to change the length of write_once UID %d\n", uid);
    status = SST_FUNCTION(s002_data[21].api, uid, (TEST_BUFF_SIZE + 1), write_buff_new,
                          PSA_SST_FLAG_WRITE_ONCE);
    TEST_ASSERT_EQUAL(status, s002_data[21].status, TEST_CHECKPOINT_NUM(21));

    /* Check that remove() still fails with PSA_SST_ERROR_WRITE_ONCE */
    val->print(PRINT_TEST, "[Check 6] Check UID removal still fails\n", 0);
    status = SST_FUNCTION(s002_data[22].api, uid);
    TEST_ASSERT_EQUAL(status, s002_data[22].status, TEST_CHECKPOINT_NUM(22));

    /* Check that info is preserved */
    status = SST_FUNCTION(s002_data[23].api, uid, &new_info);
    TEST_ASSERT_EQUAL(status, s002_data[23].status, TEST_CHECKPOINT_NUM(23));
    TEST_ASSERT_EQUAL(new_info.size, orig_info.size, TEST_CHECKPOINT_NUM(24));
    TEST_ASSERT_EQUAL(new_info.flags, orig_info.flags, TEST_CHECKPOINT_NUM(25));

    /* Check that data contents are preserved which were written with WRITE_ONCE_FLAG originally */
    status = SST_FUNCTION(s002_data[26].api, uid, 0, TEST_BUFF_SIZE, read_buff);
    TEST_ASSERT_EQUAL(status, s002_data[26].status, TEST_CHECKPOINT_NUM(26));
    TEST_ASSERT_MEMCMP(write_buff, read_buff, TEST_BUFF_SIZE, TEST_CHECKPOINT_NUM(27));

    /* Try to overwrite using set() with same UID as used before without WRITE_ONCE_FLAG */
    val->print(PRINT_TEST, "[Check 7] Try to change the WRITE_ONCE flag to None for UID %d\n", uid);
    new_info.size = 0;
    new_info.flags = 0;
    status = SST_FUNCTION(s002_data[28].api, uid, (TEST_BUFF_SIZE - 1), write_buff_new, 0);
    TEST_ASSERT_EQUAL(status, s002_data[28].status, TEST_CHECKPOINT_NUM(28));

    /* Check that remove() still fails with PSA_SST_ERROR_WRITE_ONCE */
    val->print(PRINT_TEST, "[Check 8] Check UID removal still fails\n", 0);
    status = SST_FUNCTION(s002_data[29].api, uid);
    TEST_ASSERT_EQUAL(status, s002_data[29].status, TEST_CHECKPOINT_NUM(29));

    /* Check that info is preserved */
    status = SST_FUNCTION(s002_data[30].api, uid, &new_info);
    TEST_ASSERT_EQUAL(status, s002_data[30].status, TEST_CHECKPOINT_NUM(30));
    TEST_ASSERT_EQUAL(new_info.size, orig_info.size, TEST_CHECKPOINT_NUM(31));
    TEST_ASSERT_EQUAL(new_info.flags, orig_info.flags, TEST_CHECKPOINT_NUM(32));

    return VAL_STATUS_SUCCESS;
}
