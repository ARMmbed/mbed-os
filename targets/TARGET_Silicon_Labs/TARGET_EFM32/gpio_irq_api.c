/***************************************************************************//**
 * @file gpio_irq_api.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#if DEVICE_INTERRUPTIN

#include "gpio_irq_api.h"
#include "mbed_assert.h"
#include "pinmap.h"

#include "em_gpio.h"
#include "em_cmu.h"
#include "sleep_api.h"

#define NUM_GPIO_CHANNELS (16)

/* Macro return index of the LSB flag which is set. */
#if ((__CORTEX_M == 3) || (__CORTEX_M == 4))
#define GPIOINT_MASK2IDX(mask) (__CLZ(__RBIT(mask)))
#elif __CORTEX_M == 0
#define GPIOINT_MASK2IDX(mask) (countTrailingZeros(mask))
__STATIC_INLINE uint32_t countTrailingZeros(uint32_t mask)
{
    uint32_t zeros;
    for(zeros=0; (zeros<32) && (0 == (mask&0x1)); zeros++, mask>>=1);
    return zeros;
}
#else
#error Unsupported architecture.
#endif

static uintptr_t channel_contexts[NUM_GPIO_CHANNELS] = { 0 }; // Relates pin number with interrupt action context
static uint8_t channel_ports[NUM_GPIO_CHANNELS/2] = { 0 }; // Storing 2 ports in each uint8
static gpio_irq_handler irq_handler;
static void GPIOINT_IRQDispatcher(uint32_t iflags);

static void handle_interrupt_in(uint8_t pin)
{
    // Return if pin not linked with an interrupt function
    if (channel_contexts[pin] == 0) {
        return;
    }

    // trying to discern which GPIO IRQ we got
    gpio_irq_event event = IRQ_NONE;
    if (((GPIO->EXTIFALL & (1 << pin)) != 0) && ((GPIO->EXTIRISE & (1 << pin)) == 0)) {
        // Only the fall handler is active, so this must be a falling edge
        event = IRQ_FALL;
    } else if (((GPIO->EXTIFALL & (1 << pin)) == 0) && ((GPIO->EXTIRISE & (1 << pin)) != 0)) {
        // Only the rise handler is active, so this must be a rising edge
        event = IRQ_RISE;
    } else {
        // Ambiguous as to which IRQ we've received. Poll the pin to check which one to fire.
        // NOTE: If trying to detect a pulse where the width is shorter than this handler's
        // reaction time, there will only be one callback (for the trailing edge) called.

        // we are storing two ports in each uint8, so we must acquire the one we want.
        // If pin is odd, the port is encoded in the 4 most significant bits.
        // If pin is even, the port is encoded in the 4 least significant bits
        uint8_t isRise = GPIO_PinInGet((pin & 0x1) ?
                                       channel_ports[(pin>>1) & 0x7] >> 4 & 0xF :
                                       channel_ports[(pin>>1) & 0x7] >> 0 & 0xF, pin);
        event = (isRise == 1 ? IRQ_RISE : IRQ_FALL);
    }
    GPIO_IntClear(pin);
    irq_handler(channel_contexts[pin], event);
}

void gpio_irq_preinit(gpio_irq_t *obj, PinName pin)
{
    MBED_ASSERT(pin != NC);

    /* Pin and port index encoded in one uint32.
     * The four least significant bits represent the pin number
     * The remaining bits represent the port number */
    obj->pin = pin;
    obj->risingEdge = 0;
    obj->fallingEdge = 0;
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uintptr_t context)
{
    // Init pins
    gpio_irq_preinit(obj, pin);
    // Initialize GPIO interrupt dispatcher
    NVIC_SetVector(GPIO_ODD_IRQn, (uint32_t)GPIO_ODD_IRQHandler);
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
    NVIC_SetVector(GPIO_EVEN_IRQn, (uint32_t)GPIO_EVEN_IRQHandler);
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);

    /* Relate pin to interrupt action context */
    channel_contexts[obj->pin & 0xF] = context;

    // Relate the pin number to a port. If pin in is odd store in the 4 most significant bits, if pin is even store in the 4 least significant bits
    channel_ports[(obj->pin >> 1) & 0x7] = (obj->pin & 0x1) ? (channel_ports[(obj->pin >> 1) & 0x7] & 0x0F) | (obj->pin & 0xF0) : (channel_ports[(obj->pin >> 1) & 0x7] & 0xF0) | ((obj->pin >> 4) & 0xF);
    /* Save pointer to handler */
    irq_handler = handler;

    pin_mode(obj->pin, Input);
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    // Destructor
    channel_contexts[obj->pin & 0xF] = 0;
    gpio_irq_disable(obj); // Disable interrupt channel
    pin_mode(obj->pin, Disabled); // Disable input pin
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    switch (event) {
        case (IRQ_RISE):
            obj->risingEdge = enable;
            break;
        case (IRQ_FALL):
            obj->fallingEdge = enable;
            break;
        case (IRQ_NONE):
            break;
    }

    /* Disable, set config and enable */
    gpio_irq_disable(obj);

    GPIO_IntConfig((GPIO_Port_TypeDef)((obj->pin >> 4) & 0xF), obj->pin &0xF, obj->risingEdge, obj->fallingEdge, obj->risingEdge || obj->fallingEdge);
}

inline void gpio_irq_enable(gpio_irq_t *obj)
{
    GPIO_IntEnable(1 << (obj->pin & 0xF)); // pin mask for pins to enable
}

inline void gpio_irq_disable(gpio_irq_t *obj)
{
    GPIO_IntDisable(1 << (obj->pin & 0xF)); // pin mask for pins to disable
}

/***************************************************************************//**
 * @brief
 *   Function calls users callback for registered pin interrupts.
 *
 * @details
 *   This function is called when GPIO interrupts are handled by the dispatcher.
 *   Function gets even or odd interrupt flags and calls user callback
 *   registered for that pin. Function iterates on flags starting from MSB.
 *
 * @param iflags
 *  Interrupt flags which shall be handled by the dispatcher.
 *
 ******************************************************************************/
static void GPIOINT_IRQDispatcher(uint32_t iflags)
{
    uint32_t irqIdx;

    /* check for all flags set in IF register */
    while(iflags) {
        irqIdx = GPIOINT_MASK2IDX(iflags);

        /* clear flag */
        iflags &= ~(1 << irqIdx);

        /* call user callback */
        handle_interrupt_in(irqIdx);
    }
}

/***************************************************************************//**
 * @brief
 *   GPIO EVEN interrupt handler. Interrupt handler clears all IF even flags and
 *   call the dispatcher passing the flags which triggered the interrupt.
 *
 ******************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
    uint32_t iflags;
    /* Get all even interrupts */
    iflags = GPIO_IntGetEnabled() & 0x00005555;

    /* Clean only even interrupts*/

    GPIO_IntClear(iflags);
    GPIOINT_IRQDispatcher(iflags);
}


/***************************************************************************//**
 * @brief
 *   GPIO ODD interrupt handler. Interrupt handler clears all IF odd flags and
 *   call the dispatcher passing the flags which triggered the interrupt.
 *
 ******************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
    uint32_t iflags;

    /* Get all odd interrupts */
    iflags = GPIO_IntGetEnabled() & 0x0000AAAA;

    /* Clean only even interrupts */
    GPIO_IntClear(iflags);
    GPIOINT_IRQDispatcher(iflags);
}

#endif
