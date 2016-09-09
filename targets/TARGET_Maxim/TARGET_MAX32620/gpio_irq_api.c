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
#include "ioman_regs.h"
#include "pwrman_regs.h"
#include "pwrseq_regs.h"

static gpio_irq_t *objs[MXC_GPIO_NUM_PORTS][MXC_GPIO_MAX_PINS_PER_PORT] = {{0}};
static gpio_irq_handler irq_handler;

static void gpio_irq_wud_req(gpio_irq_t *obj)
{
    unsigned int port = obj->port;
    unsigned int pin = obj->pin;
    uint32_t pin_mask = 1 << pin;

    /* Ports 0-3 are controlled by wud_req0, while 4-7 are controlled by wud_req1 */
    /* During the time the WUD IOMAN requests are asserted (1), the GPIO Pad */
    /* is in HIGH Z mode, regardless of GPIO setting. This may cause bogus interrupts. */
    if (port < 4) {
        uint32_t mask = pin_mask << (port << 3);
        if (!(MXC_IOMAN->wud_ack0 & mask)) {
            MXC_IOMAN->wud_req0 |= mask;
            while(!(MXC_IOMAN->wud_ack0 & mask));
        }
    } else if (port < 8) {
        uint32_t mask = pin_mask << ((port-4) << 3);
        if (!(MXC_IOMAN->wud_ack1 & mask)) {
            MXC_IOMAN->wud_req1 |= mask;
            while(!(MXC_IOMAN->wud_ack1 & mask));
        }
    }
}

/* Clear the selected pin from wake-up detect */
static void gpio_irq_wud_clear(gpio_irq_t *obj)
{
    unsigned int port = obj->port;
    unsigned int pin = obj->pin;

    /* Enable modifications to WUD configuration */
    MXC_PWRMAN->wud_ctrl = MXC_F_PWRMAN_WUD_CTRL_CTRL_ENABLE;

    /* Select pad in WUD control */
    /* Note: Pads are numbered from 0-48; {0-7} => {P0.0-P0.7}, {8-15} => {P1.0-P1.7}, etc. */
    MXC_PWRMAN->wud_ctrl |= (port * 8) + pin;

    /* Clear any existing WUD configuration for this pad */
    MXC_PWRMAN->wud_ctrl &= ~(MXC_F_PWRMAN_WUD_CTRL_PAD_MODE);
    MXC_PWRMAN->wud_ctrl |= (MXC_E_PWRMAN_PAD_MODE_CLEAR_SET << MXC_F_PWRMAN_WUD_CTRL_PAD_MODE_POS);
    /* Clear with PULSE0; PULSE1 enables WUD */
    MXC_PWRMAN->wud_pulse0 = 1;

    /* Disable configuration */
    MXC_PWRMAN->wud_ctrl = 0;
    MXC_IOMAN->wud_req0 = 0;
    MXC_IOMAN->wud_req1 = 0;
}

/* Configure the selected pin for wake-up detect */
static void gpio_irq_wud_config(gpio_irq_t *obj)
{
    unsigned int port = obj->port;
    unsigned int pin = obj->pin;
    uint32_t pin_mask = 1 << pin;

    /* Enable modifications to WUD configuration */
    MXC_PWRMAN->wud_ctrl = MXC_F_PWRMAN_WUD_CTRL_CTRL_ENABLE;

    /* Select pad in WUD control */
    /* Note: Pads are numbered from 0-48; {0-7} => {P0.0-P0.7}, {8-15} => {P1.0-P1.7}, etc. */
    MXC_PWRMAN->wud_ctrl |= (port * 8) + pin;

    /* First clear any existing WUD configuration for this pad */
    MXC_PWRMAN->wud_ctrl &= ~(MXC_F_PWRMAN_WUD_CTRL_PAD_MODE);
    MXC_PWRMAN->wud_ctrl |= (MXC_E_PWRMAN_PAD_MODE_CLEAR_SET << MXC_F_PWRMAN_WUD_CTRL_PAD_MODE_POS);
    /* Clear with PULSE0; PULSE1 enables WUD */
    MXC_PWRMAN->wud_pulse0 = 1;

    if (obj->fall_en || obj->rise_en) {
        /* Configure sense level on this pad */
        MXC_PWRMAN->wud_ctrl &= ~(MXC_F_PWRMAN_WUD_CTRL_PAD_MODE);
        MXC_PWRMAN->wud_ctrl |= (MXC_E_PWRMAN_PAD_MODE_ACT_HI_LO << MXC_F_PWRMAN_WUD_CTRL_PAD_MODE_POS);

        uint32_t in_val = MXC_GPIO->in_val[port] & pin_mask;
        do {
            if (in_val) {
                /* Select active low with PULSE1 (backwards from what you'd expect) */
                MXC_PWRMAN->wud_pulse1 = 1;
            } else {
                /* Select active high with PULSE0 (backwards from what you'd expect) */
                MXC_PWRMAN->wud_pulse0 = 1;
            }
        } while ((MXC_GPIO->in_val[port] & pin_mask) != in_val);

        /* Select this pad to have the wake-up function enabled */
        MXC_PWRMAN->wud_ctrl &= ~(MXC_F_PWRMAN_WUD_CTRL_PAD_MODE);
        MXC_PWRMAN->wud_ctrl |= (MXC_E_PWRMAN_PAD_MODE_CLEAR_SET << MXC_F_PWRMAN_WUD_CTRL_PAD_MODE_POS);
        /* Activate with PULSE1 */
        MXC_PWRMAN->wud_pulse1 = 1;

        // NOTE: Low Power Pullup/down is not normally needed in addition to
        // standard GPIO Pullup/downs.

        /* Enable IOWakeup, as there is at least 1 GPIO pin configured as a wake source */
        MXC_PWRSEQ->msk_flags |= MXC_F_PWRSEQ_MSK_FLAGS_PWR_IOWAKEUP;
    }

    /* Disable configuration */
    MXC_PWRMAN->wud_ctrl = 0;
    MXC_IOMAN->wud_req0 = 0;
    MXC_IOMAN->wud_req1 = 0;
}

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

            gpio_irq_wud_config(obj);
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

    /* request WUD in case the application is going to sleep */
    gpio_irq_wud_req(obj);

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
    gpio_irq_wud_clear(obj);
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    if (event == IRQ_FALL) {
        obj->fall_en = enable;
    } else if (event == IRQ_RISE) {
        obj->rise_en = enable;
    }

    if (obj->fall_en || obj->rise_en) {
        MXC_GPIO->int_mode[obj->port] |= (MXC_V_GPIO_INT_MODE_ANY_EDGE << (obj->pin*4));
        gpio_irq_wud_config(obj); /* enable WUD for this pin so we may wake from deepsleep as well */
    } else {
        MXC_GPIO->int_mode[obj->port] &= (MXC_V_GPIO_INT_MODE_ANY_EDGE << (obj->pin*4));
        gpio_irq_wud_clear(obj);
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    MXC_GPIO->inten[obj->port] |= (1 << obj->pin);
    gpio_irq_wud_config(obj);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    MXC_GPIO->inten[obj->port] &= ~(1 << obj->pin);
    gpio_irq_wud_clear(obj);
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
