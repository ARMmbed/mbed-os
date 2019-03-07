
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
#include "test_c043.h"
#include "test_data.h"

client_test_t test_c043_crypto_list[] = {
    NULL,
    psa_key_agreement_test,
    psa_key_agreement_negative_test,
    NULL,
};

static int         g_test_count = 1;
static uint8_t     output[SIZE_50B];

int32_t psa_key_agreement_test(security_t caller)
{
    int                     num_checks = sizeof(check1)/sizeof(check1[0]);
    int32_t                 i, status;
    size_t                  capacity;
    psa_key_policy_t        policy;
    psa_crypto_generator_t  generator = {0};

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

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

        /* Set the standard fields of a policy structure */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check1[i].usage,
                                                                          check1[i].key_alg);

        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check1[i].key_handle);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check1[i].key_handle,
                    &policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(4));

        /* Import the key data into the key slot */
        status = val->crypto_function(VAL_CRYPTO_IMPORT_KEY, check1[i].key_handle,
                    check1[i].key_type, check1[i].key_data, check1[i].key_length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

        /* Set up a key agreement operation */
        status = val->crypto_function(VAL_CRYPTO_KEY_AGREEMENT, &generator,
                    check1[i].key_handle, check1[i].peer_key, check1[i].peer_key_length,
                    check1[i].key_alg);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(6));

        if (check1[i].expected_status != PSA_SUCCESS)
        {
            /* Abort a generator */
            status = val->crypto_function(VAL_CRYPTO_GENERATOR_ABORT, &generator);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(7));
            /* Destroy a key and restore the slot to its default state */
            status = val->crypto_function(VAL_CRYPTO_DESTROY_KEY, check1[i].key_handle);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(8));
            continue;
        }

        /* Retrieve the current capacity of a generator */
        status = val->crypto_function(VAL_CRYPTO_GET_GENERATOR_CAPACITY, &generator, &capacity);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(9));

        /* Check if the generator capacity matches with the expected capacity */
        TEST_ASSERT_EQUAL(capacity, check1[i].expected_capacity, TEST_CHECKPOINT_NUM(10));

        /* Read some data from a generator */
        status = val->crypto_function(VAL_CRYPTO_GENERATOR_READ, &generator, output,
                    check1[i].expected_output_length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(11));

         /* Check if the output matches with the expected data */
        TEST_ASSERT_MEMCMP(output, check1[i].expected_output, check1[i].expected_output_length,
                        TEST_CHECKPOINT_NUM(12));

        /* Abort a generator */
        status = val->crypto_function(VAL_CRYPTO_GENERATOR_ABORT, &generator);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(13));
        
        /* Destroy a key and restore the slot to its default state */
        status = val->crypto_function(VAL_CRYPTO_DESTROY_KEY, check1[i].key_handle);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(14));
    }

    return VAL_STATUS_SUCCESS;
}

int32_t psa_key_agreement_negative_test(security_t caller)
{
    int                     num_checks = sizeof(check2)/sizeof(check2[0]);
    int32_t                 i, status;
    psa_key_policy_t        policy;
    psa_crypto_generator_t  generator = {0};

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    for (i = 0; i < num_checks; i++)
    {
        val->print(PRINT_TEST, "[Check %d] Test psa_key_agreement - Invalid key handle\n",
                                                                                 g_test_count++);
        /* Initialize a key policy structure to a default that forbids all
         * usage of the key
         */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

        /* Setting up the watchdog timer for each check */
        status = val->wd_reprogram_timer(WD_CRYPTO_TIMEOUT);
        TEST_ASSERT_EQUAL(status, VAL_STATUS_SUCCESS, TEST_CHECKPOINT_NUM(2));

        /* Set the standard fields of a policy structure */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check2[i].usage,
                                                                          check2[i].key_alg);

        val->print(PRINT_TEST, "[Check %d] Test psa_key_agreement - Invalid key handle\n",
                                                                                 g_test_count++);
        /* Set up a key agreement operation */
        status = val->crypto_function(VAL_CRYPTO_KEY_AGREEMENT, &generator,
                    check2[i].key_handle, check2[i].peer_key, check2[i].peer_key_length,
                    check2[i].key_alg);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_INVALID_HANDLE, TEST_CHECKPOINT_NUM(3));

        val->print(PRINT_TEST, "[Check %d] Test psa_key_agreement - Zero as key handle\n",
                                                                                 g_test_count++);
        /* Set up a key agreement operation */
        status = val->crypto_function(VAL_CRYPTO_KEY_AGREEMENT, &generator,
                    0, check2[i].peer_key, check2[i].peer_key_length,
                    check2[i].key_alg);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_INVALID_HANDLE, TEST_CHECKPOINT_NUM(4));

        val->print(PRINT_TEST, "[Check %d] Test psa_key_agreement - Empty key handle\n",
                                                                                 g_test_count++);
       /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check2[i].key_handle);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check2[i].key_handle,
                    &policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

        /* Set up a key agreement operation */
        status = val->crypto_function(VAL_CRYPTO_KEY_AGREEMENT, &generator,
                    check2[i].key_handle, check2[i].peer_key, check2[i].peer_key_length,
                    check2[i].key_alg);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_EMPTY_SLOT, TEST_CHECKPOINT_NUM(7));
    }

    return VAL_STATUS_SUCCESS;
}
