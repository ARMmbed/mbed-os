/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLATFORM_API__
#define __TFM_PLATFORM_API__

#include <limits.h>
#include "tfm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief TFM secure partition platform API version
 */
#define TFM_PLATFORM_API_VERSION_MAJOR (0)
#define TFM_PLATFORM_API_VERSION_MINOR (1)

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

    /* Following entry is only to ensure the error code of int size */
    TFM_PLATFORM_ERR_FORCE_INT_SIZE = INT_MAX
};

/*!
 * \brief Resets the system.
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t tfm_platform_system_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLATFORM_API__ */
