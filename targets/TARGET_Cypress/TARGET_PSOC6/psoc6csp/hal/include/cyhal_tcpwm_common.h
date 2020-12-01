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
 * \addtogroup group_hal_impl_tcpwm_common TCPWM Common Functionality
 * \ingroup group_hal_impl
 * \{
 * Code shared between the Cypress Timer / Counter and PWM.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#if defined(CY_IP_MXTCPWM_INSTANCES) || defined(CY_IP_M0S8TCPWM_INSTANCES)

#if defined(CY_IP_MXTCPWM_INSTANCES)
    #if (CY_IP_MXTCPWM_VERSION == 1)
        #define _CYHAL_TCPWM_INSTANCES     CY_IP_MXTCPWM_INSTANCES
        #define _CYHAL_TCPWM_CNT_NUMBER(resource) ((resource).channel_num)
    #else // (CY_IP_MXTCPWM_VERSION >= 2)
        #define _CYHAL_TCPWM_INSTANCES     TCPWM_GRP_NR
        #define _CYHAL_TCPWM_CNT_NUMBER(resource) (((resource).block_num << 8) | (resource).channel_num)
    #endif
#elif defined(CY_IP_M0S8TCPWM_INSTANCES)
    #define _CYHAL_TCPWM_CNT_NUMBER(resource) ((resource).channel_num)
    #define _CYHAL_TCPWM_INSTANCES     CY_IP_M0S8TCPWM_INSTANCES
#endif

/** Handler for TCPWM interrupts */
typedef void(*_cyhal_tcpwm_event_callback_t)(void *callback_arg, int event);

/** Holds data about a single TCPWM */
typedef struct {
    TCPWM_Type*  base; //!< TCPWM base
    en_clk_dst_t clock_dst; //!< TCPWM clock connections base
    uint32_t     max_count; //!< TCPWM counter width
    uint8_t      num_channels; //!< Number of channels on the TCPWM
    uint8_t      channel_offset; //!< Offset from channels on previous TCPWM
    uint8_t      isr_offset; //!< TCPWM base IRQn (channel 0 IRQn)
} _cyhal_tcpwm_data_t;

/** Contains data about all of the TCPWMs */
extern const _cyhal_tcpwm_data_t _CYHAL_TCPWM_DATA[_CYHAL_TCPWM_INSTANCES];

/**
 * Free a timer/counter or a PWM object's shared data
 *
 * @param[in] obj The timer/counter or the PWM resource
 */
void _cyhal_tcpwm_free(cyhal_tcpwm_t *obj);

/** Initialize a timer/counter or PWM object's callback data.
 *
 * @param[in,out] tcpwm    The shared data struct between timer/counter and PWM
 */
void _cyhal_tcpwm_init_data(cyhal_tcpwm_t *tcpwm);

/** The TCPWM interrupt handler registration
 *
 * @param[in] resource      The timer/counter or PWM resource
 * @param[in] callback      The callback handler which will be invoked when the event occurs
 * @param[in] callback_arg  Generic argument that will be provided to the callback when called
 */
void _cyhal_tcpwm_register_callback(cyhal_resource_inst_t *resource, cy_israddress callback, void *callback_arg);

/** Configure TCPWM event enablement.
 *
 * @param[in] type          The type of the timer/counter or PWM
 * @param[in] resource      The timer/counter or PWM resource
 * @param[in] event         The timer/counter or PWM event type
 * @param[in] intr_priority The priority for NVIC interrupt events
 * @param[in] enable        True to turn on events, False to turn off
 */
void _cyhal_tcpwm_enable_event(TCPWM_Type *type, cyhal_resource_inst_t *resource, uint32_t event, uint8_t intr_priority, bool enable);

/** Returns whether power management transition should be allowed.
 *
 * @return true if no tcpwm is actively blocking power mode transition
 */
bool _cyhal_tcpwm_pm_transition_pending(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* defined(CY_IP_MXTCPWM_INSTANCES) || defined(CY_IP_M0S8TCPWM_INSTANCES) */

/** \} group_hal_impl_tcpwm_common */
/** \endcond */
