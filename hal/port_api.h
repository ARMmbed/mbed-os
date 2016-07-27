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
#ifndef MBED_PORTMAP_H
#define MBED_PORTMAP_H

#include "device.h"

#if DEVICE_PORTIN || DEVICE_PORTOUT

#ifdef __cplusplus
extern "C" {
#endif

/** Port HAL structure. port_s is declared in the target's HAL
 */
typedef struct port_s port_t;

/**
 * \defgroup hal_port Port HAL functions
 * @{
 */

/** Get the pin name from the port's pin number
 *
 * @param port  The port name
 * @param pin_n The pin number within the specified port
 * @return The pin name for the port's pin number
 */
PinName port_pin(PortName port, int pin_n);

/** Initilize the port
 *
 * @param obj  The port object to initialize
 * @param port The port name
 * @param mask The bitmask to identify which bits in the port should be included (0 - ignore)
 * @param dir  The port direction
 */
void port_init(port_t *obj, PortName port, int mask, PinDirection dir);

/** Set the input port mode
 *
 * @param obj  The port object
 * @param mode THe port mode to be set
 */
void port_mode(port_t *obj, PinMode mode);

/** Set port direction (in/out)
 *
 * @param obj The port object
 * @param dir The port direction to be set
 */
void port_dir(port_t *obj, PinDirection dir);

/** Write value to the port
 *
 * @param obj   The port object
 * @param value The value to be set
 */
void port_write(port_t *obj, int value);

/** Read the current value on the port
 *
 * @param obj The port object
 * @return An integer with each bit corresponding to an associated port pin setting
 */
int port_read(port_t *obj);

/**@}*/

#ifdef __cplusplus
}
#endif
#endif

#endif
