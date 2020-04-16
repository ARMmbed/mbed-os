/***************************************************************************//**
* \file cyhal_gpio_impl.h
*
* Description:
* Provides a high level interface for interacting with the Cypress GPIO. This is
* a wrapper around the lower level PDL API.
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

#pragma once

#include "cy_gpio.h"
#include "cyhal_gpio.h"
#include "cyhal_utils.h"
#include "cy_utils.h"

#ifdef CY_IP_MXS40IOSS

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
*       Defines
*******************************************************************************/
#define CYHAL_GET_PORTADDR(pin)    (Cy_GPIO_PortToAddr(CYHAL_GET_PORT(pin)))  /**< Macro to get the port address from pin */

/*******************************************************************************
*       Functions
*******************************************************************************/

__STATIC_INLINE void cyhal_gpio_write_internal(cyhal_gpio_t pin, bool value)
{
    Cy_GPIO_Write(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), value);
}

#define cyhal_gpio_write(pin, value) cyhal_gpio_write_internal(pin, value)

__STATIC_INLINE bool cyhal_gpio_read_internal(cyhal_gpio_t pin)
{
    return 0 != Cy_GPIO_Read(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin));
}

#define cyhal_gpio_read(pin) cyhal_gpio_read_internal(pin)

__STATIC_INLINE void cyhal_gpio_toggle_internal(cyhal_gpio_t pin)
{
    Cy_GPIO_Inv(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin));
}

#define cyhal_gpio_toggle(pin) cyhal_gpio_toggle_internal(pin)

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CY_IP_MXS40IOSS */
