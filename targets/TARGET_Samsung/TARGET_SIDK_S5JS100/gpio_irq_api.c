/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
/* @file  : gpio_irq_api.c
 * @brief : GPIO interrupt API source file
 * @date  : June 2019
 *
 * @note  : Add chip dependent feature and support up to 72 Pin
 *
 */

#include <stddef.h>
#include "cmsis.h"
#include "gpio_irq_api.h"
#include "mbed_error.h"
#include "mbed_trace.h"
#include "mbed_assert.h"

#define TRACE_GROUP "GPIO"
#define PIN_NUM 73
#define PININT_IRQ0 S5JS100_IRQ_GPIO_INTR0
#define PININT_IRQ1 S5JS100_IRQ_GPIO_INTR1
#define PININT_IRQ2 S5JS100_IRQ_GPIO_INTR2

struct pin_info {
    uint8_t minor;
    uint32_t ids;
    uint32_t pincfg;
    gpio_irq_event event;

} pins[PIN_NUM] = {
    {0, (uint32_t)NULL, GPIO_GPIO0, IRQ_NONE},
    {1, (uint32_t)NULL, GPIO_GPIO1, IRQ_NONE},
    {2, (uint32_t)NULL, GPIO_GPIO2, IRQ_NONE},
    {3, (uint32_t)NULL, GPIO_GPIO3, IRQ_NONE},
    {4, (uint32_t)NULL, GPIO_GPIO4, IRQ_NONE},
    {5, (uint32_t)NULL, GPIO_GPIO5, IRQ_NONE},
    {6, (uint32_t)NULL, GPIO_GPIO6, IRQ_NONE},
    {7, (uint32_t)NULL, GPIO_GPIO7, IRQ_NONE},
    {8, (uint32_t)NULL, GPIO_GPIO8, IRQ_NONE},
    {9, (uint32_t)NULL, GPIO_GPIO9, IRQ_NONE},
    {10, (uint32_t)NULL, GPIO_GPIO10, IRQ_NONE},
    {11, (uint32_t)NULL, GPIO_GPIO11, IRQ_NONE},
    {12, (uint32_t)NULL, GPIO_GPIO12, IRQ_NONE},
    {13, (uint32_t)NULL, GPIO_GPIO13, IRQ_NONE},
    {14, (uint32_t)NULL, GPIO_GPIO14, IRQ_NONE},
    {15, (uint32_t)NULL, GPIO_GPIO15, IRQ_NONE},
    {16, (uint32_t)NULL, GPIO_GPIO16, IRQ_NONE},
    {17, (uint32_t)NULL, GPIO_GPIO17, IRQ_NONE},
    {18, (uint32_t)NULL, GPIO_GPIO18, IRQ_NONE},
    {19, (uint32_t)NULL, GPIO_GPIO19, IRQ_NONE},
    {20, (uint32_t)NULL, GPIO_GPIO20, IRQ_NONE},
    {21, (uint32_t)NULL, GPIO_GPIO21, IRQ_NONE},
    {22, (uint32_t)NULL, GPIO_GPIO22, IRQ_NONE},
    {23, (uint32_t)NULL, GPIO_GPIO23, IRQ_NONE},
    {24, (uint32_t)NULL, GPIO_GPIO24, IRQ_NONE},
    {25, (uint32_t)NULL, GPIO_GPIO25, IRQ_NONE},
    {26, (uint32_t)NULL, GPIO_GPIO26, IRQ_NONE},
    {27, (uint32_t)NULL, GPIO_GPIO27, IRQ_NONE},
    {28, (uint32_t)NULL, GPIO_GPIO28, IRQ_NONE},
    {29, (uint32_t)NULL, GPIO_GPIO29, IRQ_NONE},
    {30, (uint32_t)NULL, GPIO_GPIO30, IRQ_NONE},
    {31, (uint32_t)NULL, GPIO_GPIO31, IRQ_NONE},
    {32, (uint32_t)NULL, GPIO_GPIO32, IRQ_NONE},
    {33, (uint32_t)NULL, GPIO_GPIO33, IRQ_NONE},
    {34, (uint32_t)NULL, GPIO_GPIO34, IRQ_NONE},
    {35, (uint32_t)NULL, GPIO_GPIO35, IRQ_NONE},
    {36, (uint32_t)NULL, GPIO_GPIO36, IRQ_NONE},
    {37, (uint32_t)NULL, GPIO_GPIO37, IRQ_NONE},
    {38, (uint32_t)NULL, GPIO_GPIO38, IRQ_NONE},
    {39, (uint32_t)NULL, GPIO_GPIO39, IRQ_NONE},
    {40, (uint32_t)NULL, GPIO_GPIO40, IRQ_NONE},
    {41, (uint32_t)NULL, GPIO_GPIO41, IRQ_NONE},
    {42, (uint32_t)NULL, GPIO_GPIO42, IRQ_NONE},
    {43, (uint32_t)NULL, GPIO_GPIO43, IRQ_NONE},
    {44, (uint32_t)NULL, GPIO_GPIO44, IRQ_NONE},
    {45, (uint32_t)NULL, GPIO_GPIO45, IRQ_NONE},
    {46, (uint32_t)NULL, GPIO_GPIO46, IRQ_NONE},
    {47, (uint32_t)NULL, GPIO_GPIO47, IRQ_NONE},
    {48, (uint32_t)NULL, GPIO_GPIO48, IRQ_NONE},
    {49, (uint32_t)NULL, GPIO_GPIO49, IRQ_NONE},
    {50, (uint32_t)NULL, GPIO_GPIO50, IRQ_NONE},
    {51, (uint32_t)NULL, GPIO_GPIO51, IRQ_NONE},
    {52, (uint32_t)NULL, GPIO_GPIO52, IRQ_NONE},
    {53, (uint32_t)NULL, GPIO_GPIO53, IRQ_NONE},
    {54, (uint32_t)NULL, GPIO_GPIO54, IRQ_NONE},
    {55, (uint32_t)NULL, GPIO_GPIO55, IRQ_NONE},
    {56, (uint32_t)NULL, GPIO_GPIO56, IRQ_NONE},
    {57, (uint32_t)NULL, GPIO_GPIO57, IRQ_NONE},
    {58, (uint32_t)NULL, GPIO_GPIO58, IRQ_NONE},
    {59, (uint32_t)NULL, GPIO_GPIO59, IRQ_NONE},
    {60, (uint32_t)NULL, GPIO_GPIO60, IRQ_NONE},
    {61, (uint32_t)NULL, GPIO_GPIO61, IRQ_NONE},
    {62, (uint32_t)NULL, GPIO_GPIO62, IRQ_NONE},
    {63, (uint32_t)NULL, GPIO_GPIO63, IRQ_NONE},
    {64, (uint32_t)NULL, GPIO_GPIO64, IRQ_NONE},
    {65, (uint32_t)NULL, GPIO_GPIO65, IRQ_NONE},
    {66, (uint32_t)NULL, GPIO_GPIO66, IRQ_NONE},
    {67, (uint32_t)NULL, GPIO_GPIO67, IRQ_NONE},
    {68, (uint32_t)NULL, GPIO_GPIO68, IRQ_NONE},
    {69, (uint32_t)NULL, GPIO_GPIO69, IRQ_NONE},
    {70, (uint32_t)NULL, GPIO_GPIO70, IRQ_NONE},
    {71, (uint32_t)NULL, GPIO_GPIO71, IRQ_NONE},
    {72, (uint32_t)NULL, GPIO_GPIO72, IRQ_NONE}
};

