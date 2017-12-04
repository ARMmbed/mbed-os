/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
#include "gpio_api.h"
#include "PeripheralNames.h"
#include "pinmap.h"
#include "mbed_error.h"

#define GPIO_DATA PIN_DATA(0, 3)
extern const PinMap PinMap_GPIO_IRQ[];

uint32_t gpio_set(PinName pin)
{
    // Check that pin is valid
    MBED_ASSERT(pin != (PinName)NC);

    // Checking pin name is not interrupt pins
    if (pinmap_find_peripheral(pin, PinMap_GPIO_IRQ) == (uint32_t) NC) {
        // Set pin function as GPIO pin
        pin_function(pin, GPIO_DATA);
    }

    // Return pin mask
    return (1 << (pin & 0x07));
}

void gpio_init(gpio_t *obj, PinName pin)
{
    // Store above pin mask, pin name into GPIO object
    obj->pin      = pin;
    obj->mask     = gpio_set(pin);
    obj->port     = (GPIO_Port) (pin >> 3);
    if ((PortName)obj->port == PortH) {
        CG_SetFcPeriphA(CG_FC_PERIPH_PORTH, ENABLE);
    }
    if ((PortName)obj->port == PortJ) {
        CG_SetFcPeriphA(CG_FC_PERIPH_PORTJ, ENABLE);
    }
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    // Set pin mode
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    // Set direction
    switch (direction) {
        case PIN_INPUT:
            // Set pin input
            GPIO_SetInput(obj->port, obj->mask);
            break;
        case PIN_OUTPUT:
            // Set pin output
            GPIO_SetOutput(obj->port, obj->mask);
            break;
        case PIN_INOUT:
            // Set pin both input and output
            GPIO_SetOutputEnableReg(obj->port, obj->mask, ENABLE);
            GPIO_SetInputEnableReg(obj->port, obj->mask, ENABLE);
            break;
        default:
            error("Invalid direction\n");
            break;
    }
}

void gpio_write(gpio_t *obj, int value)
{
    // Write gpio object pin data
    if ((value == 0) || (value == 1)) {
        GPIO_WriteDataBit(obj->port, obj->mask, value);
    } else {
        error("Invalid value\n");
    }
}

int gpio_read(gpio_t *obj)
{
    // Read gpio object pin data
    return GPIO_ReadDataBit(obj->port, obj->mask);
}
