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
#include "mbed_assert.h"
#include "PinNames.h"
#include "gpio_object.h"
#include "gpio_api.h"
#include "compiler.h"
#include "ioport.h"

extern uint8_t g_sys_init;

uint32_t gpio_set(PinName pin)
{
    MBED_ASSERT(pin != (PinName)NC);
    return (1UL << (pin % 32));
}

void gpio_init(gpio_t *obj, PinName pin)
{
    MBED_ASSERT(pin != (PinName)NC);
    if (g_sys_init == 0) {
        sysclk_init();
        system_board_init();
        g_sys_init = 1;
    }
    obj->pin = pin;

    ioport_set_pin_dir(pin, IOPORT_DIR_INPUT);
    ioport_set_pin_mode(pin, IOPORT_MODE_PULLUP);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    obj->mode = mode;
    switch (mode) {
        case PullNone :
            ioport_set_pin_mode(obj->pin, IOPORT_MODE_OPEN_DRAIN);
            break;
        case PullUp:
            ioport_set_pin_mode(obj->pin, IOPORT_MODE_PULLUP);
            break;
        case PullDown:
            ioport_set_pin_mode(obj->pin, IOPORT_MODE_PULLDOWN);
            break;
    }
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    obj->direction = direction;
    switch (direction) {
        case PIN_INPUT :
            ioport_set_pin_dir(obj->pin, IOPORT_DIR_INPUT);
            break;
        case PIN_OUTPUT:
            ioport_set_pin_dir(obj->pin, IOPORT_DIR_OUTPUT);
            break;
        case PIN_INPUT_OUTPUT:
            ioport_set_pin_dir(obj->pin, IOPORT_DIR_OUTPUT);
            break;
    }
}
