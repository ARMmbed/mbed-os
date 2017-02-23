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
#ifndef MBED_DIGITALINOUT_H
#define MBED_DIGITALINOUT_H

#include "platform/platform.h"

#include "hal/gpio_api.h"
#include "platform/mbed_critical.h"

namespace mbed {
/** \addtogroup drivers */
/** @{*/

/** A digital input/output, used for setting or reading a bi-directional pin
 *
 * @Note Synchronization level: Interrupt safe
 */
class DigitalInOut {

public:
    /** Create a DigitalInOut connected to the specified pin
     *
     *  @param pin DigitalInOut pin to connect to
     */
    DigitalInOut(PinName pin) : gpio() {
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
    DigitalInOut(PinName pin, PinDirection direction, PinMode mode, int value) : gpio() {
        // No lock needed in the constructor
        gpio_init_inout(&gpio, pin, direction, mode, value);
    }

    /** Set the output, specified as 0 or 1 (int)
     *
     *  @param value An integer specifying the pin output value,
     *      0 for logical 0, 1 (or any other non-zero value) for logical 1
     */
    void write(int value) {
        // Thread safe / atomic HAL call
        gpio_write(&gpio, value);
    }

    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    an integer representing the output setting of the pin if it is an output,
     *    or read the input if set as an input
     */
    int read() {
        // Thread safe / atomic HAL call
        return gpio_read(&gpio);
    }

    /** Set as an output
     */
    void output() {
        core_util_critical_section_enter();
        gpio_dir(&gpio, PIN_OUTPUT);
        core_util_critical_section_exit();
    }

    /** Set as an input
     */
    void input() {
        core_util_critical_section_enter();
        gpio_dir(&gpio, PIN_INPUT);
        core_util_critical_section_exit();
    }

    /** Set the input pin mode
     *
     *  @param mode PullUp, PullDown, PullNone, OpenDrain
     */
    void mode(PinMode pull) {
        core_util_critical_section_enter();
        gpio_mode(&gpio, pull);
        core_util_critical_section_exit();
    }

    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    Non zero value if pin is connected to uc GPIO
     *    0 if gpio object was initialized with NC
     */
    int is_connected() {
        // Thread safe / atomic HAL call
        return gpio_is_connected(&gpio);
    }

    /** A shorthand for write()
     */
    DigitalInOut& operator= (int value) {
        // Underlying write is thread safe
        write(value);
        return *this;
    }

    DigitalInOut& operator= (DigitalInOut& rhs) {
        core_util_critical_section_enter();
        write(rhs.read());
        core_util_critical_section_exit();
        return *this;
    }

    /** A shorthand for read()
     */
    operator int() {
        // Underlying call is thread safe
        return read();
    }

protected:
    gpio_t gpio;
};

} // namespace mbed

#endif

/** @}*/
