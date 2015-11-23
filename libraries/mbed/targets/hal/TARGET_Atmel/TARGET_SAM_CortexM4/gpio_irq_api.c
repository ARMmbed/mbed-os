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
#define CHANNEL_NUM         16
#define MAX_PORT_NUM        2

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler;
extern uint8_t ioinit;

void gpio_irq_common_handler(uint32_t port_id)
{
    uint32_t i = 0, j = 0;
    uint32_t status_flag = 0;
    gpio_irq_event event;

    Pio* pio_base = arch_ioport_port_to_base(port_id);
    status_flag = pio_base->PIO_ISR;

    for (i = 0; i < MAX_PORT_NUM; i++) {
        for (j = 0; j < CHANNEL_NUM ; j++) {
            if (status_flag & (1 << j ) ) {
                if((pio_base->PIO_PDSR) & (1 << j )) { /*GPIO is in HIGH state, hence its a low to high transition*/
                    event = IRQ_RISE;
                } else {  /*GPIO is in LOW state, hence its a high to low transition*/
                    event = IRQ_FALL;
                }
                if(irq_handler) {
                    irq_handler(channel_ids[j], event);
                }
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
    if (ioinit) {
        ioport_init();
        ioinit = 0;
    }

    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t port_id;
    uint32_t vector = 0;
    uint8_t int_channel = 0;

    int_channel = pin % 32; /*to get the channel to be used*/
    channel_ids[int_channel] = id;
    obj->pin = pin;
    port_id = ioport_pin_to_port_id(pin);

    ioport_set_pin_dir(pin, IOPORT_DIR_INPUT);
    ioport_set_pin_mode(pin, IOPORT_MODE_PULLUP);

    switch (port_id) {
            /*only 2 ports for SAMG55*/ /*Setting up the vectors*/
        case IOPORT_PIOA :
            irq_n = PIOA_IRQn;
            vector = (uint32_t)gpio_irq_porta;
            break;
        case IOPORT_PIOB :
            irq_n = PIOB_IRQn;
            vector = (uint32_t)gpio_irq_portb;
            break;
    }
    NVIC_SetVector(irq_n, vector);
    NVIC_EnableIRQ(irq_n);

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    MBED_ASSERT(obj);
    channel_ids[(obj->pin) % 32] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    MBED_ASSERT(obj);

    Pio* pio_base = arch_ioport_port_to_base(arch_ioport_pin_to_port_id(obj->pin));

    pio_base->PIO_IER = 0x1u << ((obj->pin) % 32);

    if (enable) {
        if (event == IRQ_RISE) {
            obj->irqmask |= IRQ_RISE_POSITION;
        } else if (event == IRQ_FALL) {
            obj->irqmask |= IRQ_FALL_POSITION;
        }
        pio_base->PIO_IER = 0x1u << ((obj->pin) % 32);
    } else {
        if (event == IRQ_RISE) {
            obj->irqmask &= ~IRQ_RISE_POSITION;
        } else if (event == IRQ_FALL) {
            obj->irqmask &= ~IRQ_FALL_POSITION;
        }
        pio_base->PIO_IDR = 0x1u << ((obj->pin) % 32);
    }

    if (obj->irqmask == (IRQ_RISE_POSITION | IRQ_FALL_POSITION)) { /*Both Edge detection*/
        ioport_set_pin_sense_mode(obj->pin, IOPORT_SENSE_RISING);
        ioport_set_pin_sense_mode(obj->pin, IOPORT_SENSE_FALLING);
    } else if (obj->irqmask == IRQ_RISE_POSITION) {  /*Rising Detection*/
        ioport_set_pin_sense_mode(obj->pin, IOPORT_SENSE_RISING);
    } else if (obj->irqmask == IRQ_FALL_POSITION) {  /*Falling Detection*/
        ioport_set_pin_sense_mode(obj->pin, IOPORT_SENSE_FALLING);
    } else { /*None and disable*/
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    MBED_ASSERT(obj);
    uint32_t port_id;
    IRQn_Type irq_n = (IRQn_Type)0;
    port_id = ioport_pin_to_port_id(obj->pin);

    switch (port_id) {
        case IOPORT_PIOA :
            irq_n = PIOA_IRQn;
            break;
        case IOPORT_PIOB :
            irq_n = PIOB_IRQn;
            break;
    }
    NVIC_EnableIRQ(irq_n);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    MBED_ASSERT(obj);
    uint32_t port_id;
    IRQn_Type irq_n = (IRQn_Type)0;
    port_id = ioport_pin_to_port_id(obj->pin);

    switch (port_id) {
        case IOPORT_PIOA :
            irq_n = PIOA_IRQn;
            break;
        case IOPORT_PIOB :
            irq_n = PIOB_IRQn;
            break;
    }
    NVIC_DisableIRQ(irq_n);
}
