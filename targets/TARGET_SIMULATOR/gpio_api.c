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
#include "mbed_assert.h"
#include "gpio_api.h"
#include "pinmap.h"
#include "emscripten.h"

uint32_t gpio_set(PinName pin) {
    return pin;
}

void gpio_init(gpio_t *obj, PinName pin) {
    obj->pin = pin;
    if (pin == (PinName)NC)
        return;

    obj->mask = gpio_set(pin);

    EM_ASM_({
        MbedJSHal.gpio.init($0, $1);
    }, obj, obj->pin);
}

void gpio_mode(gpio_t *obj, PinMode mode) {
    EM_ASM_({
        MbedJSHal.gpio.mode($0, $1);
    }, obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    EM_ASM_({
        MbedJSHal.gpio.dir($0, $1);
    }, obj->pin, direction);
}

void gpio_init_in(gpio_t* gpio, PinName pin) {
    gpio->pin = pin;
    if (pin == (PinName)NC)
        return;

    gpio->mask = gpio_set(pin);

    EM_ASM_({
        MbedJSHal.gpio.init_in($0, $1, 3);
    }, gpio, gpio->pin);
}

void gpio_init_in_ex(gpio_t* gpio, PinName pin, PinMode mode) {
    gpio->pin = pin;
    if (pin == (PinName)NC)
        return;

    gpio->mask = gpio_set(pin);

    EM_ASM_({
        MbedJSHal.gpio.init_in($0, $1, $2);
    }, gpio, gpio->pin, mode);
}

void gpio_init_out(gpio_t* gpio, PinName pin) {
    gpio->pin = pin;
    if (pin == (PinName)NC)
        return;

    gpio->mask = gpio_set(pin);

    EM_ASM_({
        MbedJSHal.gpio.init_out($0, $1, 0);
    }, gpio, gpio->pin);
}

void gpio_init_out_ex(gpio_t* gpio, PinName pin, int value) {
    gpio->pin = pin;
    if (pin == (PinName)NC)
        return;

    gpio->mask = gpio_set(pin);

    EM_ASM_({
        MbedJSHal.gpio.init_out($0, $1, $2);
    }, gpio, gpio->pin, value);
}

void gpio_init_inout(gpio_t* gpio, PinName pin, PinDirection direction, PinMode mode, int value) {
    gpio->pin = pin;
    if (pin == (PinName)NC)
        return;

    gpio->mask = gpio_set(pin);

    EM_ASM_({
        MbedJSHal.gpio.init_inout($0, $1, $2, $3, $4);
    }, gpio, gpio->pin, direction, mode, value);
}
