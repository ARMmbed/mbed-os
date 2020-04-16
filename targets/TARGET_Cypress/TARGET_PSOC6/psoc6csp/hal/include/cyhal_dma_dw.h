/***************************************************************************//**
* \file cyhal_dma_dw.h
*
* \brief
* Defines a high level interface for interacting with the Cypress Datawire DMA.
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

#include "cyhal_dma.h"

/**
 * \addtogroup group_hal_psoc6_dma_dw DW (Datawire)
 * \ingroup group_hal_psoc6_dma
 * \{
 * Implementation specific interface for using the Datawire DMA peripheral
 */

#pragma once

#ifdef CY_IP_M4CPUSS_DMA

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/** Initialize the Datawire peripheral. 
 *
 * @param[out] obj          The DMA object to initialize
 * @param[in]  priority     The priority of this DMA operation relative to others. Values must be between 0-3 with 0 being the highest priority.
 * @return The status of the init request
 */
cy_rslt_t cyhal_dma_init_dw(cyhal_dma_t *obj, uint8_t priority);

/** Frees the Datawire specific DMA object 
 *
 * @param[in,out] obj The DMA object
 */
void cyhal_dma_free_dw(cyhal_dma_t *obj);

/** Setup a Datawire descriptor for the dma resource
 *
 * @param[in] obj    The DMA object
 * @param[in] cfg    Configuration prameters for the transfer
 * @return The status of the configure request
 */
cy_rslt_t cyhal_dma_configure_dw(cyhal_dma_t *obj, const cyhal_dma_cfg_t *cfg);

/** Start a Datawire transfer 
 *
 * Initiates DMA channel transfer for specified DMA object
 * @param[in] obj    The DMA object
 * @return The status of the start_transfer request
 */
cy_rslt_t cyhal_dma_start_transfer_dw(cyhal_dma_t *obj);

/** Configure Datawire event enablement.
 *
 * @param[in] obj           The DMA object
 * @param[in] event         The DMA event type
 * @param[in] intrPriority  The priority for NVIC interrupt events. The priority from the most recent call will take precedence, i.e all events will have the same priority.
 * @param[in] enable        True to turn on interrupts, False to turn off
 */
void cyhal_dma_enable_event_dw(cyhal_dma_t *obj, cyhal_dma_event_t event, uint8_t intrPriority, bool enable);

/** Checks whether a transfer is pending or running on the DMA channel
 *
 * @param[in] obj    The DMA object
 * @return True if DMA channel is busy
 */
bool cyhal_dma_is_busy_dw(cyhal_dma_t *obj);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CY_IP_M4CPUSS_DMA */

/** \} group_hal_psoc6_dma_dw */
