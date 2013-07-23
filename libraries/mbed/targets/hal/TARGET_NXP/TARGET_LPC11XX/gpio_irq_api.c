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
static uint32_t channel = 0;

#warning (matthewelse) This code isn't working yet, so don't rely on it, or try to use it.

static inline void handle_interrupt_in(uint32_t channel) {

#error (matthewelse) There's no way this code will work now...
    uint32_t ch_bit = (1 << channel);
    LPC_GPIO_TypeDef *port_reg = ((LPC_GPIO_TypeDef *) (LPC_GPIO0_BASE + (channel * 0x10000)));
    
}

void gpio_irq0(void) {handle_interrupt_in(0);}
void gpio_irq1(void) {handle_interrupt_in(1);}
void gpio_irq2(void) {handle_interrupt_in(2);}
void gpio_irq3(void) {handle_interrupt_in(3);}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id) {
    if (pin == NC) return -1;
    
    channel_ids[channel] = id;
    irq_handler = handler;

    //obj->pin = pin;
    obj->ch = channel;
    
    NVIC_EnableIRQ(EINT0_IRQn);
    NVIC_EnableIRQ(EINT1_IRQn);
    NVIC_EnableIRQ(EINT2_IRQn);
    NVIC_EnableIRQ(EINT3_IRQn);
    
    channel++;
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj) {
    channel_ids[obj->ch] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable) {
    // TODO: Debug this to find out what data is put in the obj object at runtime...

    LPC_GPIO_TypeDef *gpioReg;

    // Firstly, clear the interrupts for this pin.
    // Then, let the registers know whether we're looking for edge detection...
    // And enable the interrupt
    // And set it to only respond to interrupts on one edge.
    switch (obj->port) {
        case LPC_GPIO0_BASE:
            // Clear
            LPC_GPIO0->IC |= 1 << obj->pin;
            
            // Edge
            LPC_GPIO0->IS &= ~(1 << obj->pin);
            
            // Enable
            if (enable) LPC_GPIO0->IE |= 1 << obj->pin;
            else LPC_GPIO0->IE &= ~(1 << obj->pin);
            
            // One edge
            LPC_GPIO0->IBE &= ~(1 << obj->pin);
            
            // Rising/falling?
            if (event == IRQ_RISE) LPC_GPIO0->IEV |= 1 << obj->pin;
            else LPC_GPIO0->IEV &= ~(1 << obj->pin);
            break;
        case LPC_GPIO1_BASE:
            LPC_GPIO1->IC |= 1 << obj->pin;

            LPC_GPIO1->IS &= ~(1 << obj->pin);

            if (enable) LPC_GPIO1->IE |= 1 << obj->pin;
            else LPC_GPIO1->IE &= ~(1 << obj->pin);

            LPC_GPIO1->IBE &= ~(1 << obj->pin);

            if (event == IRQ_RISE) LPC_GPIO0->IEV |= 1 << obj->pin;
            else LPC_GPIO0->IEV &= ~(1 << obj->pin);
            break;
        case LPC_GPIO2_BASE:
            LPC_GPIO2->IC |= 1 << obj->pin;

            LPC_GPIO2->IS &= ~(1 << obj->pin);

            if (enable) LPC_GPIO2->IE |= 1 << obj->pin;
            else LPC_GPIO2->IE &= ~(1 << obj->pin);

            LPC_GPIO2->IBE &= ~(1 << obj->pin);

            if (event == IRQ_RISE) LPC_GPIO0->IEV |= 1 << obj->pin;
            else LPC_GPIO0->IEV &= ~(1 << obj->pin);
            break;
        case LPC_GPIO3_BASE:
            LPC_GPIO3->IC |= 1 << obj->pin;

            LPC_GPIO3->IC &= ~(1 << obj->pin);

            if (enable) LPC_GPIO3->IE |= 1 << obj->pin;
            else LPC_GPIO3->IE &= ~(1 << obj->pin);

            LPC_GPIO3->IBE &= ~(1 << obj->pin);

            if (event == IRQ_RISE) LPC_GPIO0->IEV |= 1 << obj->pin;
            else LPC_GPIO0->IEV &= ~(1 << obj->pin);
            break;
    }
}
