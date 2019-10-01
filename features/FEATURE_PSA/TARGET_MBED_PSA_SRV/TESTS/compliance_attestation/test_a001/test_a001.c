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
#include "test_a001.h"
#include "test_data.h"

client_test_t test_a001_attestation_list[] = {
    NULL,
    psa_initial_attestation_get_token_test,
    psa_initial_attestation_get_token_size_test,
    NULL,
};

static int         g_test_count = 1;

int32_t psa_initial_attestation_get_token_test(caller_security_t caller)
{
    int         num_checks = sizeof(check1)/sizeof(check1[0]);
    uint32_t    i, status, token_size;
    uint8_t     challenge[PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64+1];
    uint8_t     token_buffer[TOKEN_SIZE];

    for (i = 0; i < num_checks; i++)
    {
        val->print(PRINT_TEST, "[Check %d] ", g_test_count++);
        val->print(PRINT_TEST, check1[i].test_desc, 0);

        memset(challenge, 0x2a, sizeof(challenge));
        memset(token_buffer, 0, sizeof(token_buffer));

        status = val->attestation_function(VAL_INITIAL_ATTEST_GET_TOKEN_SIZE,
                     check1[i].challenge_size, &token_size);
        if (status != PSA_SUCCESS)
        {
            if (check1[i].challenge_size != PSA_INITIAL_ATTEST_CHALLENGE_SIZE_32 ||
                check1[i].challenge_size != PSA_INITIAL_ATTEST_CHALLENGE_SIZE_48 ||
                check1[i].challenge_size != PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64)
            {
                token_size = check1[i].token_size;
                check1[i].challenge_size = check1[i].actual_challenge_size;
            }
            else
                return status;
        }

        status = val->attestation_function(VAL_INITIAL_ATTEST_GET_TOKEN, challenge,
                     check1[i].challenge_size, token_buffer, &token_size);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(1));

        if (check1[i].expected_status != PSA_SUCCESS)
            continue;

        /* Validate the token */
        status = val->attestation_function(VAL_INITIAL_ATTEST_VERIFY_TOKEN, challenge,
                    check1[i].challenge_size, token_buffer, token_size);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(2));
    }

    return VAL_STATUS_SUCCESS;
}

int32_t psa_initial_attestation_get_token_size_test(caller_security_t caller)
{
    int         num_checks = sizeof(check2)/sizeof(check2[0]);
    uint32_t    i, status, token_size;

    for (i = 0; i < num_checks; i++)
    {
        val->print(PRINT_TEST, "[Check %d] ", g_test_count++);
        val->print(PRINT_TEST, check2[i].test_desc, 0);

        status = val->attestation_function(VAL_INITIAL_ATTEST_GET_TOKEN_SIZE,
                     check2[i].challenge_size, &token_size);

        TEST_ASSERT_EQUAL(status, check2[i].expected_status, TEST_CHECKPOINT_NUM(1));

        if (check2[i].expected_status != PSA_SUCCESS)
            continue;

        if (token_size < check2[i].challenge_size)
        {
            val->print(PRINT_ERROR, "Token size less than challenge size\n", 0);
            return VAL_STATUS_INSUFFICIENT_SIZE;
        }
    }

    return VAL_STATUS_SUCCESS;
}

