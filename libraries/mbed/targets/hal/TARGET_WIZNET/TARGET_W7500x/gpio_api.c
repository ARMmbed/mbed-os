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

extern uint32_t Get_GPIO_BaseAddress(uint32_t port_idx);

uint32_t gpio_set(PinName pin)
{
    MBED_ASSERT(pin != (PinName)NC);

    pin_function(pin, WIZ_PIN_DATA(WIZ_MODE_INPUT, WIZ_GPIO_NOPULL, 0));

    return (uint32_t)(1 << ((uint32_t)pin & 0xF)); // Return the pin mask
}

void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;
    if (pin == (PinName)NC) {
        return;
    }

    uint32_t port_index = WIZ_PORT(pin);

    GPIO_TypeDef *gpio = (GPIO_TypeDef *)Get_GPIO_BaseAddress(port_index);

    // Fill GPIO object structure for future use
    obj->mask    = gpio_set(pin);
    obj->reg_data_in    = &gpio->DATA;
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    obj->direction = direction;

    if (direction == PIN_OUTPUT) {
        pin_function(obj->pin, WIZ_PIN_DATA(WIZ_MODE_OUTPUT, WIZ_GPIO_NOPULL, 0));
    } else { // PIN_INPUT
        pin_function(obj->pin, WIZ_PIN_DATA(WIZ_MODE_INPUT, WIZ_GPIO_NOPULL, 0));
    }
}
