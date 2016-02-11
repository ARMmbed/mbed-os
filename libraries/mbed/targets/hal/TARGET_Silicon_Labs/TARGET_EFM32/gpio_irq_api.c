/***************************************************************************//**
 * @file gpio_irq_api.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#include "device.h"
#if DEVICE_INTERRUPTIN

#include "gpio_irq_api.h"
#include "mbed_assert.h"
#include "pinmap.h"

#include "em_gpio.h"
#include "em_int.h"
#include "em_cmu.h"
#include "sleep_api.h"
#include "sleepmodes.h"

#define NUM_GPIO_CHANNELS (16)
#define GPIO_LEAST_ACTIVE_SLEEPMODE EM3

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

static uint32_t channel_ids[NUM_GPIO_CHANNELS] = { 0 }; // Relates pin number with interrupt action id
static uint8_t channel_ports[NUM_GPIO_CHANNELS/2] = { 0 }; // Storing 2 ports in each uint8
static gpio_irq_handler irq_handler;
static void GPIOINT_IRQDispatcher(uint32_t iflags);

static void handle_interrupt_in(uint8_t pin)
{
    // Return if pin not linked with an interrupt function
    if (channel_ids[pin] == 0) {
        return;
    }

    //we are storing two ports in each uint8, so we must aquire the one we want.
    // If pin is odd, the port is encoded in the 4 most significant bits. If pin is even, the port is encoded in the 4 least significant bits
    uint8_t isRise = GPIO_PinInGet((pin & 0x1) ? channel_ports[(pin>>1) & 0x7] >> 4 & 0xF : channel_ports[(pin>>1) & 0x7] & 0xF, pin);

    // Get trigger event
    gpio_irq_event event = IRQ_NONE;
    if ((GPIO->EXTIFALL & (1 << pin)) && !isRise) {
        event = IRQ_FALL;
    } else if ((GPIO->EXTIRISE & (1 << pin)) && isRise) {
        event = IRQ_RISE;
    }
    GPIO_IntClear(pin);
    irq_handler(channel_ids[pin], event);
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

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    // Init pins
    gpio_irq_preinit(obj, pin);
    // Initialize GPIO interrupt dispatcher
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);

    /* Relate pin to interrupt action id */
    channel_ids[obj->pin & 0xF] = id;

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
    channel_ids[obj->pin & 0xF] = 0;
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

    bool was_disabled = false;
    if(GPIO->IEN == 0) was_disabled = true;

    GPIO_IntConfig((GPIO_Port_TypeDef)((obj->pin >> 4) & 0xF), obj->pin &0xF, obj->risingEdge, obj->fallingEdge, obj->risingEdge || obj->fallingEdge);
    if ((GPIO->IEN != 0) && (obj->risingEdge || obj->fallingEdge) && was_disabled) {
        blockSleepMode(GPIO_LEAST_ACTIVE_SLEEPMODE);
    }
}

inline void gpio_irq_enable(gpio_irq_t *obj)
{
    if(GPIO->IEN == 0) blockSleepMode(GPIO_LEAST_ACTIVE_SLEEPMODE);
    GPIO_IntEnable(1 << (obj->pin & 0xF)); // pin mask for pins to enable
}

inline void gpio_irq_disable(gpio_irq_t *obj)
{
    GPIO_IntDisable(1 << (obj->pin & 0xF)); // pin mask for pins to disable
    if(GPIO->IEN == 0) unblockSleepMode(GPIO_LEAST_ACTIVE_SLEEPMODE);
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
