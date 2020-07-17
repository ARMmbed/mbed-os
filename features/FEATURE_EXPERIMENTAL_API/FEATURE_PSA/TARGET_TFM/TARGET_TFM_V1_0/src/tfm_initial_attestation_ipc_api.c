/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/initial_attestation.h"
#include "tfm_ns_interface.h"
#include "psa/client.h"
#include "psa/crypto_types.h"
#include "psa_manifest/sid.h"

#define IOVEC_LEN(x) (sizeof(x)/sizeof(x[0]))

psa_status_t
psa_initial_attest_get_token(const uint8_t *auth_challenge,
                             size_t         challenge_size,
                             uint8_t       *token_buf,
                             size_t         token_buf_size,
                             size_t        *token_size)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_status_t status;

    psa_invec in_vec[] = {
        {auth_challenge, challenge_size}
    };
    psa_outvec out_vec[] = {
        {token_buf, token_buf_size}
    };

    handle = psa_connect(TFM_ATTEST_GET_TOKEN_SID,
                         TFM_ATTEST_GET_TOKEN_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_HANDLE_TO_ERROR(handle);
    }

    status = psa_call(handle, PSA_IPC_CALL,
                      in_vec, IOVEC_LEN(in_vec),
                      out_vec, IOVEC_LEN(out_vec));
    psa_close(handle);

    if (status == PSA_SUCCESS) {
        *token_size = out_vec[0].len;
    }

    return status;
}

psa_status_t
psa_initial_attest_get_token_size(size_t  challenge_size,
                                  size_t *token_size)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_status_t status;
    psa_invec in_vec[] = {
        {&challenge_size, sizeof(challenge_size)}
    };
    psa_outvec out_vec[] = {
        {token_size, sizeof(size_t)}
    };

    handle = psa_connect(TFM_ATTEST_GET_TOKEN_SIZE_SID,
                         TFM_ATTEST_GET_TOKEN_SIZE_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_HANDLE_TO_ERROR(handle);
    }

    status = psa_call(handle, PSA_IPC_CALL,
                      in_vec, IOVEC_LEN(in_vec),
                      out_vec, IOVEC_LEN(out_vec));
    psa_close(handle);

    return status;
}

psa_status_t
tfm_initial_attest_get_public_key(uint8_t         *public_key,
                                  size_t           public_key_buf_size,
                                  size_t          *public_key_len,
                                  psa_ecc_curve_t *elliptic_curve_type)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_status_t status;

    psa_outvec out_vec[] = {
        {.base = public_key,          .len = public_key_buf_size},
        {.base = elliptic_curve_type, .len = sizeof(*elliptic_curve_type)},
        {.base = public_key_len,      .len = sizeof(*public_key_len)}
    };

    handle = psa_connect(TFM_ATTEST_GET_PUBLIC_KEY_SID,
                         TFM_ATTEST_GET_PUBLIC_KEY_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_HANDLE_TO_ERROR(handle);
    }

    status = psa_call(handle, PSA_IPC_CALL,
                      NULL, 0,
                      out_vec, IOVEC_LEN(out_vec));
    psa_close(handle);

    return status;
}
