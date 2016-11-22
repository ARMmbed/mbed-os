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
#include "fsl_port.h"
#include "mbed_error.h"

#define CHANNEL_NUM    160

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler;
/* Array of PORT peripheral base address. */
static PORT_Type *const port_addrs[] = PORT_BASE_PTRS;
/* Array of PORT IRQ number. */
static const IRQn_Type port_irqs[] = PORT_IRQS;


#define IRQ_DISABLED        (0)
#define IRQ_RAISING_EDGE    (9)
#define IRQ_FALLING_EDGE    (10)
#define IRQ_EITHER_EDGE     (11)

static void handle_interrupt_in(PortName port, int ch_base)
{
    uint32_t i;
    uint32_t interrupt_flags;
    PORT_Type *port_base = port_addrs[port];

    interrupt_flags = PORT_GetPinsInterruptFlags(port_base);

    for (i = 0; i < 32; i++) {
        if (interrupt_flags & (1 << i)) {
            uint32_t id = channel_ids[ch_base + i];
            if (id == 0) {
                continue;
            }

            gpio_irq_event event = IRQ_NONE;
            GPIO_Type *gpio_addrs[] = GPIO_BASE_PTRS;
            GPIO_Type *gpio_base = gpio_addrs[port];

            switch ((port_base->PCR[i] & PORT_PCR_IRQC_MASK) >> PORT_PCR_IRQC_SHIFT) {
                case IRQ_RAISING_EDGE:
                    event = IRQ_RISE;
                    break;

                case IRQ_FALLING_EDGE:
                    event = IRQ_FALL;
                    break;

                case IRQ_EITHER_EDGE:
                    event = (GPIO_ReadPinInput(gpio_base, i)) ? (IRQ_RISE) : (IRQ_FALL);
                    break;
            }
            if (event != IRQ_NONE) {
                irq_handler(id, event);
            }
        }
    }
    PORT_ClearPinsInterruptFlags(port_base, interrupt_flags);
}

void gpio_irqA(void)
{
    handle_interrupt_in(PortA, 0);
}

void gpio_irqB(void)
{
    handle_interrupt_in(PortB, 32);
}

void gpio_irqC(void)
{
    handle_interrupt_in(PortC, 64);
}

void gpio_irqD(void)
{
    handle_interrupt_in(PortD, 96);
}

void gpio_irqE(void)
{
    handle_interrupt_in(PortE, 128);
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    if (pin == NC) {
        return -1;
    }

    irq_handler = handler;
    obj->port = pin >> GPIO_PORT_SHIFT;
    obj->pin = pin & 0x7F;

    uint32_t ch_base = 0;
    uint32_t vector = (uint32_t)gpio_irqA;

    switch (obj->port) {
        case PortA:
            ch_base = 0;
            vector = (uint32_t)gpio_irqA;
            break;
        case PortB:
            ch_base = 32;
            vector = (uint32_t)gpio_irqB;
            break;
        case PortC:
            ch_base = 64;
            vector = (uint32_t)gpio_irqC;
            break;
        case PortD:
            ch_base = 96;
            vector = (uint32_t)gpio_irqD;
            break;
        case PortE:
            ch_base = 128;
            vector = (uint32_t)gpio_irqE;
            break;
        default:
            error("gpio_irq only supported on port A-E.");
            break;
    }
    NVIC_SetVector(port_irqs[obj->port], vector);
    NVIC_EnableIRQ(port_irqs[obj->port]);

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
    PORT_Type *base = port_addrs[obj->port];
    port_interrupt_t irq_settings = kPORT_InterruptOrDMADisabled;

    switch ((base->PCR[obj->pin] & PORT_PCR_IRQC_MASK) >> PORT_PCR_IRQC_SHIFT) {
        case IRQ_DISABLED:
            if (enable)
                irq_settings = (event == IRQ_RISE) ? (kPORT_InterruptRisingEdge) : (kPORT_InterruptFallingEdge);
            break;

        case IRQ_RAISING_EDGE:
            if (enable) {
                irq_settings = (event == IRQ_RISE) ? (kPORT_InterruptRisingEdge) : (kPORT_InterruptEitherEdge);
            } else {
                if (event == IRQ_FALL)
                    irq_settings = kPORT_InterruptRisingEdge;
            }
            break;

        case IRQ_FALLING_EDGE:
            if (enable) {
                irq_settings = (event == IRQ_FALL) ? (kPORT_InterruptFallingEdge) : (kPORT_InterruptEitherEdge);
            } else {
                if (event == IRQ_RISE)
                    irq_settings = kPORT_InterruptFallingEdge;
            }
            break;

        case IRQ_EITHER_EDGE:
            if (enable) {
                irq_settings = kPORT_InterruptEitherEdge;
            } else {
                irq_settings = (event == IRQ_RISE) ? (kPORT_InterruptFallingEdge) : (kPORT_InterruptRisingEdge);
            }
            break;
    }

    PORT_SetPinInterruptConfig(base, obj->pin, irq_settings);
    base->PCR[obj->pin] |= PORT_PCR_ISF_MASK;
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    NVIC_EnableIRQ(port_irqs[obj->port]);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    NVIC_DisableIRQ(port_irqs[obj->port]);
}

#endif
