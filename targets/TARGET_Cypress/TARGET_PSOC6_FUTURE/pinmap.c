/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "mbed_assert.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "cy_gpio.h"
#include "psoc6_utils.h"

void pin_function(PinName pin, int function)
{
    if (pin != NC) {
        GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CY_PORT(pin));
        uint32_t mode = gpio_get_cy_drive_mode(CY_PIN_DIRECTION(function), CY_PIN_MODE(function));
        uint32_t state = Cy_GPIO_ReadOut(port, CY_PIN(pin));

        // Force output to enable pulls.
        switch (mode) {
            case CY_GPIO_DM_PULLUP:
                state = 1;
                break;
            case CY_GPIO_DM_PULLDOWN:
                state = 0;
                break;
            default:
                /* do nothing */
                break;
        }
        Cy_GPIO_Pin_FastInit(port, CY_PIN(pin), mode, state, CY_PIN_HSIOM(function));
    }
}

void pin_mode(PinName pin, PinMode mode)
{
    if (pin != NC) {
        uint32_t cymode = gpio_get_cy_drive_mode(PIN_INPUT, mode);
        GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CY_PORT(pin));

        Cy_GPIO_SetDrivemode(port, CY_PIN(pin), cymode);

        // Force output to enable pulls.
        switch (cymode) {
            case CY_GPIO_DM_PULLUP:
                Cy_GPIO_Write(port, CY_PIN(pin), 1);
                break;
            case CY_GPIO_DM_PULLDOWN:
                Cy_GPIO_Write(port, CY_PIN(pin), 0);
                break;
            default:
                /* do nothing */
                break;
        }
    }
}
