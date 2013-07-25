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
#include "gpio_api.h"

// The chip is capable of 4 external interrupts.
#define CHANNEL_NUM 4

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler;
static PinName pin_names[CHANNEL_NUM] = {};
static uint8_t trigger_events[CHANNEL_NUM] = {};

static inline void handle_interrupt_in(uint32_t channel) {
    // Find out whether the interrupt has been triggered by a high or low value...
    // As the LPC1114 doesn't have a specific register for this, we'll just have to read
    // the level of the pin as if it were just a normal input...

    // Get the number of the pin being used and the port typedef
    LPC_GPIO_TypeDef *port_reg = ((LPC_GPIO_TypeDef *) (LPC_GPIO0_BASE + (((pin_names[channel] & 0xF000) >> PORT_SHIFT) * 0x10000)));
    uint8_t pin_num = (pin_names[channel] & (0x0f << PIN_SHIFT)) >> PIN_SHIFT;
    uint8_t trigger_event = trigger_events[channel];

    if (trigger_event == 1) {
        // Rising edge.
        irq_handler(channel_ids[channel], IRQ_RISE);
    }
    else if (trigger_event == 2) {
        // Low, therefore falling edge...
        irq_handler(channel_ids[channel], IRQ_FALL);
    }
    else {
        // This is supposed to be triggered by both cases...
        irq_handler(channel_ids[channel], IRQ_RISE);
    }

    // Clear the interrupt...
    port_reg->IC |= 1 << pin_num;
}

void gpio_irq0(void) {handle_interrupt_in(0);}
void gpio_irq1(void) {handle_interrupt_in(1);}
void gpio_irq2(void) {handle_interrupt_in(2);}
void gpio_irq3(void) {handle_interrupt_in(3);}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id) {
    if (pin == NC) return -1;
    
    // Firstly, we'll put some data in *obj so we can keep track of stuff.
    obj->pin = pin;
	
	// Set the handler to be the pointer at the top...
	irq_handler = handler;

    /*
        If there are any ports or pins that aren't able to handle interrupts, put them here and uncomment.

        if (pin == ... ||
            pin == ...) {
            error("This pin does not support interrupts.");
            return -1;x
        }
    */

    // Which port are we using?
    int channel;
    uint32_t port_reg = (LPC_GPIO0_BASE + (((pin & 0xF000) >> PORT_SHIFT) * 0x10000));

    switch (port_reg) {
        case LPC_GPIO0_BASE:
            NVIC_SetVector(EINT0_IRQn, (uint32_t)gpio_irq0);
            NVIC_EnableIRQ(EINT0_IRQn);
            channel = 0;
            break;
        case LPC_GPIO1_BASE:
            NVIC_SetVector(EINT1_IRQn, (uint32_t)gpio_irq1);
            NVIC_EnableIRQ(EINT1_IRQn);
            channel = 1;
            break;
        case LPC_GPIO2_BASE:
            NVIC_SetVector(EINT2_IRQn, (uint32_t)gpio_irq2);
            NVIC_EnableIRQ(EINT2_IRQn);
            channel = 2;
            break;
        case LPC_GPIO3_BASE:
            NVIC_SetVector(EINT3_IRQn, (uint32_t)gpio_irq3);
            NVIC_EnableIRQ(EINT3_IRQn);
            channel = 3;
            break;
        default:
            channel = -1;
            error("Invalid interrupt choice.");
            break;
    }

    channel_ids[channel] = id;
    pin_names[channel] = pin;
    obj->ch = channel;
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj) {
    channel_ids[obj->ch] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable) {
    // Firstly, check if there is an existing event stored...

    LPC_GPIO_TypeDef *port_reg = ((LPC_GPIO_TypeDef *) (LPC_GPIO0_BASE + (((obj->pin & 0xF000) >> PORT_SHIFT) * 0x10000)));

    // Need to get the pin number of the pin, not the value of the enum
    uint8_t pin_num = (obj->pin & (0x0f << PIN_SHIFT)) >> PIN_SHIFT;
   

    if (trigger_events[obj->ch] != 0) {
        // We have an event.
        // Enable both edge interrupts.

        if (enable) {
            trigger_events[obj->ch] = 3;
            port_reg->IBE |= 1 << pin_num;
            port_reg->IE |= 1 << pin_num;
        }
        else {
            // These all need to be opposite, to reenable the other one.
            trigger_events[obj->ch] = event == IRQ_RISE ? 2 : 1;

            port_reg->IBE &= ~(1 << pin_num);

            if (event == IRQ_RISE)
                port_reg->IEV &= ~(1 << pin_num);
            else 
                port_reg->IEV |= 1 << pin_num;

            port_reg->IE |= 1 << pin_num;
        }
    }
    else {
        if (enable) {
            trigger_events[obj->ch] = event == IRQ_RISE ? 1 : 2;
            port_reg->IE |= 1 << pin_num;
        }
        // One edge
        port_reg->IBE &= ~(1 << pin_num);
        // Rising/falling?
        if (event == IRQ_RISE)
            port_reg->IEV |= 1 << pin_num;
        else
            port_reg->IEV &= ~(1 << pin_num);
    }
    
    /*
     Firstly, clear the interrupts for this pin,
     Then, let the registers know whether we're looking for edge detection,
     Enable the interrupt,
     And set it to only respond to interrupts on one edge.
    */

    // Clear
    port_reg->IC |= 1 << pin_num;
    
    // Make it edge sensitive.
    port_reg->IS &= ~(1 << pin_num);
}
