/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLATFORM_API__
#define __TFM_PLATFORM_API__

#include <limits.h>
#include <stdbool.h>
#include "tfm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief TFM secure partition platform API version
 */
#define TFM_PLATFORM_API_VERSION_MAJOR (0)
#define TFM_PLATFORM_API_VERSION_MINOR (2)

/* The return value is shared with the TF-M partition status value.
 * The Platform return codes shouldn't overlap with predefined TFM status
 * values.
 */
#define TFM_PLATFORM_ERR_OFFSET (TFM_PARTITION_SPECIFIC_ERROR_MIN)

/*!
 * \enum tfm_platform_err_t
 *
 * \brief Platform service error types
 *
 */
enum tfm_platform_err_t {
    TFM_PLATFORM_ERR_SUCCESS = 0,
    TFM_PLATFORM_ERR_SYSTEM_ERROR = TFM_PLATFORM_ERR_OFFSET,
    TFM_PLATFORM_ERR_INVALID_PARAM,

    /* Following entry is only to ensure the error code of int size */
    TFM_PLATFORM_ERR_FORCE_INT_SIZE = INT_MAX
};

/*!
 * \brief Resets the system.
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t tfm_platform_system_reset(void);

/*!
 * \brief Sets pin alternate function for the given pins
 *
 * \param[in]  alt_func     Alternate function to set (allowed values vary
 *                          based on the platform)
 * \param[in]  pin_mask     Pin mask of the selected pins
 * \param[out] result       Return error value
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t
tfm_platform_set_pin_alt_func(uint32_t alt_func, uint64_t pin_mask,
                              uint32_t *result);

/*!
 * \brief Sets default in value to use when the alternate function is not
 *        selected for the pin
 *
 * \param[in]  alt_func          Alternate function to use (allowed values vary
 *                               based on the platform)
 * \param[in]  pin_value         Pin value to use
 * \param[in]  default_in_value  Default in value to set
 * \param[out] result            Return error value
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t
tfm_platform_set_pin_default_in(uint32_t alt_func, uint32_t pin_value,
                                bool default_in_value, uint32_t *result);

/*!
 * \brief Sets pin mode for the selected pins
 *
 * \param[in]  pin_mask     Pin mask of the selected pins
 * \param[in]  pin_mode     Pin mode to set for the selected pins
 * \param[out] result       Return error value
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t
tfm_platform_set_pin_mode(uint64_t pin_mask, uint32_t pin_mode,
                          uint32_t *result);


#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLATFORM_API__ */
