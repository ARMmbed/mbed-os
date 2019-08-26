/* 
 * Copyright (c) 2019-2020 Nuvoton Technology Corporation
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
#include "gpio_api.h"
#include "pinmap.h"
#include "mbed_error.h"

#if DEVICE_PORTIN || DEVICE_PORTOUT || DEVICE_PORTINOUT

PinName port_pin(PortName port, int pin_n)
{
    return (PinName) NU_PORT_N_PIN_TO_PINNAME(port, pin_n);
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    obj->port      = port;
    obj->mask      = mask;
    obj->direction = dir;

    uint32_t i;
    obj->direction = dir;
    for (i = 0; i < GPIO_PIN_MAX; i++) {
        if (obj->mask & (1 << i)) {
            gpio_set(port_pin(port, i));
        }
    }
    
    port_dir(obj, dir);
}

void port_dir(port_t *obj, PinDirection dir)
{
    uint32_t i;
    obj->direction = dir;
    for (i = 0; i < GPIO_PIN_MAX; i++) {
        if (obj->mask & (1 << i)) {
            if (dir == PIN_OUTPUT) {
                GPIO_SetMode(NU_PORT_BASE(obj->port), 1 << i, GPIO_MODE_OUTPUT);
            } else { // PIN_INPUT
                GPIO_SetMode(NU_PORT_BASE(obj->port), 1 << i, GPIO_MODE_INPUT);
            }
        }
    }
}

void port_mode(port_t *obj, PinMode mode)
{
    uint32_t i;
    
    for (i = 0; i < GPIO_PIN_MAX; i++) {
        if (obj->mask & (1 << i)) {
            pin_mode(port_pin(obj->port, i), mode);
        }
    }
}

void port_write(port_t *obj, int value)
{
    uint32_t i;
    uint32_t port_index = obj->port;
    
    for (i = 0; i < GPIO_PIN_MAX; i++) {
        if (obj->mask & (1 << i)) {
            GPIO_PIN_DATA(port_index, i) = (value & obj->mask) ? 1 : 0;
        }
    }
}

int port_read(port_t *obj)
{
    uint32_t i;
    uint32_t port_index = obj->port;
    int value = 0;
    
    for (i = 0; i < GPIO_PIN_MAX; i++) {
        if (obj->mask & (1 << i)) {
            value = value | (GPIO_PIN_DATA(port_index, i) << i);
        }
    }
    
    return value;
}

#endif
