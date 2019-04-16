/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
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
 * \brief This value is used to mark an handle as invalid.
 *
 */
#define TFM_CRYPTO_INVALID_HANDLE (0xFFFFFFFF)

/**
 * \brief Define miscellaneous literal constants that are used in the module
 *
 */
enum {
    TFM_CRYPTO_NOT_IN_USE = 0,
    TFM_CRYPTO_IN_USE = 1
};

/**
 * \brief Possible return values from the TFM Crypto service. They must
 *        provide corresponding return values for psa_status_t possible
 *        values as specified in psa_crypto.h
 */
enum tfm_crypto_err_t {
    TFM_CRYPTO_ERR_PSA_SUCCESS = 0,
    TFM_CRYPTO_ERR_PSA_ERROR_UNKNOWN_ERROR = TFM_CRYPTO_ERR_PSA_ERROR_OFFSET,
    TFM_CRYPTO_ERR_PSA_ERROR_NOT_SUPPORTED,
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

    /* Add an invalid return code which forces the size of the type as well */
    TFM_CRYPTO_ERR_INVALID = INT_MAX
};

/**
 * \brief A macro to translate TFM Crypto service return values to the
 *        corresponding psa_status_t value. The user of this macro needs
 *        to cast the produced value to psa_status_t explicitly if needed
 *
 * \return Values specified by \ref psa_status_t
 */
#define TFM_CRYPTO_ERR_TO_PSA_STATUS(val) \
      ( (val == TFM_CRYPTO_ERR_PSA_SUCCESS) ? val : \
        ((val >= (enum tfm_crypto_err_t)TFM_CRYPTO_ERR_PSA_ERROR_OFFSET) ? \
         (val - ((enum tfm_crypto_err_t)TFM_CRYPTO_ERR_PSA_ERROR_OFFSET-1)) : \
          TFM_CRYPTO_ERR_INVALID) )
/**
 * \brief A macro to translate psa_status_t values to the corresponding return
 *        values for the TFM Crypto service
 *
 * \return Values specified by \ref tfm_crypto_err_t
 *
 */
#define PSA_STATUS_TO_TFM_CRYPTO_ERR(val) \
      ( (val == PSA_SUCCESS) ? (enum tfm_crypto_err_t)val : \
              (enum tfm_crypto_err_t)(val + TFM_CRYPTO_ERR_PSA_ERROR_OFFSET-1) )

#ifdef __cplusplus
}
#endif

#endif /* __TFM_CRYPTO_DEFS_H__ */
