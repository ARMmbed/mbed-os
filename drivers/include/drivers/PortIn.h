/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_PORTIN_H
#define MBED_PORTIN_H

#include "platform/platform.h"

#if DEVICE_PORTIN || defined(DOXYGEN_ONLY)

#include "hal/port_api.h"

namespace mbed {
/**
 * \defgroup drivers_PortIn PortIn class
 * \ingroup drivers-public-api-gpio
 * @{
 */

/** A multiple pin digital input
 *
 * @note Synchronization level: Interrupt safe
 *
 *  Example:
 * @code
 * // Turn on an LED if any pins of Port2[0:5] are high
 *
 * #include "mbed.h"
 *
 * PortIn     p(Port2, 0x0000003F);  // Port2 pins [0:5] only
 * DigitalOut led(LED4);
 *
 * int main() {
 *     while(1) {
 *         int pins = p.read();
 *         if(pins) {
 *             led = 1;
 *         } else {
 *             led = 0;
 *         }
 *     }
 * }
 * @endcode
 */
class PortIn {
public:

    /** Create a PortIn, connected to the specified port
     *
     *  @param port Port to connect to (as defined in target's PortNames.h)
     *  @param mask Bitmask defines which port pins should be an input (0 - ignore, 1 - include)
        */
    PortIn(PortName port, int mask = 0xFFFFFFFF);

    /** Read the value input to the port
     *
     *  @returns
     *    An integer with each bit corresponding to the associated pin value
     */
    int read()
    {
        return port_read(&_port);
    }

    /** Set the input pin mode
     *
     *  @param mode PullUp, PullDown, PullNone, OpenDrain
     */
    void mode(PinMode mode);

    /** A shorthand for read()
     */
    operator int()
    {
        return read();
    }

private:
    port_t _port;
};

/** @}*/

} // namespace mbed

#endif

#endif
