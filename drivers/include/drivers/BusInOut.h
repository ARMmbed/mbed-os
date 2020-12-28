/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_BUSINOUT_H
#define MBED_BUSINOUT_H

#include "drivers/DigitalInOut.h"
#include "platform/PlatformMutex.h"
#include "platform/NonCopyable.h"

namespace mbed {
/**
 * \defgroup drivers_BusInOut BusInOut class
 * \ingroup drivers-public-api-gpio
 * @{
 */

/** A digital input output bus, used for setting the state of a collection of pins.
 *  Implemented as an array of DigitalInOut pins, the bus can be constructed by any
 *  pins without restriction other than being capable of digital input or output
 *  capabilities
 *
 * @note Synchronization level: Thread safe
 */
class BusInOut : private NonCopyable<BusInOut> {

public:

    /** Create a BusInOut, connected to the specified pins
     *
     *  @param p0 DigitalInOut pin to connect to bus bit
     *  @param p1 DigitalInOut pin to connect to bus bit
     *  @param p2 DigitalInOut pin to connect to bus bit
     *  @param p3 DigitalInOut pin to connect to bus bit
     *  @param p4 DigitalInOut pin to connect to bus bit
     *  @param p5 DigitalInOut pin to connect to bus bit
     *  @param p6 DigitalInOut pin to connect to bus bit
     *  @param p7 DigitalInOut pin to connect to bus bit
     *  @param p8 DigitalInOut pin to connect to bus bit
     *  @param p9 DigitalInOut pin to connect to bus bit
     *  @param p10 DigitalInOut pin to connect to bus bit
     *  @param p11 DigitalInOut pin to connect to bus bit
     *  @param p12 DigitalInOut pin to connect to bus bit
     *  @param p13 DigitalInOut pin to connect to bus bit
     *  @param p14 DigitalInOut pin to connect to bus bit
     *  @param p15 DigitalInOut pin to connect to bus bit
     *
     *  @note
     *  It is only required to specify as many pin variables as is required
     *  for the bus; the rest will default to NC (not connected)
     */
    BusInOut(PinName p0, PinName p1 = NC, PinName p2 = NC, PinName p3 = NC,
             PinName p4 = NC, PinName p5 = NC, PinName p6 = NC, PinName p7 = NC,
             PinName p8 = NC, PinName p9 = NC, PinName p10 = NC, PinName p11 = NC,
             PinName p12 = NC, PinName p13 = NC, PinName p14 = NC, PinName p15 = NC);

    /** Create a BusInOut, connected to the specified pins
     *
     *  @param pins An array of pins (PinName) to construct a BusInOut from. The maximum
     *  number of pins in the array is 16 and any pins that are unspecified or are not to be
     *  connected must be specified as NC in the array that is passed in
     */
    BusInOut(PinName pins[16]);

    virtual ~BusInOut();

    /* Group: Access Methods */

    /** Write the value to the output bus
     *
     *  @param value An integer specifying a bit to write for every corresponding DigitalInOut pin
     */
    void write(int value);

    /** Read the value currently output on the bus
     *
     *  @returns
     *    An integer with each bit corresponding to associated DigitalInOut pin setting
     */
    int read();

    /** Set all the pins in bus as output
     */
    void output();

    /** Set all the pins in bus as an input
     */
    void input();

    /** Set the input pin mode for all the pins in bus
     *
     *  @param pull PullUp, PullDown, PullNone
     */
    void mode(PinMode pull);

    /** Binary mask of bus pins connected to actual pins (not NC pins)
     *  If bus pin is in NC state make corresponding bit will be cleared (set to 0), else bit will be set to 1
     *
     *  @returns
     *    Binary mask of connected pins
     */
    int mask()
    {
        // No lock needed since _nc_mask is not modified outside the constructor
        return _nc_mask;
    }

    /** A shorthand for write()
     * \sa BusInOut::write()
     */
    BusInOut &operator= (int v);
    BusInOut &operator= (BusInOut &rhs);

    /** Access to particular bit in random-iterator fashion
     * @param index  Bit Position
     */
    DigitalInOut &operator[](int index);

    /** A shorthand for read()
     * \sa BusInOut::read()
     */
    operator int();

protected:
#if !defined(DOXYGEN_ONLY)
    virtual void lock();
    virtual void unlock();
    DigitalInOut *_pin[16];

    /* Mask of bus's NC pins
     * If bit[n] is set to 1 - pin is connected
     * if bit[n] is cleared - pin is not connected (NC)
     */
    int _nc_mask;

    PlatformMutex _mutex;
#endif //!defined(DOXYGEN_ONLY)
};

/** @}*/

} // namespace mbed

#endif
