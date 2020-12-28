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
#include "PeripheralPins.h"
#include "mbed_error.h"
#include "gpio_addrdefine.h"
#include "mbed_drv_cfg.h"

PinName gpio_multi_guard = (PinName)NC; /* If set pin name here, setting of the "pin" is just one time */

void pin_function(PinName pin, int function)
{
    uint32_t reg_group = PINGROUP(pin);
    uint32_t bitmask = (1 << PINNO(pin));

    if (reg_group > GPIO_GROUP_MAX) {
        return;
    }

    if (gpio_multi_guard == pin) {
        gpio_multi_guard = (PinName)NC;
        return;
    }

    if (function == 0) {
        *PMR(reg_group) &= ~bitmask;
    } else {
        GPIO.PWPR.BIT.B0WI  = 0;
        GPIO.PWPR.BIT.PFSWE = 1;
        *PFS(reg_group, PINNO(pin)) = function;
        GPIO.PWPR.BIT.PFSWE = 0;
        GPIO.PWPR.BIT.B0WI  = 1;
        *PMR(reg_group) |= bitmask;
    }
}

void pin_mode(PinName pin, PinMode mode)
{
//    if (pin == (PinName)NC) { return; }
}
