/** @file
 * Copyright (c) 2018, Arm Limited or its affiliates. All rights reserved.
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
#include "test_c009.h"
#include "test_data.h"
#include "val_crypto.h"

#define  MAX_KEYS   100

client_test_t test_c009_crypto_list[] = {
    NULL,
    psa_allocate_key_test,
    psa_allocate_key_negative_test,
    NULL,
};

static int g_test_count = 1;

int32_t psa_allocate_key_test(security_t caller)
{
    int              num_checks = sizeof(check1)/sizeof(check1[0]);
    int32_t          i, status;

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    /* Set the key data buffer to the input base on algorithm */
    for (i = 0; i < num_checks; i++)
    {
        val->print(PRINT_TEST, "[Check %d] ", g_test_count++);
        val->print(PRINT_TEST, check1[i].test_desc, 0);

        /* Setting up the watchdog timer for each check */
        status = val->wd_reprogram_timer(WD_CRYPTO_TIMEOUT);
        TEST_ASSERT_EQUAL(status, VAL_STATUS_SUCCESS, TEST_CHECKPOINT_NUM(2));

        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check1[i].key_handle);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(3));

    }

    return VAL_STATUS_SUCCESS;
}

int32_t psa_allocate_key_negative_test(security_t caller)
{
    int32_t             i, status;
    psa_key_handle_t    key_handle[MAX_KEYS];

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    val->print(PRINT_TEST, "[Check %d] Testing the insufficient memory\n", g_test_count++);

    for (i = 0; i < MAX_KEYS; i++)
    {
        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &key_handle[i]);
        if (status != PSA_SUCCESS)
            break;
    }

    TEST_ASSERT_EQUAL(status, PSA_ERROR_INSUFFICIENT_MEMORY, TEST_CHECKPOINT_NUM(2));

    return VAL_STATUS_SUCCESS;
}
