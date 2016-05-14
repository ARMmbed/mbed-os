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
#ifndef MBED_GPIO_OBJECT_H
#define MBED_GPIO_OBJECT_H

#include "mbed_assert.h"
#include "ioport.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName  pin;
    uint8_t mode;
    uint8_t direction;
} gpio_t;

static inline void gpio_write(gpio_t *obj, int value)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    if (value)
        ioport_set_pin_level(obj->pin, IOPORT_PIN_LEVEL_HIGH);
    else
        ioport_set_pin_level(obj->pin, IOPORT_PIN_LEVEL_LOW);
}

static inline int gpio_read(gpio_t *obj)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    return (ioport_get_pin_level(obj->pin) ? 1 : 0);
}

static inline int gpio_is_connected(const gpio_t *obj)
{
    return obj->pin != (PinName)NC;
}

#ifdef __cplusplus
}
#endif

#endif
