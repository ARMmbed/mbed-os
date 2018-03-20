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

static GPIO_Type * const gpio_addrs[] = GPIO_BASE_PTRS;

uint32_t gpio_set(PinName pin)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t pin_num = pin & 0xFF;

    pin_function(pin, GPIO_MUX_PORT);
    return 1 << pin_num;
}

void gpio_init(gpio_t *obj, PinName pin)
{
    clock_ip_name_t gpio_clocks[] = GPIO_CLOCKS;

    CLOCK_EnableClock(gpio_clocks[pin >> GPIO_PORT_SHIFT]);

    obj->pin = pin;
    if (pin == (PinName)NC)
        return;

    pin_function(pin, GPIO_MUX_PORT);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t port = (obj->pin >> GPIO_PORT_SHIFT) & 0xF;
    uint32_t gpio_pin_num = obj->pin & 0xFF;
    GPIO_Type *base = gpio_addrs[port];

    switch (direction) {
        case PIN_INPUT:
            base->GDIR &= ~(1U << gpio_pin_num);
            break;
        case PIN_OUTPUT:
            base->GDIR |= (1U << gpio_pin_num);
            break;
    }
}

void gpio_write(gpio_t *obj, int value)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t port = (obj->pin >> GPIO_PORT_SHIFT) & 0xF;
    uint32_t gpio_pin_num = obj->pin & 0xFF;

    GPIO_WritePinOutput(gpio_addrs[port], gpio_pin_num, value);
}

int gpio_read(gpio_t *obj)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t port = (obj->pin >> GPIO_PORT_SHIFT) & 0xF;
    uint32_t gpio_pin_num = obj->pin & 0xFF;

    return (int)GPIO_ReadPinInput(gpio_addrs[port], gpio_pin_num);
}
