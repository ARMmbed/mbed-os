/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include "mbed_error.h"
#include "PinNames.h"
#include "cyhal_gpio.h"
#include "cyhal_hwmgr.h"
#include "cy_gpio.h"
#include "gpio_irq_api.h"

#define GPIO_DEFAULT_IRQ_PRIORITY 5

#ifdef __cplusplus
extern "C" {
#endif

void cy_gpio_irq_handler_impl(void *handler_arg, cyhal_gpio_irq_event_t event)
{
    gpio_irq_t *obj = (gpio_irq_t *)handler_arg;
    cyhal_gpio_irq_event_t masked = (cyhal_gpio_irq_event_t)(event & obj->mask);
    void (*handler)(uint32_t, int) = (void (*)(uint32_t, int))obj->handler;
    if (NULL != handler && CYHAL_GPIO_IRQ_NONE != masked) {
        if (CYHAL_GPIO_IRQ_NONE != (masked & CYHAL_GPIO_IRQ_RISE)) {
            (*handler)(obj->id, IRQ_RISE);
        }
        if (CYHAL_GPIO_IRQ_NONE != (masked & CYHAL_GPIO_IRQ_FALL)) {
            (*handler)(obj->id, IRQ_FALL);
        }
    }
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    obj->pin = pin;
    obj->handler = (void *)handler;
    obj->id = id;
    obj->mask = CYHAL_GPIO_IRQ_NONE;
    if (pin != NC) {
        gpio_irq_enable(obj);   // InterruptIn expects IRQ to be initially enabled
    }
    return pin != NC ? 0 : -1;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    if (obj->pin != NC) {
        gpio_irq_disable(obj);
        cyhal_gpio_irq_enable(obj->pin, CYHAL_GPIO_IRQ_BOTH, false);
    }
    obj->pin = NC;
    obj->handler = NULL;
    obj->id = 0;
    obj->mask = CYHAL_GPIO_IRQ_NONE;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    cyhal_gpio_irq_event_t bits;
    if (event == IRQ_RISE) {
        bits = CYHAL_GPIO_IRQ_RISE;
    } else if (event == IRQ_FALL) {
        bits = CYHAL_GPIO_IRQ_FALL;
    } else {
        bits = CYHAL_GPIO_IRQ_NONE;
    }
    obj->mask = (cyhal_gpio_irq_event_t)(enable ? (obj->mask | bits) : (obj->mask & ~bits));
    cyhal_gpio_irq_enable(obj->pin, obj->mask, true);
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    MBED_ASSERT(obj->pin != NC);
    cyhal_gpio_register_irq(obj->pin, GPIO_DEFAULT_IRQ_PRIORITY, &cy_gpio_irq_handler_impl, obj);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    MBED_ASSERT(obj->pin != NC);
    cyhal_gpio_register_irq(obj->pin, GPIO_DEFAULT_IRQ_PRIORITY, NULL, NULL);
}

#ifdef __cplusplus
}
#endif