static gpio_irq_handler irq_handler;
uint32_t s5js100_get_intpin(unsigned int ch);
int s5js100_configinterrupt(uint32_t cfgset, uint32_t nirq);
void s5js100_gpio_clear_pending(uint32_t pincfg);

static inline void handle_interrupt_in(uint32_t channel)
{
    int pin;
    gpio_irq_event event;


    pin = s5js100_get_intpin(channel);
    if (pin < 0) {
        return;
    }
    event = pins[pin].event;
    MBED_ASSERT(pin < PIN_NUM);

    irq_handler(pins[pin].ids, pins[pin].event); //should be fixed by polarity
#if GPIO_EINT_DEBOUNCE
    hw_delay_us(200000);
#endif
    // toggle Interrupt condition
    if (event == IRQ_RISE) {
        pins[pin].pincfg &= ~(GPIO_EINT_MASK);
        pins[pin].event = IRQ_FALL;
        pins[pin].pincfg |= GPIO_EINT | GPIO_EINT_FALLING_EDGE;
    }
    if (event == IRQ_FALL) {
        pins[pin].pincfg &= ~(GPIO_EINT_MASK);
        pins[pin].event = IRQ_RISE;
        pins[pin].pincfg |= GPIO_EINT | GPIO_EINT_RISING_EDGE;
    }

    s5js100_configinterrupt(pins[pin].pincfg, (pins[pin].pincfg & GPIO_IRQ_MASK) >> GPIO_IRQ_SHIFT);
    s5js100_gpio_clear_pending(pins[pin].pincfg); //
}

