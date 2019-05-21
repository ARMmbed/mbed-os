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
#include "gpio_api.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "gpio_include.h"

extern const PinMap PinMap_GPIO_IRQ[];
#define GPIO_DATA                PIN_DATA(0, 2)

uint32_t gpio_set(PinName pin)
{
    // Check that pin is valid
    MBED_ASSERT(pin != (PinName)NC);
    // Checking pin name is not interrupt pins
    // Set pin function as GPIO pin
    pin_function(pin, GPIO_DATA);
    // Return pin mask
    return (1 << (pin & 0x07));
}

void gpio_init(gpio_t *obj, PinName pin)
{
    // Store above pin mask, pin name into GPIO object
    obj->pin = pin;
    if (pin != NC) {
        obj->mask = gpio_set(pin);
        obj->port = (PortName) (pin >> 3);
        TSB_CG->FSYSENA |= (1<<(obj->port));
    }
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin, mode);
}

// Set gpio object pin direction
void gpio_dir(gpio_t *obj, PinDirection direction)
{
    // Set direction
    switch (direction) {
        case PIN_INPUT:
            pin_function(obj->pin, PIN_INPUT);
            break;

        case PIN_OUTPUT:
            pin_function(obj->pin, PIN_OUTPUT);
            break;

        case  PIN_INOUT:
            pin_function(obj->pin, PIN_INOUT);
            break;

        default:
            error("Invalid direction\n");
            break;
    }
}

// Write gpio object pin data
void gpio_write(gpio_t *obj, int value)
{
    int port     = 0;
    uint8_t bit  = 0;
    uint32_t base;

    // Calculate port and pin position
    port = PIN_PORT(obj->pin);
    bit  = PIN_POS(obj->pin);

    base = BITBAND_PORT_BASE(port);
    base = BITBAND_PORT_MODE_BASE(base, GPIO_Mode_DATA);
    if(value == GPIO_PIN_SET)
        BITBAND_PORT_SET(base, bit);
    else if(value == GPIO_PIN_RESET)
        BITBAND_PORT_CLR(base, bit);
    else
        error("Invalid value\n");
}

// Read gpio object pin data
int  gpio_read (gpio_t *obj)
{
    int port     = 0;
    uint8_t bit  = 0;
    uint32_t base;
    uint32_t val;
    int BitValue;

    // Calculate port and pin position
    port = PIN_PORT(obj->pin);
    bit  = PIN_POS(obj->pin);

    base = BITBAND_PORT_BASE(port);
    base = BITBAND_PORT_MODE_BASE(base, GPIO_Mode_DATA);
    BITBAND_PORT_READ(val, base, bit);
    if(val == GPIO_PIN_RESET)
        BitValue = GPIO_PIN_RESET;
    else
        BitValue = GPIO_PIN_SET;
    return (BitValue);
}
