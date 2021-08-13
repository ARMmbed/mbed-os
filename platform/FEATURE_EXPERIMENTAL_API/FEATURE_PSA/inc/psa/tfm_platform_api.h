/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLATFORM_API__
#define __TFM_PLATFORM_API__

#include <limits.h>
#include <stdint.h>
#include "psa/client.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief TFM secure partition platform API version
 */
#define TFM_PLATFORM_API_VERSION_MAJOR (0)
#define TFM_PLATFORM_API_VERSION_MINOR (3)

/*!
 * \enum tfm_platform_err_t
 *
 * \brief Platform service error types
 *
 */
enum tfm_platform_err_t {
    TFM_PLATFORM_ERR_SUCCESS = 0,
    TFM_PLATFORM_ERR_SYSTEM_ERROR,
    TFM_PLATFORM_ERR_INVALID_PARAM,
    TFM_PLATFORM_ERR_NOT_SUPPORTED,

    /* Following entry is only to ensure the error code of int size */
    TFM_PLATFORM_ERR_FORCE_INT_SIZE = INT_MAX
};

typedef int32_t tfm_platform_ioctl_req_t;

/*!
 * \brief Performs a platform-specific service
 *
 * \param[in]  request      Request identifier (valid values vary
 *                          based on the platform)
 * \param[in]  input        Input buffer to the requested service (or NULL)
 * \param[in,out] output    Output buffer to the requested service (or NULL)
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t tfm_platform_ioctl(tfm_platform_ioctl_req_t request,
                                           psa_invec *input,
                                           psa_outvec *output);


#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLATFORM_API__ */
