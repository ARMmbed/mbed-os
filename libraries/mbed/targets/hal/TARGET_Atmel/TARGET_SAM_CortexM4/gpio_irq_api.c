/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "gpio_api.h"
#include "ioport.h"

#define IRQ_RISE_POSITION   1
#define IRQ_FALL_POSITION   2
#define CHANNEL_NUM         48
#define MAX_PINS_IN_PORT    32

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler;
extern uint8_t g_sys_init;

static IRQn_Type pin_to_irq (uint32_t pin);

void gpio_irq_common_handler(uint32_t port_id)
{
    uint32_t i = 0, status = 0, mask = 0, temp = 0;
    gpio_irq_event event;

    Pio* pio_base = arch_ioport_port_to_base(port_id);
    mask = pio_base->PIO_IMR;
    status = pio_base->PIO_ISR;
    status = status & mask;

    for (i = 0; i < MAX_PINS_IN_PORT ; i++) {
        temp = (1 << i );
        if (status & temp ) {
            if((pio_base->PIO_PDSR) & temp) {
                event = IRQ_RISE;
            } else {
                event = IRQ_FALL;
            }
            if(irq_handler) {
                irq_handler(channel_ids[(port_id * 32) + i], event);
            }
        }
    }
}

void gpio_irq_porta(void)
{
    gpio_irq_common_handler(IOPORT_PIOA);
}

void gpio_irq_portb(void)
{
    gpio_irq_common_handler(IOPORT_PIOB);
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    MBED_ASSERT(obj);
    if (pin == NC)
        return -1;
    if (g_sys_init == 0) {
        sysclk_init();
        system_board_init();
        g_sys_init = 1;
    }

    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t port_id;
    uint32_t vector = 0;
    uint8_t int_channel = 0;
    Pio* pio_base;

    irq_handler = handler;  // assuming the usage of these apis in mbed layer only
    int_channel = ((pin / 32) * 32)  + (pin % 32); /*to get the channel to be used*/
    channel_ids[int_channel] = id;
    obj->pin = pin;
    port_id = ioport_pin_to_port_id(pin);
    pio_base = arch_ioport_port_to_base(port_id);

    ioport_set_pin_dir(pin, IOPORT_DIR_INPUT); /*Pin to be configured input for GPIO Interrupt*/
    ioport_set_pin_mode(pin, IOPORT_MODE_PULLUP);

    irq_n = pin_to_irq(pin);

    switch (port_id) {
            /*only 2 ports for SAMG55*/ /*Setting up the vectors*/
        case IOPORT_PIOA :
            vector = (uint32_t)gpio_irq_porta;
            break;
        case IOPORT_PIOB :
            vector = (uint32_t)gpio_irq_portb;
            break;
    }
    pio_base->PIO_ISR; /*To read and clear status register*/
    NVIC_SetVector(irq_n, vector);
    NVIC_EnableIRQ(irq_n);

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    MBED_ASSERT(obj);
    channel_ids[((obj->pin / 32) * 32)  + (obj->pin % 32)] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    MBED_ASSERT(obj);
    uint32_t mask = 0;

    Pio* pio_base = arch_ioport_port_to_base(arch_ioport_pin_to_port_id(obj->pin));
    mask = (1 << (obj->pin % 32));

    if (enable) {
        if (event == IRQ_RISE) {
            obj->irqmask |= IRQ_RISE_POSITION;
        } else if (event == IRQ_FALL) {
            obj->irqmask |= IRQ_FALL_POSITION;
        }
    } else {

        if (event == IRQ_RISE) {
            obj->irqmask &= ~IRQ_RISE_POSITION;
        } else if (event == IRQ_FALL) {
            obj->irqmask &= ~IRQ_FALL_POSITION;
        }
    }
    pio_base->PIO_ISR; /*To read and clear status register*/
    if (obj->irqmask == (IRQ_RISE_POSITION | IRQ_FALL_POSITION)) { /*both edge detection*/
        pio_base->PIO_AIMDR = mask;
        pio_base->PIO_IER = mask;
    } else if (obj->irqmask == IRQ_RISE_POSITION) {  /*rising detection*/
        pio_base->PIO_ESR = mask;
        pio_base->PIO_REHLSR = mask;
        pio_base->PIO_AIMER = mask;
        pio_base->PIO_IER = mask;
    } else if (obj->irqmask == IRQ_FALL_POSITION) {  /*falling detection*/
        pio_base->PIO_ESR = mask;
        pio_base->PIO_FELLSR = mask;
        pio_base->PIO_AIMER = mask;
        pio_base->PIO_IER = mask;
    } else { /*none and disable*/
        pio_base->PIO_IDR = mask;
    }
}

static IRQn_Type pin_to_irq (uint32_t pin)
{
    uint32_t port_id;
    IRQn_Type irq_n = (IRQn_Type)0;
    port_id = ioport_pin_to_port_id(pin);

    switch (port_id) {
        case IOPORT_PIOA :
            irq_n = PIOA_IRQn;
            break;
        case IOPORT_PIOB :
            irq_n = PIOB_IRQn;
            break;
    }
    return irq_n;
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    MBED_ASSERT(obj);
    NVIC_EnableIRQ(pin_to_irq(obj->pin));
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    MBED_ASSERT(obj);
    NVIC_DisableIRQ(pin_to_irq(obj->pin));
}
