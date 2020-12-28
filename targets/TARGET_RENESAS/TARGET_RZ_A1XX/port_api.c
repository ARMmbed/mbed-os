/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
#include "gpio_addrdefine.h"

PinName port_pin(PortName port, int pin_n) {
    return (PinName)((port*0x10)+pin_n);
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir) {
    uint32_t i;

    obj->port = port;
    obj->mask = mask;
    obj->reg_dir = (volatile uint32_t *)PMSR(port);
    obj->reg_out = (volatile uint32_t *)PORT(port);
    obj->reg_in  = (volatile uint32_t *)PPR(port);
    obj->reg_buf = (volatile uint32_t *)PIBC(port);
    // Do not use masking, because it prevents the use of the unmasked pins
    // port_reg->FIOMASK = ~mask;

    // The function is set per pin: reuse gpio logic
    for (i = 0; i < 32; i++) {
        if (obj->mask & (1 << i)) {
            gpio_set(port_pin(obj->port, i));
        }
    }

    port_dir(obj, dir);
}

void port_mode(port_t *obj, PinMode mode) {
    uint32_t i;
    // The mode is set per pin: reuse pinmap logic
    for (i = 0; i < 32; i++) {
        if (obj->mask & (1 << i)) {
            pin_mode(port_pin(obj->port, i), mode);
        }
    }
}

void port_dir(port_t *obj, PinDirection dir) {
    switch (dir) {
        case PIN_INPUT : *obj->reg_dir = (obj->mask << 16) | obj->mask;
             *obj->reg_buf |=  obj->mask;
        break;
        case PIN_OUTPUT: *obj->reg_dir = (obj->mask << 16) | ~obj->mask;
             *obj->reg_buf &= ~obj->mask;
        break;
        default:
            // do nothing
        break;
    }
}

void port_write(port_t *obj, int value) {
    *obj->reg_out = (obj->mask << 16) | (value & obj->mask);
}

int port_read(port_t *obj) {
    return (*obj->reg_in & obj->mask);
}
