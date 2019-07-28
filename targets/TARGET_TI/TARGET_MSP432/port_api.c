/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include <stdint.h>
#include "port_api.h"
#include "gpio_msp432.h"

#if DEVICE_PORTIN || DEVICE_PORTOUT

/** Get the pin name from the port's pin number
 *
 * @param port  The port name
 * @param pin_n The pin number within the specified port
 * @return The pin name for the port's pin number
 */
PinName port_pin(PortName port, int pin_n)
{
    return (PinName)((port << 4) + pin_n);
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
    /* Initialize the port object */
    obj->port     = port;
    obj->mask     = mask;
    obj->port_in  = PORT_REG(port_base[port], REG_IN);
    obj->port_out = PORT_REG(port_base[port], REG_OUT);
    obj->port_dir = PORT_REG(port_base[port], REG_DIR);
    port_dir(obj, dir);
}

/** Set the input port mode
 *
 * @param obj  The port object
 * @param mode THe port mode to be set
 */
void port_mode(port_t *obj, PinMode mode)
{
    uint32_t i;
    for (i = 0; i < 8; i++) {
        /* Is pin used ? */
        if (obj->mask & (1 << i)) {
            pin_mode(port_pin(obj->port, i), mode);
        }
    }
}

/** Set port direction (in/out)
 *
 * @param obj The port object
 * @param dir The port direction to be set
 */
void port_dir(port_t *obj, PinDirection dir)
{
    if (dir == PIN_OUTPUT) {
        *obj->port_dir |= (obj->mask);
    } else {
        *obj->port_dir &= ~(obj->mask);
    }
}

/** Write value to the port
 *
 * @param obj   The port object
 * @param value The value to be set
 */
void port_write(port_t *obj, int value)
{
    *obj->port_out = (*obj->port_out & ~obj->mask) | (value & obj->mask);
}

/** Read the current value on the port
 *
 * @param obj The port object
 * @return An integer with each bit corresponding to an associated port pin setting
 */
int port_read(port_t *obj)
{
    return *obj->port_in & obj->mask;
}

#endif /* DEVICE_PORTIN || DEVICE_PORTOUT */
