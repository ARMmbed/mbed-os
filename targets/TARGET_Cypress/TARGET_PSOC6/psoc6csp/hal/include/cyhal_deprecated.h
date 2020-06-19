/***************************************************************************//**
* \file cyhal_deprecated.h
*
* \brief
* Provides access to items that are device specific and no longer part of the
* common HAL API.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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

#pragma once

#include "cy_pdl.h"
#include "cyhal_general_types.h"
#include "cyhal_hw_resources.h"
#include "cy_result.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* \addtogroup group_hal_psoc6_deprecated Deprecated
* \ingroup group_hal_psoc6
* \{
* The following PSoC 6 specific items have been deprecated and replaced by more generic item. Each item
* will continue to work for now, but will be removed in a future release. All deprecated items reference
* the item that replaces it.
*/

/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_INVALID_CLK_DIVIDER (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 1))
/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_UNABLE_TO_SET_CLK_FREQ (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 2))
/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_SRC_CLK_DISABLED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 3))
/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_NO_VALID_DIVIDER (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 4))

/** Enum for clock type to configure. HFCLKs are configured using different APIs and does not using this enum.
 *  \warning This type is deprecated. Use \ref cyhal_clock_block_t instead.
 */
typedef enum
{
    CYHAL_SYSTEM_CLOCK_CM4,
    CYHAL_SYSTEM_CLOCK_CM0,
    CYHAL_SYSTEM_CLOCK_PERI,
} cyhal_system_clock_t;

/** Available clock divider types.
 * \warning This type is deprecated. Use \ref cyhal_clock_block_t instead.
 */
typedef cy_en_divider_types_t cyhal_clock_divider_types_t;

/** Divider for CM4, CM0 and Peri clock. Supports values between [1, 256]
 *  \warning This type is deprecated. Use \ref cyhal_clock_t instead.
 */
typedef uint16_t cyhal_system_divider_t;

/** Peripheral clock divider type.
 *  \warning This type is deprecated. Use \ref cyhal_clock_t instead.
 */
typedef cyhal_clock_t cyhal_clock_divider_t;

/** Allocate (pick and reserve) an Clock Divider resource and provide a reference to it.
 *
 * \warning This function is deprecated. Use \ref cyhal_clock_allocate() instead.
 *
 * @param[out] obj           The resource object that was allocated
 * @param[in]  div           The type of divider to allocate
 * @param[in]  accept_larger Whether a larger type can be provided if there are none of the requested size available
 * @return The status of the reserve request
 */
cy_rslt_t cyhal_hwmgr_allocate_clock(cyhal_clock_divider_t* obj, cyhal_clock_divider_types_t div, bool accept_larger) /* __attribute__ ((deprecated)) */;

/** Free the specified Clock Divider resource and allow it be used by something else.
 *
 * \warning This function is deprecated. Use \ref cyhal_clock_free() instead.
 *
 * @param[in] obj           The resource object that was allocated
 */
void cyhal_hwmgr_free_clock(cyhal_clock_divider_t* obj) /* __attribute__ ((deprecated)) */;

/** Gets the specified clock's current frequency.
 *
 * \warning This function is deprecated. Use \ref cyhal_clock_get_frequency() instead.
 *
 * @param[in]  clock        ID of clock to configure
 * @param[out] frequency_hz The frequency the clock is currently running at
 * @return The status of the get_frequency request
 */
cy_rslt_t cyhal_system_clock_get_frequency(uint8_t clock, uint32_t *frequency_hz) /* __attribute__ ((deprecated)) */;

/** Sets the specified clock's frequency and enables it. This will turn on any
 * additional clocks needed to drive this.
 *
 * \warning This function is deprecated. Use \ref cyhal_clock_set_frequency() instead.
 *
 * @param[in]  clock        ID of clock to configure
 * @param[in]  frequency_hz The frequency to run the clock at
 * @return The status of the set_frequency request
 */
cy_rslt_t cyhal_system_clock_set_frequency(uint8_t clock, uint32_t frequency_hz) /* __attribute__ ((deprecated)) */;

/** Divides the clock frequency by the divider.
 *
 * \warning This function is deprecated. Use \ref cyhal_clock_set_divider() instead.
 *
 * @param[in] clock   The clock to configure divider value for
 * @param[in] divider The divider value to divide the frequency by
 * @return The status of the set_divider request
 */
cy_rslt_t cyhal_system_clock_set_divider(cyhal_system_clock_t clock, cyhal_system_divider_t divider) /* __attribute__ ((deprecated)) */;

/** \} group_hal_psoc6_deprecated */

#if defined(__cplusplus)
}
#endif /* __cplusplus */
