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
#include "test_c020.h"
#include "test_data.h"
#include "val_crypto.h"

#define  SLOT_1     0
#define  SLOT_2     1
#define  SLOT_3     2
#define  SLOT_4     3

client_test_t test_c020_crypto_list[] = {
    NULL,
    psa_generator_import_key_test,
    psa_generator_import_key_negative_test,
    NULL,
};

static int                     g_test_count = 1;
static uint8_t                 data[BUFFER_SIZE];
static psa_crypto_generator_t  generator;

int32_t psa_generator_import_key_test(security_t caller)
{
    int                     num_checks = sizeof(check1)/sizeof(check1[0]);
    uint32_t                i, j, data_sum, remaining_size, length = 0;
    uint32_t                salt = 0, label = 0;
    size_t                  salt_length = 0, label_length = 0;
    psa_key_policy_t        policy;
    int32_t                 status;

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

        memset(&generator, 0, sizeof(generator));
        memset(data, 0, sizeof(data));

        /* Set the standard fields of a policy structure */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check1[i].usage[SLOT_1],
                                                                      check1[i].key_alg[SLOT_1]);

        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check1[i].key_handle[SLOT_1]);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check1[i].key_handle[SLOT_1],
                                      &policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

        /* Import the key data into the key slot */
        status = val->crypto_function(VAL_CRYPTO_IMPORT_KEY, check1[i].key_handle[SLOT_1],
                    check1[i].key_type[SLOT_1], check1[i].key_data, check1[i].key_length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(4));

        /* Set up a key derivation operation. Using this function to initialize the generate as
         * XOR or PRNG generator initialization is not implemented.
         */
        status = val->crypto_function(VAL_CRYPTO_KEY_DERIVATION, &generator,
                    check1[i].key_handle[SLOT_1],  check1[i].key_alg[SLOT_1], &salt, salt_length,
                    &label, label_length, check1[i].capacity);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

        /* Initialize a key policy structure to a default that forbids all
         * usage of the key
         */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

        /* Set the standard fields of a policy structure */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check1[i].usage[SLOT_2],
                                                                         check1[i].key_alg[SLOT_2]);

        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check1[i].key_handle[SLOT_2]);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check1[i].key_handle[SLOT_2],
                                      &policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

        /* Create a symmetric key from data read from a generator */
        status = val->crypto_function(VAL_CRYPTO_GENERATOR_IMPORT_KEY, check1[i].key_handle[SLOT_2],
                    check1[i].key_type[SLOT_2], BYTES_TO_BITS(check1[i].size), &generator);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(7));

        if (check1[i].expected_status != PSA_SUCCESS)
        {
            /* Abort a generator */
            status = val->crypto_function(VAL_CRYPTO_GENERATOR_ABORT, &generator);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(8));
            continue;
        }

        /* Export a key in binary format */
        status = val->crypto_function(VAL_CRYPTO_EXPORT_KEY, check1[i].key_handle[SLOT_2], data,
                                      BUFFER_SIZE, &length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(9));

        TEST_ASSERT_EQUAL(length, check1[i].size, TEST_CHECKPOINT_NUM(10));

        data_sum = 0;
        /* Check that if generated data are zero */
        for (j = 0; j < check1[i].size; j++)
        {
            data_sum += data[j];
        }

        memset(data, 0, sizeof(data));
        TEST_ASSERT_NOT_EQUAL(data_sum, 0, TEST_CHECKPOINT_NUM(12));

        remaining_size = check1[i].capacity - check1[i].size;
        if (remaining_size > 0)
        {
            length = 0;
            /* Initialize a key policy structure to a default that forbids all
             * usage of the key
             */
            val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

            /* Set the standard fields of a policy structure */
            val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check1[i].usage[SLOT_2],
                                                                        check1[i].key_alg[SLOT_2]);

            /* Allocate a key slot for a transient key */
            status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check1[i].key_handle[SLOT_3]);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

            /* Set the usage policy on a key slot */
            status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check1[i].key_handle[SLOT_3],
                                          &policy);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(12));

            /* Create a symmetric key from data read from a generator */
            status = val->crypto_function(VAL_CRYPTO_GENERATOR_IMPORT_KEY,
                        check1[i].key_handle[SLOT_3], check1[i].key_type[SLOT_2],
                        BYTES_TO_BITS(check1[i].size), &generator);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(13));

            /* Export a key in binary format */
            status = val->crypto_function(VAL_CRYPTO_EXPORT_KEY, check1[i].key_handle[SLOT_3], data,
                        BUFFER_SIZE, &length);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(14));

            TEST_ASSERT_EQUAL(length, remaining_size, TEST_CHECKPOINT_NUM(15));

            data_sum = 0;
            /* Check that if generated data are zero */
            for (j = 0; j < remaining_size; j++)
            {
                data_sum += data[j];
            }

            memset(data, 0, sizeof(data));
            TEST_ASSERT_NOT_EQUAL(data_sum, 0, TEST_CHECKPOINT_NUM(16));

            /* Initialize a key policy structure to a default that forbids all
             * usage of the key
             */
            val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

            /* Set the standard fields of a policy structure */
            val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check1[i].usage[SLOT_2],
                                                                        check1[i].key_alg[SLOT_2]);

            /* Allocate a key slot for a transient key */
            status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check1[i].key_handle[SLOT_4]);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

            /* Set the usage policy on a key slot */
            status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check1[i].key_handle[SLOT_4],
                                          &policy);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(17));

            /* Create a symmetric key from data read from a generator */
            status = val->crypto_function(VAL_CRYPTO_GENERATOR_IMPORT_KEY,
                        check1[i].key_handle[SLOT_4], check1[i].key_type[SLOT_2],
                        BYTES_TO_BITS(check1[i].size), &generator);
            TEST_ASSERT_EQUAL(status, PSA_ERROR_INSUFFICIENT_CAPACITY, TEST_CHECKPOINT_NUM(18));
        }

        /* Abort a generator */
        status = val->crypto_function(VAL_CRYPTO_GENERATOR_ABORT, &generator);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(19));
    }

    return VAL_STATUS_SUCCESS;
}

