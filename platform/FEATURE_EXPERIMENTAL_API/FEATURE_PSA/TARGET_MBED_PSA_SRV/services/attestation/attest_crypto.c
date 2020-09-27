/*
 * attest_crypto.c
 *
 * Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "t_cose_crypto.h"
#include "tfm_plat_defs.h"
#include "psa/crypto.h"
#include "tfm_plat_crypto_keys.h"
#include <string.h>

#define PSA_ATTESTATION_PRIVATE_KEY_ID 17

/**
 * \brief Context for PSA hash adaptation.
 *
 * Hash context for PSA hash implementation. This is fit into and cast
 * to/from struct \ref t_cose_crypto_hash.
 */
struct t_cose_psa_crypto_hash {
    psa_status_t         status;
    psa_hash_operation_t operation;
};

enum t_cose_err_t
t_cose_crypto_pub_key_sign(int32_t cose_alg_id,
                           int32_t key_select,
                           struct useful_buf_c hash_to_sign,
                           struct useful_buf signature_buffer,
                           struct useful_buf_c *signature) {
    enum t_cose_err_t cose_ret = T_COSE_SUCCESS;
    psa_status_t crypto_ret;
    const size_t sig_size = t_cose_signature_size(cose_alg_id);

    (void)key_select;

    psa_key_handle_t handle;

    if (sig_size > signature_buffer.len)
    {
        return T_COSE_ERR_SIG_BUFFER_SIZE;
    }

    crypto_ret = psa_open_key(PSA_ATTESTATION_PRIVATE_KEY_ID, &handle);
    if (crypto_ret != PSA_SUCCESS)
    {
        return T_COSE_ERR_NO_KID;
    }

    crypto_ret = psa_sign_hash(handle,
                               PSA_ALG_DETERMINISTIC_ECDSA(PSA_ALG_SHA_256),
                               hash_to_sign.ptr,
                               hash_to_sign.len,
                               signature_buffer.ptr,
                               signature_buffer.len,
                               &(signature->len));


    if (crypto_ret != PSA_SUCCESS)
    {
        psa_close_key(handle);
        cose_ret = T_COSE_ERR_UNSUPPORTED_SIGNING_ALG;
    } else
    {
        signature->ptr = signature_buffer.ptr;
    }

    psa_close_key(handle);
    return cose_ret;
}

enum t_cose_err_t
t_cose_crypto_get_ec_pub_key(int32_t key_select,
                             struct useful_buf_c kid,
                             int32_t *cose_curve_id,
                             struct useful_buf buf_to_hold_x_coord,
                             struct useful_buf buf_to_hold_y_coord,
                             struct useful_buf_c  *x_coord,
                             struct useful_buf_c  *y_coord) {

    enum tfm_plat_err_t err;
    enum ecc_curve_t cose_curve;
    struct ecc_key_t attest_key = {0};
    uint8_t key_buf[ECC_P_256_KEY_SIZE] = {0};

    (void)key_select;

    /* Get the initial attestation key */
    err = tfm_plat_get_initial_attest_key(key_buf, sizeof(key_buf),
                                          &attest_key, &cose_curve);

    /* Check the availability of the private key */
    if (err != TFM_PLAT_ERR_SUCCESS ||
            attest_key.pubx_key == NULL ||
            attest_key.puby_key == NULL)
    {
        return T_COSE_ERR_KEY_BUFFER_SIZE;
    }

    *cose_curve_id = (int32_t)cose_curve;

    /* Check buffer size to avoid overflow */
    if (buf_to_hold_x_coord.len < attest_key.pubx_key_size)
    {
        return T_COSE_ERR_KEY_BUFFER_SIZE;
    }

    /* Copy the X coordinate of the public key to the buffer */
    memcpy(buf_to_hold_x_coord.ptr,
           (const void *)attest_key.pubx_key,
           attest_key.pubx_key_size);

    /* Update size */
    buf_to_hold_x_coord.len = attest_key.pubx_key_size;

    /* Check buffer size to avoid overflow */
    if (buf_to_hold_y_coord.len < attest_key.puby_key_size)
    {
        return T_COSE_ERR_KEY_BUFFER_SIZE;
    }

    /* Copy the Y coordinate of the public key to the buffer */
    memcpy(buf_to_hold_y_coord.ptr,
           (const void *)attest_key.puby_key,
           attest_key.puby_key_size);

    /* Update size */
    buf_to_hold_y_coord.len = attest_key.puby_key_size;

    x_coord->ptr = buf_to_hold_x_coord.ptr;
    x_coord->len = buf_to_hold_x_coord.len;
    y_coord->ptr = buf_to_hold_y_coord.ptr;
    y_coord->len = buf_to_hold_y_coord.len;

    return T_COSE_SUCCESS;
}

