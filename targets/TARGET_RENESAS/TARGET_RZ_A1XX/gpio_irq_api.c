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
#include "iodefine.h"
#include "PeripheralPins.h"
#include "cmsis.h"
#include "gpio_addrdefine.h"

#define CHANNEL_NUM     8

static void gpio_irq0(void);
static void gpio_irq1(void);
static void gpio_irq2(void);
static void gpio_irq3(void);
static void gpio_irq4(void);
static void gpio_irq5(void);
static void gpio_irq6(void);
static void gpio_irq7(void);

static gpio_irq_t *channel_obj[CHANNEL_NUM] = {NULL};
static gpio_irq_handler irq_handler;
static const int nIRQn_h = 32;
extern PinName gpio_multi_guard;

static const IRQHandler irq_tbl[CHANNEL_NUM] = {
    &gpio_irq0,
    &gpio_irq1,
    &gpio_irq2,
    &gpio_irq3,
    &gpio_irq4,
    &gpio_irq5,
    &gpio_irq6,
    &gpio_irq7,
};

static void handle_interrupt_in(int irq_num) {
    uint16_t irqs;
    uint16_t edge_req;
    gpio_irq_t *obj;
    gpio_irq_event irq_event;

    irqs = INTCIRQRR;
    if (irqs & (1 << irq_num)) {
        obj = channel_obj[irq_num];
        if (obj != NULL) {
            edge_req = ((INTCICR1 >> (obj->ch * 2)) & 3);
            if (edge_req == 1) {
                irq_event = IRQ_FALL;
            } else if (edge_req == 2) {
                irq_event = IRQ_RISE;
            } else {
                uint32_t      mask    = (1 << (obj->pin & 0x0F));
                __I  uint32_t *reg_in = (volatile uint32_t *) PPR((int)PINGROUP(obj->pin));

                if ((*reg_in & mask) == 0) {
                    irq_event = IRQ_FALL;
                } else {
                    irq_event = IRQ_RISE;
                }
	        }
            irq_handler(obj->port, irq_event);
        }
        INTCIRQRR &= ~(1 << irq_num);
    }
}

static void gpio_irq0(void) {
    handle_interrupt_in(0);
}

static void gpio_irq1(void) {
    handle_interrupt_in(1);
}

static void gpio_irq2(void) {
    handle_interrupt_in(2);
}

static void gpio_irq3(void) {
    handle_interrupt_in(3);
}

static void gpio_irq4(void) {
    handle_interrupt_in(4);
}

static void gpio_irq5(void) {
    handle_interrupt_in(5);
}

static void gpio_irq6(void) {
    handle_interrupt_in(6);
}

static void gpio_irq7(void) {
    handle_interrupt_in(7);
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id) {
    int shift;
    if (pin == NC) return -1;

    obj->ch = pinmap_peripheral(pin, PinMap_IRQ);
    obj->pin = (int)pin ;
    obj->port = (int)id ;

    shift = obj->ch*2;
    channel_obj[obj->ch] = obj;
    irq_handler = handler;
    
    pinmap_pinout(pin, PinMap_IRQ);
    gpio_multi_guard = pin;           /* Set multi guard */

    // INTC settings
    InterruptHandlerRegister((IRQn_Type)(nIRQn_h+obj->ch), (void (*)(uint32_t))irq_tbl[obj->ch]);
    INTCICR1 &= ~(0x3 << shift);
    GIC_SetPriority((IRQn_Type)(nIRQn_h+obj->ch), 5);
    GIC_SetConfiguration((IRQn_Type)(nIRQn_h + obj->ch), 1);
    obj->int_enable = 1;
    __enable_irq();

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj) {
    channel_obj[obj->ch] = NULL;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable) {
    int shift = obj->ch*2;
    uint16_t val = event == IRQ_RISE ? 2 : 
                   event == IRQ_FALL ? 1 : 0;
    uint16_t work_icr_val;

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
        INTCIRQRR &= ~(1 << obj->ch);
        INTCICR1  = work_icr_val;
    } else if (obj->int_enable == 1) {
        INTCICR1  = work_icr_val;
        GIC_EnableIRQ((IRQn_Type)(nIRQn_h + obj->ch));
    } else {
        INTCICR1  = work_icr_val;
    }
}

void gpio_irq_enable(gpio_irq_t *obj) {
    int shift = obj->ch*2;
    uint16_t work_icr_val = INTCICR1;

    /* check edge interrupt setting */
    if ((work_icr_val & (3 << shift)) != 0) {
        GIC_EnableIRQ((IRQn_Type)(nIRQn_h + obj->ch));
    }
    obj->int_enable = 1;
}

void gpio_irq_disable(gpio_irq_t *obj) {
    GIC_DisableIRQ((IRQn_Type)(nIRQn_h + obj->ch));
    obj->int_enable = 0;
}

