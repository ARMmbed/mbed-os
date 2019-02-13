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

#include "t_cose/src/t_cose_crypto.h"
#include "tfm_plat_defs.h"
#include "crypto.h"
#include "tfm_plat_crypto_keys.h"
#include <string.h>

static psa_hash_operation_t hash_handle;

enum t_cose_err_t
t_cose_crypto_pub_key_sign(int32_t cose_alg_id,
                           int32_t key_select,
                           struct useful_buf_c hash_to_sign,
                           struct useful_buf signature_buffer,
                           struct useful_buf_c *signature) {
    enum t_cose_err_t cose_ret = T_COSE_SUCCESS;
    psa_status_t crypto_ret;

    (void)key_select;

    const psa_key_id_t key_id = 17;
    psa_key_handle_t handle = 0;

    crypto_ret = psa_crypto_init();
    if (crypto_ret != PSA_SUCCESS)
    {
        return T_COSE_ERR_HASH_GENERAL_FAIL;
    }

    crypto_ret = psa_open_key(PSA_KEY_LIFETIME_PERSISTENT, key_id, &handle);
    if (crypto_ret != PSA_SUCCESS)
    {
        return T_COSE_ERR_NO_KID;
    }

    crypto_ret = psa_asymmetric_sign(handle,
                                     PSA_ALG_ECDSA(PSA_ALG_SHA_256),
                                     hash_to_sign.ptr,
                                     hash_to_sign.len,
                                     signature_buffer.ptr,
                                     signature_buffer.len,
                                     &(signature->len));


    if (crypto_ret != PSA_SUCCESS)
    {
        psa_close_key(handle);
        cose_ret = T_COSE_ERR_UNKNOWN_SIGNING_ALG;
    } else
    {
        signature->ptr = signature_buffer.ptr;
    }

    psa_close_key(handle);
    return cose_ret;
}

enum t_cose_err_t
t_cose_crypto_get_ec_pub_key(int32_t key_select,
                             int32_t *cose_curve_id,
                             struct useful_buf buf_to_hold_x_coord,
                             struct useful_buf buf_to_hold_y_coord,
                             struct useful_buf_c  *x_coord,
                             struct useful_buf_c  *y_coord) {

    enum tfm_plat_err_t err;
    enum ecc_curve_t cose_curve;
    struct ecc_key_t attest_key = {0};
    uint8_t  key_buf[ECC_P_256_KEY_SIZE];

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

enum t_cose_err_t
t_cose_crypto_hash_start(struct t_cose_crypto_hash *hash_ctx,
                         int32_t cose_hash_alg_id) {
    enum t_cose_err_t cose_ret = T_COSE_SUCCESS;
    psa_status_t crypto_ret;

    crypto_ret = psa_hash_setup(&hash_handle, PSA_ALG_SHA_256);

    if (crypto_ret == PSA_ERROR_NOT_SUPPORTED)
    {
        cose_ret = T_COSE_ERR_UNSUPPORTED_HASH;
    } else if (crypto_ret != PSA_SUCCESS)
    {
        cose_ret = T_COSE_ERR_HASH_GENERAL_FAIL;
    }

    return cose_ret;
}

void t_cose_crypto_hash_update(struct t_cose_crypto_hash *hash_ctx,
                               struct useful_buf_c data_to_hash)
{
    if (data_to_hash.ptr != NULL) {
        psa_hash_update(&hash_handle, data_to_hash.ptr, data_to_hash.len);
    } else {
        /* Intentionally do nothing, just computing the size of the token */
    }
}

enum t_cose_err_t
t_cose_crypto_hash_finish(struct t_cose_crypto_hash *hash_ctx,
                          struct useful_buf buffer_to_hold_result,
                          struct useful_buf_c *hash_result) {
    enum t_cose_err_t cose_ret = T_COSE_SUCCESS;
    psa_status_t crypto_ret;

    crypto_ret = psa_hash_finish(&hash_handle,
                                 buffer_to_hold_result.ptr,
                                 buffer_to_hold_result.len,
                                 &(hash_result->len));

    if (crypto_ret == PSA_ERROR_BUFFER_TOO_SMALL)
    {
        cose_ret = T_COSE_ERR_HASH_BUFFER_SIZE;
    } else if (crypto_ret != PSA_SUCCESS)
    {
        cose_ret = T_COSE_ERR_HASH_GENERAL_FAIL;
    }

    if (cose_ret == T_COSE_SUCCESS)
    {
        hash_result->ptr = buffer_to_hold_result.ptr;
    }

    return crypto_ret;
}
