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
#ifndef MBED_GPIO_OBJECT_H
#define MBED_GPIO_OBJECT_H

#include "em_gpio.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName pin;
    uint32_t mask;
    GPIO_Port_TypeDef port;
    PinMode mode;
    uint32_t dir;
} gpio_t;

static inline void gpio_write(gpio_t *obj, int value)
{
    if (value) {
        GPIO_PinOutSet(obj->port, obj->pin & 0xF); // Pin number encoded in first four bits of obj->pin
    } else {
        GPIO_PinOutClear(obj->port, obj->pin & 0xF);
    }
}

static inline int gpio_read(gpio_t *obj)
{
    if (obj->dir == PIN_INPUT) {
        return GPIO_PinInGet(obj->port, obj->pin & 0xF); // Pin number encoded in first four bits of obj->pin
    } else {
        return GPIO_PinOutGet(obj->port, obj->pin & 0xF);
    }
}

static inline int gpio_is_connected(const gpio_t *obj) {
    return obj->pin != (PinName)NC;
}

#ifdef __cplusplus
}
#endif

#endif
