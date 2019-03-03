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
#include "test_c016.h"
#include "test_data.h"
#include "val_crypto.h"

client_test_t test_c016_crypto_list[] = {
    NULL,
    psa_generate_key_test,
    psa_generate_key_negative_test,
    NULL,
};

static int       g_test_count = 1;
static uint8_t   data[BUFFER_SIZE];

int32_t psa_generate_key_test(security_t caller)
{
    int                     num_checks = sizeof(check1)/sizeof(check1[0]);
    uint32_t                i, length;
    psa_key_policy_t        policy;
    psa_key_type_t          key_type;
    size_t                  bits;
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

        /* Initialize a key policy structure to a default that forbids all
        * usage of the key
        */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

        /* Set the standard fields of a policy structure */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check1[i].usage,
                                                                          check1[i].key_alg);

        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check1[i].key_handle);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check1[i].key_handle, &policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(4));

        /* Generate a key or key pair */
        status = val->crypto_function(VAL_CRYPTO_GENERATE_KEY, check1[i].key_handle,
                    check1[i].key_type, check1[i].bits, check1[i].extra, check1[i].extra_size);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(5));

        if (check1[i].expected_status != PSA_SUCCESS)
            continue;

        /* Get basic metadata about a key */
        status = val->crypto_function(VAL_CRYPTO_GET_KEY_INFORMATION, check1[i].key_handle,
                    &key_type, &bits);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

        TEST_ASSERT_EQUAL(key_type, check1[i].key_type, TEST_CHECKPOINT_NUM(7));

        TEST_ASSERT_EQUAL(bits, check1[i].expected_bit_length, TEST_CHECKPOINT_NUM(8));

        /* Export a key in binary format */
        status = val->crypto_function(VAL_CRYPTO_EXPORT_KEY, check1[i].key_handle, data,
                                      BUFFER_SIZE, &length);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(9));

        TEST_ASSERT_EQUAL(length, check1[i].expected_key_length, TEST_CHECKPOINT_NUM(10));
    }

    return VAL_STATUS_SUCCESS;
}

int32_t psa_generate_key_negative_test(security_t caller)
{
    int                     num_checks = sizeof(check2)/sizeof(check2[0]);
    uint32_t                i;
    psa_key_policy_t        policy;
    int32_t                 status;

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    for (i = 0; i < num_checks; i++)
    {
        /* Setting up the watchdog timer for each check */
        status = val->wd_reprogram_timer(WD_CRYPTO_TIMEOUT);
        TEST_ASSERT_EQUAL(status, VAL_STATUS_SUCCESS, TEST_CHECKPOINT_NUM(2));

        val->print(PRINT_TEST, "[Check %d] Test psa_generate_key with unallocated key handle\n",
                                                                                 g_test_count++);
        /* Generate a key or key pair */
        status = val->crypto_function(VAL_CRYPTO_GENERATE_KEY, check2[i].key_handle,
                    check2[i].key_type, check2[i].bits, check2[i].extra, check2[i].extra_size);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_INVALID_HANDLE, TEST_CHECKPOINT_NUM(3));

        val->print(PRINT_TEST, "[Check %d] Test psa_generate_key with zero as key handle\n",
                                                                                 g_test_count++);
        /* Generate a key or key pair */
        status = val->crypto_function(VAL_CRYPTO_GENERATE_KEY, 0, check2[i].key_type,
                                          check2[i].bits, check2[i].extra, check2[i].extra_size);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_INVALID_HANDLE, TEST_CHECKPOINT_NUM(4));

        val->print(PRINT_TEST, "[Check %d] Test psa_generate_key with pre-occupied key handle\n",
                                                                                 g_test_count++);
        /* Initialize a key policy structure to a default that forbids all
        * usage of the key
        */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

        /* Set the standard fields of a policy structure */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check2[i].usage,
                                                                          check2[i].key_alg);

        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check2[i].key_handle);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check2[i].key_handle, &policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

        /* Generate a key or key pair */
        status = val->crypto_function(VAL_CRYPTO_GENERATE_KEY, check2[i].key_handle,
                    check2[i].key_type, check2[i].bits, check2[i].extra, check2[i].extra_size);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(7));

        /* Generate a key or key pair */
        status = val->crypto_function(VAL_CRYPTO_GENERATE_KEY, check2[i].key_handle,
                    check2[i].key_type, check2[i].bits, check2[i].extra, check2[i].extra_size);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_OCCUPIED_SLOT, TEST_CHECKPOINT_NUM(8));

        val->print(PRINT_TEST, "[Check %d] Test psa_generate_key with destroyed key handle\n",
                                                                                 g_test_count++);
        status = val->crypto_function(VAL_CRYPTO_DESTROY_KEY, check2[i].key_handle);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(9));

        /* Generate a key or key pair */
        status = val->crypto_function(VAL_CRYPTO_GENERATE_KEY, check2[i].key_handle,
                    check2[i].key_type, check2[i].bits, check2[i].extra, check2[i].extra_size);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_INVALID_HANDLE, TEST_CHECKPOINT_NUM(10));
    }

    return VAL_STATUS_SUCCESS;
}

