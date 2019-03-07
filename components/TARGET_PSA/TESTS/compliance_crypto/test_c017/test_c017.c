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
#include "test_c017.h"
#include "test_data.h"
#include "val_crypto.h"

client_test_t test_c017_crypto_list[] = {
    NULL,
    psa_generate_random_test,
    NULL,
};

static int g_test_count = 1;

int32_t psa_generate_random_test(security_t caller)
{
    int                     num_checks = sizeof(check1)/sizeof(check1[0]);
    uint32_t                i, j, data_sum;
    uint8_t                 data[BUFFER_SIZE] = {0};
    int32_t                 status;

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    for (i = 0; i < num_checks; i++)
    {
        val->print(PRINT_TEST, "[Check %d] ", g_test_count++);
        val->print(PRINT_TEST, check1[i].test_desc, 0);

        /* Setting up the watchdog timer for each check */
        status = val->wd_reprogram_timer(WD_CRYPTO_TIMEOUT);
        TEST_ASSERT_EQUAL(status, VAL_STATUS_SUCCESS, TEST_CHECKPOINT_NUM(2));

        /* Generate random bytes */
        status = val->crypto_function(VAL_CRYPTO_GENERATE_RANDOM, data, check1[i].size);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(3));

        if (check1[i].expected_status != PSA_SUCCESS)
            continue;

        data_sum = 0;
        /* Check that if generated data are zero */
        for (j = 0; j < check1[i].size; j++)
        {
            data_sum += data[j];
            data[j] = 0;
        }

        if (check1[i].size != 0)
            TEST_ASSERT_NOT_EQUAL(data_sum, 0, TEST_CHECKPOINT_NUM(4));
        else
            TEST_ASSERT_EQUAL(data_sum, 0, TEST_CHECKPOINT_NUM(5));
    }

    return VAL_STATUS_SUCCESS;
}
