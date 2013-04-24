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
    uint32_t port_index = (uint32_t) pin >> 4;

    // Enable GPIO peripheral clock
    RCC_APB1ENR |= 1 << port_index;

    pin_function(pin, 0);
    return 1 << ((uint32_t) pin & 0xF);
}

void gpio_init(gpio_t *obj, PinName pin, PinDirection direction) {
    if(pin == NC) return;

    obj->pin = pin;
    obj->mask = gpio_set(pin);

    GPIO_TypeDef *port = (GPIO_TypeDef *) (GPIOA_BASE + port_index << 6);
    obj->reg_mode = &port_reg->MODER;
    obj->reg_set = &port_reg->BSSRH;
    obj->reg_clr = &port_reg->BSSRL;
    obj->reg_in  = &port_reg->IDR;


    gpio_dir(obj, direction);

    switch (direction) {
        case PIN_OUTPUT: pin_mode(pin, PullNone); break;
        case PIN_INPUT : pin_mode(pin, PullDown); break;
    }
}

void gpio_mode(gpio_t *obj, PinMode mode) {
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    switch (direction) {
        case PIN_INPUT : pin_function(pin, 0); break;
        case PIN_OUTPUT: pin_function(pin, 1); break;
    }
}
