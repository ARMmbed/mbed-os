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

#include "mbed_assert.h"
#include "gpio_api.h"
#include "pinmap.h"

/* ----------------------------------------------------------------
 * MACROS
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * TYPES
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * GLOBAL VARIABLES
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * MBED API CALLS
 * ----------------------------------------------------------------*/

void gpio_init(gpio_t *obj, PinName pin)
{
    if (pin == (PinName)NC) {
        return;
    }
    
    MBED_ASSERT (pin <  NUM_PINS);

    obj->pin = pin;
    obj->mask = (1ul << pin);

    obj->reg_set = &GPIO_OUT_BITSET;
    obj->reg_clr = &GPIO_OUT_BITCLR;
    obj->reg_val = &GPIO_VALUE;
    obj->reg_dir = &GPIO_DIR;

    /* Claim the pin */
    pin_function(pin, PIN_FUNCTION_GPIO);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj->pin != (PinName)NC);

    switch (direction) {
        case PIN_INPUT:
        {
            *(obj->reg_dir) &= ~(obj->mask);
        }
        break;
        case PIN_OUTPUT:
        {
            *(obj->reg_dir) |= obj->mask;
        }
        break;
        /* TODO: looks as though the default is
         * not normally trapped here */
    }
}
