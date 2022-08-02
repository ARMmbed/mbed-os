/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
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

static gpio_irq_t *objs[MXC_CFG_GPIO_INSTANCES][MXC_CFG_GPIO_PINS_PORT] = {{0}};
static gpio_irq_handler irq_handler;

void gpio_irq_0(void)
{
    uint32_t intfl, in_val;
    uint32_t mask;
    unsigned int pin;

    /* Get GPIO Register Structure */
    mxc_gpio_regs_t *gpio = MXC_GPIO0; //MXC_GPIO_GET_GPIO(port);

    /* Read pin state */
    in_val = gpio->in;

    /* Read interrupts */
    intfl = gpio->int_stat & gpio->int_en;

    mask = 1;

    /* Determine related routine*/
    for (pin = 0; pin < MXC_CFG_GPIO_PINS_PORT; pin++) {
        if (intfl & mask) {
            gpio->int_clr |= mask;/* clear interrupt */
            gpio_irq_event event = (in_val & mask) ? IRQ_RISE : IRQ_FALL;
            gpio_irq_t *obj = objs[0][pin];
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

int gpio_irq_init(gpio_irq_t *obj, PinName name, gpio_irq_handler handler, uintptr_t context)
{
    if (name == NC) {
        return -1;
    }

    uint8_t port = PINNAME_TO_PORT(name);
    uint8_t pin = PINNAME_TO_PIN(name);

    if ((port > MXC_CFG_GPIO_INSTANCES) || (pin > MXC_CFG_GPIO_PINS_PORT)) {
        return 1;
    }

    obj->port = port;
    obj->pin = pin;
    obj->id = context;
    objs[port][pin] = obj;

    /* register handlers */
    irq_handler = handler;
    NVIC_SetVector(GPIO0_IRQn, (uint32_t)gpio_irq_0);

    /* disable the interrupt locally */
    MXC_GPIO0->int_en &= ~(1 << pin);

    /* clear a pending request */
    MXC_GPIO0->int_stat |= (1 << pin);

    NVIC_EnableIRQ(GPIO0_IRQn);

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    MXC_GPIO0->int_en &= ~(1 << obj->pin);
    objs[obj->port][obj->pin] = NULL;

}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    /* Note that MAX32660 supports only one edge interrupt at a time */
    if (event == IRQ_FALL) {
       obj->fall_en = enable;
       MXC_GPIO0->int_mod |= (1 << obj->pin);
       MXC_GPIO0->int_pol  &= ~(1 << obj->pin);
       MXC_GPIO0->int_en |= (1 << obj->pin);
    } else if (event == IRQ_RISE) {
       obj->rise_en = enable;
       MXC_GPIO0->int_mod |= (1 << obj->pin);
       MXC_GPIO0->int_pol  |= (1 << obj->pin);
       MXC_GPIO0->int_en |= (1 << obj->pin);
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    MXC_GPIO0->int_en |= (1 << obj->pin);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    MXC_GPIO0->int_en &= ~(1 << obj->pin);
}

gpio_irq_t *gpio_irq_get_obj(PinName name)
{
    if (name == NC) {
        return NULL;
    }

    unsigned int port = PINNAME_TO_PORT(name);
    unsigned int pin = PINNAME_TO_PIN(name);

    if ((port > MXC_CFG_GPIO_INSTANCES) || (pin > MXC_CFG_GPIO_PINS_PORT)) {
        return NULL;
    }

    return objs[port][pin];
}
