/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include "pinmap.h"
#include "mbed_error.h"

#define EDGE_NONE (0)
#define EDGE_RISE (1)
#define EDGE_FALL (2)

extern uint32_t gpio_clock_enable(uint32_t port_idx);
static gpio_irq_handler irq_handler;

typedef struct {
    uint32_t exti_idx;
    uint32_t exti_gpiox; /* base address of gpio */
    uint32_t exti_pinx;  /* pin number */
} gpio_exti_info_struct;

/* EXTI0...EXTI15 */
static gpio_exti_info_struct exti_info_array[16] = {0};

/** handle EXTI interrupt in EXTI0 to EXTI15

 * @param irq_index     the line of EXTI(0~15)
 */
static void exti_handle_interrupt(uint32_t irq_index)
{
    gpio_exti_info_struct *gpio_exti = &exti_info_array[irq_index];

    /* get the port and pin of EXTI */
    uint32_t gpio = (uint32_t)(gpio_exti->exti_gpiox);
    uint32_t pin = (uint32_t)(1 << (gpio_exti->exti_pinx));

    /* clear interrupt flag */
    if (exti_interrupt_flag_get((exti_line_enum)pin) != RESET) {
        exti_interrupt_flag_clear((exti_line_enum)pin);
        /* check which edge has generated the irq */
        if ((GPIO_ISTAT(gpio) & pin) == 0) {
            irq_handler(gpio_exti->exti_idx, IRQ_FALL);
        } else {
            irq_handler(gpio_exti->exti_idx, IRQ_RISE);
        }
    }

}

/* EXTI0 interrupt handler */
static void gpio_irq_exti0(void)
{
    exti_handle_interrupt(0);
}
/* EXTI1 interrupt handler */
static void gpio_irq_exti1(void)
{
    exti_handle_interrupt(1);
}
/* EXTI2 interrupt handler */
static void gpio_irq_exti2(void)
{
    exti_handle_interrupt(2);
}
/* EXTI3 interrupt handler */
static void gpio_irq_exti3(void)
{
    exti_handle_interrupt(3);
}
/* EXTI4 interrupt handler */
static void gpio_irq_exti4(void)
{
    exti_handle_interrupt(4);
}
/* EXTI5 interrupt handler */
static void gpio_irq_exti5(void)
{
    exti_handle_interrupt(5);
}
/* EXTI6 interrupt handler */
static void gpio_irq_exti6(void)
{
    exti_handle_interrupt(6);
}
/* EXTI7 interrupt handler */
static void gpio_irq_exti7(void)
{
    exti_handle_interrupt(7);
}
/* EXTI8 interrupt handler */
static void gpio_irq_exti8(void)
{
    exti_handle_interrupt(8);
}
/* EXTI9 interrupt handler */
static void gpio_irq_exti9(void)
{
    exti_handle_interrupt(9);
}
/* EXTI10 interrupt handler */
static void gpio_irq_exti10(void)
{
    exti_handle_interrupt(10);
}
/* EXTI11 interrupt handler */
static void gpio_irq_exti11(void)
{
    exti_handle_interrupt(11);
}
/* EXTI12 interrupt handler */
static void gpio_irq_exti12(void)
{
    exti_handle_interrupt(12);
}
/* EXTI13 interrupt handler */
static void gpio_irq_exti13(void)
{
    exti_handle_interrupt(13);
}
/* EXTI14 interrupt handler */
static void gpio_irq_exti14(void)
{
    exti_handle_interrupt(14);
}
/* EXTI15 interrupt handler */
static void gpio_irq_exti15(void)
{
    exti_handle_interrupt(15);
}

/** Initialize the GPIO IRQ pin
 *
 * @param obj     The GPIO object to initialize
 * @param pin     The GPIO pin name
 * @param handler The handler to be attached to GPIO IRQ
 * @param id      The object ID (id != 0, 0 is reserved)
 * @return -1 if pin is NC, 0 otherwise
 */
