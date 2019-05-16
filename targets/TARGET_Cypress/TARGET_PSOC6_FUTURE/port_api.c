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

#include "cmsis.h"
#include "device.h"
#include "mbed_assert.h"
#include "gpio_object.h"
#include "port_api.h"
#include "psoc6_utils.h"
#include "mbed_error.h"

static void port_init_pins(port_t *obj)
{
    uint32_t pin;
    uint32_t cy_mode;

    MBED_ASSERT(obj);
    MBED_ASSERT(obj->port);

    cy_mode = gpio_get_cy_drive_mode(obj->direction, obj->mode);
    for (pin = 0; pin < 8; ++pin) {
        if (obj->mask & (1 << pin)) {
            Cy_GPIO_Pin_FastInit(obj->port, pin, cy_mode, 0, HSIOM_SEL_GPIO);
        }
    }

    // Force output to enable pulls.
    if (obj->direction == PIN_INPUT) {
        switch (cy_mode) {
            case CY_GPIO_DM_PULLUP:
                port_write(obj, 0xff);
                break;        // Force output to enable pulls.

            case CY_GPIO_DM_PULLDOWN:
                port_write(obj, 0);
                break;
        }
    }
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    uint32_t pin;

    MBED_ASSERT(obj);

    for (pin = 0; pin < 8; ++pin) {
        if (mask & (1 << pin)) {
            if (cy_reserve_io_pin((PinName)((port << 8)+pin))) {
                error("Port pin reservation conflict.");
            }
        }
    }
    obj->port_id = port;
    obj->port = Cy_GPIO_PortToAddr(port);
    obj->mask = mask & 0xff; // Only 8 bits exist on a port in PSoC.
    obj->direction = dir;
    obj->mode = PullDefault;
    port_init_pins(obj);
}

void port_mode(port_t *obj, PinMode mode)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(obj->port);

    obj->mode = mode;
    port_init_pins(obj);
}

void port_dir(port_t *obj, PinDirection dir)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(obj->port);

    obj->direction = dir;
    port_init_pins(obj);
}

void port_write(port_t *obj, int value)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(obj->port);

    if (obj->mask == 0xff) {
        obj->port->OUT = value;
    } else {
        // In case some bits are used for different functionality we need to write
        // each bit separately to not break things out, eg. pull up state on other bits.
        uint32_t pin;
        for (pin = 0; pin < 8; ++pin) {
            if (obj->mask & (1 << pin)) {
                Cy_GPIO_Write(obj->port, pin, value & 0x1);
            }
            value >>= 1;
        }
    }
}

int port_read(port_t *obj)
{
    return obj->port->IN & obj->mask;
}
