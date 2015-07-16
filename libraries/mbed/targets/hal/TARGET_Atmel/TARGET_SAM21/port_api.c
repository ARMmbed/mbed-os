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
#include "pinmap.h"
#include "gpio_api.h"
#include "port.h"

#define PORTA_MASK	0xDBDFFFF3  // mask for available pins in Port A
#define PORTB_MASK	0xC0C3C30D  // mask for available pins in Port B
#define PORTC_MASK	0x000D0000  // mask for available pins in Port C

#define PORTA_START	0  // 32 pins in a port
#define PORTB_START	32
#define PORTC_START	64

uint32_t start_pin(PortName port)
{
    switch (port) {
        case PortA:
            return PORTA_START;
        case PortB:
            return PORTB_START;
        case PortC:
            return PORTC_START;
        default:
            return NC;
    }
}
void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    MBED_ASSERT(obj);
    struct port_config pin_conf;
    int i, j;
    uint32_t start;

    PortGroup *const port_base = (PortGroup*)port_get_group_from_gpio_pin(port * 32);  // 32 pins in port // function reused to get the port base
    switch (port) {
        case PortA:
            obj->mask = (uint32_t)mask & PORTA_MASK;
            break;
        case PortB:
            obj->mask = (uint32_t)mask & PORTB_MASK;
            break;
        case PortC:
            obj->mask = (uint32_t)mask & PORTC_MASK;
            break;
    }
    start = start_pin(port);
    if(start == NC)
        return;
    obj->port = port;
    obj->direction = dir;
    port_get_config_defaults(&pin_conf);
    obj->powersave = pin_conf.powersave;
    obj->mode = PORT_PIN_PULL_UP;
    switch (dir) {
        case PIN_INPUT :
            pin_conf.direction  = PORT_PIN_DIR_INPUT;
            break;
        case PIN_OUTPUT:
            pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
            break;
        case PIN_INPUT_OUTPUT:
            pin_conf.direction  = PORT_PIN_DIR_OUTPUT_WTH_READBACK;
            break;
    }
    for (i = start, j = 0; i < (start + 32); i++, j++) {
        if (obj->mask & (1<<j)) {
            port_pin_set_config((PinName)i, &pin_conf);
        }
    }
    obj->OUTCLR = &port_base->OUTCLR.reg;
    obj->OUTSET = &port_base->OUTSET.reg;
    obj->IN = &port_base->IN.reg;
    obj->OUT = &port_base->OUT.reg;
}

void port_mode(port_t *obj, PinMode mode)
{
    MBED_ASSERT(obj);
    int i, j;
    uint32_t start;
    start = start_pin(obj->port);
    if(start == NC)
        return;
    for (i = start, j = 0; i < (start + 32); i++, j++) {
        if (obj->mask & (1<<j)) {
            pin_mode((PinName)i , mode);
        }
    }
}

void port_dir(port_t *obj, PinDirection dir)
{
    MBED_ASSERT(obj);
    struct port_config pin_conf;
    int i, j;
    uint32_t start;
    start = start_pin(obj->port);
    if(start == NC)
        return;
    obj->direction = dir;
    pin_conf.input_pull = obj->mode;
    pin_conf.powersave  = obj->powersave;
    switch (dir) {
        case PIN_INPUT :
            pin_conf.direction  = PORT_PIN_DIR_INPUT;
            break;
        case PIN_OUTPUT:
            pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
            break;
        case PIN_INPUT_OUTPUT:
            pin_conf.direction  = PORT_PIN_DIR_OUTPUT_WTH_READBACK;
            break;
    }
    for (i = start, j = 0; i < (start + 32); i++, j++) {
        if (obj->mask & (1<<j)) {
            port_pin_set_config((PinName)i, &pin_conf);
        }
    }
}

void port_write(port_t *obj, int value)
{
    MBED_ASSERT(obj);
    int i;
    uint32_t start;
    start = start_pin(obj->port);
    if(start == NC)
        return;
    for (i = 0; i < 32 ; i++) {
        if (obj->mask & (1<<i)) {
            if (value & (1<<i)) {
                *obj->OUTSET = 1 << i;
            } else {
                *obj->OUTCLR = 1 << i;
            }
        }
    }
}

int port_read(port_t *obj)
{
    MBED_ASSERT(obj);
    if (obj->direction  == PIN_INPUT) {
        return (*obj->IN & obj->mask);
    } else {
        return (*obj->OUT & obj->mask);
    }
}
