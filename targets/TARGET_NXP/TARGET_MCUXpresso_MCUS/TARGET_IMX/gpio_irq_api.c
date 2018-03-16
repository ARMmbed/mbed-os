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
#include "fsl_gpio.h"
#include "mbed_error.h"

#define CHANNEL_NUM    160

#define IRQ_DISABLED       (0)
#define IRQ_RISING_EDGE    (2)
#define IRQ_FALLING_EDGE   (3)
#define IRQ_EITHER_EDGE    (4)

static uint32_t channel_ids[CHANNEL_NUM] = {0};

static gpio_irq_handler irq_handler;

static GPIO_Type * const gpio_addrs[] = GPIO_BASE_PTRS;

/* Array of PORT IRQ number. */
static const IRQn_Type gpio_irqs[] = GPIO_IRQS;

static void handle_interrupt_in(PortName port, int ch_base)
{
    uint32_t i;
    uint32_t interrupt_flags;
    GPIO_Type *port_base = gpio_addrs[port];

    interrupt_flags = GPIO_GetPinsInterruptFlags(port_base);

    for (i = 0; i < 32; i++) {
        if (interrupt_flags & (1 << i)) {
            uint32_t id = channel_ids[ch_base + i];
            if (id == 0) {
                continue;
            }

            gpio_irq_event event = IRQ_NONE;

            if (port_base->EDGE_SEL  & (1U << i)) {
                /* Either edge was selected, find the edge that triggered the interrupt */
                event = (GPIO_ReadPinInput(port_base, i)) ? (IRQ_RISE) : (IRQ_FALL);
            } else {
                /* Detect if falling or rising edge */
                uint32_t icr;
                uint32_t icrShift;

                icrShift = i;

                if (i < 16) {
                    icr = port_base->ICR1;
                } else {
                    icr = port_base->ICR2;
                    icrShift -= 16;
                }

                if (((icr >> (2 * icrShift)) & 0x3) == IRQ_RISING_EDGE) {
                    event = IRQ_RISE;
                }
                if (((icr >> (2 * icrShift)) & 0x3) == IRQ_FALLING_EDGE) {
                    event = IRQ_FALL;
                }
            }

            if (event != IRQ_NONE) {
                irq_handler(id, event);
            }
        }
    }
    GPIO_ClearPinsInterruptFlags(port_base, interrupt_flags);
}

void gpio1_irq(void)
{
    handle_interrupt_in(Gpio1, 0);
}

void gpio2_irq(void)
{
    handle_interrupt_in(Gpio2, 32);
}

void gpio3_irq(void)
{
    handle_interrupt_in(Gpio3, 64);
}

void gpio4_irq(void)
{
    handle_interrupt_in(Gpio4, 96);
}

void gpio5_irq(void)
{
    handle_interrupt_in(Gpio5, 128);
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    if (pin == NC) {
        return -1;
    }

    irq_handler = handler;
    obj->port = (pin >> GPIO_PORT_SHIFT) & 0xF;
    obj->pin = (pin & 0xFF);

    uint32_t ch_base = 0;
    uint32_t vector = (uint32_t)gpio1_irq;

    switch (obj->port) {
        case Gpio1:
            ch_base = 0;
            vector = (uint32_t)gpio1_irq;
            break;
        case Gpio2:
            ch_base = 32;
            vector = (uint32_t)gpio2_irq;
            break;
        case Gpio3:
            ch_base = 64;
            vector = (uint32_t)gpio3_irq;
            break;
        case Gpio4:
            ch_base = 96;
            vector = (uint32_t)gpio4_irq;
            break;
        case Gpio5:
            ch_base = 128;
            vector = (uint32_t)gpio5_irq;
            break;
        default:
            error("gpio_irq only supported on port A-E.");
            break;
    }
    NVIC_SetVector(gpio_irqs[obj->port], vector);
    NVIC_EnableIRQ(gpio_irqs[obj->port]);

    obj->ch = ch_base + obj->pin;
    channel_ids[obj->ch] = id;

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    channel_ids[obj->ch] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    GPIO_Type *base = gpio_addrs[obj->port];
    gpio_interrupt_mode_t irq_settings = kGPIO_NoIntmode;
    uint32_t icr;
    uint32_t icrShift;
    uint32_t irq_curr_settings = IRQ_DISABLED;

    icrShift = obj->pin;

    if (obj->pin < 16) {
        icr = base->ICR1;
    } else {
        icr = base->ICR2;
        icrShift -= 16;
    }

    if (base->EDGE_SEL & (1U << obj->pin)) {
        irq_curr_settings = IRQ_EITHER_EDGE;
    } else {
        if (((icr >> (2 * icrShift)) & 0x3) == IRQ_RISING_EDGE) {
            irq_curr_settings = IRQ_RISING_EDGE;
        }
        if (((icr >> (2 * icrShift)) & 0x3) == IRQ_FALLING_EDGE) {
            irq_curr_settings = IRQ_FALLING_EDGE;
        }
    }

    switch (irq_curr_settings) {
        case IRQ_DISABLED:
            if (enable)
                irq_settings = (event == IRQ_RISE) ? (kGPIO_IntRisingEdge) : (kGPIO_IntFallingEdge);
            break;
        case IRQ_RISING_EDGE:
            if (enable) {
                irq_settings = (event == IRQ_RISE) ? (kGPIO_IntRisingEdge) : (kGPIO_IntRisingOrFallingEdge);
            } else {
                if (event == IRQ_FALL)
                    irq_settings = kGPIO_IntRisingEdge;
            }
            break;
        case IRQ_FALLING_EDGE:
            if (enable) {
                irq_settings = (event == IRQ_FALL) ? (kGPIO_IntFallingEdge) : (kGPIO_IntRisingOrFallingEdge);
            } else {
                if (event == IRQ_RISE)
                    irq_settings = kGPIO_IntFallingEdge;
            }
            break;
        case IRQ_EITHER_EDGE:
            if (enable) {
                irq_settings = kGPIO_IntRisingOrFallingEdge;
            } else {
                irq_settings = (event == IRQ_RISE) ? (kGPIO_IntFallingEdge) : (kGPIO_IntRisingEdge);
            }
            break;
    }

    if (irq_settings == kGPIO_NoIntmode) {
        GPIO_DisableInterrupts(base, 1U << obj->pin);
    } else {
        GPIO_SetPinInterruptConfig(base, obj->pin, irq_settings);
        GPIO_EnableInterrupts(base, 1U << obj->pin);
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    NVIC_EnableIRQ(gpio_irqs[obj->port]);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    NVIC_DisableIRQ(gpio_irqs[obj->port]);
}

#endif
