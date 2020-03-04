/***************************************************************************//**
* \file cyhal_pwm_impl.h
*
* Description:
* Provides a high level interface for interacting with the Cypress PWM.
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

#pragma once

#include "cyhal_pwm.h"
#include "cyhal_tcpwm_common.h"

#if defined(CY_IP_MXTCPWM_INSTANCES)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

__STATIC_INLINE uint32_t cyhal_pwm_convert_event(cyhal_pwm_event_t event)
{
    uint32_t pdl_event = 0U;
    if (event & CYHAL_PWM_IRQ_TERMINAL_COUNT)
    {
        pdl_event |= CY_TCPWM_INT_ON_TC;
    }
    if (event & CYHAL_PWM_IRQ_CAPTURE_COMPARE)
    {
        pdl_event |= CY_TCPWM_INT_ON_CC;
    }
    return pdl_event;
}

__STATIC_INLINE void cyhal_pwm_register_callback_internal(cyhal_pwm_t *obj, cyhal_pwm_event_callback_t callback, void *callback_arg)
{
    cyhal_tcpwm_register_callback(&(obj->resource), (cy_israddress) callback, callback_arg);
}

#define cyhal_pwm_register_callback(obj, callback, callback_arg) cyhal_pwm_register_callback_internal(obj, callback, callback_arg)

__STATIC_INLINE void cyhal_pwm_enable_event_internal(cyhal_pwm_t *obj, cyhal_pwm_event_t event, uint8_t intrPriority, bool enable)
{
    uint32_t converted = cyhal_pwm_convert_event(event);
    cyhal_tcpwm_enable_event(obj->base, &(obj->resource), converted, intrPriority, enable);
}

#define cyhal_pwm_enable_event(obj, event, intrPriority, enable) cyhal_pwm_enable_event_internal(obj, event, intrPriority, enable)

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* defined(CY_IP_MXTCPWM_INSTANCES) */
