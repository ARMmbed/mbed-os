
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
#include "test_c038.h"
#include "test_data.h"

client_test_t test_c038_crypto_list[] = {
    NULL,
    psa_cipher_abort_test,
    psa_cipher_abort_before_update_test,
    NULL,
};

static int         g_test_count = 1;
static uint8_t     output[SIZE_32B];

int32_t psa_cipher_abort_test(security_t caller)
{
    int                     num_checks = sizeof(check1)/sizeof(check1[0]);
    int32_t                 i, status;
    psa_key_policy_t        policy;
    psa_cipher_operation_t  operation;

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
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check1[i].key_handle,
                        &policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(4));

        /* Import the key data into the key slot */
        status = val->crypto_function(VAL_CRYPTO_IMPORT_KEY, check1[i].key_handle,
                    check1[i].key_type, check1[i].key_data, check1[i].key_length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

        if (check1[i].usage == PSA_KEY_USAGE_ENCRYPT)
        {
            /* Set the key for a multipart symmetric encryption operation */
            status = val->crypto_function(VAL_CRYPTO_CIPHER_ENCRYPT_SETUP, &operation,
                        check1[i].key_handle, check1[i].key_alg);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));
        }
        else if (check1[i].usage == PSA_KEY_USAGE_DECRYPT)
        {
            /* Set the key for a multipart symmetric decryption operation */
            status = val->crypto_function(VAL_CRYPTO_CIPHER_DECRYPT_SETUP, &operation,
                        check1[i].key_handle, check1[i].key_alg);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(7));
        }

        /* Abort a cipher operation */
        status = val->crypto_function(VAL_CRYPTO_CIPHER_ABORT, &operation);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(8));

        /* Multiple abort cipher operation should return success*/
        status = val->crypto_function(VAL_CRYPTO_CIPHER_ABORT, &operation);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(9));
    }

    return VAL_STATUS_SUCCESS;
}

int32_t psa_cipher_abort_before_update_test(security_t caller)
{
    size_t                  length;
    psa_key_policy_t        policy;
    psa_algorithm_t         key_alg = PSA_ALG_CBC_NO_PADDING;
    psa_key_usage_t         usage = PSA_KEY_USAGE_ENCRYPT;
    psa_key_handle_t        key_handle = 13;
    psa_key_type_t          key_type = PSA_KEY_TYPE_AES;
    psa_cipher_operation_t  operation;
    uint8_t                 key_data[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab,
                                          0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    uint8_t                 input[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9,
                                       0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
    uint8_t                 iv[] = {0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
                                    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a};
    size_t                  key_length = sizeof(key_data)/sizeof(key_data[0]);
    size_t                  input_length = sizeof(input)/sizeof(input[0]);
    size_t                  iv_size = sizeof(iv)/sizeof(iv[0]);
    int32_t                 status;

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    val->print(PRINT_TEST, "[Check %d] ", g_test_count++);
    val->print(PRINT_TEST, "Test psa_cipher_update after psa_cipher_abort should fail\n", 0);
    memset(&operation, 0, sizeof(operation));

    /* Initialize a key policy structure to a default that forbids all
     * usage of the key
     */
    val->crypto_function(VAL_CRYPTO_KEY_POLICY_INIT, &policy);

    /* Setting up the watchdog timer for each check */
    status = val->wd_reprogram_timer(WD_CRYPTO_TIMEOUT);
    TEST_ASSERT_EQUAL(status, VAL_STATUS_SUCCESS, TEST_CHECKPOINT_NUM(2));

    /* Set the standard fields of a policy structure */
    val->crypto_function(VAL_CRYPTO_KEY_POLICY_SET_USAGE, &policy, usage, key_alg);

    /* Allocate a key slot for a transient key */
    status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &key_handle);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

    /* Set the usage policy on a key slot */
    status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, key_handle,  &policy);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(4));

    /* Import the key data into the key slot */
    status = val->crypto_function(VAL_CRYPTO_IMPORT_KEY, key_handle,
                key_type, key_data, key_length);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

    /* Set the key for a multipart symmetric encryption operation */
    status = val->crypto_function(VAL_CRYPTO_CIPHER_ENCRYPT_SETUP, &operation,
                key_handle, key_alg);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

    /* Set an IV for a symmetric encryption operation */
    status = val->crypto_function(VAL_CRYPTO_CIPHER_SET_IV, &operation, iv, iv_size);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(7));

    /* Abort a cipher operation */
    status = val->crypto_function(VAL_CRYPTO_CIPHER_ABORT, &operation);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(8));

    /* Encrypt or decrypt a message fragment in an active cipher operation */
    status = val->crypto_function(VAL_CRYPTO_CIPHER_UPDATE, &operation, input,
                input_length, output, SIZE_32B, &length);
    TEST_ASSERT_EQUAL(status, PSA_ERROR_BAD_STATE, TEST_CHECKPOINT_NUM(9));

    return VAL_STATUS_SUCCESS;
}
