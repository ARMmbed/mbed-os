/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_ATTEST_HAL_H__
#define __TFM_ATTEST_HAL_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Security lifecycle of the device
 */
enum tfm_security_lifecycle_t {
    TFM_SLC_UNKNOWN                   = 0x0000u,
    TFM_SLC_ASSEMBLY_AND_TEST         = 0x1000u,
    TFM_SLC_PSA_ROT_PROVISIONING      = 0x2000u,
    TFM_SLC_SECURED                   = 0x3000u,
    TFM_SLC_NON_PSA_ROT_DEBUG         = 0x4000u,
    TFM_SLC_RECOVERABLE_PSA_ROT_DEBUG = 0x5000u,
    TFM_SLC_DECOMMISSIONED            = 0x6000u,
};

/**
 * \brief Retrieve the security lifecycle of the device
 *
 * Security lifecycle is a mandatory claim in the initial attestation token.
 *
 * \return According to \ref tfm_security_lifecycle_t
 */
enum tfm_security_lifecycle_t tfm_attest_hal_get_security_lifecycle(void);

/**
 * \brief Retrieve the verification service indicator for initial attestation.
 *
 * It is used by relying party to locate a validation service for the token.
 * It can be a text string that can be used to locate the service or can be a
 * URL specifying the address of the service.
 *
 * \param[out]  size  Length of the string, without the termination zero byte.
 *
 * \return NULL pointer if not available otherwise the address of the
 *         verification service string in the device memory.
 */
const char *
tfm_attest_hal_get_verification_service(uint32_t *size);

/**
 * \brief Retrieve the name of the profile definition document for initial
 *        attestation.
 *
 *  This document describes the 'profile' of the initial attestation token,
 *  being a full description of the claims, their usage, verification and
 *  token signing.
 *
 * \param[out]  size  Length of the document name, without the termination zero
 *                    byte.
 *
 * \return NULL pointer if not available otherwise the address of the document
 *         name string in the device memory.
 */
const char *
tfm_attest_hal_get_profile_definition(uint32_t *size);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_ATTEST_HAL_H__ */
