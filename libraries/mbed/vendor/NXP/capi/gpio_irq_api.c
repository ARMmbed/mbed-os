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
#include "gpio_irq_api.h"
#include "error.h"

#if DEVICE_INTERRUPTIN

#include <stddef.h>
#include "cmsis.h"

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
#define CHANNEL_NUM     48

#elif defined(TARGET_LPC11U24)
#define CHANNEL_NUM    8
#endif

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler;

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
static void handle_interrupt_in(void) {
    // Read in all current interrupt registers. We do this once as the
    // GPIO interrupt registers are on the APB bus, and this is slow.
    uint32_t rise0 = LPC_GPIOINT->IO0IntStatR;
    uint32_t fall0 = LPC_GPIOINT->IO0IntStatF;
    uint32_t rise2 = LPC_GPIOINT->IO2IntStatR;
    uint32_t fall2 = LPC_GPIOINT->IO2IntStatF;
    uint32_t mask0 = 0;
    uint32_t mask2 = 0;
    int i;

    // P0.0-0.31
    for (i = 0; i < 32; i++) {
        uint32_t pmask = (1 << i);
        if (rise0 & pmask) {
            mask0 |= pmask;
            if (channel_ids[i] != 0)
                irq_handler(channel_ids[i], IRQ_RISE);
        }
        if (fall0 & pmask) {
            mask0 |= pmask;
            if (channel_ids[i] != 0)
                irq_handler(channel_ids[i], IRQ_FALL);
        }
    }

    // P2.0-2.15
    for (i = 0; i < 16; i++) {
        uint32_t pmask = (1 << i);
        int channel_index = i + 32;
        if (rise2 & pmask) {
            mask2 |= pmask;
            if (channel_ids[channel_index] != 0)
                irq_handler(channel_ids[channel_index], IRQ_RISE);
        }
        if (fall2 & pmask) {
            mask2 |= pmask;
            if (channel_ids[channel_index] != 0)
                irq_handler(channel_ids[channel_index], IRQ_FALL);
        }
    }

    // Clear the interrupts we just handled
    LPC_GPIOINT->IO0IntClr = mask0;
    LPC_GPIOINT->IO2IntClr = mask2;
}

#elif defined(TARGET_LPC11U24)
static inline void handle_interrupt_in(uint32_t channel) {
    uint32_t ch_bit = (1 << channel);
    // Return immediately if:
    //   * The interrupt was already served
    //   * There is no user handler
    //   * It is a level interrupt, not an edge interrupt
    if ( ((LPC_GPIO_PIN_INT->IST & ch_bit) == 0) ||
         (channel_ids[channel] == 0            ) ||
         (LPC_GPIO_PIN_INT->ISEL & ch_bit      ) ) return;

    if ((LPC_GPIO_PIN_INT->IENR & ch_bit) && (LPC_GPIO_PIN_INT->RISE & ch_bit)) {
        irq_handler(channel_ids[channel], IRQ_RISE);
        LPC_GPIO_PIN_INT->RISE = ch_bit;
    }
    if ((LPC_GPIO_PIN_INT->IENF & ch_bit) && (LPC_GPIO_PIN_INT->FALL & ch_bit)) {
        irq_handler(channel_ids[channel], IRQ_FALL);
    }
    LPC_GPIO_PIN_INT->IST = ch_bit;
}

