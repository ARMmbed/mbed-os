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
#ifndef MBED_DIGITALINOUT_H
#define MBED_DIGITALINOUT_H

#include "platform/platform.h"

#include "hal/gpio_api.h"

namespace mbed {
/**
 * \defgroup drivers_DigitalInOut DigitalInOut class
 * \ingroup drivers-public-api-gpio
 * @{
 */

/** A digital input/output, used for setting or reading a bi-directional pin
 *
 * @note Synchronization level: Interrupt safe
 */
class DigitalInOut {

public:
    /** Create a DigitalInOut connected to the specified pin
     *
     *  @param pin DigitalInOut pin to connect to
     */
    DigitalInOut(PinName pin) : gpio()
    {
        // No lock needed in the constructor
        gpio_init_in(&gpio, pin);
    }

    /** Create a DigitalInOut connected to the specified pin
     *
     *  @param pin DigitalInOut pin to connect to
     *  @param direction the initial direction of the pin
     *  @param mode the initial mode of the pin
     *  @param value the initial value of the pin if is an output
     */
    DigitalInOut(PinName pin, PinDirection direction, PinMode mode, int value) : gpio()
    {
        // No lock needed in the constructor
        gpio_init_inout(&gpio, pin, direction, mode, value);
    }

    /** Set the output, specified as 0 or 1 (int)
     *
     *  @param value An integer specifying the pin output value,
     *      0 for logical 0, 1 (or any other non-zero value) for logical 1
     */
    void write(int value)
    {
        // Thread safe / atomic HAL call
        gpio_write(&gpio, value);
    }

    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    an integer representing the output setting of the pin if it is an output,
     *    or read the input if set as an input
     */
    int read()
    {
        // Thread safe / atomic HAL call
        return gpio_read(&gpio);
    }

    /** Set as an output
     */
    void output();

    /** Set as an input
     */
    void input();

    /** Set the input pin mode
     *
     *  @param pull PullUp, PullDown, PullNone, OpenDrain
     */
    void mode(PinMode pull);

    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    Non zero value if pin is connected to uc GPIO
     *    0 if gpio object was initialized with NC
     */
    int is_connected()
    {
        // Thread safe / atomic HAL call
        return gpio_is_connected(&gpio);
    }

    /** A shorthand for write()
     * \sa DigitalInOut::write()
     * @code
     *      DigitalInOut  inout(PIN);
     *      DigitalIn     button(BUTTON1);
     *      inout.output();
     *
     *      inout = button;     // Equivalent to inout.write(button.read())
     * @endcode
     */
    DigitalInOut &operator= (int value)
    {
        // Underlying write is thread safe
        write(value);
        return *this;
    }

    /**A shorthand for write() using the assignment operator which copies the
     * state from the DigitalInOut argument.
     * \sa DigitalInOut::write()
     */
    DigitalInOut &operator= (DigitalInOut &rhs);

    /** A shorthand for read()
     * \sa DigitalInOut::read()
     * @code
     *      DigitalInOut inout(PIN);
     *      DigitalOut led(LED1);
     *
     *      inout.input();
     *      led = inout;   // Equivalent to led.write(inout.read())
     * @endcode
     */
    operator int()
    {
        // Underlying call is thread safe
        return read();
    }

protected:
#if !defined(DOXYGEN_ONLY)
    gpio_t gpio;
#endif //!defined(DOXYGEN_ONLY)
};

/** @}*/

} // namespace mbed

#endif
