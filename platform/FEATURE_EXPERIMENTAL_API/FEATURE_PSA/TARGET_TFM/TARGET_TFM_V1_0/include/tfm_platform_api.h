/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLATFORM_API__
#define __TFM_PLATFORM_API__

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include "tfm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief TFM secure partition platform API version
 */
#define TFM_PLATFORM_API_VERSION_MAJOR (0)
#define TFM_PLATFORM_API_VERSION_MINOR (3)

#define TFM_PLATFORM_API_ID_NV_READ       (1010)
#define TFM_PLATFORM_API_ID_NV_INCREMENT  (1011)

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
 * \brief Resets the system.
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t tfm_platform_system_reset(void);

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

/*!
 * \brief Increments the given non-volatile (NV) counter by one
 *
 * \param[in]  counter_id  NV counter ID.
 *
 * \return  TFM_PLATFORM_ERR_SUCCESS if the value is read correctly. Otherwise,
 *          it returns TFM_PLATFORM_ERR_SYSTEM_ERROR.
 */
enum tfm_platform_err_t
tfm_platform_nv_counter_increment(uint32_t counter_id);

/*!
 * \brief Reads the given non-volatile (NV) counter
 *
 * \param[in]  counter_id  NV counter ID.
 * \param[in]  size        Size of the buffer to store NV counter value
 *                         in bytes.
 * \param[out] val         Pointer to store the current NV counter value.
 *
 * \return  TFM_PLATFORM_ERR_SUCCESS if the value is read correctly. Otherwise,
 *          it returns TFM_PLATFORM_ERR_SYSTEM_ERROR.
 */
enum tfm_platform_err_t
tfm_platform_nv_counter_read(uint32_t counter_id,
                             uint32_t size, uint8_t *val);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLATFORM_API__ */
