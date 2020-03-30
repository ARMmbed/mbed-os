/*
 * Copyright (c) 2015-2016, Nuvoton Technology Corporation
 *
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
#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName         pin;
    uint32_t        mask;
    PinDirection    direction;
    PinMode         mode;
} gpio_t;

static inline void gpio_write(gpio_t *obj, int value)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t pin_index = NU_PINNAME_TO_PIN(obj->pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(obj->pin);

    GPIO_PIN_DATA(port_index, pin_index) = value ? 1 : 0;
}

static inline int gpio_read(gpio_t *obj)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t pin_index = NU_PINNAME_TO_PIN(obj->pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(obj->pin);

    return (GPIO_PIN_DATA(port_index, pin_index) ? 1 : 0);
}

#ifdef __cplusplus
}
#endif

#endif
