/*
* Copyright (c) 2018-2019 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "psa_initial_attestation_api.h"
#include "psa/client.h"
#include "attestation.h"
#include <string.h>

int32_t g_caller_id = 0;

enum psa_attest_err_t
psa_initial_attest_get_token(const uint8_t *challenge_obj,
                             uint32_t       challenge_size,
                             uint8_t       *token,
                             uint32_t      *token_size) {
    enum psa_attest_err_t err;

    err = attest_init();
    if (err != PSA_ATTEST_ERR_SUCCESS)
    {
        return err;
    }

    psa_invec in_vec[1] = { { challenge_obj, challenge_size } };
    psa_outvec out_vec[1] = { { token, *token_size } };

    err = initial_attest_get_token(in_vec, 1, out_vec, 1);
    if (err != PSA_ATTEST_ERR_SUCCESS)
    {
        return err;
    }

    *token_size = out_vec[0].len;

    return err;
}

enum psa_attest_err_t
psa_initial_attest_get_token_size(uint32_t  challenge_size,
                                  uint32_t *token_size) {
    enum psa_attest_err_t err;

    err = attest_init();
    if (err != PSA_ATTEST_ERR_SUCCESS)
    {
        return err;
    }

    psa_invec in_vec[1] = { { &challenge_size, sizeof(challenge_size) } };
    psa_outvec out_vec[1] = { { token_size, sizeof(*token_size) } };

    err = initial_attest_get_token_size(in_vec, 1, out_vec, 1);

    return err;
}
