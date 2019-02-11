/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
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
    if (err != PSA_ATTEST_ERR_SUCCESS)
    {
        return err;
    }

    return err;
}
