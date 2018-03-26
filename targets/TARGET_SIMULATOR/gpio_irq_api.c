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
#include <stddef.h>

#include "gpio_irq_api.h"
#include "mbed_error.h"
#include "emscripten.h"

static gpio_irq_handler irq_handler;

EMSCRIPTEN_KEEPALIVE
void handle_interrupt_in(uint32_t id, gpio_irq_event event) {
    irq_handler(id, event);
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id) {
    if (pin == NC) return -1;

    irq_handler = handler;

    obj->port = pin;
    obj->pin = pin;

    EM_ASM_({
        MbedJSHal.gpio.irq_init($0, $1);
    }, id, obj->pin);

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj) {
    EM_ASM_({
        MbedJSHal.gpio.irq_free($0);
    }, obj->pin);
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable) {
    EM_ASM_({
        MbedJSHal.gpio.irq_set($0, $1, $2);
    }, obj->pin, event, enable);
}

void gpio_irq_enable(gpio_irq_t *obj) {
}

void gpio_irq_disable(gpio_irq_t *obj) {
}

