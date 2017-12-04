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
#ifndef MBED_BUSOUT_H
#define MBED_BUSOUT_H

#include "drivers/DigitalOut.h"
#include "platform/PlatformMutex.h"
#include "platform/NonCopyable.h"

namespace mbed {
/** \addtogroup drivers */

/** A digital output bus, used for setting the state of a collection of pins
 * @ingroup drivers
 */
class BusOut : private NonCopyable<BusOut> {

public:

    /** Create an BusOut, connected to the specified pins
     *
     *  @param p0 DigitalOut pin to connect to bus bit
     *  @param p1 DigitalOut pin to connect to bus bit
     *  @param p2 DigitalOut pin to connect to bus bit
     *  @param p3 DigitalOut pin to connect to bus bit
     *  @param p4 DigitalOut pin to connect to bus bit
     *  @param p5 DigitalOut pin to connect to bus bit
     *  @param p6 DigitalOut pin to connect to bus bit
     *  @param p7 DigitalOut pin to connect to bus bit
     *  @param p8 DigitalOut pin to connect to bus bit
     *  @param p9 DigitalOut pin to connect to bus bit
     *  @param p10 DigitalOut pin to connect to bus bit
     *  @param p11 DigitalOut pin to connect to bus bit
     *  @param p12 DigitalOut pin to connect to bus bit
     *  @param p13 DigitalOut pin to connect to bus bit
     *  @param p14 DigitalOut pin to connect to bus bit
     *  @param p15 DigitalOut pin to connect to bus bit
     *
     *  @note Synchronization level: Thread safe
     *
     *  @note
     *  It is only required to specify as many pin variables as is required
     *  for the bus; the rest will default to NC (not connected)
     */
    BusOut(PinName p0, PinName p1 = NC, PinName p2 = NC, PinName p3 = NC,
           PinName p4 = NC, PinName p5 = NC, PinName p6 = NC, PinName p7 = NC,
           PinName p8 = NC, PinName p9 = NC, PinName p10 = NC, PinName p11 = NC,
           PinName p12 = NC, PinName p13 = NC, PinName p14 = NC, PinName p15 = NC);

    /** Create an BusOut, connected to the specified pins
     *
     *  @param pins An array of pins to connect to bus the bit
     */
    BusOut(PinName pins[16]);

    virtual ~BusOut();

    /** Write the value to the output bus
     *
     *  @param value An integer specifying a bit to write for every corresponding DigitalOut pin
     */
    void write(int value);

    /** Read the value currently output on the bus
     *
     *  @returns
     *    An integer with each bit corresponding to associated DigitalOut pin setting
     */
    int read();

    /** Binary mask of bus pins connected to actual pins (not NC pins)
     *  If bus pin is in NC state make corresponding bit will be cleared (set to 0), else bit will be set to 1
     *
     *  @returns
     *    Binary mask of connected pins
     */
    int mask() {
        // No lock needed since _nc_mask is not modified outside the constructor
        return _nc_mask;
    }

    /** A shorthand for write()
     * \sa BusOut::write()
     */
    BusOut& operator= (int v);
    BusOut& operator= (BusOut& rhs);

    /** Access to particular bit in random-iterator fashion
     * @param index  Bit Position
     */
    DigitalOut& operator[] (int index);

    /** A shorthand for read()
     * \sa BusOut::read()
     */
    operator int();

protected:
    virtual void lock();
    virtual void unlock();
    DigitalOut* _pin[16];

    /* Mask of bus's NC pins
     * If bit[n] is set to 1 - pin is connected
     * if bit[n] is cleared - pin is not connected (NC)
     */
    int _nc_mask;

    PlatformMutex _mutex;
};

} // namespace mbed

#endif
