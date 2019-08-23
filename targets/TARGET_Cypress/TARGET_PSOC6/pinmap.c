/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2018-2019 Cypress Semiconductor Corporation
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
 */

#include "pinmap.h"
#include "cy_gpio.h"
#include "cyhal_gpio.h"
#include "cyhal_utils.h"

void pin_function(PinName pin, int function)
{
    if (pin != NC) {
        GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(pin));
        uint32_t mode = CY_GPIO_CFG_GET_MODE(function);

        Cy_GPIO_Pin_FastInit(port, CYHAL_GET_PIN(pin), mode, 1, CY_GPIO_CFG_GET_HSIOM(function));
        // Force output to enable pulls.
        switch (mode) {
            case CY_GPIO_DM_PULLUP:
            case CY_GPIO_DM_PULLUP_IN_OFF:
                Cy_GPIO_Write(port, CYHAL_GET_PIN(pin), 1);
                break;
            case CY_GPIO_DM_PULLDOWN:
            case CY_GPIO_DM_PULLDOWN_IN_OFF:
                Cy_GPIO_Write(port, CYHAL_GET_PIN(pin), 0);
                break;
            default:
                /* do nothing */
                break;
        }
    }
}

void pin_mode(PinName pin, PinMode mode)
{
    if (pin != NC) {
        GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(pin));
        Cy_GPIO_SetDrivemode(port, CYHAL_GET_PIN(pin), mode);

        // Force output to enable pulls.
        switch (mode) {
            case CYHAL_GPIO_DRIVE_PULLUP:
                Cy_GPIO_Write(port, CYHAL_GET_PIN(pin), 1);
                break;
            case CYHAL_GPIO_DRIVE_PULLDOWN:
                Cy_GPIO_Write(port, CYHAL_GET_PIN(pin), 0);
                break;
            default:
                /* do nothing */
                break;
        }
    }
}
