/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "tfm_plat_boot_seed.h"
#include "attestation_bootloader_data.h"
#include "tfm_attest_hal.h"

#include "psa_initial_attestation_api.h"
#include "attestation.h"
#include "crypto.h"

extern int32_t g_caller_id;

#define ATTEST_PUB_KEY_SHA_256_SIZE (32u)

/* Hash of attestation public key */
static enum tfm_plat_err_t attest_public_key_sha256(uint32_t *size, uint8_t *buf)
{
    const psa_key_id_t key_id = 17;
    psa_key_handle_t handle = 0;

    uint8_t *public_key = NULL;
    psa_key_type_t type;
    psa_key_type_t public_type;
    size_t bits;
    size_t public_key_size = 0;
    size_t public_key_length = 0;

    psa_status_t crypto_ret;
    psa_hash_operation_t hash_handle;

    crypto_ret = psa_crypto_init();
    if (crypto_ret != PSA_SUCCESS) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    crypto_ret = psa_open_key(PSA_KEY_LIFETIME_PERSISTENT, key_id, &handle);
    if (crypto_ret != PSA_SUCCESS) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    crypto_ret = psa_get_key_information(handle, &type, &bits);
    if (crypto_ret != PSA_SUCCESS) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    if (!PSA_KEY_TYPE_IS_ECC(type)) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    public_type = PSA_KEY_TYPE_PUBLIC_KEY_OF_KEYPAIR(type);
    public_key_size = PSA_KEY_EXPORT_MAX_SIZE(public_type, bits);
    public_key = (uint8_t *) malloc(public_key_size);
    if (public_key == NULL) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    crypto_ret = psa_export_public_key(handle,
                                       public_key, public_key_size,
                                       &public_key_length);
    if (crypto_ret != PSA_SUCCESS) {
        free(public_key);
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    crypto_ret = psa_hash_setup(&hash_handle, PSA_ALG_SHA_256);
    if (crypto_ret != PSA_SUCCESS) {
        free(public_key);
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    psa_hash_update(&hash_handle, public_key, public_key_length);

    crypto_ret = psa_hash_finish(&hash_handle,
                                 buf,
                                 *size,
                                 (size_t *) size);
    if (crypto_ret != PSA_SUCCESS) {
        free(public_key);
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    free(public_key);
    return TFM_PLAT_ERR_SUCCESS;
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

enum tfm_plat_err_t tfm_plat_get_boot_seed(uint32_t size, uint8_t *buf)
{
    return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
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
    uint8_t *p_dst;
    uint8_t p_src[ATTEST_PUB_KEY_SHA_256_SIZE];
    uint32_t p_src_size = ATTEST_PUB_KEY_SHA_256_SIZE;

    buf[0] = 0x01; /* First byte is type byte:  0x01 indicates GUID */
    p_dst = &buf[1];

    status = attest_public_key_sha256(&p_src_size, p_src);

    copy_id(p_dst, p_src, p_src_size);

    /* Instance ID size:  1 type byte + size of public key hash */
    *size = p_src_size + 1;

    return status;
}

enum tfm_plat_err_t tfm_plat_get_hw_version(uint32_t *size, uint8_t *buf)
{
    return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
}

enum tfm_plat_err_t tfm_plat_get_implementation_id(uint32_t *size, uint8_t *buf)
{
    memcpy(buf, impl_id_data, *size);
    return PSA_ATTEST_ERR_SUCCESS;
}

enum tfm_security_lifecycle_t tfm_attest_hal_get_security_lifecycle(void)
{
    return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
}

const char *
tfm_attest_hal_get_verification_service(uint32_t *size)
{
    return NULL;
}

const char *
tfm_attest_hal_get_profile_definition(uint32_t *size)
{
    return NULL;
}


