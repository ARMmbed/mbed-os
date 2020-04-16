/***************************************************************************//**
* \file cyhal_tcpwm_common.h
*
* \brief
* Code shared between the Cypress Timer/Counter and PWM.
*
********************************************************************************
* \copyright
* Copyright 2019-2020 Cypress Semiconductor Corporation
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
 * \addtogroup group_hal_psoc6_tcpwm_common TCPWM Common Functionality
 * \ingroup group_hal_psoc6
 * \{
 * Code shared between the Cypress Timer / Counter and PWM.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cyhal_hw_types.h"

/** Handler for TCPWM interrupts */
typedef void(*cyhal_tcpwm_event_callback_t)(void *callback_arg, int event);

/** Holds data about a single TCPWM */
typedef struct {
    TCPWM_Type*  base; //!< TCPWM base
    en_clk_dst_t clock_dst; //!< TCPWM clock connections base
    uint32_t     max_count; //!< TCPWM counter width
    uint8_t      num_channels; //!< Number of channels on the TCPWM
    uint8_t      channel_offset; //!< Offset from channels on previous TCPWM
    uint8_t      isr_offset; //!< TCPWM base IRQn (channel 0 IRQn)
} cyhal_tcpwm_data_t;

/** Contains data about all of the TCPWMs */
extern const cyhal_tcpwm_data_t CYHAL_TCPWM_DATA[CY_IP_MXTCPWM_INSTANCES];

/** Initialize a timer/counter or PWM object's callback data.
 *
 * @param[in]     resource      The timer/counter or PWM resource
 * @param[in,out] callback_data The callback data object belonging to the timer/counter or PWM
 */
void cyhal_tcpwm_init_callback_data(cyhal_resource_inst_t *resource, cyhal_event_callback_data_t *callback_data);

/** The TCPWM interrupt handler registration
 *
 * @param[in] resource      The timer/counter or PWM resource
 * @param[in] callback      The callback handler which will be invoked when the event occurs
 * @param[in] callback_arg  Generic argument that will be provided to the callback when called
 */
void cyhal_tcpwm_register_callback(cyhal_resource_inst_t *resource, cy_israddress callback, void *callback_arg);

/** Configure TCPWM event enablement.
 *
 * @param[in] type          The type of the timer/counter or PWM
 * @param[in] resource      The timer/counter or PWM resource
 * @param[in] event         The timer/counter or PWM event type
 * @param[in] intrPriority  The priority for NVIC interrupt events
 * @param[in] enable        True to turn on events, False to turn off
 */
void cyhal_tcpwm_enable_event(TCPWM_Type *type, cyhal_resource_inst_t *resource, uint32_t event, uint8_t intrPriority, bool enable);

/** \} group_hal_psoc6_tcpwm_common */
/** \endcond */
