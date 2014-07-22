/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
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

#include "device.h"
#if DEVICE_INTERRUPTIN

#include "gpio_irq_api.h"
#include "mbed_assert.h"
#include "pinmap.h"

#include "em_gpio.h"
#include "em_int.h"
#include "em_cmu.h"
#include "gpiointerrupt.h"

#define NUM_GPIO_CHANNELS (16)

static uint32_t channel_ids[NUM_GPIO_CHANNELS] = { 0 }; // Relates pin number with interrupt action id
static uint32_t channel_ports[NUM_GPIO_CHANNELS] = { 0 };
static gpio_irq_handler irq_handler;

static void handle_interrupt_in(uint8_t pin)
{
    // Return if pin not linked with an interrupt function
    if (channel_ids[pin] == 0) {
        return;
    }

    uint32_t isRise = GPIO_PinInGet(channel_ports[pin], pin);

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

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    MBED_ASSERT(pin != NC);

    /* Pin and port index encoded in one uint32.
     * The four least significant bits represent the pin number
     * The remaining bits represent the port number */
    obj->pin = pin & 0xF;
    obj->port = pin >> 4;
    obj->risingEdge = 0;
    obj->fallingEdge = 0;

    /* Enable GPIO clock */
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* Initialize GPIO interrupt dispatcher */
    GPIOINT_Init();

    /* Relate pin to interrupt action id */
    channel_ids[obj->pin] = id;
    /* Relate the pin number to a port */
    channel_ports[obj->pin] = obj->port;
    /* Save pointer to handler */
    irq_handler = handler;

    /* Register callbacks before setting up and enabling pin interrupt. */
    GPIOINT_CallbackRegister(obj->pin, &handle_interrupt_in);

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    // Destructor
    channel_ids[obj->pin] = 0;
    gpio_irq_disable(obj); // Disable interrupt channel
    pin_mode(obj->pin | (obj->port << 4), Disabled); // Disable input pin
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
    GPIO_IntConfig(obj->port, obj->pin, obj->risingEdge, obj->fallingEdge, obj->risingEdge || obj->fallingEdge);
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    GPIO_IntEnable(1 << obj->pin); // pin mask for pins to enable
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    GPIO_IntDisable(1 << obj->pin); // pin mask for pins to disable
}

#endif
