/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_CRYPTO_DEFS_H__
#define __TFM_CRYPTO_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <limits.h>
#include "tfm_api.h"

/* The return value is shared with the TFM service status value. The Crypto
 * return codes shouldn't overlap with predefined TFM status values.
 */
#define TFM_CRYPTO_ERR_PSA_ERROR_OFFSET (TFM_PARTITION_SPECIFIC_ERROR_MIN)

/**
 * \brief This defines the maximum supported key length in bytes
 *
 */
#define TFM_CRYPTO_MAX_KEY_LENGTH (16)

/**
  * \brief Define miscellaneous literal constants that are used in the module
  *
  */
enum {
    TFM_CRYPTO_NOT_IN_USE = 0,
    TFM_CRYPTO_IN_USE = 1
};

enum tfm_crypto_err_t {
    TFM_CRYPTO_ERR_PSA_SUCCESS = 0,
    TFM_CRYPTO_ERR_PSA_ERROR_NOT_SUPPORTED = TFM_CRYPTO_ERR_PSA_ERROR_OFFSET,
    TFM_CRYPTO_ERR_PSA_ERROR_NOT_PERMITTED,
    TFM_CRYPTO_ERR_PSA_ERROR_BUFFER_TOO_SMALL,
    TFM_CRYPTO_ERR_PSA_ERROR_OCCUPIED_SLOT,
    TFM_CRYPTO_ERR_PSA_ERROR_EMPTY_SLOT,
    TFM_CRYPTO_ERR_PSA_ERROR_BAD_STATE,
    TFM_CRYPTO_ERR_PSA_ERROR_INVALID_ARGUMENT,
    TFM_CRYPTO_ERR_PSA_ERROR_INSUFFICIENT_MEMORY,
    TFM_CRYPTO_ERR_PSA_ERROR_INSUFFICIENT_STORAGE,
    TFM_CRYPTO_ERR_PSA_ERROR_COMMUNICATION_FAILURE,
    TFM_CRYPTO_ERR_PSA_ERROR_STORAGE_FAILURE,
    TFM_CRYPTO_ERR_PSA_ERROR_HARDWARE_FAILURE,
    TFM_CRYPTO_ERR_PSA_ERROR_TAMPERING_DETECTED,
    TFM_CRYPTO_ERR_PSA_ERROR_INSUFFICIENT_ENTROPY,
    TFM_CRYPTO_ERR_PSA_ERROR_INVALID_SIGNATURE,
    TFM_CRYPTO_ERR_PSA_ERROR_INVALID_PADDING,
    TFM_CRYPTO_ERR_PSA_ERROR_UNKNOWN_ERROR,

    /* Add an invalid return code which forces the size of the type as well */
    TFM_CRYPTO_ERR_INVALID = INT_MAX
};

/**
 * \brief A macro to translate PSA API return values including the offset
 *        needed by TFM, to the corresponding PSA value
 */
#define TFM_CRYPTO_PSA_RETURN(val) \
                        ( (val == TFM_CRYPTO_ERR_PSA_SUCCESS) ? val : \
                         ((val >= TFM_CRYPTO_ERR_PSA_ERROR_NOT_SUPPORTED) ? \
                          (val - (TFM_CRYPTO_ERR_PSA_ERROR_NOT_SUPPORTED-1)) : \
                           TFM_CRYPTO_ERR_INVALID) )

#ifdef __cplusplus
}
#endif

#endif /* __TFM_CRYPTO_DEFS_H__ */
