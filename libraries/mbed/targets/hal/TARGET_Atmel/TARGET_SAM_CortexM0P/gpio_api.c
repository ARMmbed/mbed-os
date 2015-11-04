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
#include "port.h"

uint32_t gpio_set(PinName pin)
{
    MBED_ASSERT(pin != (PinName)NC);
    return (1UL << (pin % 32));
}

void gpio_init(gpio_t *obj, PinName pin)
{
    MBED_ASSERT(pin != (PinName)NC);
    struct port_config pin_conf;
    PortGroup *const port_base = (PortGroup*)port_get_group_from_gpio_pin(pin);

    obj->pin = pin;
    if (pin == (PinName)NC)
        return;

    obj->mask = gpio_set(pin);
    port_get_config_defaults(&pin_conf);
    obj->powersave = pin_conf.powersave;
    obj->direction = PORT_PIN_DIR_INPUT;
    obj->mode = PORT_PIN_PULL_UP;
    port_pin_set_config(pin, &pin_conf);

    obj->OUTCLR = &port_base->OUTCLR.reg;
    obj->OUTSET = &port_base->OUTSET.reg;
    obj->IN = &port_base->IN.reg;
    obj->OUT = &port_base->OUT.reg;
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    struct port_config pin_conf;

    obj->mode = mode;
    pin_conf.direction = obj->direction;
    pin_conf.powersave  = obj->powersave;
    switch (mode) {
        case PullNone :
            pin_conf.input_pull  = PORT_PIN_PULL_NONE;
            break;
        case PullUp:
            pin_conf.input_pull  = PORT_PIN_PULL_UP;
            break;
        case PullDown:
            pin_conf.input_pull  = PORT_PIN_PULL_DOWN;
            break;
    }
    port_pin_set_config(obj->pin, &pin_conf);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    struct port_config pin_conf;

    obj->direction = direction;
    pin_conf.input_pull = obj->mode;
    pin_conf.powersave  = obj->powersave;
    switch (direction) {
        case PIN_INPUT :
            pin_conf.direction  = PORT_PIN_DIR_INPUT;
            break;
        case PIN_OUTPUT:
            pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
            break;
        case PIN_INPUT_OUTPUT:
            pin_conf.direction  = PORT_PIN_DIR_OUTPUT_WTH_READBACK;
            break;
    }
    port_pin_set_config(obj->pin, &pin_conf);
}