void gpio_irq0(void) {handle_interrupt_in(0);}
void gpio_irq1(void) {handle_interrupt_in(1);}
void gpio_irq2(void) {handle_interrupt_in(2);}
void gpio_irq3(void) {handle_interrupt_in(3);}
void gpio_irq4(void) {handle_interrupt_in(4);}
void gpio_irq5(void) {handle_interrupt_in(5);}
void gpio_irq6(void) {handle_interrupt_in(6);}
void gpio_irq7(void) {handle_interrupt_in(7);}
#endif

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id) {
    if (pin == NC) return -1;

    irq_handler = handler;

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
    obj->port = (int)pin & ~0x1F;
    obj->pin = (int)pin & 0x1F;

    // Interrupts available only on GPIO0 and GPIO2
    if (obj->port != LPC_GPIO0_BASE && obj->port != LPC_GPIO2_BASE) {
        error("pins on this port cannot generate interrupts\n");
    }

    // put us in the interrupt table
    int index = (obj->port == LPC_GPIO0_BASE) ? obj->pin : obj->pin + 32;
    channel_ids[index] = id;
    obj->ch = index;

    NVIC_SetVector(EINT3_IRQn, (uint32_t)handle_interrupt_in);
    NVIC_EnableIRQ(EINT3_IRQn);

#elif defined(TARGET_LPC11U24)
    int found_free_channel = 0;
    int i = 0;
    for (i=0; i<CHANNEL_NUM; i++) {
        if (channel_ids[i] == 0) {
            channel_ids[i] = id;
            obj->ch = i;
            found_free_channel = 1;
            break;
        }
    }
    if (!found_free_channel) return -1;

    /* Enable AHB clock to the GPIO domain. */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

    /* Enable AHB clock to the FlexInt, GroupedInt domain. */
    LPC_SYSCON->SYSAHBCLKCTRL |= ((1<<19) | (1<<23) | (1<<24));

    /* To select a pin for any of the eight pin interrupts, write the pin number
     * as 0 to 23 for pins PIO0_0 to PIO0_23 and 24 to 55.
     * @see: mbed_capi/PinNames.h
     */
    LPC_SYSCON->PINTSEL[obj->ch] = (pin >> 5) ? (pin - 8) : (pin);

    // Interrupt Wake-Up Enable
    LPC_SYSCON->STARTERP0 |= 1 << obj->ch;

    void (*channels_irq)(void) = NULL;
    switch (obj->ch) {
        case 0: channels_irq = &gpio_irq0; break;
        case 1: channels_irq = &gpio_irq1; break;
        case 2: channels_irq = &gpio_irq2; break;
        case 3: channels_irq = &gpio_irq3; break;
        case 4: channels_irq = &gpio_irq4; break;
        case 5: channels_irq = &gpio_irq5; break;
        case 6: channels_irq = &gpio_irq6; break;
        case 7: channels_irq = &gpio_irq7; break;
    }
    NVIC_SetVector((IRQn_Type)obj->ch, (uint32_t)channels_irq);
    NVIC_EnableIRQ((IRQn_Type)obj->ch);
#endif
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj) {
    channel_ids[obj->ch] = 0;
#if defined(TARGET_LPC11U24)
    LPC_SYSCON->STARTERP0 &= ~(1 << obj->ch);
#endif
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable) {
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
    // ensure nothing is pending
    switch (obj->port) {
         case LPC_GPIO0_BASE: LPC_GPIOINT->IO0IntClr = 1 << obj->pin; break;
         case LPC_GPIO2_BASE: LPC_GPIOINT->IO2IntClr = 1 << obj->pin; break;
    }

    // enable the pin interrupt
    if (event == IRQ_RISE) {
        switch (obj->port) {
            case LPC_GPIO0_BASE:
                if (enable) {
                    LPC_GPIOINT->IO0IntEnR |= 1 << obj->pin;
                } else {
                    LPC_GPIOINT->IO0IntEnR &= ~(1 << obj->pin);
                }
                break;
            case LPC_GPIO2_BASE:
                if (enable) {
                    LPC_GPIOINT->IO2IntEnR |= 1 << obj->pin;
                } else {
                    LPC_GPIOINT->IO2IntEnR &= ~(1 << obj->pin);
                }
                break;
        }
    } else {
        switch (obj->port) {
            case LPC_GPIO0_BASE:
                if (enable) {
                    LPC_GPIOINT->IO0IntEnF |= 1 << obj->pin;
                } else {
                    LPC_GPIOINT->IO0IntEnF &= ~(1 << obj->pin);
                }
                break;
            case LPC_GPIO2_BASE:
                if (enable) {
                    LPC_GPIOINT->IO2IntEnF |= 1 << obj->pin;
                } else {
                    LPC_GPIOINT->IO2IntEnF &= ~(1 << obj->pin);
                }
                break;
        }
    }
#elif defined(TARGET_LPC11U24)
    unsigned int ch_bit = (1 << obj->ch);

    // Clear interrupt
    if (!(LPC_GPIO_PIN_INT->ISEL & ch_bit))
        LPC_GPIO_PIN_INT->IST = ch_bit;

    // Edge trigger
    LPC_GPIO_PIN_INT->ISEL &= ~ch_bit;
    if (event == IRQ_RISE) {
        if (enable) {
            LPC_GPIO_PIN_INT->IENR |= ch_bit;
        } else {
            LPC_GPIO_PIN_INT->IENR &= ~ch_bit;
        }
    } else {
        if (enable) {
            LPC_GPIO_PIN_INT->IENF |= ch_bit;
        } else {
            LPC_GPIO_PIN_INT->IENF &= ~ch_bit;
        }
    }
#endif
}

#endif
