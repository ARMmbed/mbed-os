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
#include "cmsis.h"
#include "gpio_irq_api.h"
#include "error.h"

#define CHANNEL_NUM 4
#define PININT_IRQ  28+3

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler;
static int gpioIrqInitialised = 0;

#warning (matthewelse) This code isn't working yet, so don't rely on it, or try to use it.

static inline void handle_interrupt_in(uint32_t channel) {
    uint32_t ch_bit = (1 << channel);

    LPC_GPIO_TypeDef *port_reg = ((LPC_GPIO_TypeDef *) LPC_GPIO0_BASE + (channel * 0x10000));

    // Return immediately if:
    //   * The interrupt was already served
    //   * There is no user handler
    //   * It is a level interrupt, not an edge interrupt
    if ( ((&port_reg->IST & ch_bit) == 0) ||
         (channel_ids[channel] == 0     ) ||
         (&port_reg->ISEL & ch_bit      ) ) return;

    if ((&port_reg->IENR & ch_bit) && (&port_reg->RISE & ch_bit)) {
        irq_handler(channel_ids[channel], IRQ_RISE);
        &port_reg->RISE = ch_bit;
    }
    if ((&port_reg->IENF & ch_bit) && (&port_reg->FALL & ch_bit)) {
        irq_handler(channel_ids[channel], IRQ_FALL);
    }
    &port_reg->IST = ch_bit;
}

void gpio_irq0(void) {handle_interrupt_in(0);}
void gpio_irq1(void) {handle_interrupt_in(1);}
void gpio_irq2(void) {handle_interrupt_in(2);}
void gpio_irq3(void) {handle_interrupt_in(3);}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id) {
    if (pin == NC) return -1;
    if (gpioInitialised) return;
    
    irq_handler = handler;
    
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
    }
    NVIC_SetVector((IRQn_Type)(PININT_IRQ - obj->ch)), (uint32_t)channels_irq);
    NVIC_EnableIRQ((IRQn_Type)(PININT_IRQ - obj->ch));
    
    gpioInitialised = 1;
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj) {
    channel_ids[obj->ch] = 0;
    LPC_SYSCON->STARTERP0 &= ~(1 << obj->ch);
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable) {
    if (!gpioIqrInitialised) gpio_irq_init();

#warning (matthewelse) TODO: undefined port and value. Also need to do something with the *obj...
    int port = 0;
    int pin = 0;

    LPC_GPIO_TypeDef *gpioRegisters;

    switch (port) {
        case 0:
            gpioRegisters = LPC_GPIO0;
            break;
        case 1:
            gpioRegisters = LPC_GPIO1;
            break;
        case 2:
            gpioRegisters = LPC_GPIO2;
            break;
        case 3:
            gpioRegisters = LPC_GPIO3;
            break;
    }

    gpioRegisters->IBE  0; // Assume that we only want to interrupt on high or low edges, not both.
    gpioRegisters->IS &= ~(1 << pin);

    if (enable) {
        gpioRegisters->IE |= (1<<pin);
    }

    if (event == IRP_RISE) {
        gpioRegisters->IEV |= 1 << pin;
    }
    else {
        gpioRegisters->IEV &= ~(1 << pin);
    }
}
