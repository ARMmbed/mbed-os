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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "tfm_plat_boot_seed.h"
#include "attestation_bootloader_data.h"
#include "tfm_attest_hal.h"
#include "psa_initial_attestation_api.h"
#include "attestation.h"
#include "psa/crypto.h"
#include "psa/lifecycle.h"

extern int32_t g_caller_id;

#define ATTEST_PUB_KEY_SHA_256_SIZE (32u)
#define PSA_ATTESTATION_PRIVATE_KEY_ID 17

static enum tfm_security_lifecycle_t security_lifecycle_psa_to_tfm(void)
{
    uint32_t lc = psa_security_lifecycle_state();
    switch (lc) {
        case PSA_LIFECYCLE_UNKNOWN:
            return TFM_SLC_UNKNOWN;
        case PSA_LIFECYCLE_ASSEMBLY_AND_TEST:
            return TFM_SLC_ASSEMBLY_AND_TEST;
        case PSA_LIFECYCLE_PSA_ROT_PROVISIONING:
            return TFM_SLC_PSA_ROT_PROVISIONING;
        case PSA_LIFECYCLE_SECURED:
            return TFM_SLC_SECURED;
        case PSA_LIFECYCLE_NON_PSA_ROT_DEBUG:
            return TFM_SLC_NON_PSA_ROT_DEBUG;
        case PSA_LIFECYCLE_RECOVERABLE_PSA_ROT_DEBUG:
            return TFM_SLC_RECOVERABLE_PSA_ROT_DEBUG;
        case PSA_LIFECYCLE_DECOMMISSIONED:
            return TFM_SLC_DECOMMISSIONED;
        default:
            return TFM_SLC_UNKNOWN;
    }
}

/* Hash of attestation public key */
static enum tfm_plat_err_t attest_public_key_sha256(uint32_t *size, uint8_t *buf)
{
    psa_key_handle_t handle = 0;

    uint8_t *public_key = NULL;
    psa_key_type_t type;
    psa_key_type_t public_type;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    size_t bits;
    size_t public_key_size = 0;
    size_t public_key_length = 0;

    psa_status_t crypto_ret;
    enum tfm_plat_err_t status = TFM_PLAT_ERR_SUCCESS;
    psa_hash_operation_t hash_handle = {0};

    crypto_ret = psa_open_key(PSA_ATTESTATION_PRIVATE_KEY_ID, &handle);
    if (crypto_ret != PSA_SUCCESS) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    crypto_ret = psa_get_key_attributes(handle, &attributes);
    if (crypto_ret != PSA_SUCCESS) {
        status = TFM_PLAT_ERR_SYSTEM_ERR;
        goto exit;
    }
    type = psa_get_key_type(&attributes);
    if (!PSA_KEY_TYPE_IS_ECC(type)) {
        status = TFM_PLAT_ERR_SYSTEM_ERR;
        goto exit;
    }
    public_type = PSA_KEY_TYPE_PUBLIC_KEY_OF_KEY_PAIR(type);
    bits = psa_get_key_bits(&attributes);
    public_key_size = PSA_KEY_EXPORT_MAX_SIZE(public_type, bits);
    public_key = (uint8_t *) malloc(public_key_size);
    if (public_key == NULL) {
        status = TFM_PLAT_ERR_SYSTEM_ERR;
        goto exit;
    }

    crypto_ret = psa_export_public_key(handle,
                                       public_key, public_key_size,
                                       &public_key_length);
    if (crypto_ret != PSA_SUCCESS) {
        status = TFM_PLAT_ERR_SYSTEM_ERR;
        goto exit;
    }

    crypto_ret = psa_hash_setup(&hash_handle, PSA_ALG_SHA_256);
    if (crypto_ret != PSA_SUCCESS) {
        status = TFM_PLAT_ERR_SYSTEM_ERR;
        goto exit;
    }

    psa_hash_update(&hash_handle, public_key, public_key_length);

    crypto_ret = psa_hash_finish(&hash_handle,
                                 buf,
                                 ATTEST_PUB_KEY_SHA_256_SIZE,
                                 (size_t *) size);
    if (crypto_ret != PSA_SUCCESS) {
        status = TFM_PLAT_ERR_SYSTEM_ERR;
        goto exit;
    }

exit:
    if (public_key != NULL) {
        free(public_key);
    }
    psa_close_key(handle);
    return status;
}

/**
 * \brief Copy the device specific ID to the destination buffer
 *
 * \param[out]  p_dst  Pointer to buffer where to store ID
 * \param[in]   p_src  Pointer to the ID
 * \param[in]   size   Length of the ID
 */
static inline void copy_id(uint8_t *p_dst, uint8_t *p_src, size_t size)
{
    uint32_t i;

    for (i = size; i > 0; i--) {
        *p_dst = *p_src;
        p_src++;
        p_dst++;
    }
}

enum psa_attest_err_t attest_get_caller_client_id(int32_t *caller_id)
{
    *caller_id = g_caller_id;
    return PSA_ATTEST_ERR_SUCCESS;
}

/* Boot seed data is part of bootloader status*/
enum tfm_plat_err_t tfm_plat_get_boot_seed(uint32_t size, uint8_t *buf)
{
    return (enum tfm_plat_err_t)PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
}

/**
 * Instance ID is mapped to EAT Universal Entity ID (UEID)
 * This implementation creates the instance ID as follows:
 *  - byte 0:    0x01 indicates the type of UEID to be GUID
 *  - byte 1-32: Hash of attestation public key. Public key is hashed in raw
 *               format without any encoding.
 */
enum tfm_plat_err_t tfm_plat_get_instance_id(uint32_t *size, uint8_t *buf)
{
    enum tfm_plat_err_t status;

    buf[0] = 0x01; /* First byte is type byte:  0x01 indicates GUID */

    status = attest_public_key_sha256(size, &buf[1]);

    /* Instance ID size:  1 type byte + size of public key hash */
    *size = *size + 1;

    return status;
}

/* HW version data is part of bootloader status*/
enum tfm_plat_err_t tfm_plat_get_hw_version(uint32_t *size, uint8_t *buf)
{
    return (enum tfm_plat_err_t)PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
}

enum tfm_plat_err_t tfm_plat_get_implementation_id(uint32_t *size, uint8_t *buf)
{
    memcpy(buf, impl_id_data, *size);
    return (enum tfm_plat_err_t)PSA_ATTEST_ERR_SUCCESS;
}

/* Temporary Implementation of security lifecycle data: mandatory claim.
** tfm_attest_hal_get_security_lifecycle function should return
** 'PSA_ATTEST_ERR_CLAIM_UNAVAILABLE' if been called.
** Security lifecycle data is part of bootloader status data.
** Temp implementation of using psa_security_lifecycle_state */

enum tfm_security_lifecycle_t tfm_attest_hal_get_security_lifecycle(void)
{
    return security_lifecycle_psa_to_tfm();
}


const char *
tfm_attest_hal_get_verification_service(uint32_t *size)
{
    *size = sizeof(verification_service_url) - 1;
    return verification_service_url;
}

const char *
tfm_attest_hal_get_profile_definition(uint32_t *size)
{
    *size = sizeof(attestation_profile_definition) - 1;
    return attestation_profile_definition;
}


