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
#include "gpio_api.h"
#include "pinmap.h"
#include "gpio_addrdefine.h"
#include "mbed_drv_cfg.h"

uint32_t gpio_set(PinName pin)
{
    pin_function(pin, 0);
    return (1 << PINNO(pin));
}

void gpio_init(gpio_t *obj, PinName pin)
{
    uint32_t reg_group = PINGROUP(pin);

    obj->pin     = pin;
    if (reg_group > GPIO_GROUP_MAX) {
        return;
    }

    obj->mask    = gpio_set(pin);
    obj->reg_dir = (volatile uint16_t *)PDR(reg_group);
    obj->reg_set = (volatile uint8_t *)PODR(reg_group);
    obj->reg_in  = (volatile uint8_t *)PIDR(reg_group);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    /* Pull up and Pull down settings aren't supported because RZ/A1H doesn't have pull up/down for pins(signals). */
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    switch (direction) {
        case PIN_INPUT :
            *obj->reg_dir &= ~((uint16_t)(0x3 << (PINNO(obj->pin) * 2)));
            *obj->reg_dir |= (0x2 << (PINNO(obj->pin) * 2));
            break;
        case PIN_OUTPUT:
            *obj->reg_dir |= (0x3 << (PINNO(obj->pin) * 2));
            break;
        default:
            /* do nothing */
            break;
    }
}
