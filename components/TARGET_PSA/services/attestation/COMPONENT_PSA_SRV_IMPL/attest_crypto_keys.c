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

#include "tfm_plat_crypto_keys.h"
#include "psa/crypto.h"

#include <stdlib.h>
#include <math.h>


#define ONE_BYTE (1u)
#define PSA_ATTESTATION_PRIVATE_KEY_ID 17

/**
 * \brief Copy the key to the destination buffer
 *
 * \param[out]  p_dst  Pointer to buffer where to store the key
 * \param[in]   p_src  Pointer to the key
 * \param[in]   size   Length of the key
 */
static inline void copy_key(uint8_t *p_dst, const uint8_t *p_src, size_t size)
{
    uint32_t i;

    for (i = size; i > 0; i--) {
        *p_dst = *p_src;
        p_src++;
        p_dst++;
    }
}

static psa_status_t get_curve(psa_key_type_t type, enum ecc_curve_t *curve_type)
{
    psa_ecc_curve_t curve = PSA_KEY_TYPE_GET_CURVE(type);
    switch (curve) {
        case PSA_ECC_CURVE_SECP256R1:
            *curve_type = P_256;
            break;
        case PSA_ECC_CURVE_SECP384R1:
            *curve_type = P_384;
            break;
        case PSA_ECC_CURVE_SECP521R1:
            *curve_type = P_521;
            break;
        case PSA_ECC_CURVE_CURVE25519:
            *curve_type = X25519;
            break;
        case PSA_ECC_CURVE_CURVE448:
            *curve_type = X448;
            break;
        default:
            return (PSA_ERROR_NOT_SUPPORTED);
    }

    return PSA_SUCCESS;
}

enum tfm_plat_err_t
tfm_plat_get_initial_attest_key(uint8_t          *key_buf,
                                uint32_t          size,
                                struct ecc_key_t *ecc_key,
                                enum ecc_curve_t *curve_type)

{
    uint8_t *key_dst = NULL;
    uint8_t *key_src;
    uint32_t key_size;

    psa_status_t crypto_ret;

    uint8_t *public_key = NULL;
    psa_key_type_t type;
    psa_key_type_t public_type;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    size_t bits;
    size_t public_key_size = 0;
    size_t public_key_length = 0;

    uint32_t initial_attestation_public_x_key_size = 0;
    uint32_t initial_attestation_public_y_key_size = 0;

    psa_key_handle_t handle;

    crypto_ret = psa_open_key(PSA_ATTESTATION_PRIVATE_KEY_ID, &handle);
    if (crypto_ret != PSA_SUCCESS)
    {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    crypto_ret = psa_get_key_attributes(handle, &attributes);
    if (crypto_ret != PSA_SUCCESS)
    {
        psa_close_key(handle);
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    type = psa_get_key_type(&attributes);
    if (!PSA_KEY_TYPE_IS_ECC(type))
    {
        psa_close_key(handle);
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    public_type = PSA_KEY_TYPE_PUBLIC_KEY_OF_KEY_PAIR(type);
    bits = psa_get_key_bits(&attributes);
    public_key_size = PSA_KEY_EXPORT_MAX_SIZE(public_type, bits);
    public_key = (uint8_t *) malloc(public_key_size);
    if (public_key == NULL)
    {
        psa_close_key(handle);
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    crypto_ret = psa_export_public_key(handle,
                                       public_key, public_key_size,
                                       &public_key_length);
    if (crypto_ret != PSA_SUCCESS)
    {
        free(public_key);
        psa_close_key(handle);
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    /* Set the EC curve type which the key belongs to */
    crypto_ret = get_curve(type, curve_type);
    if (crypto_ret != PSA_SUCCESS)
    {
        free(public_key);
        psa_close_key(handle);
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    key_src = public_key;
    key_dst  = key_buf;

    /* The representation of an ECC public key is:
    ** -The byte 0x04
    ** - `x_P` as a `ceiling(m/8)`-byte string, big-endian
    ** - `y_P` as a `ceiling(m/8)`-byte string, big-endian
    ** - where m is the bit size associated with the curve
    ** - 1 byte + 2 * point size
    */
    initial_attestation_public_x_key_size = ceil((public_key_length - 1) / 2);
    initial_attestation_public_y_key_size = ceil((public_key_length - 1) / 2);

    /* Copy the x-coordinate of public key to the buffer */
    if (initial_attestation_public_x_key_size != 0)
    {
        key_src = key_src + ONE_BYTE;
        key_size = initial_attestation_public_x_key_size;
        copy_key(key_dst, key_src, key_size);
        ecc_key->pubx_key = key_dst;
        ecc_key->pubx_key_size = key_size;
        key_dst  = key_dst + key_size;
    } else
    {
        ecc_key->pubx_key = NULL;
        ecc_key->pubx_key_size = 0;
    }

    /* Copy the y-coordinate of public key to the buffer */
    if (initial_attestation_public_y_key_size != 0)
    {
        key_src += initial_attestation_public_x_key_size;
        key_size = initial_attestation_public_y_key_size;
        copy_key(key_dst, key_src, key_size);
        ecc_key->puby_key = key_dst;
        ecc_key->puby_key_size = key_size;
    } else
    {
        ecc_key->puby_key = NULL;
        ecc_key->puby_key_size = 0;
    }

    free(public_key);
    psa_close_key(handle);
    return TFM_PLAT_ERR_SUCCESS;
}
