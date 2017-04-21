/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "pinmap.h"
#include "gpio_api.h"
#include "ioport.h"

extern uint8_t g_sys_init;

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    MBED_ASSERT(obj);
    if (g_sys_init == 0) {
        sysclk_init();
        system_board_init();
        g_sys_init = 1;
    }
    obj->port = port;
    obj->mask = mask;

    switch (dir) {
        case PIN_INPUT :
            ioport_set_port_dir(port, mask, IOPORT_DIR_INPUT);
            break;
        case PIN_OUTPUT:
            ioport_set_port_dir(port, mask, IOPORT_DIR_OUTPUT);
            break;
        case PIN_INPUT_OUTPUT:
            ioport_set_port_dir(port, mask, IOPORT_DIR_OUTPUT);
            break;
    }
    ioport_set_port_mode(port, mask, IOPORT_MODE_PULLUP);
}

void port_mode(port_t *obj, PinMode mode)
{
    MBED_ASSERT(obj);
    obj->mode = mode;
    switch (mode) {
        case PullNone :
            ioport_set_port_mode(obj->port, obj->mask, IOPORT_MODE_OPEN_DRAIN);
            break;
        case PullUp:
            ioport_set_port_mode(obj->port, obj->mask, IOPORT_MODE_PULLUP);
            break;
        case PullDown:
            ioport_set_port_mode(obj->port, obj->mask, IOPORT_MODE_PULLDOWN);
            break;
    }
}

void port_dir(port_t *obj, PinDirection dir)
{
    MBED_ASSERT(obj);
    obj->direction = dir;
    switch (dir) {
        case PIN_INPUT :
            ioport_set_port_dir(obj->port, obj->mask, IOPORT_DIR_INPUT);
            break;
        case PIN_OUTPUT:
            ioport_set_port_dir(obj->port, obj->mask, IOPORT_DIR_OUTPUT);
            break;
        case PIN_INPUT_OUTPUT:
            ioport_set_port_dir(obj->port, obj->mask, IOPORT_DIR_OUTPUT);
            break;
    }
}

void port_write(port_t *obj, int value)
{
    MBED_ASSERT(obj);
    ioport_set_port_level(obj->port, obj->mask, value);
}

int port_read(port_t *obj)
{
    MBED_ASSERT(obj);
    return ioport_get_port_level(obj->port, obj->mask);
}