int32_t psa_generator_import_key_negative_test(security_t caller)
{
    int                     num_checks = sizeof(check2)/sizeof(check2[0]);
    int32_t                 i, status;
    uint32_t                salt = 0, label = 0;
    size_t                  salt_length = 0, label_length = 0;
    uint8_t                 data[BUFFER_SIZE];
    psa_key_policy_t        policy;

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    for (i = 0; i < num_checks; i++)
    {
        /* Initialize a key policy structure to a default that forbids all
         * usage of the key
         */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

        /* Setting up the watchdog timer for each check */
        status = val->wd_reprogram_timer(WD_CRYPTO_TIMEOUT);
        TEST_ASSERT_EQUAL(status, VAL_STATUS_SUCCESS, TEST_CHECKPOINT_NUM(2));

        memset(&generator, 0, sizeof(generator));
        memset(data, 0, sizeof(data));

        /* Set the standard fields of a policy structure */
        val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, check2[i].usage[SLOT_1],
                                                                     check2[i].key_alg[SLOT_1]);

        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check2[i].key_handle[SLOT_1]);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check2[i].key_handle[SLOT_1],
                                      &policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(4));

        /* Import the key data into the key slot */
        status = val->crypto_function(VAL_CRYPTO_IMPORT_KEY, check2[i].key_handle[SLOT_1],
                    check2[i].key_type[SLOT_1], check2[i].key_data, check2[i].key_length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

        /* Set up a key derivation operation. Using this function to initialize the generate as
         * XOR or PRNG generator initialization is not implemented.
         */
        status = val->crypto_function(VAL_CRYPTO_KEY_DERIVATION, &generator,
                    check2[i].key_handle[SLOT_1], check2[i].key_alg[SLOT_1], &salt, salt_length,
                    &label, label_length, check2[i].capacity);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

        val->print(PRINT_TEST, "[Check %d] Test psa_generator_import_key with invalid handle\n",
                                                                                 g_test_count++);
        /* Create a symmetric key from data read from a generator */
        status = val->crypto_function(VAL_CRYPTO_GENERATOR_IMPORT_KEY, check2[i].key_handle[SLOT_2],
                    check2[i].key_type[SLOT_2], check2[i].size, &generator);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_INVALID_HANDLE, TEST_CHECKPOINT_NUM(7));

        val->print(PRINT_TEST, "[Check %d] Test psa_generator_import_key with zero handle\n",
                                                                                 g_test_count++);
        /* Create a symmetric key from data read from a generator */
        status = val->crypto_function(VAL_CRYPTO_GENERATOR_IMPORT_KEY, ZERO_KEY_SLOT,
                    check2[i].key_type[SLOT_2], check2[i].size, &generator);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_INVALID_HANDLE, TEST_CHECKPOINT_NUM(7));

        val->print(PRINT_TEST, "[Check %d] Test psa_generator_import_key with"
                                                     " pre-occupied key slot\n", g_test_count++);
        /* Create a symmetric key from data read from a generator */
        status = val->crypto_function(VAL_CRYPTO_GENERATOR_IMPORT_KEY, check2[i].key_handle[SLOT_1],
                    check2[i].key_type[SLOT_2], check2[i].size, &generator);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_OCCUPIED_SLOT, TEST_CHECKPOINT_NUM(9));

        /* Abort a generator */
        status = val->crypto_function(VAL_CRYPTO_GENERATOR_ABORT, &generator);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(8));
    }

    return VAL_STATUS_SUCCESS;
}