int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    uint32_t vector = 0;
    gpio_exti_info_struct *gpio_exti;
    if (pin == NC) {
        return -1;
    }

    /* Enable AF Clock */
    rcu_periph_clock_enable(RCU_AF);

    uint32_t port_index  = GD_PORT_GET(pin);
    uint32_t pin_index  = GD_PIN_GET(pin);
    /* Enable GPIO clock */
    uint32_t gpio_add = gpio_clock_enable(port_index);

    /* fill EXTI information according to pin_index .
       eg. use PORTE_9 as EXTI interrupt, the irq type is EXTI5_9_IRQn */
    if (pin_index == 0) {
        vector = (uint32_t)&gpio_irq_exti0;
        obj->irq_index = 0;
        obj->irq_n = EXTI0_IRQn;
    } else if (pin_index == 1) {
        vector = (uint32_t)&gpio_irq_exti1;
        obj->irq_index = 1;
        obj->irq_n = EXTI1_IRQn;
    } else if (pin_index == 2) {
        vector = (uint32_t)&gpio_irq_exti2;
        obj->irq_index = 2;
        obj->irq_n = EXTI2_IRQn;
    } else if (pin_index == 3) {
        vector = (uint32_t)&gpio_irq_exti3;
        obj->irq_index = 3;
        obj->irq_n = EXTI3_IRQn;
    } else if (pin_index == 4) {
        vector = (uint32_t)&gpio_irq_exti4;
        obj->irq_index = 4;
        obj->irq_n = EXTI4_IRQn;
    } else if (pin_index == 5) {
        vector = (uint32_t)&gpio_irq_exti5;
        obj->irq_index = 5;
        obj->irq_n = EXTI5_9_IRQn;
    } else if (pin_index == 6) {
        vector = (uint32_t)&gpio_irq_exti6;
        obj->irq_index = 6;
        obj->irq_n = EXTI5_9_IRQn;
    } else if (pin_index == 7) {
        vector = (uint32_t)&gpio_irq_exti7;
        obj->irq_index = 7;
        obj->irq_n = EXTI5_9_IRQn;
    } else if (pin_index == 8) {
        vector = (uint32_t)&gpio_irq_exti8;
        obj->irq_index = 8;
        obj->irq_n = EXTI5_9_IRQn;
    } else if (pin_index == 9) {
        vector = (uint32_t)&gpio_irq_exti9;
        obj->irq_index = 9;
        obj->irq_n = EXTI5_9_IRQn;
    } else if (pin_index == 10) {
        vector = (uint32_t)&gpio_irq_exti10;
        obj->irq_index = 10;
        obj->irq_n = EXTI10_15_IRQn;
    } else if (pin_index == 11) {
        vector = (uint32_t)&gpio_irq_exti11;
        obj->irq_index = 11;
        obj->irq_n = EXTI10_15_IRQn;
    } else if (pin_index == 12) {
        vector = (uint32_t)&gpio_irq_exti12;
        obj->irq_index = 12;
        obj->irq_n = EXTI10_15_IRQn;
    } else if (pin_index == 13) {
        vector = (uint32_t)&gpio_irq_exti13;
        obj->irq_index = 13;
        obj->irq_n = EXTI10_15_IRQn;
    } else if (pin_index == 14) {
        vector = (uint32_t)&gpio_irq_exti14;
        obj->irq_index = 14;
        obj->irq_n = EXTI10_15_IRQn;
    } else if (pin_index == 15) {
        vector = (uint32_t)&gpio_irq_exti15;
        obj->irq_index = 15;
        obj->irq_n = EXTI10_15_IRQn;
    } else {
        error("pin not supported for interrupt in.\n");
        return -1;
    }

    /* Save informations for future use */
    obj->event = EDGE_NONE;
    obj->pin = pin;

    gpio_exti = &exti_info_array[obj->irq_index];
    gpio_exti->exti_idx = id;
    gpio_exti->exti_gpiox = gpio_add;
    gpio_exti->exti_pinx = pin_index;

    irq_handler = handler;

    /* Enable EXTI interrupt */
    NVIC_SetVector(obj->irq_n, vector);
    gpio_irq_enable(obj);

    return 0;
}

/** Release the GPIO IRQ PIN
 *
 * @param obj The gpio object
 */
void gpio_irq_free(gpio_irq_t *obj)
{
    gpio_exti_info_struct *gpio_exti = &exti_info_array[obj->irq_index];

    /* Disable EXTI interrupt */
    gpio_irq_disable(obj);
    /* Reset struct of exti information */
    gpio_exti->exti_idx = 0;
    gpio_exti->exti_gpiox = 0;
    gpio_exti->exti_pinx = 0;
}

/** Enable/disable pin IRQ event
 *
 * @param obj    The GPIO object
 * @param event  The GPIO IRQ event
 * @param enable The enable flag
 */
void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    if (event == IRQ_RISE) {
        if (enable) {
            exti_init((exti_line_enum)(1 << GD_PIN_GET(obj->pin)), EXTI_INTERRUPT, EXTI_TRIG_RISING);
            /* Clear interrupt enable bit, rising/falling bit */
        } else {
            EXTI_INTEN &= ~(uint32_t)(exti_line_enum)(1 << GD_PIN_GET(obj->pin));
            EXTI_RTEN &= ~(uint32_t)(exti_line_enum)(1 << GD_PIN_GET(obj->pin));
            EXTI_FTEN &= ~(uint32_t)(exti_line_enum)(1 << GD_PIN_GET(obj->pin));
        }
    }
    if (event == IRQ_FALL) {
        if (enable) {
            exti_init((exti_line_enum)(1 << (GD_PIN_GET(obj->pin))), EXTI_INTERRUPT, EXTI_TRIG_FALLING);
            /* Clear interrupt enable bit, rising/falling bit */
        } else {
            EXTI_INTEN &= ~(uint32_t)(exti_line_enum)(1 << GD_PIN_GET(obj->pin));
            EXTI_RTEN &= ~(uint32_t)(exti_line_enum)(1 << GD_PIN_GET(obj->pin));
            EXTI_FTEN &= ~(uint32_t)(exti_line_enum)(1 << GD_PIN_GET(obj->pin));
        }
    }
}

/** Enable GPIO IRQ
 *
 * This is target dependent, as it might enable the entire port or just a pin
 * @param obj The GPIO object
 */
void gpio_irq_enable(gpio_irq_t *obj)
{
    /* Select EXTI Source  */
    gpio_exti_source_select(GD_PORT_GET(obj->pin), GD_PIN_GET(obj->pin));
    exti_interrupt_enable((exti_line_enum)(1 << GD_PIN_GET(obj->pin)));
    NVIC_EnableIRQ(obj->irq_n);
}

/** Disable GPIO IRQ
 *
 * This is target dependent, as it might disable the entire port or just a pin
 * @param obj The GPIO object
 */
void gpio_irq_disable(gpio_irq_t *obj)
{
    /* Clear EXTI line configuration */
    exti_interrupt_disable((exti_line_enum)(1 << GD_PIN_GET(obj->pin)));
    NVIC_DisableIRQ(obj->irq_n);
    NVIC_ClearPendingIRQ(obj->irq_n);
    obj->event = EDGE_NONE;
}
