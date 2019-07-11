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

/**
* \addtogroup group_hal_hwmgr HWMGR (Hardware Manager)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress Hardware Manager.
*
* \defgroup group_hal_hwmgr_macros Macros
* \defgroup group_hal_hwmgr_functions Functions
* \defgroup group_hal_hwmgr_data_structures Data Structures
* \defgroup group_hal_hwmgr_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_hal_hwmgr_enums
* \{
*/

/** Enum to in indicate which module an errors occurred in. */
enum cyhal_rslt_module_chip
{
    CYHAL_RSLT_MODULE_CHIP_HWMGR = CY_RSLT_MODULE_ABSTRACTION_HAL_BASE, //!< An error occurred in hardware management module
    CYHAL_RSLT_MODULE_ADC,                                              //!< An error occurred in ADC module
    CYHAL_RSLT_MODULE_COMP,                                             //!< An error occurred in comparator module
    CYHAL_RSLT_MODULE_CRC,                                              //!< An error occurred in crypto CRC module
    CYHAL_RSLT_MODULE_DAC,                                              //!< An error occurred in DAC module
    CYHAL_RSLT_MODULE_DMA,                                              //!< An error occurred in DMA module
    CYHAL_RSLT_MODULE_FLASH,                                            //!< An error occurred in flash module
    CYHAL_RSLT_MODULE_GPIO,                                             //!< An error occurred in GPIO module
    CYHAL_RSLT_MODULE_I2C,                                              //!< An error occurred in I2C module
    CYHAL_RSLT_MODULE_I2S,                                              //!< An error occurred in I2S module
    CYHAL_RSLT_MODULE_INTERCONNECT,                                     //!< An error occurred in Interconnct module
    CYHAL_RSLT_MODULE_OPAMP,                                            //!< An error occurred in OpAmp module
    CYHAL_RSLT_MODULE_PDMPCM,                                           //!< An error occurred in PDM/PCM module
    CYHAL_RSLT_MODULE_PWM,                                              //!< An error occurred in PWM module
    CYHAL_RSLT_MODULE_QSPI,                                             //!< An error occurred in QSPI module
    CYHAL_RSLT_MODULE_RTC,                                              //!< An error occurred in RTC module
    CYHAL_RSLT_MODULE_SDHC,                                             //!< An error occurred in SDHC module
    CYHAL_RSLT_MODULE_SDIO,                                             //!< An error occurred in SDIO module
    CYHAL_RSLT_MODULE_SPI,                                              //!< An error occurred in SPI module
    CYHAL_RSLT_MODULE_SYSTEM,                                           //!< An error occurred in System module
    CYHAL_RSLT_MODULE_TIMER,                                            //!< An error occurred in Timer module
    CYHAL_RSLT_MODULE_TRNG,                                             //!< An error occurred in RNG module
    CYHAL_RSLT_MODULE_UART,                                             //!< An error occurred in UART module
    CYHAL_RSLT_MODULE_USB,                                              //!< An error occurred in USB module
    CYHAL_RSLT_MODULE_WDT,                                              //!< An error occurred in WDT module
};

/** \} group_hal_hwmgr_enums */


/**
* \addtogroup group_hal_hwmgr_macros
* \{
*/

/** The requested resource type is invalid */
#define CYHAL_HWMGR_RSLT_ERR_INVALID (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_CHIP_HWMGR, 0))
/** The requested resource is already in use */
#define CYHAL_HWMGR_RSLT_ERR_INUSE (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_CHIP_HWMGR, 1))
/** No resources of the requested type are available */
#define CYHAL_HWMGR_RSLT_ERR_NONE_FREE (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_CHIP_HWMGR, 2))

/** Attempt to free a resource that was not used */
#define CYHAL_HWMGR_RSLT_WARN_UNUSED (CY_RSLT_CREATE(CY_RSLT_TYPE_WARNING, CYHAL_RSLT_MODULE_CHIP_HWMGR, 50))

/** \} group_hal_hwmgr_macros */


/**
* \addtogroup group_hal_hwmgr_functions
* \{
*/

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

/** Reserve the specified resource.
 *
 * @param[in]  type The type of resource that should be reserved
 * @param[out] obj  The resource object that was be reserved
 * @return The status of the allocate request
 */
cy_rslt_t cyhal_hwmgr_allocate(cyhal_resource_t type, cyhal_resource_inst_t* obj);

/** Allocates a free block of the specified type if available
 *
 * @param[in]  type The type of resource to allocate
 * @param[out] obj  The resource object to free
 * @return The status of the allocate request
 */
cy_rslt_t cyhal_hwmgr_allocate(cyhal_resource_t type, cyhal_resource_inst_t* obj);

/** Allocate (pick and reserve) an DMA resource and provide a reference to it.
 *
 * @param[out] obj The resource object that was allocated
 * @return The status of the reserve request
 */
cy_rslt_t cyhal_hwmgr_allocate_dma(cyhal_resource_inst_t* obj);

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

/** Marks the specified resource as having already been configured (eg: it doesn't need to be configured again).
 *
 * @param[in]  type    The type of hardware block to reserve
 * @param[in]  block   The block number of to reserve
 * @param[in]  channel The block's channel instance number to reserve (0 if there are no channels in the block)
 * @return The status of the set request
 */
cy_rslt_t cyhal_hwmgr_set_configured(cyhal_resource_t type, uint8_t block, uint8_t channel);

/** Marks the specified resource as not having already been configured (eg: it still needs to be configured before being used).
 *
 * @param[in]  type    The type of hardware block to reserve
 * @param[in]  block   The block number of to reserve
 * @param[in]  channel The block's channel instance number to reserve (0 if there are no channels in the block)
 * @return The status of the set request
 */
cy_rslt_t cyhal_hwmgr_set_unconfigured(cyhal_resource_t type, uint8_t block, uint8_t channel);

/** Checks to see if the specified resource has already been configured (eg: it doesn't need to be configured again).
 *
 * @param[in]  type    The type of hardware block to reserve
 * @param[in]  block   The block number of to reserve
 * @param[in]  channel The block's channel instance number to reserve (0 if there are no channels in the block)
 * @return Whether the block is currently configured
 */
bool cyhal_hwmgr_is_configured(cyhal_resource_t type, uint8_t block, uint8_t channel);

/** \} group_hal_hwmgr_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_hwmgr */
