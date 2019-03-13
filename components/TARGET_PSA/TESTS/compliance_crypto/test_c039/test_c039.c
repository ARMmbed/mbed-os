
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
#include "test_c039.h"
#include "test_data.h"

client_test_t test_c039_crypto_list[] = {
    NULL,
    psa_asymmetric_encrypt_test,
    psa_asymmetric_encrypt_negative_test,
    NULL,
};

static bool_t is_buffer_empty(uint8_t *buffer, size_t size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        if (buffer[i] != 0)
            return FALSE;
    }

    return TRUE;
}

static int         g_test_count = 1;
static uint8_t     output[SIZE_128B];

int32_t psa_asymmetric_encrypt_test(security_t caller)
{
    int                     num_checks = sizeof(check1)/sizeof(check1[0]);
    int32_t                 i, status;
    const uint8_t           *key_data;
    uint8_t                 *salt;
    size_t                  length;
    psa_key_policy_t        policy;

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

        memset(output, 0, sizeof(output));

        /* Set the key data based on key type */
        if (PSA_KEY_TYPE_IS_RSA(check1[i].key_type))
        {
            if (check1[i].key_type == PSA_KEY_TYPE_RSA_KEYPAIR)
            {
                if (check1[i].expected_bit_length == BYTES_TO_BITS(384))
                    key_data = rsa_384_keypair;
                else if (check1[i].expected_bit_length == BYTES_TO_BITS(256))
                    key_data = rsa_256_keypair;
                else if (check1[i].expected_bit_length == BYTES_TO_BITS(128))
                    key_data = rsa_128_keypair;
                else
                    return VAL_STATUS_INVALID;
            }
            else
            {
                if (check1[i].expected_bit_length == BYTES_TO_BITS(384))
                    key_data = rsa_384_keydata;
                else if (check1[i].expected_bit_length == BYTES_TO_BITS(256))
                    key_data = rsa_256_keydata;
                else if (check1[i].expected_bit_length == BYTES_TO_BITS(128))
                    key_data = rsa_128_keydata;
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

        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check1[i].key_handle);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check1[i].key_handle,
                    &policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(4));

        /* Import the key data into the key slot */
        status = val->crypto_function(VAL_CRYPTO_IMPORT_KEY, check1[i].key_handle,
                    check1[i].key_type, key_data, check1[i].key_length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

        if (is_buffer_empty(check1[i].salt, check1[i].salt_length) == TRUE)
        {
            salt = NULL;
            check1[i].salt_length = 0;
        }
        else
            salt = check1[i].salt;

        /* Encrypt a short message with a public key */
        status = val->crypto_function(VAL_CRYPTO_ASYMMTERIC_ENCRYPT, check1[i].key_handle,
                    check1[i].key_alg, check1[i].input, check1[i].input_length, salt,
                    check1[i].salt_length, output, check1[i].output_size, &length);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(6));

        if (check1[i].expected_status != PSA_SUCCESS)
            continue;

        /* Check if the output length matches with the expected output length */
        TEST_ASSERT_EQUAL(length, check1[i].expected_output_length, TEST_CHECKPOINT_NUM(7));

        /* We test encryption by checking that encrypt-then-decrypt gives back
         * the original plaintext because of the non-optional random
         * part of encryption process which prevents using fixed vectors. */
        if ((check1[i].usage & PSA_KEY_USAGE_DECRYPT) == PSA_KEY_USAGE_DECRYPT)
        {
            status = val->crypto_function(VAL_CRYPTO_ASYMMTERIC_DECRYPT,
                        check1[i].key_handle, check1[i].key_alg, output, length, salt,
                        check1[i].salt_length, output, check1[i].output_size, &length);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(8));

            /* Check if the output length matches with the input length */
            TEST_ASSERT_EQUAL(length, check1[i].input_length, TEST_CHECKPOINT_NUM(9));

            /* Check if the output matches with the given input data */
            TEST_ASSERT_MEMCMP(output, check1[i].input, length, TEST_CHECKPOINT_NUM(10));
        }
    }

    return VAL_STATUS_SUCCESS;
}

int32_t psa_asymmetric_encrypt_negative_test(security_t caller)
{
    int                     num_checks = sizeof(check2)/sizeof(check2[0]);
    int32_t                 i, status;
    uint8_t                 *salt;
    size_t                  length;
    psa_key_policy_t        policy;

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    for (i = 0; i < num_checks; i++)
    {
        val->print(PRINT_TEST, "[Check %d] Test psa_asymmetric_encrypt - Invalid key handle\n",
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

        val->print(PRINT_TEST, "[Check %d] Test psa_asymmetric_encrypt - Invalid key handle\n",
                                                                                 g_test_count++);
        /* Encrypt a short message with a public key */
        status = val->crypto_function(VAL_CRYPTO_ASYMMTERIC_ENCRYPT, check2[i].key_handle,
                    check2[i].key_alg, check2[i].input, check2[i].input_length, salt,
                    check2[i].salt_length, output, check2[i].output_size, &length);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_INVALID_HANDLE, TEST_CHECKPOINT_NUM(3));

        val->print(PRINT_TEST, "[Check %d] Test psa_asymmetric_encrypt - Zero as key handle\n",
                                                                                 g_test_count++);
        /* Encrypt a short message with a public key */
        status = val->crypto_function(VAL_CRYPTO_ASYMMTERIC_ENCRYPT, 0,
                    check2[i].key_alg, check2[i].input, check2[i].input_length, salt,
                    check2[i].salt_length, output, check2[i].output_size, &length);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_INVALID_HANDLE, TEST_CHECKPOINT_NUM(4));

        val->print(PRINT_TEST, "[Check %d] Test psa_asymmetric_encrypt - Empty key handle\n",
                                                                                 g_test_count++);
        /* Allocate a key slot for a transient key */
        status = val->crypto_function(VAL_CRYPTO_ALLOCATE_KEY, &check2[i].key_handle);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

        /* Set the usage policy on a key slot */
        status = val->crypto_function(VAL_CRYPTO_SET_KEY_POLICY, check2[i].key_handle,
                    &policy);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

        if (is_buffer_empty(check1[i].salt, check1[i].salt_length) == TRUE)
        {
            salt = NULL;
            check1[i].salt_length = 0;
        }
        else
            salt = check1[i].salt;

        /* Encrypt a short message with a public key */
        status = val->crypto_function(VAL_CRYPTO_ASYMMTERIC_ENCRYPT, check2[i].key_handle,
                    check2[i].key_alg, check2[i].input, check2[i].input_length, salt,
                    check2[i].salt_length, output, check2[i].output_size, &length);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_EMPTY_SLOT, TEST_CHECKPOINT_NUM(7));
    }

    return VAL_STATUS_SUCCESS;
}

