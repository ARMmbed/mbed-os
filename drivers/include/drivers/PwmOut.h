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
#ifndef MBED_PWMOUT_H
#define MBED_PWMOUT_H

#include "platform/platform.h"

#if DEVICE_PWMOUT || defined(DOXYGEN_ONLY)
#include "hal/pwmout_api.h"

namespace mbed {
/**
 * \defgroup drivers_PwmOut PwmOut class
 * \ingroup drivers-public-api-gpio
 * @{
 */

/** A pulse-width modulation digital output
 *
 * @note Synchronization level: Interrupt safe
 *
 * Example
 * @code
 * // Gradually change the intensity of the LED.
 * #include "mbed.h"
 *
 * PwmOut led(LED1);
 *
 * int main() {
 *     while(1) {
 *         led = led + 0.01;
 *         ThisThread::sleep_for(200);
 *         if(led == 1.0) {
 *             led = 0;
 *         }
 *     }
 * }
 * @endcode
 */
class PwmOut {

public:

    /** Create a PwmOut connected to the specified pin
     *
     *  @param pin PwmOut pin to connect to
     */
    PwmOut(PinName pin);

    /** Create a PwmOut connected to the specified pin
     *
     *  @param pinmap reference to structure which holds static pinmap.
     */
    PwmOut(const PinMap &pinmap);
    PwmOut(const PinMap &&) = delete; // prevent passing of temporary objects

    ~PwmOut();

    /** Set the output duty-cycle, specified as a percentage (float)
     *
     *  @param value A floating-point value representing the output duty-cycle,
     *    specified as a percentage. The value should lie between
     *    0.0f (representing on 0%) and 1.0f (representing on 100%).
     *    Values outside this range will be saturated to 0.0f or 1.0f.
     */
    void write(float value);

    /** Return the current output duty-cycle setting, measured as a percentage (float)
     *
     *  @returns
     *    A floating-point value representing the current duty-cycle being output on the pin,
     *    measured as a percentage. The returned value will lie between
     *    0.0f (representing on 0%) and 1.0f (representing on 100%).
     *
     *  @note
     *  This value may not match exactly the value set by a previous write().
     */
    float read();

    /** Set the PWM period, specified in seconds (float), keeping the duty cycle the same.
     *
     *  @param seconds Change the period of a PWM signal in seconds (float) without modifying the duty cycle
     *  @note
     *   The resolution is currently in microseconds; periods smaller than this
     *   will be set to zero.
     */
    void period(float seconds);

    /** Set the PWM period, specified in milliseconds (int), keeping the duty cycle the same.
     *  @param ms Change the period of a PWM signal in milliseconds without modifying the duty cycle
     */
    void period_ms(int ms);

    /** Set the PWM period, specified in microseconds (int), keeping the duty cycle the same.
     *  @param us Change the period of a PWM signal in microseconds without modifying the duty cycle
     */
    void period_us(int us);

    /** Read the PWM period
     *  @returns
     *   The PWM period, specified in microseconds (int)
     */
    int read_period_us();

    /** Set the PWM pulsewidth, specified in seconds (float), keeping the period the same.
     *  @param seconds Change the pulse width of a PWM signal specified in seconds (float)
     */
    void pulsewidth(float seconds);

    /** Set the PWM pulsewidth, specified in milliseconds (int), keeping the period the same.
     *  @param ms Change the pulse width of a PWM signal specified in milliseconds
     */
    void pulsewidth_ms(int ms);

    /** Set the PWM pulsewidth, specified in microseconds (int), keeping the period the same.
     *  @param us Change the pulse width of a PWM signal specified in microseconds
     */
    void pulsewidth_us(int us);

    /** Read the PWM pulsewidth
     *  @returns
     *   The PWM pulsewith, specified in microseconds (int)
     */
    int read_pulsewitdth_us();

    /** Suspend PWM operation
     *
     * Control the PWM state. This is primarily intended
     * for temporary power-saving; This call can
     * allow pwm to be temporarily disabled to permit power saving without
     * losing device state. The subsequent function call must be PwmOut::resume
     * for PWM to resume; any other calls prior to resuming are undefined behavior.
     */
    void suspend();

    /** Resume PWM operation
     *
     * Control the PWM state. This is primarily intended
     * to resume PWM operations after a previous PwmOut::suspend call;
     * This call restores the device state prior to suspension.
     */
    void resume();

    /** A operator shorthand for write()
     *  \sa PwmOut::write()
     */
    PwmOut &operator= (float value)
    {
        // Underlying call is thread safe
        write(value);
        return *this;
    }

    /** A operator shorthand for write()
     * \sa PwmOut::write()
     */
    PwmOut &operator= (PwmOut &rhs)
    {
        // Underlying call is thread safe
        write(rhs.read());
        return *this;
    }

    /** An operator shorthand for read()
     * \sa PwmOut::read()
     */
    operator float()
    {
        // Underlying call is thread safe
        return read();
    }

#if !(DOXYGEN_ONLY)
protected:
    /** Lock deep sleep only if it is not yet locked */
    void lock_deep_sleep();

    /** Unlock deep sleep in case it is locked */
    void unlock_deep_sleep();

    /** Initialize this instance */
    void init();

    /** Power down this instance */
    void deinit();

    pwmout_t _pwm;
    PinName _pin;
    bool _deep_sleep_locked;
    bool _initialized;
    float _duty_cycle;
    int _period_us;
#endif
};

/** @}*/

} // namespace mbed

#endif

#endif
