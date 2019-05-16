
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
#include "test_c031.h"
#include "test_data.h"

client_test_t test_c031_crypto_list[] = {
    NULL,
    psa_mac_abort_test,
    psa_mac_abort_before_finish_test,
    NULL,
};

static int         g_test_count = 1;
static uint8_t     data[BUFFER_SIZE];

int32_t psa_mac_abort_test(security_t caller)
{
    int                 num_checks = sizeof(check1)/sizeof(check1[0]);
    int32_t             i, status;
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

        /* Abort a MAC operation */
        status = val->crypto_function(VAL_CRYPTO_MAC_ABORT, &operation);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(7));

        /* Multiple Abort a MAC operation should succeed */
        status = val->crypto_function(VAL_CRYPTO_MAC_ABORT, &operation);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(8));
    }

    return VAL_STATUS_SUCCESS;
}

int32_t psa_mac_abort_before_finish_test(security_t caller)
{
    size_t              length;
    psa_key_policy_t    policy;
    psa_algorithm_t     key_alg = PSA_ALG_CMAC;
    psa_key_usage_t     usage = PSA_KEY_USAGE_SIGN;
    psa_key_handle_t    key_handle = 10;
    psa_key_type_t      key_type = PSA_KEY_TYPE_AES;
    psa_mac_operation_t operation;
    uint8_t             key_data[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7,
                                      0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    uint8_t             input_data[] = {0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65};
    size_t              key_length = sizeof(key_data)/sizeof(key_data[0]);
    size_t              inputdata_size = sizeof(input_data)/sizeof(input_data[0]);
    int32_t             status;

    memset(data, 0, sizeof(data));
    memset(&operation, 0, sizeof(operation));

    val->print(PRINT_TEST, "[Check %d] ", g_test_count++);
    val->print(PRINT_TEST, "Test psa_mac_sign_finish after calling psa_mac_abort\n", 0);

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    /* Setting up the watchdog timer for each check */
    status = val->wd_reprogram_timer(WD_CRYPTO_TIMEOUT);
    TEST_ASSERT_EQUAL(status, VAL_STATUS_SUCCESS, TEST_CHECKPOINT_NUM(2));

    /* Initialize a key policy structure to a default that forbids all
     * usage of the key
     */
    val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

    /* Set the standard fields of a policy structure */
    val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, usage, key_alg);

    /* Allocate a key slot for a transient key */
    status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &key_handle);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

    /* Set the usage policy on a key slot */
    status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, key_handle, &policy);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(4));

    /* Import the key data into the key slot */
    status = val->crypto_function(VAL_CRYPTO_IMPORT_KEY, key_handle, key_type, key_data,
                key_length);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

    /* Start a multipart MAC calculation operation */
    status = val->crypto_function(VAL_CRYPTO_MAC_SIGN_SETUP, &operation, key_handle, key_alg);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

    /* Add a message fragment to a multipart MAC operation */
    status = val->crypto_function(VAL_CRYPTO_MAC_UPDATE, &operation, input_data, inputdata_size);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(7));

    /* Abort a MAC operation */
    status = val->crypto_function(VAL_CRYPTO_MAC_ABORT, &operation);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(8));

    /* Finish the calculation of the MAC of a message */
    status = val->crypto_function(VAL_CRYPTO_MAC_SIGN_FINISH, &operation, data,
                BUFFER_SIZE, &length);
    TEST_ASSERT_EQUAL(status, PSA_ERROR_BAD_STATE, TEST_CHECKPOINT_NUM(9));

    return VAL_STATUS_SUCCESS;
}
