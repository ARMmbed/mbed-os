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
#ifndef MBED_BUSIN_H
#define MBED_BUSIN_H

#include "platform/platform.h"
#include "drivers/DigitalIn.h"
#include "platform/PlatformMutex.h"
#include "platform/NonCopyable.h"

namespace mbed {
/** \addtogroup drivers */

/** A digital input bus, used for reading the state of a collection of pins
 *
 * @note Synchronization level: Thread safe
 * @ingroup drivers
 */
class BusIn : private NonCopyable<BusIn> {

public:
    /* Group: Configuration Methods */

    /** Create an BusIn, connected to the specified pins
     *
     * @param p0 DigitalIn pin to connect to bus bit
     * @param p1 DigitalIn pin to connect to bus bit
     * @param p2 DigitalIn pin to connect to bus bit
     * @param p3 DigitalIn pin to connect to bus bit
     * @param p4 DigitalIn pin to connect to bus bit
     * @param p5 DigitalIn pin to connect to bus bit
     * @param p6 DigitalIn pin to connect to bus bit
     * @param p7 DigitalIn pin to connect to bus bit
     * @param p8 DigitalIn pin to connect to bus bit
     * @param p9 DigitalIn pin to connect to bus bit
     * @param p10 DigitalIn pin to connect to bus bit
     * @param p11 DigitalIn pin to connect to bus bit
     * @param p12 DigitalIn pin to connect to bus bit
     * @param p13 DigitalIn pin to connect to bus bit
     * @param p14 DigitalIn pin to connect to bus bit
     * @param p15 DigitalIn pin to connect to bus bit
     *
     * @note
     *  It is only required to specify as many pin variables as is required
     *  for the bus; the rest will default to NC (not connected)
     */
    BusIn(PinName p0, PinName p1 = NC, PinName p2 = NC, PinName p3 = NC,
          PinName p4 = NC, PinName p5 = NC, PinName p6 = NC, PinName p7 = NC,
          PinName p8 = NC, PinName p9 = NC, PinName p10 = NC, PinName p11 = NC,
          PinName p12 = NC, PinName p13 = NC, PinName p14 = NC, PinName p15 = NC);


    /** Create an BusIn, connected to the specified pins
     *
     * @param pins An array of pins to connect to bus bit
     */
    BusIn(PinName pins[16]);

    virtual ~BusIn();

    /** Read the value of the input bus
     *
     *  @returns
     *   An integer with each bit corresponding to the value read from the associated DigitalIn pin
     */
    int read();

    /** Set the input pin mode
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

    /** A shorthand for read()
     *  \sa DigitalIn::read()
     */
    operator int();

    /** Access to particular bit in random-iterator fashion
     * @param index  Position of bit
     */
    DigitalIn &operator[](int index);

#if !defined(DOXYGEN_ONLY)
protected:
    DigitalIn *_pin[16];

    /* Mask of bus's NC pins
     * If bit[n] is set to 1 - pin is connected
     * if bit[n] is cleared - pin is not connected (NC)
     */
    int _nc_mask;

    PlatformMutex _mutex;

private:
    virtual void lock();
    virtual void unlock();
#endif
};

} // namespace mbed

#endif

