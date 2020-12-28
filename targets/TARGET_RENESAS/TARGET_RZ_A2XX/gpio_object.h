/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
#ifndef MBED_GPIO_OBJECT_H
#define MBED_GPIO_OBJECT_H

#include "mbed_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName  pin;
    uint32_t mask;

    __IO uint16_t *reg_dir;
    __IO uint8_t  *reg_set;
    __I  uint8_t  *reg_in;
} gpio_t;

static inline void gpio_write(gpio_t *obj, int value)
{
    if (value != 0) {
        *obj->reg_set |= obj->mask;
    } else {
        *obj->reg_set &= ~obj->mask;
    }
}

static inline int gpio_read(gpio_t *obj)
{
    return ((*obj->reg_in & obj->mask) ? 1 : 0);
}

static inline int gpio_is_connected(const gpio_t *obj)
{
    return obj->pin != (PinName)NC;
}

#ifdef __cplusplus
}
#endif

#endif
