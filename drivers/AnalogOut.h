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
#ifndef MBED_ANALOGOUT_H
#define MBED_ANALOGOUT_H

#include "platform/platform.h"

#if DEVICE_ANALOGOUT || defined(DOXYGEN_ONLY)

#include "hal/analogout_api.h"
#include "platform/PlatformMutex.h"

namespace mbed {
/**
 * \defgroup drivers_AnalogOut AnalogOut class
 * \ingroup drivers-public-api-gpio
 * @{
 */

/** An analog output, used for setting the voltage on a pin
 *
 * @note Synchronization level: Thread safe
 *
 * Example:
 * @code
 * // Make a sawtooth output
 *
 * #include "mbed.h"
 *
 * AnalogOut tri(p18);
 * int main() {
 *     while(1) {
 *         tri = tri + 0.01;
 *         wait_us(1);
 *         if(tri == 1) {
 *             tri = 0;
 *         }
 *     }
 * }
 * @endcode
 */
class AnalogOut {

public:

    /** Create an AnalogOut connected to the specified pin
     *
     * @param pin AnalogOut pin to connect to
     */
    AnalogOut(PinName pin)
    {
        analogout_init(&_dac, pin);
    }

    /** Create an AnalogOut connected to the specified pin
     *
     * @param pinmap reference to structure which holds static pinmap.
     */
    AnalogOut(const PinMap &&) = delete; // prevent passing of temporary objects
    AnalogOut(const PinMap &pinmap)
    {
        analogout_init_direct(&_dac, &pinmap);
    }

    /** Set the output voltage, specified as a percentage (float)
     *
     *  @param value A floating-point value representing the output voltage,
     *    specified as a percentage. The value should lie between
     *    0.0f (representing 0v / 0%) and 1.0f (representing 3.3v / 100%).
     *    Values outside this range will be saturated to 0.0f or 1.0f.
     */
    void write(float value);

    /** Set the output voltage, represented as an unsigned short in the range [0x0, 0xFFFF]
     *
     *  @param value 16-bit unsigned short representing the output voltage,
     *            normalized to a 16-bit value (0x0000 = 0v, 0xFFFF = 3.3v)
     */
    void write_u16(unsigned short value);

    /** Return the current output voltage setting, measured as a percentage (float)
     *
     *  @returns
     *    A floating-point value representing the current voltage being output on the pin,
     *    measured as a percentage. The returned value will lie between
     *    0.0f (representing 0v / 0%) and 1.0f (representing 3.3v / 100%).
     *
     *  @note
     *    This value may not match exactly the value set by a previous write().
     */
    float read();

    /** An operator shorthand for write()
     * \sa AnalogOut::write()
     */
    AnalogOut &operator= (float percent)
    {
        // Underlying write call is thread safe
        write(percent);
        return *this;
    }

    /** An operator shorthand for write()
     * \sa AnalogOut::write()
     */
    AnalogOut &operator= (AnalogOut &rhs)
    {
        // Underlying write call is thread safe
        write(rhs.read());
        return *this;
    }

    /** An operator shorthand for read()
     * \sa AnalogOut::read()
     */
    operator float()
    {
        // Underlying read call is thread safe
        return read();
    }

    virtual ~AnalogOut()
    {
        /** Deinitialize pin configuration.
         */
        analogout_free(&_dac);
    }

protected:
#if !defined(DOXYGEN_ONLY)
    virtual void lock()
    {
        _mutex.lock();
    }

    virtual void unlock()
    {
        _mutex.unlock();
    }

    dac_t _dac;
    PlatformMutex _mutex;
#endif //!defined(DOXYGEN_ONLY)
};

/** @}*/

} // namespace mbed

#endif

#endif
