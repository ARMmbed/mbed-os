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

#include "gpio_irq_api.h"
#include "intc_iodefine.h"
#include "pinmap.h"
#include "cmsis.h"

#define CHANNEL_NUM     8

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler;
static const int nIRQn_h = 32;
extern PinName gpio_multi_guard;

enum {
    IRQ0,IRQ1, 
    IRQ2,IRQ3, 
    IRQ4,IRQ5, 
    IRQ6,IRQ7, 

} IRQNo;

static const PinMap PinMap_IRQ[] = {
    {P1_0,  IRQ0, 4}, {P1_1,  IRQ1, 4}, {P1_2,  IRQ2, 4},
    {P1_3,  IRQ3, 4}, {P1_5,  IRQ5, 4}, {P1_7,  IRQ7, 4},
    {P1_8,  IRQ2, 3}, {P1_9,  IRQ3, 3}, {P1_10, IRQ4, 3},
    {P1_11, IRQ5, 3}, // 9
    {P2_0,  IRQ5, 6}, {P2_13, IRQ7, 8}, {P2_14, IRQ0, 8},
    {P2_15, IRQ1, 8}, // 13
    {P3_0,  IRQ2, 3}, {P3_3,  IRQ4, 3}, // 15
    {P4_8,  IRQ0, 8}, {P4_9,  IRQ1, 8}, {P4_10, IRQ2, 8},
    {P4_11, IRQ3, 8}, {P4_12, IRQ4, 8}, {P4_13, IRQ5, 8},
    {P4_14, IRQ6, 8}, {P4_15, IRQ7, 8}, // 23
    {P5_6,  IRQ6, 6}, {P5_8,  IRQ0, 2}, {P5_9,  IRQ2, 4}, // 26
    {P6_0,  IRQ5, 6}, {P6_1,  IRQ4, 4}, {P6_2,  IRQ7, 4},
    {P6_3,  IRQ2, 4}, {P6_4,  IRQ3, 4}, {P6_8,  IRQ0, 8},
    {P6_9,  IRQ1, 8}, {P6_10, IRQ2, 8}, {P6_11, IRQ3, 8},
    {P6_12, IRQ4, 8}, {P6_13, IRQ5, 8}, {P6_14, IRQ6, 8},
    {P6_15, IRQ7, 8}, // 39
    {P7_8,  IRQ1, 8}, {P7_9,  IRQ0, 8}, {P7_10, IRQ2, 8},
    {P7_11, IRQ3, 8}, {P7_12, IRQ4, 8}, {P7_13, IRQ5, 8},
    {P7_14, IRQ6, 8}, // 46
    {P8_2,  IRQ0, 5}, {P8_3,  IRQ1, 6}, {P8_7,  IRQ5, 4},
    {P9_1,  IRQ0, 4}, // 50
    {P11_12,IRQ3, 3}, {P11_15,IRQ1, 3}, // 52

    {NC,    NC,     0}
};

static gpio_irq_event irq_event;

static void handle_interrupt_in(void) {
    int i;
    uint16_t irqs;
    int irq_num;
    
    irqs = INTCIRQRR;
    for(i = 0; i< 8; i++) {
        if (channel_ids[i] && (irqs & (1 << i))) {
            irq_handler(channel_ids[i], irq_event);
            INTCIRQRR &= ~(1 << i);
            irq_num = i;
        }
    }
    GIC_EndInterrupt((IRQn_Type)(nIRQn_h + irq_num));
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id) {
    int shift;
    if (pin == NC) return -1;

    obj->ch = pinmap_peripheral(pin, PinMap_IRQ);
    obj->pin = (int)pin ;
    obj->port = (int)id ;
    
    shift = obj->ch*2;
    channel_ids[obj->ch] = id;
    irq_handler = handler;
    
    pinmap_pinout(pin, PinMap_IRQ);
    gpio_multi_guard = pin;           /* Set multi guard */

    // INTC settings
    InterruptHandlerRegister((IRQn_Type)(nIRQn_h+obj->ch), (void (*)(uint32_t))handle_interrupt_in);
    INTCICR1 &= ~(0x3 << shift);
    INTCICR1 |= (0x3 << shift);
    irq_event = IRQ_RISE;
    GIC_SetPriority((IRQn_Type)(nIRQn_h+obj->ch), 5);
    GIC_EnableIRQ((IRQn_Type)(nIRQn_h+obj->ch));
    __enable_irq();

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj) {
    channel_ids[obj->ch] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable) {
    int shift = obj->ch*2;
    uint16_t val = event == IRQ_RISE ? 2 : 
                   event == IRQ_FALL ? 1 : 0;
    uint16_t work_icr_val;
    uint16_t work_irqrr_val;
    
    /* check edge interrupt setting */
    work_icr_val = INTCICR1;
    if (enable == 1) {
        /* Set interrupt serect */
        work_icr_val |=  (val << shift);
    } else {
        /* Clear interrupt serect */
        work_icr_val &= ~(val << shift);
    }
    
    if ((work_icr_val & (3 << shift)) == 0) {
        /* No edge interrupt setting */
        GIC_DisableIRQ((IRQn_Type)(nIRQn_h+obj->ch));
        /* Clear Interrupt flags */
        work_irqrr_val = INTCIRQRR;
        INTCIRQRR = (work_irqrr_val & ~(1 << obj->ch));
    } else {
        /* Edge interrupt setting */
        if ((work_icr_val & (3 << shift)) == 2) {
            /* Setting of rising edge */
            irq_event = IRQ_RISE;
        } else { 
            /* Setting of falling edge of both edge */
            irq_event = IRQ_FALL;
        }
        GIC_EnableIRQ((IRQn_Type)(nIRQn_h+obj->ch));
    }
    INTCICR1  = work_icr_val;
}

void gpio_irq_enable(gpio_irq_t *obj) {
    GIC_EnableIRQ((IRQn_Type)(nIRQn_h + obj->ch));
}

void gpio_irq_disable(gpio_irq_t *obj) {
    GIC_DisableIRQ((IRQn_Type)(nIRQn_h + obj->ch));
}

