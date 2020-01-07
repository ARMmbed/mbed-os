/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/initial_attestation.h"
#include "tfm_veneers.h"
#include "tfm_ns_interface.h"
#include "psa/client.h"
#include "psa_manifest/sid.h"

#define IOVEC_LEN(x) (sizeof(x)/sizeof(x[0]))

enum psa_attest_err_t
psa_initial_attest_get_token(const uint8_t *challenge_obj,
                             uint32_t       challenge_size,
                             uint8_t       *token,
                             uint32_t      *token_size)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_status_t status;

    psa_invec in_vec[] = {
        {challenge_obj, challenge_size}
    };
    psa_outvec out_vec[] = {
        {token, *token_size}
    };

    handle = psa_connect(TFM_ATTEST_GET_TOKEN_SID,
                         TFM_ATTEST_GET_TOKEN_VERSION);
    if (handle <= 0) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    status = psa_call(handle, PSA_IPC_CALL,
                      in_vec, IOVEC_LEN(in_vec),
                      out_vec, IOVEC_LEN(out_vec));
    psa_close(handle);

    if (status < PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    if (status == PSA_SUCCESS) {
        *token_size = out_vec[0].len;
    }

    return (enum psa_attest_err_t)status;
}

enum psa_attest_err_t
psa_initial_attest_get_token_size(uint32_t  challenge_size,
                                  uint32_t *token_size)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_status_t status;
    psa_invec in_vec[] = {
        {&challenge_size, sizeof(challenge_size)}
    };
    psa_outvec out_vec[] = {
        {token_size, sizeof(uint32_t)}
    };

    handle = psa_connect(TFM_ATTEST_GET_TOKEN_SIZE_SID,
                         TFM_ATTEST_GET_TOKEN_SIZE_VERSION);
    if (handle <= 0) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    status = psa_call(handle, PSA_IPC_CALL,
                      in_vec, IOVEC_LEN(in_vec),
                      out_vec, IOVEC_LEN(out_vec));
    psa_close(handle);

    if (status < PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    return (enum psa_attest_err_t)status;
}
