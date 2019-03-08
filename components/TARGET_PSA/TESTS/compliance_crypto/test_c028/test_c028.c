
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
#include "test_c028.h"
#include "test_data.h"

client_test_t test_c028_crypto_list[] = {
    NULL,
    psa_mac_sign_finish_test,
    NULL,
};

static int           g_test_count = 1;
static uint8_t       data[BUFFER_SIZE];

int32_t psa_mac_sign_finish_test(security_t caller)
{
    int                 num_checks = sizeof(check1)/sizeof(check1[0]);
    int32_t             i, status;
    size_t              length;
    psa_key_policy_t    policy;
    psa_mac_operation_t operation;


    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    for (i = 0; i < num_checks; i++)
    {
        val->print(PRINT_TEST, "[Check %d] ", g_test_count++);
        val->print(PRINT_TEST, check1[i].test_desc, 0);
        memset(&operation, 0, sizeof(operation));
        memset(data, 0, sizeof(data));

        /* Initialize a key policy structure to a default that forbids all
         * usage of the key
         */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

        /* Setting up the watchdog timer for each check */
        status = val->wd_reprogram_timer(WD_CRYPTO_TIMEOUT);
        TEST_ASSERT_EQUAL(status, VAL_STATUS_SUCCESS, TEST_CHECKPOINT_NUM(2));

        /* Set the standard fields of a policy structure */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check1[i].usage,
                                                                          check1[i].key_alg);

        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check1[i].key_handle);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check1[i].key_handle, &policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(4));

        /* Import the key data into the key slot */
        status = val->crypto_function(VAL_CRYPTO_IMPORT_KEY, check1[i].key_handle,
                    check1[i].key_type, check1[i].key_data, check1[i].key_length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

        /* Start a multipart MAC calculation operation */
        status = val->crypto_function(VAL_CRYPTO_MAC_SIGN_SETUP, &operation,
                    check1[i].key_handle, check1[i].key_alg);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

        /* Add a message fragment to a multipart MAC operation */
        status = val->crypto_function(VAL_CRYPTO_MAC_UPDATE, &operation, check1[i].data,
                    check1[i].data_size);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(7));

        /* Finish the calculation of the MAC of a message */
        status = val->crypto_function(VAL_CRYPTO_MAC_SIGN_FINISH, &operation, data,
                    check1[i].mac_size, &length);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(8));

        if (check1[i].expected_status != PSA_SUCCESS)
        {
            /* Abort a MAC operation */
            status = val->crypto_function(VAL_CRYPTO_MAC_ABORT, &operation);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(9));
            continue;
        }

        /* Check if the MAC length matches with the expected length */
        TEST_ASSERT_EQUAL(length, check1[i].expected_length, TEST_CHECKPOINT_NUM(10));

        /* Check if the MAC data matches with the expected data */
        TEST_ASSERT_MEMCMP(check1[i].expected_data, data, length, TEST_CHECKPOINT_NUM(11));

        memset(data, 0, sizeof(data));

        /* Finish the calculation of the MAC of a message using same operation
         * should return error
         */
        status = val->crypto_function(VAL_CRYPTO_MAC_SIGN_FINISH, &operation, data,
                    check1[i].mac_size, &length);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_BAD_STATE, TEST_CHECKPOINT_NUM(12));

        /* Abort a MAC operation */
        status = val->crypto_function(VAL_CRYPTO_MAC_ABORT, &operation);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(13));
    }

    return VAL_STATUS_SUCCESS;
}
