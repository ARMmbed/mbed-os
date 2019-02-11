/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ATTESTATION_KEY_H__
#define __ATTESTATION_KEY_H__

#include "psa_initial_attestation_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \def ATTEST_PRIVATE_KEY_SLOT
 *
 * \brief Key slot number to store the initial attestation private key.
 *
 * Private key is used by initial attestation service to sign the
 * initial attestation token (IAT).
 */
#define ATTEST_PRIVATE_KEY_SLOT (1u)

/**
 * \def ATTEST_PUBLIC_KEY_SLOT
 *
 * \brief Key slot number to store the initial attestation public key.
 *
 * Public key is used by initial attestation test suit to verify the signature
 * of the initial attestation token (IAT).
 */
#define ATTEST_PUBLIC_KEY_SLOT  (2u)

/**
 * \brief Get the initial attestation key from platform layer and register it
 *        to crypto service for further usage (signing or verification).
 *
 * Private key MUST be present on the device, public key is optional.
 *
 * \retval  PSA_ATTEST_ERR_SUCCESS   Key(s) was registered.
 * \retval  PSA_ATTEST_ERR_GENERAL   Key(s) could not be registered.
 */
enum psa_attest_err_t attest_register_initial_attestation_key(void);

/**
 * \brief Unregister the initial attestation key(s) from crypto service to do
 *        not occupy key slot(s).
 *
 * \retval  PSA_ATTEST_ERR_SUCCESS   Key(s) was unregistered.
 * \retval  PSA_ATTEST_ERR_GENERAL   Key(s) could not be unregistered.
 */
enum psa_attest_err_t attest_unregister_initial_attestation_key(void);

#ifdef __cplusplus
}
#endif

#endif /* __ATTESTATION_KEY_H__ */
