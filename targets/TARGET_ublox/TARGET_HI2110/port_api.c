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

#include "port_api.h"
#include "pinmap.h"
#include "gpio_api.h"

PinName port_pin(PortName port, int pin_n)
{
    MBED_ASSERT (port < PortMaxNumber);

    (void) port;
    return (PinName)(pin_n);
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    MBED_ASSERT (port < PortMaxNumber);

    obj->port = port;
    obj->mask = mask;

    obj->reg_dir = &GPIO_DIR;
    obj->reg_out = &GPIO_OUT;
    obj->reg_val = &GPIO_VALUE;
    obj->reg_drv = &GPIO_DRIVE;

    /* Claim the pins */
    for (uint8_t x = 0; x < NUM_PINS; x++) {
        if (mask & (1ul << x)) {
            pin_function((PinName) x, PIN_FUNCTION_GPIO);
        }
    }
    
    /* Set up the pins */
    port_dir(obj, dir);
}

void port_mode(port_t *obj, PinMode mode)
{
    switch (mode) {
        case PullUp:
        {
            MBED_ASSERT(false);  /* Not currently supported on HI2110 */
        }
        break;
        case PullDown:
        {
            GPIO_PULLEN_BITSET |= obj->mask;
        }
        break;
        case PullNone:
        {
            GPIO_PULLEN_BITCLR &= ~(obj->mask);
        }
        break;
        default:
        break;
    }
}

void port_dir(port_t *obj, PinDirection dir)
{
    switch (dir) {
        case PIN_INPUT:
        {
            *(obj->reg_dir) &= ~(obj->mask);
        }
        break;
        case PIN_OUTPUT:
        {
            *(obj->reg_dir) |= obj->mask;
            /* TODO: do we need to set the drive strength?  If so, how do we decide which way? */
            /* obj->reg_drv |= obj->mask; */
        }
        break;
    }
}

void port_write(port_t *obj, int value)
{
    *(obj->reg_out) = value;
}

int port_read(port_t *obj)
{
    return *(obj->reg_val);
}
