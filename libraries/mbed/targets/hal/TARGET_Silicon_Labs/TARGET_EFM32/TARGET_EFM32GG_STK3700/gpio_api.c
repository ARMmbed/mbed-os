/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

uint32_t gpio_set(PinName pin) {
        return 1 << ((uint32_t) pin & 0xF);
}

void gpio_init(gpio_t *obj, PinName pin) {
        if(pin == NC) return;

        //Enable GPIO clock TODO: Use static var to ensure only one call?
        CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;


        obj->pin = pin;
        obj->mask = gpio_set(pin);
        obj->port = pin >> 4;
}

void gpio_mode(gpio_t *obj, PinMode mode) {
		obj->mode = mode;
        pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
        obj->dir = direction;
}


