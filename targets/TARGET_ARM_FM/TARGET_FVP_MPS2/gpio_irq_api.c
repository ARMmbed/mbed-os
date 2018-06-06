/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#include "cmsis.h"
#include "gpio_irq_api.h"
#include "mbed_error.h"

#define CHANNEL_NUM    32
#define CMSDK_GPIO_0 CMSDK_GPIO0
#define CMSDK_GPIO_1 CMSDK_GPIO1
#define PININT_IRQ 0

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler;

static inline void handle_interrupt_in(uint32_t channel)
{
    uint32_t ch_bit = (1 << channel);
    // Return immediately if:
    //   * The interrupt was already served
    //   * There is no user handler
    //   * It is a level interrupt, not an edge interrupt
    if (ch_bit < 16) {
        if (((CMSDK_GPIO_0->INTSTATUS) == 0) || (channel_ids[channel] == 0) || ((CMSDK_GPIO_0->INTTYPESET) == 0)) {
            return;
        }

        if ((CMSDK_GPIO_0->INTTYPESET & ch_bit) && (CMSDK_GPIO_0->INTPOLSET & ch_bit)) {
            irq_handler(channel_ids[channel], IRQ_RISE);
            CMSDK_GPIO_0->INTPOLSET = ch_bit;
        }
        if ((CMSDK_GPIO_0->INTTYPESET & ch_bit) && ~(CMSDK_GPIO_0->INTPOLSET & ch_bit)) {
            irq_handler(channel_ids[channel], IRQ_FALL);
        }
        CMSDK_GPIO_0->INTCLEAR = ch_bit;
    }

    if (ch_bit >= 16) {
        if (((CMSDK_GPIO_1->INTSTATUS) == 0) || (channel_ids[channel] == 0) || ((CMSDK_GPIO_1->INTTYPESET) == 0)) {
            return;
        }

        if ((CMSDK_GPIO_1->INTTYPESET & ch_bit) && (CMSDK_GPIO_1->INTPOLSET & ch_bit)) {
            irq_handler(channel_ids[channel], IRQ_RISE);
            CMSDK_GPIO_1->INTPOLSET = ch_bit;
        }
        if ((CMSDK_GPIO_1->INTTYPESET & ch_bit) && ~(CMSDK_GPIO_1->INTPOLSET & ch_bit)) {
            irq_handler(channel_ids[channel], IRQ_FALL);
        }
        CMSDK_GPIO_1->INTCLEAR = ch_bit;
    }
}

void gpio0_irq0(void)
{
    handle_interrupt_in(0);
}
void gpio0_irq1(void)
{
    handle_interrupt_in(1);
}
void gpio0_irq2(void)
{
    handle_interrupt_in(2);
}
void gpio0_irq3(void)
{
    handle_interrupt_in(3);
}
void gpio0_irq4(void)
{
    handle_interrupt_in(4);
}
void gpio0_irq5(void)
{
    handle_interrupt_in(5);
}
void gpio0_irq6(void)
{
    handle_interrupt_in(6);
}
void gpio0_irq7(void)
{
    handle_interrupt_in(7);
}
void gpio0_irq8(void)
{
    handle_interrupt_in(8);
}
void gpio0_irq9(void)
{
    handle_interrupt_in(9);
}
void gpio0_irq10(void)
{
    handle_interrupt_in(10);
}
void gpio0_irq11(void)
{
    handle_interrupt_in(11);
}
void gpio0_irq12(void)
{
    handle_interrupt_in(12);
}
void gpio0_irq13(void)
{
    handle_interrupt_in(13);
}
void gpio0_irq14(void)
{
    handle_interrupt_in(14);
}
void gpio0_irq15(void)
{
    handle_interrupt_in(15);
}
void gpio1_irq0(void)
{
    handle_interrupt_in(16);
}
void gpio1_irq1(void)
{
    handle_interrupt_in(17);
}
void gpio1_irq2(void)
{
    handle_interrupt_in(18);
}
void gpio1_irq3(void)
{
    handle_interrupt_in(19);
}
void gpio1_irq4(void)
{
    handle_interrupt_in(20);
}
void gpio1_irq5(void)
{
    handle_interrupt_in(21);
}
void gpio1_irq6(void)
{
    handle_interrupt_in(22);
}
void gpio1_irq7(void)
{
    handle_interrupt_in(23);
}
void gpio1_irq8(void)
{
    handle_interrupt_in(24);
}
void gpio1_irq9(void)
{
    handle_interrupt_in(25);
}
void gpio1_irq10(void)
{
    handle_interrupt_in(26);
}
void gpio1_irq11(void)
{
    handle_interrupt_in(27);
}
void gpio1_irq12(void)
{
    handle_interrupt_in(28);
}
void gpio1_irq13(void)
{
    handle_interrupt_in(29);
}
void gpio1_irq14(void)
{
    handle_interrupt_in(30);
}
void gpio1_irq15(void)
{
    handle_interrupt_in(31);
}


