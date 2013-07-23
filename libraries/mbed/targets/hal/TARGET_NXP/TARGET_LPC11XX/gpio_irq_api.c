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

// The chip is capable of 4 external interrupts.
#define CHANNEL_NUM 4

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler;
static int channel = 0;
static PinName pin_names[CHANNEL_NUM] = {};

static inline void handle_interrupt_in(uint32_t channel) {
    // Find out whether the interrupt has been triggered by a high or low value...
    // As the LPC1114 doesn't have a specific register for this, we'll just have to read
    // the level of the pin as if it were just a normal input...

    // Get the number of the pin being used and the port typedef
    uint8_t pin_number = (pin_names[channel] & (0x0f << 8)) >> 8;
    LPC_GPIO_TypeDef *port_reg = ((LPC_GPIO_TypeDef *) (LPC_GPIO0_BASE + (((pin & 0xF000) >> PORT_SHIFT) * 0x10000)));

    if ((port_reg->MASKED_ACCESS & (1 << pin_number)) >> pin_number) {
        // High, therefore rising edge...
        irq_handler(channel_ids[channel], IRQ_RISE);
    }
    else {
        // Low, therefore falling edge...
        irq_handler(channel_ids[channel], IRQ_FALL);
    }
}

void gpio_irq0(void) {handle_interrupt_in(0);}
void gpio_irq1(void) {handle_interrupt_in(1);}
void gpio_irq2(void) {handle_interrupt_in(2);}
void gpio_irq3(void) {handle_interrupt_in(3);}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id) {
    if (pin == NC) return -1;
    
    // Firstly, we'll put some data in *obj so we can keep track of stuff.
    obj->pin = pin;

    /*
        If there are any ports or pins that aren't able to handle interrupts, put them here and uncomment.

        if (pin == ... ||
            pin == ...) {
            error("This pin does not suppor interrupts.");
            return -1;
        }
    */

    channel_ids[channnel] = id;
    pin_names[channel] = pin;
    obj->ch = channel;

    // Which port are we using?
    switch (channel) {
        case 0:
            NVIC_SetVector(EINT0_IRQn, (uint32_t)gpio_irq0);
            NVIC_EnableIrq(EINT0_IRQn);
            break;
        case 1:
            NVIC_SetVector(EINT1_IRQn, (uint32_t)gpio_irq1);
            NVIC_EnableIrq(EINT1_IRQn);
            break;
        case 2:
            NVIC_SetVector(EINT2_IRQn, (uint32_t)gpio_irq2);
            NVIC_EnableIrq(EINT2_IRQn);
            break;
        case 3:
            NVIC_SetVector(EINT3_IRQn, (uint32_t)gpio_irq3);
            NVIC_EnableIrq(EINT3_IRQn);
            break;
    }

    channel++;
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj) {
    channel_ids[obj->ch] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable) {
    pin = obj->pin;
    LPC_GPIO_TypeDef *port_reg = ((LPC_GPIO_TypeDef *) (LPC_GPIO0_BASE + (((pin & 0xF000) >> PORT_SHIFT) * 0x10000)));

    /*
     Firstly, clear the interrupts for this pin,
     Then, let the registers know whether we're looking for edge detection,
     Enable the interrupt,
     And set it to only respond to interrupts on one edge.
    */
    
    // Clear
    port_reg->IC |= 1 << obj->pin;
    
    // Edge
    port_reg->IS &= ~(1 << obj->pin);
    
    // Enable
    if (enable) port_reg->IE |= 1 << obj->pin;
    else port_reg->IE &= ~(1 << obj->pin);
    
    // One edge
    port_reg->IBE &= ~(1 << obj->pin);
    
    // Rising/falling?
    if (event == IRQ_RISE) port_reg->IEV |= 1 << obj->pin;
    else port_reg->IEV &= ~(1 << obj->pin);
}
