/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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
#include "cmsis.h"
#include "PeripheralPins.h"

#include "ti/devices/cc32xx/inc/hw_types.h"
#include "ti/devices/cc32xx/inc/hw_gpio.h"
#include "ti/devices/cc32xx/driverlib/gpio.h"
#include "ti/devices/cc32xx/inc/hw_ints.h"
#include "ti/devices/cc32xx/inc/hw_memmap.h"
#include "ti/devices/cc32xx/inc/hw_common_reg.h"

#define CHANNEL_NUM     32

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler;

static void handle_interrupt_in(unsigned long gpio_port)
{

    uint32_t chan_base = 0;
    switch ((unsigned long) gpio_port) {
        case CC3220SF_GPIOA0_BASE:
            chan_base = 0;
            break;
        case CC3220SF_GPIOA1_BASE:
            chan_base = 8;
            break;
        case CC3220SF_GPIOA2_BASE:
            chan_base = 16;
            break;
        case CC3220SF_GPIOA3_BASE:
            chan_base = 24;
            break;
        default:
            return;
            break;
    }

    uint16_t pin_mask = 0x01;
    for (int i = 0; i < 8; i++) {

        //checking for interrupt on each GPIO pin
        if ((GPIOIntStatus((unsigned long)gpio_port, true) & pin_mask) > 0) {
            gpio_irq_event event = (gpio_irq_event)GPIOIntTypeGet((unsigned long)gpio_port, pin_mask);
            if (event == GPIO_RISING_EDGE) {
                event = IRQ_RISE;
            } else if (event == GPIO_FALLING_EDGE) {
                event = IRQ_FALL;
            }

            if (channel_ids[chan_base + i] == 0) {
                continue;
            }

            irq_handler(channel_ids[chan_base + i], (gpio_irq_event)event);

        }
        GPIOIntClear((unsigned long)gpio_port, pin_mask);
        pin_mask = pin_mask << 1;
    }

}

void gpio_irqA0(void)
{
    handle_interrupt_in(CC3220SF_GPIOA0_BASE);
}

void gpio_irqA1(void)
{
    handle_interrupt_in(CC3220SF_GPIOA1_BASE);
}

void gpio_irqA2(void)
{
    handle_interrupt_in(CC3220SF_GPIOA2_BASE);
}

void gpio_irqA3(void)
{
    handle_interrupt_in(CC3220SF_GPIOA3_BASE);
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    if (pin == NC) {
        return -1;
    }



    unsigned long gpio_base = (unsigned long)pinmap_peripheral(pin, PinMap_GPIO);
    unsigned long ch_num = pinmap_find_function(pin, PinMap_GPIO);
    obj->baseAddr = gpio_base;
    obj->pin = pin;
    obj->ch = ch_num;
    obj->pin_mask = 1 << (ch_num % 8);
    irq_handler = handler;
    uint32_t vector = (uint32_t)gpio_irqA0;
    switch (gpio_base) {
        case CC3220SF_GPIOA0_BASE:
            vector = (uint32_t)gpio_irqA0;
            obj->irq_offset = INT_GPIOA0_IRQn;
            break;
        case CC3220SF_GPIOA1_BASE:
            vector = (uint32_t)gpio_irqA1;
            obj->irq_offset = INT_GPIOA1_IRQn;
            break;
        case CC3220SF_GPIOA2_BASE:
            vector = (uint32_t)gpio_irqA2;
            obj->irq_offset = INT_GPIOA2_IRQn;
            break;
        case CC3220SF_GPIOA3_BASE:
            vector = (uint32_t)gpio_irqA3;
            obj->irq_offset = INT_GPIOA3_IRQn;
            break;
        default:
            break;
    }
    channel_ids[obj->ch] = id;
    NVIC_ClearPendingIRQ((IRQn_Type)obj->irq_offset);
    NVIC_DisableIRQ((IRQn_Type)obj->irq_offset);
    NVIC_SetVector((IRQn_Type)obj->irq_offset, vector);
    NVIC_EnableIRQ((IRQn_Type)obj->irq_offset);

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    channel_ids[obj->ch] = 0;
    GPIOIntDisable(obj->baseAddr, obj->pin_mask);
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{

    if (enable) {
        GPIOIntEnable(obj->baseAddr, obj->pin_mask);
    } else {
        GPIOIntDisable(obj->baseAddr, obj->pin_mask);
    }

    switch (event) {
        case IRQ_RISE:
            GPIOIntTypeSet(obj->baseAddr, obj->pin_mask, GPIO_RISING_EDGE);
            break;
        case IRQ_FALL:
            GPIOIntTypeSet(obj->baseAddr, obj->pin_mask, GPIO_FALLING_EDGE);
            break;
        default:
            break;
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    GPIOIntEnable(obj->baseAddr, obj->pin_mask);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    GPIOIntDisable(obj->baseAddr, obj->pin_mask);
}
