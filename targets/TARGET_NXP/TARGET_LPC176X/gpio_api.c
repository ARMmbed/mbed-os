/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "mbed_assert.h"
#include "gpio_api.h"
#include "pinmap.h"

uint32_t gpio_set(PinName pin) {
    MBED_ASSERT(pin != (PinName)NC);
    pin_function(pin, 0);
    return (1 << ((int)pin & 0x1F));
}

void gpio_init(gpio_t *obj, PinName pin) {
    obj->pin = pin;
    if (pin == (PinName)NC)
        return;

    obj->mask = gpio_set(pin);
    LPC_GPIO_TypeDef *port_reg = (LPC_GPIO_TypeDef *)((int)pin & ~0x1F);
    obj->reg_set = &port_reg->FIOSET;
    obj->reg_clr = &port_reg->FIOCLR;
    obj->reg_in  = &port_reg->FIOPIN;
    obj->reg_dir = &port_reg->FIODIR;
}

void gpio_mode(gpio_t *obj, PinMode mode) {
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    MBED_ASSERT(obj->pin != (PinName)NC);
    switch (direction) {
        case PIN_INPUT :
            *obj->reg_dir &= ~obj->mask;
            break;
        case PIN_OUTPUT:
            *obj->reg_dir |= obj->mask;
            break;
    }
}
