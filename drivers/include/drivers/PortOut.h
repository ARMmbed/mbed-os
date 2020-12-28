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
#ifndef MBED_PORTOUT_H
#define MBED_PORTOUT_H

#include "platform/platform.h"

#if DEVICE_PORTOUT || defined(DOXYGEN_ONLY)

#include "hal/port_api.h"

namespace mbed {
/**
 * \defgroup drivers_PortOut PortOut class
 * \ingroup drivers-public-api-gpio
 * @{
 */

/** A multiple pin digital output
 *
 * @note Synchronization level: Interrupt safe
 *
 * Example:
 * @code
 * // Toggle all four LEDs
 *
 * #include "mbed.h"
 *
 * // LED1 = P1.18  LED2 = P1.20  LED3 = P1.21  LED4 = P1.23
 * #define LED_MASK 0x00B40000
 *
 * PortOut ledport(Port1, LED_MASK);
 *
 * int main() {
 *     while(1) {
 *         ledport = LED_MASK;
 *         ThisThread::sleep_for(1000);
 *         ledport = 0;
 *         ThisThread::sleep_for(1000);
 *     }
 * }
 * @endcode
 */
class PortOut {
public:

    /** Create a PortOut, connected to the specified port
     *
     *  @param port Port to connect to (as defined in target's PortNames.h)
     *  @param mask Bitmask defines which port pins are an output (0 - ignore, 1 - include)
     */
    PortOut(PortName port, int mask = 0xFFFFFFFF);

    /** Write the value to the output port
     *
     *  @param value An integer specifying a bit to write for every corresponding PortOut pin
     */
    void write(int value)
    {
        port_write(&_port, value);
    }

    /** Read the value currently output on the port
     *
     *  @returns
     *    An integer with each bit corresponding to associated pin value
     */
    int read()
    {
        return port_read(&_port);
    }

    /** A shorthand for write()
     * \sa PortOut::write()
     */
    PortOut &operator= (int value)
    {
        write(value);
        return *this;
    }

    /** A shorthand for read()
     * \sa PortOut::read()
     */
    PortOut &operator= (PortOut &rhs)
    {
        write(rhs.read());
        return *this;
    }

    /** A shorthand for read()
     * \sa PortOut::read()
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
