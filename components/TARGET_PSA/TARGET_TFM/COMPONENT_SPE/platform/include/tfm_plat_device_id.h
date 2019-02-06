/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLAT_DEVICE_ID_H__
#define __TFM_PLAT_DEVICE_ID_H__
/**
 * \file tfm_plat_device_id.h
 * Provide the Universal Entity ID (UEID) of the device.
 * It identifies the entire device or a submodule or subsystem. Must be
 * universally and globally unique and immutable. Variable length with a
 * maximum size of 33 bytes: 1 type byte and 256 bits.
 */

/**
 * \note The interfaces defined in this file must be implemented for each
 *       SoC.
 */

#include <stdint.h>
#include "tfm_plat_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \def DEVICE_ID_MAX_SIZE
 *
 * \brief Maximum size of device ID in bytes
 */
#define DEVICE_ID_MAX_SIZE (33u)

/**
 * \brief Get the UEID of the device.
 *
 * \param[in]  size The size of the buffer in bytes to store the UEID
 * \param[out] buf  Pointer to the buffer to store the UEID
 *
 * \return  The size of device ID in bytes, if buffer big enough to store the
 *          ID, otherwise -1.
 */
int32_t tfm_plat_get_device_id(uint32_t size, uint8_t *buf);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLAT_DEVICE_ID_H__ */
