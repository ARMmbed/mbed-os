/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#include "PinNames.h"
#include "adi_gpio.h"

void pin_function(PinName pin, int function)
{
    // pin is composed of port and pin
    // function is the function number (the mux selection number shifted by the pin value
    // and written to pin mux register, each pin mux takes 2 bits hence multiplying by 2)

    MBED_ASSERT(pin != (PinName)NC);

    uint8_t port = pin >> GPIO_PORT_SHIFT;
    uint32_t cfg_reg, mask;
    volatile uint32_t *pGPIO_CFG;

    switch (port) {
        case 0:
            pGPIO_CFG = (volatile uint32_t *)REG_GPIO0_CFG;
            break;
        case 1:
            pGPIO_CFG = (volatile uint32_t *)REG_GPIO1_CFG;
            break;
        case 2:
            pGPIO_CFG = (volatile uint32_t *)REG_GPIO2_CFG;
            break;

        default:
            return;
    }

    cfg_reg = *pGPIO_CFG;
    // clear the corresponding 2 bit field first before writing the function
    // bits
    mask = ~(3 << (pin * 2));
    cfg_reg = cfg_reg & mask | (function << (pin*2));
    *pGPIO_CFG = cfg_reg;
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);

    uint8_t port = pin >> GPIO_PORT_SHIFT;
    uint32_t pin_reg_value = 2 ^ (0xFF & pin);

    switch (mode) {
        case PullNone:
            adi_gpio_PullUpEnable((ADI_GPIO_PORT)port, (ADI_GPIO_DATA)pin_reg_value,false);
            break;

        case PullDown:
        case PullUp:
            adi_gpio_PullUpEnable((ADI_GPIO_PORT)port, (ADI_GPIO_DATA)pin_reg_value,true);
            break;

        default:
            break;

    }

}
