/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * SPDX-License-Identifier: Apache-2.0
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
#include "mbed_error.h"
#include "pinmap.h"
#include "txz_gpio.h"

#define PIN_FUNC_MAX                        8

extern _gpio_t gpio_port_add;

void pin_function(PinName pin, int function)
{
    uint32_t port = 0;
    uint8_t bit = 0;
    uint8_t func = 0;
    uint8_t dir = 0;
    // Assert that pin is valid
    MBED_ASSERT(pin != NC);

    // Calculate pin function and pin direction
    func = PIN_FUNC(function);
    dir  = PIN_DIR(function);
    // Calculate port and pin position
    port = PIN_PORT(pin);
    bit  = PIN_POS(pin);
    // find function is in range or not
    if (func <= PIN_FUNC_MAX) {
        // Set pin function and direction if direction is in range
        switch (dir) {
            case PIN_INPUT:
                // Set pin input
                gpio_func(&gpio_port_add, (gpio_gr_t)port, (gpio_num_t)bit, (uint32_t)func, GPIO_PIN_INPUT);
                break;
            case PIN_OUTPUT:
                // Set pin output
                gpio_func(&gpio_port_add, (gpio_gr_t)port, (gpio_num_t)bit, (uint32_t)func, GPIO_PIN_OUTPUT);
                break;
            case PIN_INOUT:
                // Set pin both input and output
                gpio_func(&gpio_port_add, (gpio_gr_t)port, (gpio_num_t)bit, (uint32_t)func, GPIO_PIN_INOUT);
                break;
            default:
                break;
        }
    } else {
        // do nothing
    }
}

void pin_mode(PinName pin, PinMode mode)
{
    uint32_t port = 0;
    uint8_t bit = 0;

    // Assert that pin is valid
    MBED_ASSERT(pin != NC);

    // Check if function is in range
    if (mode > OpenDrain) {
        return;
    }
    // Calculate port and pin position
    port = PIN_PORT(pin);
    bit = PIN_POS(pin);
    // Set pin mode
    switch (mode) {
        case PullNone:
            gpio_SetPullUp(&gpio_port_add, (gpio_gr_t)port, (gpio_num_t)bit, GPIO_PIN_RESET);
            gpio_SetPullDown(&gpio_port_add, (gpio_gr_t)port, (gpio_num_t)bit, GPIO_PIN_RESET);
            gpio_SetOpenDrain(&gpio_port_add, (gpio_gr_t)port, (gpio_num_t)bit, GPIO_PIN_RESET);
            break;
        case PullUp:
            gpio_SetPullUp(&gpio_port_add, (gpio_gr_t)port, (gpio_num_t)bit, GPIO_PIN_SET);
            break;
        case PullDown:
            gpio_SetPullDown(&gpio_port_add, (gpio_gr_t)port, (gpio_num_t)bit, GPIO_PIN_SET);
            break;
        case OpenDrain:
            gpio_SetOpenDrain(&gpio_port_add, (gpio_gr_t)port, (gpio_num_t)bit, GPIO_PIN_SET);
            break;
        default:
            break;
    }
}
