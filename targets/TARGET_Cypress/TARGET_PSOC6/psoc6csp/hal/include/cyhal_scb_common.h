/***************************************************************************//**
* \file cyhal_scb_common.h
*
* \brief
* Provides a struct definitions for configuration resources in the SCB (UART, I2C, SPI).
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

/** \cond INTERNAL */
/**
 * \addtogroup group_hal_psoc6_scb_common SCB Common Functionality
 * \ingroup group_hal_psoc6
 * \{
 * Code shared between the SCB resources (UART, I2C, and SPI).
 */

#pragma once

#include "cy_device.h"
#include "cy_pdl.h"
#include "cyhal_utils.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** The start address of the SCB blocks */
extern CySCB_Type* const CYHAL_SCB_BASE_ADDRESSES[CY_IP_MXSCB_INSTANCES];
/** The interrupt number of the SCB blocks. */
extern const IRQn_Type CYHAL_SCB_IRQ_N[CY_IP_MXSCB_INSTANCES];

/** The configuration structs for the resource in use on each SCB block (e.g. cyhal_i2c_t) */
extern void *cyhal_scb_config_structs[CY_IP_MXSCB_INSTANCES];


/** Get the SCB block corresponding to an IRQn.
 *
 * @param[in] irqn The IRQn to get the corresponding block from
 * @return         The corresponding SCB block
 */
uint8_t cyhal_scb_get_block_from_irqn(IRQn_Type irqn);

/** Get the SCB object corresponding to the currently running ISR.
 *
 * @return A pointer to the SCB object corresponding to the currently running ISR.
 */
__STATIC_INLINE void *cyhal_scb_get_irq_obj(void)
{
    IRQn_Type irqn = CYHAL_GET_CURRENT_IRQN();
    uint8_t block = cyhal_scb_get_block_from_irqn(irqn);
    return cyhal_scb_config_structs[block];
}

#if defined(__cplusplus)
}
#endif

/** \} group_hal_psoc6_scb_common */
/** \endcond */
