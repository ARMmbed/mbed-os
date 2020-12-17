/***************************************************************************//**
* \file cyhal_interconnect.c
*
* \brief
* Provides a high level interface for interacting with the internal digital
* routing on the chip. This is a wrapper around the lower level PDL API.
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

#include "cyhal_interconnect.h"
#include "cyhal_gpio_impl.h"

#if defined(CY_IP_MXPERI) || defined(CY_IP_M0S8PERI)

#if defined(__cplusplus)
extern "C"
{
#endif

cy_rslt_t cyhal_connect_pin(const cyhal_resource_pin_mapping_t *pin_connection)
{
    cyhal_gpio_t pin = pin_connection->pin;
    GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(pin));
    en_hsiom_sel_t hsiom = pin_connection->hsiom;
    uint8_t mode = pin_connection->drive_mode;

    Cy_GPIO_Pin_FastInit(port, CYHAL_GET_PIN(pin), mode, 1, hsiom);
    // Force output to enable pulls.
    switch (mode) {
        case CY_GPIO_DM_PULLUP:
            Cy_GPIO_Write(port, CYHAL_GET_PIN(pin), 1);
            break;
        case CY_GPIO_DM_PULLDOWN:
            Cy_GPIO_Write(port, CYHAL_GET_PIN(pin), 0);
            break;
        default:
            /* do nothing */
            break;
    }

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_disconnect_pin(cyhal_gpio_t pin)
{
    GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(pin));
    Cy_GPIO_Pin_FastInit(port, CYHAL_GET_PIN(pin), CY_GPIO_DM_HIGHZ, 1, HSIOM_SEL_GPIO);
    return CY_RSLT_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXPERI) || defined(CY_IP_M0S8PERI) */