void gpio0_irq(void)
{
    handle_interrupt_in(0);
}
void gpio1_irq(void)
{
    handle_interrupt_in(1);
}
void gpio2_irq(void)
{
    handle_interrupt_in(2);
}

int gpio_pin_mode(PinName pin, PinMode mode)
{
    if (mode == PullUp) {
        pins[pin].pincfg &= ~(GPIO_PUPD_MASK);
        pins[pin].pincfg |= GPIO_PULLUP;
    }
    if (mode == PullDown) {
        pins[pin].pincfg &= ~(GPIO_PUPD_MASK);
        pins[pin].pincfg |= GPIO_PULLDOWN;
    }
    return 0;
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin,
                  gpio_irq_handler handler, uint32_t id)
{
    if (pin == NC) {
        return -1;
    }
    MBED_ASSERT(pin < PIN_NUM);

    obj->ch = pin;
    irq_handler = handler;

    // set handler for apps
    pins[obj->ch].ids = id;

    NVIC_SetVector((IRQn_Type)(PININT_IRQ0), (uint32_t)gpio0_irq);
    NVIC_SetVector((IRQn_Type)(PININT_IRQ1), (uint32_t)gpio1_irq);
    NVIC_SetVector((IRQn_Type)(PININT_IRQ2), (uint32_t)gpio2_irq);
#if defined (__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    SCB_InvalidateICache();
#endif
    NVIC_EnableIRQ((IRQn_Type)(PININT_IRQ0));
    NVIC_EnableIRQ((IRQn_Type)(PININT_IRQ1));
    NVIC_EnableIRQ((IRQn_Type)(PININT_IRQ2));

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{

}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    if (IRQ_RISE == event) {
        //obj->flagR = (uint8_t)enable;
#if GPIO_EINT_LEVEL
        pins[obj->ch].pincfg &= ~(GPIO_INT_MASK | GPIO_EINT_MASK | GPIO_PUPD_MASK);
        pins[obj->ch].pincfg |= GPIO_EINT | GPIO_EINT_HIGH | GPIO_PULLDOWN;
#else
        pins[obj->ch].pincfg |= GPIO_EINT | GPIO_EINT_RISING_EDGE;
#endif
        pins[obj->ch].event = event;
    } else {
        //obj->flagF = (uint8_t)enable;
#if GPIO_EINT_LEVEL
        pins[obj->ch].pincfg &= ~(GPIO_INT_MASK | GPIO_EINT_MASK | GPIO_PUPD_MASK);
        pins[obj->ch].pincfg |= GPIO_EINT | GPIO_EINT_LOW | GPIO_PULLUP;
#else
        pins[obj->ch].pincfg |= GPIO_EINT | GPIO_EINT_FALLING_EDGE;
#endif
        pins[obj->ch].event = event;
    }
    s5js100_configgpio(pins[obj->ch].pincfg);
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    NVIC_EnableIRQ((IRQn_Type)(PININT_IRQ0 + (obj->ch / 32)));
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    NVIC_DisableIRQ((IRQn_Type)(PININT_IRQ0 + (obj->ch / 32)));
}
