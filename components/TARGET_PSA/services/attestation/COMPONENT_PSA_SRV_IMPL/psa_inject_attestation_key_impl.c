/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa_inject_attestation_key_impl.h"

#define ECDSA_P256_KEY_SIZE_IN_BYTES 32

psa_status_t
psa_attestation_inject_key_impl(const uint8_t *key_data,
                                size_t key_data_length,
                                psa_key_type_t type,
                                uint8_t *public_key_data,
                                size_t public_key_data_size,
                                size_t *public_key_data_length)
{
    psa_status_t status = PSA_SUCCESS;
    size_t key_data_bits = 0;
    psa_key_handle_t handle = 1;
    psa_key_id_t key_id = 17;
    psa_key_lifetime_t lifetime = PSA_KEY_LIFETIME_PERSISTENT;
    psa_key_policy_t policy = PSA_KEY_POLICY_INIT;
    psa_key_usage_t usage = PSA_KEY_USAGE_EXPORT | PSA_KEY_USAGE_SIGN;
    psa_key_type_t public_type;
    size_t bits;
    size_t exported_size = 0;
    psa_key_type_t type_key;

#if defined(MBEDTLS_ECP_C)

    status = psa_crypto_init();
    if (status != PSA_SUCCESS) {
        return (status);
    }

    status = psa_create_key(lifetime, key_id, &handle);
    if (status != PSA_SUCCESS) {
        return (status);
    }

    psa_key_policy_init();
    psa_key_policy_set_usage(&policy, usage, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
    status = psa_set_key_policy(handle, &policy);
    if (status != PSA_SUCCESS) {
        return (status);
    }

    if (! PSA_KEY_TYPE_IS_ECC_KEYPAIR(type)) {
        return (PSA_ERROR_INVALID_ARGUMENT);
    }

    if (key_data != NULL) {
        if (key_data_length != ECDSA_P256_KEY_SIZE_IN_BYTES) {
            status = PSA_ERROR_INVALID_ARGUMENT;
            goto exit;
        }
        status = psa_import_key(handle, type, key_data, key_data_length);
        if (status != PSA_SUCCESS) {
            goto exit;
        }
    } else {
        /* generating key pair */
        key_data_bits = ECDSA_P256_KEY_SIZE_IN_BYTES * 8;
        status = psa_generate_key(handle, type, key_data_bits, NULL, 0);
        if (status != PSA_SUCCESS) {
            goto exit;
        }
    }

    status = psa_get_key_information(handle, &type_key, &bits);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    public_type = PSA_KEY_TYPE_PUBLIC_KEY_OF_KEYPAIR(type_key);
    exported_size = PSA_KEY_EXPORT_MAX_SIZE(public_type, bits);

    status = psa_export_public_key(handle,
                                   public_key_data,
                                   public_key_data_size,
                                   public_key_data_length);
    if (status != PSA_SUCCESS) {
        goto exit;
    }
    if (*public_key_data_length > exported_size) {
        status = PSA_ERROR_INVALID_ARGUMENT;
        goto exit;
    }

exit:
    psa_close_key(handle);
    return (status);
#endif /* MBEDTLS_ECP_C */

    return (PSA_ERROR_NOT_SUPPORTED);
}
