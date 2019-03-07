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
#include "test_c019.h"
#include "test_data.h"
#include "val_crypto.h"

client_test_t test_c019_crypto_list[] = {
    NULL,
    psa_get_generator_capacity_test,
    NULL,
};

static int       g_test_count = 1;
static uint8_t   data[BUFFER_SIZE];

int32_t psa_get_generator_capacity_test(security_t caller)
{
    int                     num_checks = sizeof(check1)/sizeof(check1[0]);
    uint32_t                i, remaining_size;
    size_t                  capacity;
    psa_key_policy_t        policy;
    psa_crypto_generator_t  generator;
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

        /* Set up a key derivation operation. Using this function to initialize the generate as
         * XOR or PRNG generator initialization is not implemented.
         */
        status = val->crypto_function(VAL_CRYPTO_KEY_DERIVATION, &generator, check1[i].key_handle,
                    check1[i].key_alg, check1[i].salt, check1[i].salt_length, check1[i].label,
                    check1[i].label_length, check1[i].capacity);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

        /* Retrieve the current capacity of a generator */
        status = val->crypto_function(VAL_CRYPTO_GET_GENERATOR_CAPACITY, &generator, &capacity);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(7));

        if (check1[i].expected_status != PSA_SUCCESS)
        {
            /* Abort a generator */
            status = val->crypto_function(VAL_CRYPTO_GENERATOR_ABORT, &generator);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(8));
            continue;
        }

        TEST_ASSERT_EQUAL(capacity, check1[i].capacity, TEST_CHECKPOINT_NUM(9));

        /* Generate random bytes */
        status = val->crypto_function(VAL_CRYPTO_GENERATOR_READ, &generator, data,
                                      check1[i].size);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(10));

        remaining_size = check1[i].capacity - check1[i].size;

        /* Retrieve the current capacity of a generator */
        status = val->crypto_function(VAL_CRYPTO_GET_GENERATOR_CAPACITY, &generator, &capacity);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(11));

        TEST_ASSERT_EQUAL(capacity, remaining_size, TEST_CHECKPOINT_NUM(12));

        /* Abort a generator */
        status = val->crypto_function(VAL_CRYPTO_GENERATOR_ABORT, &generator);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(13));
    }

    return VAL_STATUS_SUCCESS;
}
