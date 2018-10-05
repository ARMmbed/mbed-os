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
#include "gpio_api.h"
#include "pinmap.h"
#include "fsl_gpio.h"

uint32_t gpio_set(PinName pin)
{
    MBED_ASSERT(pin != (PinName)NC);
    pin_function(pin, 0);

    return (1 << ((int)pin & 0x1F));
}

void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;
    if (pin == (PinName)NC)
        return;

    pin_function(pin, 0);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t pin_number = obj->pin & 0x1F;
    uint8_t port_number = obj->pin / 32;

    switch (direction) {
        case PIN_INPUT:
            GPIO->DIR[port_number] &= ~(1U << pin_number);
            break;
        case PIN_OUTPUT:
            GPIO->DIR[port_number] |= (1U << pin_number);
            break;
    }
}

void gpio_write(gpio_t *obj, int value)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t pin_number = obj->pin & 0x1F;
    uint8_t port_number = obj->pin / 32;

    GPIO_WritePinOutput(GPIO, port_number, pin_number, value);
}

int gpio_read(gpio_t *obj)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t pin_number = obj->pin & 0x1F;
    uint8_t port_number = obj->pin / 32;

    return (int)GPIO_ReadPinInput(GPIO, port_number, pin_number);
}
