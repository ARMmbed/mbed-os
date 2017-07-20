/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include <stddef.h>
#include "cmsis.h"
#include "gpio_irq_api.h"
#include "mbed_error.h"

static gpio_irq_t *objs[MXC_GPIO_NUM_PORTS][MXC_GPIO_MAX_PINS_PER_PORT] = {{0}};
static gpio_irq_handler irq_handler;

static void handle_irq(unsigned int port)
{
    uint32_t intfl, in_val;
    uint32_t mask;
    unsigned int pin;

    /* Read pin state */
    in_val = MXC_GPIO->in_val[port];

    /* Read interrupts */
    intfl = MXC_GPIO->intfl[port] & MXC_GPIO->inten[port];

    mask = 1;

    for (pin = 0; pin < MXC_GPIO_MAX_PINS_PER_PORT; pin++) {
        if (intfl & mask) {
            MXC_GPIO->intfl[port] = mask;    /* clear interrupt */
            gpio_irq_event event = (in_val & mask) ? IRQ_RISE : IRQ_FALL;
            gpio_irq_t *obj = objs[port][pin];
            if (obj && obj->id) {
                if ((event == IRQ_RISE) && obj->rise_en) {
                    irq_handler(obj->id, IRQ_RISE);
                } else if ((event == IRQ_FALL) && obj->fall_en) {
                    irq_handler(obj->id, IRQ_FALL);
                }
            }
        }
        mask <<= 1;
    }
}

void gpio_irq_0(void) { handle_irq(0); }
void gpio_irq_1(void) { handle_irq(1); }
void gpio_irq_2(void) { handle_irq(2); }
void gpio_irq_3(void) { handle_irq(3); }
void gpio_irq_4(void) { handle_irq(4); }
void gpio_irq_5(void) { handle_irq(5); }
void gpio_irq_6(void) { handle_irq(6); }
void gpio_irq_7(void) { handle_irq(7); }
void gpio_irq_8(void) { handle_irq(8); }

int gpio_irq_init(gpio_irq_t *obj, PinName name, gpio_irq_handler handler, uint32_t id)
{
    if (name == NC) {
        return -1;
    }

    uint8_t port = PINNAME_TO_PORT(name);
    uint8_t pin = PINNAME_TO_PIN(name);

    if ((port > MXC_GPIO_NUM_PORTS) || (pin > MXC_GPIO_MAX_PINS_PER_PORT)) {
        return 1;
    }

    obj->port = port;
    obj->pin = pin;
    obj->id = id;
    objs[port][pin] = obj;

    /* register handlers */
    irq_handler = handler;
    NVIC_SetVector(GPIO_P0_IRQn, (uint32_t)gpio_irq_0);
    NVIC_SetVector(GPIO_P1_IRQn, (uint32_t)gpio_irq_1);
    NVIC_SetVector(GPIO_P2_IRQn, (uint32_t)gpio_irq_2);
    NVIC_SetVector(GPIO_P3_IRQn, (uint32_t)gpio_irq_3);
    NVIC_SetVector(GPIO_P4_IRQn, (uint32_t)gpio_irq_4);
    NVIC_SetVector(GPIO_P5_IRQn, (uint32_t)gpio_irq_5);
    NVIC_SetVector(GPIO_P6_IRQn, (uint32_t)gpio_irq_6);
    NVIC_SetVector(GPIO_P7_IRQn, (uint32_t)gpio_irq_7);
    NVIC_SetVector(GPIO_P8_IRQn, (uint32_t)gpio_irq_8);

    /* disable the interrupt locally */
    MXC_GPIO->int_mode[port] &= ~(0xF << (pin*4));

    /* clear a pending request */
    MXC_GPIO->intfl[port] = 1 << pin;

    /* enable the requested interrupt */
    MXC_GPIO->inten[port] |= (1 << pin);
    NVIC_EnableIRQ((IRQn_Type)((uint32_t)GPIO_P0_IRQn + port));

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    /* disable interrupt */
    MXC_GPIO->inten[obj->port] &= ~(1 << obj->pin);
    MXC_GPIO->int_mode[obj->port] &= ~(MXC_V_GPIO_INT_MODE_ANY_EDGE << (obj->pin*4));
    objs[obj->port][obj->pin] = NULL;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    if (event == IRQ_FALL) {
        obj->fall_en = enable;
    } else if (event == IRQ_RISE) {
        obj->rise_en = enable;
    }

    if (obj->fall_en && obj->rise_en) {
        MXC_GPIO->int_mode[obj->port] |= (MXC_V_GPIO_INT_MODE_ANY_EDGE << (obj->pin*4));
    } else if (obj->fall_en) {
        uint32_t int_mode = MXC_GPIO->int_mode[obj->port];
        int_mode &= ~(MXC_V_GPIO_INT_MODE_ANY_EDGE << (obj->pin*4));
        int_mode |=  (MXC_V_GPIO_INT_MODE_FALLING_EDGE << (obj->pin*4));
        MXC_GPIO->int_mode[obj->port] = int_mode;
    } else if (obj->rise_en) {
        uint32_t int_mode = MXC_GPIO->int_mode[obj->port];
        int_mode &= ~(MXC_V_GPIO_INT_MODE_ANY_EDGE << (obj->pin*4));
        int_mode |=  (MXC_V_GPIO_INT_MODE_RISING_EDGE << (obj->pin*4));
        MXC_GPIO->int_mode[obj->port] = int_mode;
    } else {
        MXC_GPIO->int_mode[obj->port] &= ~(MXC_V_GPIO_INT_MODE_ANY_EDGE << (obj->pin*4));
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    MXC_GPIO->inten[obj->port] |= (1 << obj->pin);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    MXC_GPIO->inten[obj->port] &= ~(1 << obj->pin);
}

gpio_irq_t *gpio_irq_get_obj(PinName name)
{
    if (name == NC) {
        return NULL;
    }

    unsigned int port = PINNAME_TO_PORT(name);
    unsigned int pin = PINNAME_TO_PIN(name);

    if ((port > MXC_GPIO_NUM_PORTS) || (pin > MXC_GPIO_MAX_PINS_PER_PORT)) {
        return NULL;
    }

    return objs[port][pin];
}
