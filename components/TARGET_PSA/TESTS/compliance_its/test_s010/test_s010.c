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
#include "test_s010.h"
#ifdef ITS_TEST
#include "test_its_data.h"
#elif PS_TEST
#include "test_ps_data.h"
#endif

#define TEST_BUFF_SIZE 1

client_test_t test_s010_sst_list[] = {
    NULL,
    psa_sst_uid_value_zero_check,
    NULL,
};

static uint8_t write_buff[TEST_BUFF_SIZE] = {0xFF};

int32_t psa_sst_uid_value_zero_check(caller_security_t caller)
{
    int32_t status;
    psa_sst_uid_t uid = 0;

    /* Set with UID value zero should fail */
    val->print(PRINT_TEST, "[Check 1] Creating storage with UID 0 should fail\n", 0 );
    status = SST_FUNCTION(s010_data[0].api, uid, TEST_BUFF_SIZE, write_buff, 0);
    TEST_ASSERT_NOT_EQUAL(status, s010_data[0].status, TEST_CHECKPOINT_NUM(1));

    return VAL_STATUS_SUCCESS;
}

