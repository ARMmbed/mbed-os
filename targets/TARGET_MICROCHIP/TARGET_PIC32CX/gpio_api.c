/***************************************************************************//**
 * @file gpio_api.c
 *******************************************************************************
 * @section License
 * <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "compiler.h"
#include "gpio_api.h"
#include "pinmap.h"
#include "ioport.h"
#include "mbed_assert.h"


void gpio_write(gpio_t *obj, int value)
{
    MBED_ASSERT(obj->pin != NC);

    ioport_set_pin_level((ioport_pin_t)obj->pin, (bool)value);
}

int gpio_read(gpio_t *obj)
{
    MBED_ASSERT(obj->pin != NC);

    return (int)ioport_get_pin_level((ioport_pin_t)obj->pin);
}

int gpio_is_connected(const gpio_t *obj)
{
    return ((uint32_t)obj->pin | 0xFFFFFF00 ) != (uint32_t)((PinName)NC);
}

void gpio_init(gpio_t *obj, PinName pin)
{
    ioport_init((ioport_pin_t)pin);
    obj->pin = pin;
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    MBED_ASSERT(obj->pin != NC);

    ioport_set_pin_mode((ioport_pin_t)obj->pin, (ioport_mode_t)mode);
}

// Used by DigitalInOut to set correct mode when direction is set
void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj->pin != NC);

    ioport_set_pin_dir(obj->pin, (enum ioport_direction)direction);
    obj->dir = direction;
}
