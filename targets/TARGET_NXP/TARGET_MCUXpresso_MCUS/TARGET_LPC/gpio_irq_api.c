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

#if DEVICE_INTERRUPTIN

#include "gpio_api.h"
#include "fsl_inputmux.h"
#include "fsl_pint.h"
#include "mbed_error.h"

#define INTERRUPT_PORTS 2

static uint32_t channel_ids[NUMBER_OF_GPIO_INTS] = {0};
static gpio_irq_handler irq_handler;
/* Array of PORT IRQ number. */
static const IRQn_Type pint_irqs[] = PINT_IRQS;

void pint_intr_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    uint32_t ch_bit = (1 << pintr);

    // Return immediately if:
    //   * The interrupt was already served
    //   * There is no user handler
    //   * It is a level interrupt, not an edge interrupt
    if (((PINT->IST & ch_bit) == 0) ||
        (channel_ids[pintr] == 0) ||
        (PINT->ISEL & ch_bit)) {
        return;
    }

    if ((PINT->IENR & ch_bit) && (PINT->RISE & ch_bit)){
        irq_handler(channel_ids[pintr], IRQ_RISE);
        PINT->RISE = ch_bit;
    }

    if ((PINT->IENF & ch_bit) && (PINT->FALL & ch_bit)) {
        irq_handler(channel_ids[pintr], IRQ_FALL);
        PINT->FALL = ch_bit;
    }
    PINT_PinInterruptClrStatus(PINT, pintr);
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    int found_free_channel = 0;
    int i = 0;

    if (pin == NC) {
        return -1;
    }

    irq_handler = handler;

    for (i = 0; i < NUMBER_OF_GPIO_INTS; i++) {
        if (channel_ids[i] == 0) {
            channel_ids[i] = id;
            obj->ch = i;
            found_free_channel = 1;
            break;
        }
    }

    if (!found_free_channel) {
        return -1;
    }

    obj->pin = pin & 0x1F;
    obj->port = pin / 32;

    if (obj->port >= INTERRUPT_PORTS) {
        return -1;
    }

    /* Connect trigger sources to PINT */
    INPUTMUX_Init(INPUTMUX);

    INPUTMUX->PINTSEL[obj->ch] = pin;

    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

    /* Initialize PINT */
    PINT_Init(PINT);

    NVIC_EnableIRQ(pint_irqs[obj->ch]);

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    channel_ids[obj->ch] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    unsigned int ch_bit = (1 << obj->ch);

    // Clear interrupt
    if (!(PINT->ISEL & ch_bit))
        PINT->IST = ch_bit;

    // Edge trigger
    PINT->ISEL &= ~ch_bit;

    /* Setup Pin Interrupt for rising or falling edge */
    if (enable) {
        if (event == IRQ_RISE) {
            /* Checking if falling edge interrupt is already enabled on this pin */
            if (PINT->IENF & (1U << obj->ch)) {
                PINT_PinInterruptConfig(PINT, (pint_pin_int_t)obj->ch, kPINT_PinIntEnableBothEdges, pint_intr_callback);
            } else {
                PINT_PinInterruptConfig(PINT, (pint_pin_int_t)obj->ch, kPINT_PinIntEnableRiseEdge, pint_intr_callback);
            }
        } else {
            /* Checking if rising edge interrupt is already enabled on this pin */
            if (PINT->IENR & (1U << obj->ch)) {
                PINT_PinInterruptConfig(PINT, (pint_pin_int_t)obj->ch, kPINT_PinIntEnableBothEdges, pint_intr_callback);
            } else {
                PINT_PinInterruptConfig(PINT, (pint_pin_int_t)obj->ch, kPINT_PinIntEnableFallEdge, pint_intr_callback);
            }
        }
    } else {
        PINT_PinInterruptConfig(PINT, (pint_pin_int_t)obj->ch, kPINT_PinIntEnableNone, NULL);
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    NVIC_EnableIRQ(pint_irqs[obj->ch]);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    NVIC_DisableIRQ(pint_irqs[obj->ch]);
}

#endif
