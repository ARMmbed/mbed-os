/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stddef.h>
#include "cmsis.h"
#include "gpio_irq_api.h"
#include "mbed_error.h"

/****************************************************************************
 * Pre-processor definitions
 ****************************************************************************/
#define CHANNEL_NUM                     48
#define MAX_PIN_PER_PORT                16
static gpio_irq_handler irq_handler;
static uintptr_t channel_contexts[CHANNEL_NUM];
static gpio_irq_event pins_event[CHANNEL_NUM];

static inline void handle_gpio_irq(uint32_t port)
{
    int i;
    uint32_t pin_name = port << 4;

    BP_GPIO_TypeDef *gpio_base = (BP_GPIO_TypeDef *)GPIO_PORT_BASE(port);

    for (i = 0; i < MAX_PIN_PER_PORT; i++) {
        if (gpio_base->INTSTAT & (1u << i)) {
            pin_name += i;
            NVIC_ClearPendingIRQ(PORT0_0_IRQn + port * 16 + i);
            gpio_base->INTCLR = 1u << i;
            irq_handler(channel_contexts[pin_name], pins_event[pin_name]);
            break;
        }
    }
}

void gpio0_irq(void)
{
    handle_gpio_irq(0);
    NVIC_ClearPendingIRQ(PORT0_ALL_IRQn);
}
void gpio1_irq(void)
{
    handle_gpio_irq(1);
    NVIC_ClearPendingIRQ(PORT1_ALL_IRQn);
}
void gpio2_irq(void)
{
    handle_gpio_irq(2);
    NVIC_ClearPendingIRQ(PORT2_ALL_IRQn);
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin,
                  gpio_irq_handler handler, uintptr_t context)
{
    if (pin == (PinName)NC) {
        return -1;
    }

    obj->pin = pin;
    irq_handler = handler;
    channel_contexts[pin] = context;

    bp6a_gpio_set_dir(BP6A_PORT_IDX(obj->pin), BP6A_PIN_IDX(obj->pin), true);
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    channel_contexts[obj->pin] = 0;
    gpio_irq_disable(obj);
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    pins_event[obj->pin] = event;

    if (event == IRQ_NONE) {
        return;
    }

    if (event == IRQ_RISE) {
        bp6a_gpio_set_int_type(BP6A_PORT_IDX(obj->pin), BP6A_PIN_IDX(obj->pin),
                               true, true);
    } else {
        bp6a_gpio_set_int_type(BP6A_PORT_IDX(obj->pin), BP6A_PIN_IDX(obj->pin),
                               false, true);
    }
    if (enable) {
        gpio_irq_enable(obj);
    } else {
        gpio_irq_disable(obj);
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    uint32_t port = BP6A_PORT_IDX(obj->pin);

    bp6a_gpio_set_int(port, BP6A_PIN_IDX(obj->pin), true);

    void (*port_irq)(void) = NULL;

    switch (port) {
        case 0:
            port_irq = &gpio0_irq;
            break;
        case 1:
            port_irq = &gpio1_irq;
            break;
        case 2:
            port_irq = &gpio2_irq;
            break;
    }

    NVIC_SetVector((IRQn_Type)(PORT0_0_IRQn + port * 16 + BP6A_PIN_IDX(obj->pin)), (uint32_t)port_irq);

#if defined (__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    SCB_InvalidateICache();
#endif

    NVIC_EnableIRQ((IRQn_Type)(PORT0_0_IRQn + port * 16 + BP6A_PIN_IDX(obj->pin)));
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    uint32_t port_n =  BP6A_PORT_IDX(obj->pin);
    uint32_t pin_n = BP6A_PIN_IDX(obj->pin);

    NVIC_ClearPendingIRQ((IRQn_Type)(PORT0_ALL_IRQn + port_n));
    bp6a_gpio_clear_pend(port_n, pin_n);

    NVIC_DisableIRQ((IRQn_Type)(PORT0_ALL_IRQn + port_n));
    bp6a_gpio_set_int(port_n, pin_n, false);
}