int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    if (pin == NC) {
        return -1;
    } else {

        irq_handler = handler;

        int found_free_channel = 0;
        int i = 0;
        for (i = 0; i < CHANNEL_NUM; i++) {
            if (channel_ids[i] == 0) {
                channel_ids[i] = id;
                obj->ch = i;
                found_free_channel = 1;
                break;
            }
        }
        if (!found_free_channel) {
            return -1;
        }


        /* To select a pin for any of the eight pin interrupts, write the pin number
         * as 0 to 23 for pins PIO0_0 to PIO0_23 and 24 to 55.
         * @see: mbed_capi/PinNames.h
         */
        if (pin < 16) {
            CMSDK_GPIO_0->INTENSET |= (0x1 << pin);
        }

        if (pin >= 16) {
            CMSDK_GPIO_1->INTENSET |= (0x1 << pin);
        }

        void (*channels_irq)(void) = NULL;
        switch (obj->ch) {
            case 0:
                channels_irq = &gpio0_irq0;
                break;
            case 1:
                channels_irq = &gpio0_irq1;
                break;
            case 2:
                channels_irq = &gpio0_irq2;
                break;
            case 3:
                channels_irq = &gpio0_irq3;
                break;
            case 4:
                channels_irq = &gpio0_irq4;
                break;
            case 5:
                channels_irq = &gpio0_irq5;
                break;
            case 6:
                channels_irq = &gpio0_irq6;
                break;
            case 7:
                channels_irq = &gpio0_irq7;
                break;
            case 8:
                channels_irq = &gpio0_irq8;
                break;
            case 9:
                channels_irq = &gpio0_irq9;
                break;
            case 10:
                channels_irq = &gpio0_irq10;
                break;
            case 11:
                channels_irq = &gpio0_irq11;
                break;
            case 12:
                channels_irq = &gpio0_irq12;
                break;
            case 13:
                channels_irq = &gpio0_irq13;
                break;
            case 14:
                channels_irq = &gpio0_irq14;
                break;
            case 15:
                channels_irq = &gpio0_irq15;
                break;
            case 16:
                channels_irq = &gpio1_irq0;
                break;
            case 17:
                channels_irq = &gpio1_irq1;
                break;
            case 18:
                channels_irq = &gpio1_irq2;
                break;
            case 19:
                channels_irq = &gpio1_irq3;
                break;
            case 20:
                channels_irq = &gpio1_irq4;
                break;
            case 21:
                channels_irq = &gpio1_irq5;
                break;
            case 22:
                channels_irq = &gpio1_irq6;
                break;
            case 23:
                channels_irq = &gpio1_irq7;
                break;
            case 24:
                channels_irq = &gpio1_irq8;
                break;
            case 25:
                channels_irq = &gpio1_irq9;
                break;
            case 26:
                channels_irq = &gpio1_irq10;
                break;
            case 27:
                channels_irq = &gpio1_irq11;
                break;
            case 28:
                channels_irq = &gpio1_irq12;
                break;
            case 29:
                channels_irq = &gpio1_irq13;
                break;
            case 30:
                channels_irq = &gpio1_irq14;
                break;
            case 31:
                channels_irq = &gpio1_irq15;
                break;

        }
        NVIC_SetVector((IRQn_Type)(PININT_IRQ + obj->ch), (uint32_t)channels_irq);
        NVIC_EnableIRQ((IRQn_Type)(PININT_IRQ + obj->ch));

        return 0;
    }
}

void gpio_irq_free(gpio_irq_t *obj)
{
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    unsigned int ch_bit = (1 << obj->ch);

    // Clear interrupt
    if (obj->ch < 16) {
        if (!(CMSDK_GPIO_0->INTTYPESET & ch_bit)) {
            CMSDK_GPIO_0->INTCLEAR = ch_bit;
        }
    }
    if (obj->ch >= 16) {
        if (!(CMSDK_GPIO_1->INTTYPESET & ch_bit)) {
            CMSDK_GPIO_1->INTCLEAR = ch_bit;
        }
    }

    // Edge trigger
    if (obj->ch < 16) {
        CMSDK_GPIO_0->INTTYPESET &= ch_bit;
        if (event == IRQ_RISE) {
            CMSDK_GPIO_0->INTPOLSET |= ch_bit;
            if (enable) {
                CMSDK_GPIO_0->INTENSET |= ch_bit;
            } else {
                CMSDK_GPIO_0->INTENCLR |= ch_bit;
            }
        } else {
            CMSDK_GPIO_0->INTPOLCLR |= ch_bit;
            if (enable) {
                CMSDK_GPIO_0->INTENSET |= ch_bit;
            } else {
                CMSDK_GPIO_0->INTENCLR |= ch_bit;
            }
        }
    }
    if (obj->ch >= 16) {
        CMSDK_GPIO_1->INTTYPESET &= ch_bit;
        if (event == IRQ_RISE) {
            CMSDK_GPIO_1->INTPOLSET |= ch_bit;
            if (enable) {
                CMSDK_GPIO_1->INTENSET |= ch_bit;
            } else {
                CMSDK_GPIO_1->INTENCLR |= ch_bit;
            }
        } else {
            CMSDK_GPIO_1->INTPOLCLR |= ch_bit;
            if (enable) {
                CMSDK_GPIO_1->INTENSET |= ch_bit;
            } else {
                CMSDK_GPIO_1->INTENCLR |= ch_bit;
            }
        }
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    NVIC_EnableIRQ((IRQn_Type)(PININT_IRQ + obj->ch));
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    NVIC_DisableIRQ((IRQn_Type)(PININT_IRQ + obj->ch));
}
