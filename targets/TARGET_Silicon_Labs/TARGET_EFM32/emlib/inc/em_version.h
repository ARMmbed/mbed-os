/***************************************************************************//**
 * @file
 * @brief Assign correct part number for include file
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef EM_VERSION_H
#define EM_VERSION_H

#include "em_device.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup VERSION
 * @brief Version API.
 * @details
 *  Macros specifying the emlib and CMSIS version.
 * @{
 ******************************************************************************/

/* *INDENT-OFF* */
/** Version number of targeted CMSIS package. */
#define _CMSIS_VERSION 5.3.0
/* *INDENT-ON* */

/** Major version of CMSIS. */
#define _CMSIS_VERSION_MAJOR 5

/** Minor version of CMSIS. */
#define _CMSIS_VERSION_MINOR 3

/** Patch revision of CMSIS. */
#define _CMSIS_VERSION_PATCH 0

/** @} (end addtogroup Version) */
/** @} (end addtogroup emlib) */

#ifdef __cplusplus
}
#endif

#endif /* EM_VERSION_H */
