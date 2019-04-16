/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLATFORM_VENEERS_H__
#define __TFM_PLATFORM_VENEERS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tfm_platform_api.h"

/*!
 * \brief Resets the system.
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t tfm_platform_veneer_system_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLATFORM_VENEERS_H__ */