/**
 * \brief Check some of the sizes for hash implementation.
 *
 * \return  Value from \ref t_cose_err_t error if sizes are not correct.
 *
 * It makes sure the constants in the header file match the local
 * implementation.  This gets evaluated at compile time and will
 * optimize out to nothing when all checks pass.
 */
static inline enum t_cose_err_t check_hash_sizes()
{
    if (T_COSE_CRYPTO_SHA256_SIZE != PSA_HASH_SIZE(PSA_ALG_SHA_256)) {
        return T_COSE_ERR_HASH_GENERAL_FAIL;
    }

    return T_COSE_SUCCESS;
}

/**
 * \brief Convert COSE algorithm ID to a PSA algorithm ID
 *
 * \param[in] cose_hash_alg_id   The COSE-based ID for the
 *
 * \return PSA-based hash algorithm ID, or MD4 in the case of error.
 *
 */
static inline psa_algorithm_t cose_hash_alg_id_to_psa(int32_t cose_hash_alg_id)
{
    psa_algorithm_t return_value;

    switch (cose_hash_alg_id) {
        case COSE_ALG_SHA256_PROPRIETARY:
            return_value = PSA_ALG_SHA_256;
            break;
        default:
            return_value = PSA_ALG_MD4;
            break;
    }

    return return_value;
}

enum t_cose_err_t
t_cose_crypto_hash_start(struct t_cose_crypto_hash *hash_ctx,
                         int32_t cose_hash_alg_id) {
    enum t_cose_err_t cose_ret = T_COSE_SUCCESS;
    psa_status_t psa_ret;
    struct t_cose_psa_crypto_hash *psa_hash_ctx;

    /* These next 3 lines optimize to nothing except when there is
     * failure.
     */
    cose_ret = check_hash_sizes();
    if (cose_ret != T_COSE_SUCCESS)
    {
        return cose_ret;
    }

    psa_hash_ctx = (struct t_cose_psa_crypto_hash *)hash_ctx;
    memset(&psa_hash_ctx->operation, 0, sizeof(psa_hash_operation_t));

    psa_ret = psa_hash_setup(&psa_hash_ctx->operation,
                             cose_hash_alg_id_to_psa(cose_hash_alg_id));

    if (psa_ret == PSA_SUCCESS)
    {
        psa_hash_ctx->status = PSA_SUCCESS;
        cose_ret = T_COSE_SUCCESS;
    } else if (psa_ret == PSA_ERROR_NOT_SUPPORTED)
    {
        cose_ret = T_COSE_ERR_UNSUPPORTED_HASH;
    } else
    {
        cose_ret = T_COSE_ERR_HASH_GENERAL_FAIL;
    }

    return cose_ret;
}

void t_cose_crypto_hash_update(struct t_cose_crypto_hash *hash_ctx,
                               struct useful_buf_c data_to_hash)
{
    struct t_cose_psa_crypto_hash *psa_hash_ctx;

    psa_hash_ctx = (struct t_cose_psa_crypto_hash *)hash_ctx;

    if (psa_hash_ctx->status == PSA_SUCCESS) {
        if (data_to_hash.ptr != NULL) {
            psa_hash_ctx->status = psa_hash_update(&psa_hash_ctx->operation,
                                                   data_to_hash.ptr,
                                                   data_to_hash.len);
        }
    }
}

enum t_cose_err_t
t_cose_crypto_hash_finish(struct t_cose_crypto_hash *hash_ctx,
                          struct useful_buf buffer_to_hold_result,
                          struct useful_buf_c *hash_result) {
    enum t_cose_err_t cose_ret = T_COSE_SUCCESS;
    psa_status_t psa_ret;
    struct t_cose_psa_crypto_hash *psa_hash_ctx;

    psa_hash_ctx = (struct t_cose_psa_crypto_hash *)hash_ctx;

    if (psa_hash_ctx->status == PSA_SUCCESS)
    {
        psa_ret = psa_hash_finish(&psa_hash_ctx->operation,
                                  buffer_to_hold_result.ptr,
                                  buffer_to_hold_result.len,
                                  &(hash_result->len));

        if (psa_ret == PSA_SUCCESS) {
            hash_result->ptr = buffer_to_hold_result.ptr;
            cose_ret = T_COSE_SUCCESS;
        } else if (psa_ret == PSA_ERROR_BUFFER_TOO_SMALL) {
            cose_ret = T_COSE_ERR_HASH_BUFFER_SIZE;
        } else {
            cose_ret = T_COSE_ERR_HASH_GENERAL_FAIL;
        }
    } else
    {
        cose_ret = T_COSE_ERR_HASH_GENERAL_FAIL;
    }

    return cose_ret;
}
