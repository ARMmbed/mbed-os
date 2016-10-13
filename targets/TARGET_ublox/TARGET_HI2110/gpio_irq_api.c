/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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

/* TODO: this needs testing */

#include <stddef.h>
#include "cmsis.h"

#include "gpio_irq_api.h"
#include "mbed_error.h"

/* ----------------------------------------------------------------
 * MACROS
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * TYPES
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * GLOBAL VARIABLES
 * ----------------------------------------------------------------*/

/* Each GPIO pin is given an ID, if the ID is 0 the pin can be ignored. */
static uint8_t channel_ids[20] = {0};
static gpio_irq_handler irq_handler;

static bool initialised = false;

/* ----------------------------------------------------------------
 * FUNCTION PROTOTYPES
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * NON-API FUNCTIONS
 * ----------------------------------------------------------------*/

void IRQ5_GPIO_Handler()
{
    if (initialised) {
        for (uint8_t i = 0; i < sizeof (channel_ids) / sizeof (channel_ids[0]); i++) {
            uint8_t id = channel_ids[i];
            uint32_t mask = 1 << id;
    
            if (id != 0) {
                if (GPIO_IRQ | mask) {
                    if (GPIO_INT_RISE | mask) {
                        irq_handler(id, IRQ_RISE);
                    } else if (GPIO_INT_FALL | mask) {
                        irq_handler(id, IRQ_FALL);
                    }
                }
            }    
        }
    }
    
    /* Clear all the interrupt bits (rather than wasting time on
     * each individual one), or we might get stuck if a spurious
     * one should arrive. */
    GPIO_INT_CLR = 0xFFFFFFFF;
}

/* ----------------------------------------------------------------
 * MBED API CALLS
 * ----------------------------------------------------------------*/

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    bool return_value = -1;

    if (initialised) {
        return_value = 0;
    } else {
        if (pin != NC) {
            MBED_ASSERT (pin < NUM_PINS);

            irq_handler      = handler;
            obj->ch          = pin;
            channel_ids[pin] = id;

            uint32_t mask = 1 << obj->ch;

            /* Remove any existing setting */
            GPIO_INT_RISE_BITCLR &= ~mask;
            GPIO_INT_FALL_BITCLR &= ~mask;
            GPIO_INT_LOW_BITCLR &= ~mask;
            GPIO_INT_HIGH_BITCLR &= ~mask;

            initialised = true;
            NVIC_EnableIRQ  (GPIO_IRQn);

            return_value = 0;
        }
    }
    
    return return_value;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    channel_ids[obj->ch] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    uint32_t mask = 1 << obj->ch;

    if (enable) {
        if (event == IRQ_RISE) {
            GPIO_INT_RISE_BITSET |= mask;
        } else if (event == IRQ_FALL) {
            GPIO_INT_FALL_BITSET |= mask;
        }
    }
    else
    {
        if (event == IRQ_RISE) {
            GPIO_INT_RISE_BITSET &= ~mask;
        } else if (event == IRQ_FALL) {
            GPIO_INT_FALL_BITSET &= ~mask;
        }
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    (void) obj;
    NVIC_EnableIRQ  (GPIO_IRQn);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    (void) obj;
    NVIC_DisableIRQ  (GPIO_IRQn);
}
