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
#ifndef MBED_PORTINOUT_H
#define MBED_PORTINOUT_H

#include "platform/platform.h"

#if DEVICE_PORTINOUT || defined(DOXYGEN_ONLY)

#include "hal/port_api.h"

namespace mbed {
/**
 * \defgroup drivers_PortInOut PortInOut class
 * \ingroup drivers-public-api-gpio
 * @{
 */

/** A multiple pin digital in/out used to set/read multiple bi-directional pins
 *
 * @note Synchronization level: Interrupt safe
 */
class PortInOut {
public:

    /** Create an PortInOut, connected to the specified port
     *
     *  @param port Port to connect to (Port0-Port5)
     *  @param mask A bitmask to identify which bits in the port should be included (0 - ignore)
     */
    PortInOut(PortName port, int mask = 0xFFFFFFFF);

    /** Write the value to the output port
     *
     *  @param value An integer specifying a bit to write for every corresponding port pin
     */
    void write(int value)
    {
        port_write(&_port, value);
    }

    /** Read the value currently output on the port
     *
     *  @returns
     *    An integer with each bit corresponding to associated port pin setting
     */
    int read()
    {
        return port_read(&_port);
    }

    /** Set as an output
     */
    void output();

    /** Set as an input
     */
    void input();

    /** Set the input pin mode
     *
     *  @param mode PullUp, PullDown, PullNone, OpenDrain
     */
    void mode(PinMode mode);

    /** A shorthand for write()
     * \sa PortInOut::write()
     */
    PortInOut &operator= (int value)
    {
        write(value);
        return *this;
    }

    /** A shorthand for write()
     * \sa PortInOut::write()
     */
    PortInOut &operator= (PortInOut &rhs)
    {
        write(rhs.read());
        return *this;
    }

    /** A shorthand for read()
     * \sa PortInOut::read()
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
