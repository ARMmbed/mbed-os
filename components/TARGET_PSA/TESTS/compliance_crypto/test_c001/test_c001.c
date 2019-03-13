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
#include "test_c001.h"

client_test_t test_c001_crypto_list[] = {
    NULL,
    psa_generate_random_without_init_test,
    psa_crypto_init_test,
    multiple_psa_crypto_init_test,
    NULL,
};

int32_t psa_generate_random_without_init_test(security_t caller)
{
    uint8_t         output[GENERATE_SIZE];
    int32_t         status;

    val->print(PRINT_TEST, "[Check 1] Test calling crypto functions before psa_crypto_init\n", 0);

    /* Generate random bytes */
    status = val->crypto_function(VAL_CRYPTO_GENERATE_RANDOM, output, GENERATE_SIZE);
    if (status == PSA_SUCCESS)
        return RESULT_SKIP(VAL_STATUS_INIT_ALREADY_DONE);
    else
        TEST_ASSERT_EQUAL(status, PSA_ERROR_BAD_STATE, TEST_CHECKPOINT_NUM(1));

    return VAL_STATUS_SUCCESS;
}

int32_t psa_crypto_init_test(security_t caller)
{
    int32_t        status;

    val->print(PRINT_TEST, "[Check 2] Test psa_crypto_init\n", 0);

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    return VAL_STATUS_SUCCESS;
}

int32_t multiple_psa_crypto_init_test(security_t caller)
{
    int32_t         i, status;

    val->print(PRINT_TEST, "[Check 3] Test multiple psa_crypto_init \n", 0);
    for (i = 0; i < 5; i++)
    {
        /* Initialize the PSA crypto library*/
        status = val->crypto_function(VAL_CRYPTO_INIT);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));
    }

    return VAL_STATUS_SUCCESS;
}
