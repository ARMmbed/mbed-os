/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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

#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName  pin;
    uint32_t mask;
    uint32_t pin_number;
    __IO uint32_t *reg_dir;
    __IO uint32_t *reg_dirclr;
    __IO uint32_t *reg_data;
    __I uint32_t *reg_in;
} gpio_t;

static inline void gpio_write(gpio_t *obj, int value)
{
    if (value) {
        *obj->reg_data |= (obj->mask);
    } else {
        *obj->reg_data &= ~(obj->mask);
    }
}

static inline int gpio_read(gpio_t *obj)
{
    return ((*obj->reg_in & obj->mask) ? 1 : 0);
}

#ifdef __cplusplus
}
#endif

#endif
