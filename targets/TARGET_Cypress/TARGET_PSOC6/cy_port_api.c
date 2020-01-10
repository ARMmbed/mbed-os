/* mbed Microcontroller Library
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#if DEVICE_PORTIN || DEVICE_PORTOUT

#ifdef __cplusplus
extern "C" {
#endif

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    gpio_t gpio;
    for (uint8_t pin = 0; pin < 8; pin++) {
        if ((1 << pin) & mask) {
            gpio_init(&gpio, port_pin(port, pin));
            gpio_dir(&gpio, dir);
        }
    }
    obj->port = port;
    obj->mask = mask;
    obj->drive_mode = gpio.drive_mode;
    obj->direction = gpio.direction;
}

void port_mode(port_t *obj, PinMode mode)
{
    gpio_t gpio = {.direction = obj->direction, .drive_mode = obj->drive_mode};
    for (uint8_t pin = 0; pin < 8; pin++) {
        if ((1 << pin) & obj->mask) {
            gpio.pin = port_pin(obj->port, pin);
            gpio_mode(&gpio, mode);
        }
    }
}

void port_dir(port_t *obj, PinDirection dir)
{
    gpio_t gpio = {.direction = obj->direction, .drive_mode = obj->drive_mode};
    for (uint8_t pin = 0; pin < 8; pin++) {
        if ((1 << pin) & obj->mask) {
            gpio.pin = port_pin(obj->port, pin);
            gpio_dir(&gpio, dir);
        }
    }
}

void port_write(port_t *obj, int value)
{
    GPIO_PRT_Type *port_type = Cy_GPIO_PortToAddr(obj->port);
    if (obj->mask == 0xff) {
        // Optimization for when all pins on the port is used.
        port_type->OUT = value;
    } else {
        port_type->OUT_SET = value & obj->mask;
        port_type->OUT_CLR = (~value) & obj->mask;
    }
}

int port_read(port_t *obj)
{
    return Cy_GPIO_PortToAddr(obj->port)->IN & obj->mask;
}

#ifdef __cplusplus
}
#endif
#endif
