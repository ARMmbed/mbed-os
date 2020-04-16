/***************************************************************************//**
* \file cyhal_wdt.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Watchdog Timer.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2019 Cypress Semiconductor Corporation
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
* \addtogroup group_hal_wdt WDT (Watchdog Timer)
* \ingroup group_hal
* \{
* High level interface to the Watchdog Timer (WDT).
*
* cyhal_wdt_init() initializes the WDT and passes a pointer to the WDT block through obj.
* The timeout_ms parameter takes in the timeout in milliseconds.
* It can be used for recovering from a CPU or firmware failure.
  The watchdog timer is initialized with a timeout interval. Once the WDT is started,
  if cyhal_wdt_kick() must be called at least once within the timeout interval. In case
  the firmware fails to do so, it is considered to be a CPU crash or firmware failure and the device
  will be reset.
*
*
*\section subsection_wdt_features Features
* WDT supports Device Reset generation if not serviced within the configured timeout interval.
*
*
* \section subsection_wdt_quickstart Quick Start
*
* \ref cyhal_wdt_init() can be used for initialization by providing the WDT object (obj) and the timeout parameter
* (timeout period in ms).
* The timeout parameter can have a minimum value of 1ms. The maximum value of the timeout
* parameter can be obtained using the cyhal_wdt_get_max_timeout_ms().
*
*
* \section subsection_wdt_sample_use_case Sample use case
*
* \subsection subsection_wdt_use_case Use Case: Initialization and reset functionality
* The following snippet initializes the WDT and depicts the reset functionality of WDT in case of CPU or
* firmware failure.
* \snippet wdt.c snippet_cyhal_wdt_init_and_reset
*/

#pragma once

#include "cyhal_hwmgr.h"
#include "cy_result.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** WDT timeout out of range */
#define CY_RSLT_WDT_INVALID_TIMEOUT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_WDT, 0))
/** WDT already initialized */
#define CY_RSLT_WDT_ALREADY_INITIALIZED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_WDT, 1))

/** Initialize and start the WDT
*
* Initialize or re-initialize the WDT.
* \attention The specified timeout must be at least 1ms and at most the WDT's maximum timeout (see cyhal_wdt_get_max_timeout_ms()).
*
* @param[out] obj The WDT object
* @param[in] timeout_ms The time in milliseconds before the WDT times out (1ms - max) (see cyhal_wdt_get_max_timeout_ms())
* @return The status of the init request
*
* Returns \ref CY_RSLT_SUCCESS if the operation was successfull.
*/
cy_rslt_t cyhal_wdt_init(cyhal_wdt_t *obj, uint32_t timeout_ms);

/** Deinitialize the WDT
*
* Powers down the WDT.
* After calling this function no other WDT functions should be called except
* cyhal_wdt_init(). Calling any function other than init after freeing is
* undefined.
*
* @param[inout] obj The WDT object
*
*/

void cyhal_wdt_free(cyhal_wdt_t *obj);

/** Refresh the WDT
*
* Refreshes the WDT.  This function should be called periodically to prevent the WDT from timing out.
* In the event of a timeout, the WDT resets the system.
*
* @param[inout] obj The WDT object
*
* See \ref subsection_wdt_use_case
*/
void cyhal_wdt_kick(cyhal_wdt_t *obj);

/** Start the WDT
*
* Enables the WDT.
*
* @param[inout] obj The WDT object
* @return The status of the start request
*/
void cyhal_wdt_start(cyhal_wdt_t *obj);

/** Stop the WDT
*
* Disables the WDT.
*
* @param[inout] obj The WDT object
* @return The status of the stop request
*/
void cyhal_wdt_stop(cyhal_wdt_t *obj);

/** Get the WDT timeout
*
* Gets the time in milliseconds before the WDT times out.
*
* @param[inout] obj The WDT object
* @return The time in milliseconds before the WDT times out
*/
uint32_t cyhal_wdt_get_timeout_ms(cyhal_wdt_t *obj);

/** Gets the maximum WDT timeout
*
* Gets the maximum timeout for the WDT.
*
* @return The maximum timeout for the WDT
*/
uint32_t cyhal_wdt_get_max_timeout_ms(void);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_WDT_IMPL_HEADER
#include CYHAL_WDT_IMPL_HEADER
#endif /* CYHAL_WDT_IMPL_HEADER */

/** \} group_hal_wdt */
