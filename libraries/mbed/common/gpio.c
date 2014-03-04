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

#define GPIO_INIT_IN(obj, pin, mode) \
    gpio_init(obj, pin), \
    gpio_mode(obj, mode), \
    gpio_dir(obj, PIN_INPUT)
    
#define GPIO_INIT_OUT(obj, pin, mode, value) \
    gpio_init(obj, pin), \
    gpio_write(obj, value), \
    gpio_dir(obj, PIN_OUTPUT), \
    gpio_mode(obj, mode)

void gpio_init_in(gpio_t* gpio, PinName pin) {
    gpio_init_in_ex(gpio, pin, PullDefault);
}

void gpio_init_in_ex(gpio_t* gpio, PinName pin, PinMode mode) {
    GPIO_INIT_IN(gpio, pin, mode);
}

void gpio_init_out(gpio_t* gpio, PinName pin) {
    gpio_init_out_ex(gpio, pin, 0);
}

void gpio_init_out_ex(gpio_t* gpio, PinName pin, int value) {
    GPIO_INIT_OUT(gpio, pin, PullNone, value);
}

void gpio_init_inout(gpio_t* gpio, PinName pin, PinDirection direction, PinMode mode, int value) {
    if (direction == PIN_INPUT) {
        GPIO_INIT_IN(gpio, pin, mode);
        gpio_write(gpio, value); // we prepare the value in case direction is switched later
    } else {
        GPIO_INIT_OUT(gpio, pin, mode, value);
    }
}
