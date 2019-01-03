/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include "port_api.h"
#include "pinmap.h"
#include "gpio_api.h"
#include "mbed_error.h"

#if DEVICE_PORTIN || DEVICE_PORTOUT

extern uint32_t gpio_clock_enable(uint32_t port_idx);

/** Get the pin name from the port's pin number
 *
 * @param port  The port name
 * @param pin_n The pin number within the specified port
 * @return The pin name for the port's pin number
 * BIT[7:4] port number
   BIT[3:0] pin number
 */
PinName port_pin(PortName port, int pin_n)
{
    return (PinName)(pin_n + (port << 4));
}

/** Initilize the port
 *
 * @param obj  The port object to initialize
 * @param port The port name
 * @param mask The bitmask to identify which bits in the port should be included (0 - ignore)
 * @param dir  The port direction
 */
void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    uint32_t port_index = (uint32_t)port;
    uint32_t gpio =  gpio_clock_enable(port_index);

    obj->port      = port;
    obj->mask      = mask;
    obj->direction = dir;
    obj->reg_in    = &GPIO_ISTAT(gpio);
    obj->reg_out   = &GPIO_OCTL(gpio);

    port_dir(obj, dir);
}

/** Set port direction (in/out)
 *
 * @param obj The port object
 * @param dir The port direction to be set
 */
void port_dir(port_t *obj, PinDirection dir)
{
    uint32_t i;
    obj->direction = dir;
    for (i = 0; i < 16; i++) {
        if (obj->mask & (1 << i)) {
            if (PIN_INPUT != dir) {
                pin_function(port_pin(obj->port, i), SET_PIN_FUNCTION(PIN_MODE_OUTPUT, PIN_OTYPE_PP, PIN_PUPD_NONE, PIN_AF_0));
            } else {
                pin_function(port_pin(obj->port, i), SET_PIN_FUNCTION(PIN_MODE_INPUT, PIN_OTYPE_PP, PIN_PUPD_NONE, PIN_AF_0));
            }
        }
    }
}

/** Set the input port mode
 *
 * @param obj  The port object
 * @param mode THe port mode to be set
 */
void port_mode(port_t *obj, PinMode mode)
{
    uint32_t i;
    for (i = 0; i < 16; i++) {
        if (obj->mask & (1 << i)) {
            pin_mode(port_pin(obj->port, i), mode);
        }
    }
}

/** Write value to the port
 *
 * @param obj   The port object
 * @param value The value to be set
 */
void port_write(port_t *obj, int value)
{
    *obj->reg_out = (*obj->reg_out & ~obj->mask) | (value & obj->mask);
}

/** Read the current value on the port
 *
 * @param obj The port object
 * @return An integer with each bit corresponding to an associated port pin setting
 */
int port_read(port_t *obj)
{
    if (PIN_INPUT != obj->direction) {
        return (*obj->reg_out & obj->mask);
    } else {
        return (*obj->reg_in & obj->mask);
    }
}

#endif
