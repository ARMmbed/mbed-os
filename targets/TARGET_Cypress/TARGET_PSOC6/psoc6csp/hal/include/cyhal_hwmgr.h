/***************************************************************************//**
* \file cyhal_hwmgr.h
*
* \brief
* Provides a high level interface for managing hardware resources. This is
* used to track what hardware blocks are already being used so they are not over
* allocated.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
* \addtogroup group_hal_hwmgr HWMGR (Hardware Manager)
* \ingroup group_hal
* \{
* High level interface for interacting with the Hardware Manager.
*
* This provides two related functions:
* * Allows HAL drivers (or application firmware) to mark specific hardware blocks
*   as consumed, so that other firmware will not accidentally try to use the block
*   for a conflicting purpose.
* * For resources which are interchangeable, such as clock dividers, provides allocation
*   and reservation of an available instance (if one exists).
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"
#include "cyhal_modules.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** The requested resource type is invalid */
#define CYHAL_HWMGR_RSLT_ERR_INVALID (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_CHIP_HWMGR, 0))
/** The requested resource is already in use */
#define CYHAL_HWMGR_RSLT_ERR_INUSE (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_CHIP_HWMGR, 1))
/** No resources of the requested type are available */
#define CYHAL_HWMGR_RSLT_ERR_NONE_FREE (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_CHIP_HWMGR, 2))

/** Attempt to free a resource that was not used */
#define CYHAL_HWMGR_RSLT_WARN_UNUSED (CY_RSLT_CREATE(CY_RSLT_TYPE_WARNING, CYHAL_RSLT_MODULE_CHIP_HWMGR, 50))

/** Initializes the hardware manager to keep track of what resources are being used.
 *
 * @return The status of the init request
 */
cy_rslt_t cyhal_hwmgr_init();

/** Reserve the specified resource.
 *
 * @param[in] obj  The resource object that should be reserved
 * @return The status of the reserve request
 */
cy_rslt_t cyhal_hwmgr_reserve(const cyhal_resource_inst_t* obj);

/** Free the specified resource to allow it to be used by something else.
 *
 * @param[in,out] obj The resource object to free
 */
void cyhal_hwmgr_free(const cyhal_resource_inst_t* obj);

/** Allocates a free block of the specified type if available
 *
 * @param[in]  type The type of resource to allocate
 * @param[out] obj  The resource object that was allocated
 * @return The status of the allocate request
 */
cy_rslt_t cyhal_hwmgr_allocate(cyhal_resource_t type, cyhal_resource_inst_t* obj);

/** Allocate (pick and reserve) an Clock Divider resource and provide a reference to it.
 *
 * @param[out] obj           The resource object that was allocated
 * @param[in]  div           The type of divider to allocate
 * @param[in]  accept_larger Whether a larger type can be provided if there are none of the requested size available
 * @return The status of the reserve request
 */
cy_rslt_t cyhal_hwmgr_allocate_clock(cyhal_clock_divider_t* obj, cyhal_clock_divider_types_t div, bool accept_larger);

/** Free the specified Clock Divider resource and allow it be used by something else.
 *
 * @param[in] obj           The resource object that was allocated
 */
void cyhal_hwmgr_free_clock(cyhal_clock_divider_t* obj);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_HWMGR_IMPL_HEADER
#include CYHAL_HWMGR_IMPL_HEADER
#endif /* CYHAL_HWMGR_IMPL_HEADER */

/** \} group_hal_hwmgr */
