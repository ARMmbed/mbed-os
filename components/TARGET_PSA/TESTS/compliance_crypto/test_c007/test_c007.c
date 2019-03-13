/** @file
 * Copyright (c) 2018-2019, Arm Limited or its affiliates. All rights reserved.
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
#include "test_c007.h"
#include "test_data.h"
#include "val_crypto.h"

client_test_t test_c007_crypto_list[] = {
    NULL,
    psa_set_key_policy_test,
    psa_set_key_policy_negative_test,
    NULL,
};

static int g_test_count = 1;

int32_t psa_set_key_policy_test(security_t caller)
{
    const uint8_t    *key_data;
    psa_key_policy_t policy, expected_policy;
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

        /* Initialize a key policy structure to a default that forbids all
         * usage of the key
         */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

        /* Setting up the watchdog timer for each check */
        status = val->wd_reprogram_timer(WD_CRYPTO_TIMEOUT);
        TEST_ASSERT_EQUAL(status, VAL_STATUS_SUCCESS, TEST_CHECKPOINT_NUM(2));

        if (PSA_KEY_TYPE_IS_RSA(check1[i].key_type))
        {
            if (check1[i].key_type == PSA_KEY_TYPE_RSA_KEYPAIR)
            {
                if (check1[i].expected_bit_length == BYTES_TO_BITS(384))
                    key_data = rsa_384_keypair;
                else if (check1[i].expected_bit_length == BYTES_TO_BITS(256))
                    key_data = rsa_256_keypair;
                else
                    return VAL_STATUS_INVALID;
            }
            else
            {
                if (check1[i].expected_bit_length == BYTES_TO_BITS(384))
                    key_data = rsa_384_keydata;
                else if (check1[i].expected_bit_length == BYTES_TO_BITS(256))
                    key_data = rsa_256_keydata;
                else
                    return VAL_STATUS_INVALID;
            }
        }
        else if (PSA_KEY_TYPE_IS_ECC(check1[i].key_type))
        {
            if (PSA_KEY_TYPE_IS_ECC_KEYPAIR(check1[i].key_type))
                key_data = ec_keypair;
            else
                key_data = ec_keydata;
        }
        else
            key_data = check1[i].key_data;

        /* Set the standard fields of a policy structure */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check1[i].usage,
                                                                         check1[i].key_alg);

        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check1[i].key_handle);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check1[i].key_handle, &policy);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(4));

        if (check1[i].expected_status != PSA_SUCCESS)
            continue;

        /* Import the key data into the key slot */
        status = val->crypto_function(VAL_CRYPTO_IMPORT_KEY, check1[i].key_handle,
                                      check1[i].key_type, key_data, check1[i].key_length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

        /*  Get the usage policy for a key slot */
        status = val->crypto_function(VAL_CRYPTO_GET_KEY_POLICY, check1[i].key_handle,
                                      &expected_policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

        /* Check if the usage is same as programmed */
        TEST_ASSERT_EQUAL(expected_policy.usage, check1[i].usage, TEST_CHECKPOINT_NUM(7));

        /* Check if the algorithm is same as programmed */
        TEST_ASSERT_EQUAL(expected_policy.alg, check1[i].key_alg, TEST_CHECKPOINT_NUM(8));
    }

    return VAL_STATUS_SUCCESS;
}

int32_t psa_set_key_policy_negative_test(security_t caller)
{
    int              num_checks = sizeof(check2)/sizeof(check2[0]);
    int32_t          i, status;
    psa_key_policy_t policy;

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    /* Initialize a key policy structure to a default that forbids all
    * usage of the key
    */
    val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

    for (i = 0; i < num_checks; i++)
    {
        val->print(PRINT_TEST, "[Check %d] Test psa_set_key_policy with unallocated key handle\n",
                                                                                  g_test_count++);
        /* Setting up the watchdog timer for each check */
        status = val->wd_reprogram_timer(WD_CRYPTO_TIMEOUT);
        TEST_ASSERT_EQUAL(status, VAL_STATUS_SUCCESS, TEST_CHECKPOINT_NUM(2));

        /* Set the usage policy on a key slot */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check2[i].usage,
                                                                           check2[i].key_alg);
        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check2[i].key_handle, &policy);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_INVALID_HANDLE, TEST_CHECKPOINT_NUM(3));

        val->print(PRINT_TEST, "[Check %d] Test psa_set_key_policy with zero as key handle\n",
                                                                                  g_test_count++);
        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, 0, &policy);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_INVALID_HANDLE, TEST_CHECKPOINT_NUM(4));

        val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

        /* Set the usage policy on a key slot */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check2[i].usage,
                                                                           check2[i].key_alg);
        val->print(PRINT_TEST, "[Check %d] Test psa_set_key_policy with already occupied handle\n",
                                                                                  g_test_count++);
        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check2[i].key_handle);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check2[i].key_handle, &policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

        /* Import the key data into the key slot */
        status = val->crypto_function(VAL_CRYPTO_IMPORT_KEY, check2[i].key_handle,
                                      check2[i].key_type, check2[i].key_data, check2[i].key_length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(7));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check2[i].key_handle, &policy);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_OCCUPIED_SLOT, TEST_CHECKPOINT_NUM(8));
    }

    return VAL_STATUS_SUCCESS;
}
