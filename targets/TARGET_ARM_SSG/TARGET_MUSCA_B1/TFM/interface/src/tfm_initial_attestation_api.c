/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa_initial_attestation_api.h"
#include "tfm_veneers.h"
#include "tfm_ns_lock.h"
#include "psa_client.h"

enum psa_attest_err_t
psa_initial_attest_get_token(const uint8_t *challenge_obj,
                             uint32_t       challenge_size,
                             uint8_t       *token,
                             uint32_t      *token_size)
{
    psa_invec  in_vec[1];
    psa_outvec out_vec[1];
    uint32_t res;

    in_vec[0].base = challenge_obj;
    in_vec[0].len  = challenge_size;

    out_vec[0].base = token;
    out_vec[0].len  = *token_size;

    res = tfm_ns_lock_dispatch((veneer_fn)tfm_initial_attest_get_token_veneer,
                                (uint32_t)in_vec,  1,
                                (uint32_t)out_vec, 1);

    *token_size = out_vec[0].len;

    return res;
}

enum psa_attest_err_t
psa_initial_attest_get_token_size(uint32_t  challenge_size,
                                  uint32_t *token_size)
{
    psa_invec  in_vec[1];
    psa_outvec out_vec[1];

    in_vec[0].base = &challenge_size;
    in_vec[0].len  = sizeof(uint32_t);

    out_vec[0].base = token_size;
    out_vec[0].len  = sizeof(uint32_t);

    return tfm_ns_lock_dispatch((veneer_fn) \
                                tfm_initial_attest_get_token_size_veneer,
                                (uint32_t)in_vec,  1,
                                (uint32_t)out_vec, 1);
}
