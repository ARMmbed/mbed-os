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
#include "port_api.h"

#if DEVICE_PORTIN || DEVICE_PORTOUT

#include "pinmap.h"
#include "gpio_api.h"

/* Array of GPIO peripheral base address. */
static GPIO_Type *const port_addrs[] = GPIO_BASE_PTRS;

PinName port_pin(PortName port, int pin_n)
{
    return (PinName)((port << GPIO_PORT_SHIFT) | pin_n);
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    obj->port = port;
    obj->mask = mask;

    // The function is set per pin: reuse gpio logic
    for (uint32_t i = 0; i < 32; i++) {
        if (obj->mask & (1 << i)) {
            gpio_set(port_pin(obj->port, i));
        }
    }

    port_dir(obj, dir);
}

void port_mode(port_t *obj, PinMode mode)
{
    // The mode is set per pin: reuse pinmap logic
    for (uint32_t i = 0; i < 32; i++) {
        if (obj->mask & (1 << i)) {
            pin_mode(port_pin(obj->port, i), mode);
        }
    }
}

void port_dir(port_t *obj, PinDirection dir)
{
    GPIO_Type *base = port_addrs[obj->port];
    uint32_t direction = base->GDIR;

    switch (dir) {
        case PIN_INPUT :
            direction &= ~obj->mask;
            break;
        case PIN_OUTPUT:
            direction |= obj->mask;
            break;
    }
    base->GDIR = direction;
}

void port_write(port_t *obj, int value)
{
    GPIO_Type *base = port_addrs[obj->port];
    uint32_t input = base->DR & ~obj->mask;

    base->DR = (input | (uint32_t)(value & obj->mask));
}

int port_read(port_t *obj)
{
    GPIO_Type *base = port_addrs[obj->port];

    return (int)(base->DR & obj->mask);
}

#endif
