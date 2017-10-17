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
#ifndef MBED_PWMOUT_H
#define MBED_PWMOUT_H

#include "platform/platform.h"

#if defined (DEVICE_PWMOUT) || defined(DOXYGEN_ONLY)
#include "hal/pwmout_api.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_sleep.h"

namespace mbed {
/** \addtogroup drivers */

/** A pulse-width modulation digital output
 *
 * @note Synchronization level: Interrupt safe
 *
 * Example
 * @code
 * // Fade a led on.
 * #include "mbed.h"
 *
 * PwmOut led(LED1);
 *
 * int main() {
 *     while(1) {
 *         led = led + 0.01;
 *         wait(0.2);
 *         if(led == 1.0) {
 *             led = 0;
 *         }
 *     }
 * }
 * @endcode
 * @ingroup drivers
 */
class PwmOut {

public:

    /** Create a PwmOut connected to the specified pin
     *
     *  @param pin PwmOut pin to connect to
     */
    PwmOut(PinName pin) : _deep_sleep_locked(false) {
        core_util_critical_section_enter();
        pwmout_init(&_pwm, pin);
        core_util_critical_section_exit();
    }

    ~PwmOut() {
        core_util_critical_section_enter();
        unlock_deep_sleep();
        core_util_critical_section_exit();
    }

    /** Set the ouput duty-cycle, specified as a percentage (float)
     *
     *  @param value A floating-point value representing the output duty-cycle,
     *    specified as a percentage. The value should lie between
     *    0.0f (representing on 0%) and 1.0f (representing on 100%).
     *    Values outside this range will be saturated to 0.0f or 1.0f.
     */
    void write(float value) {
        core_util_critical_section_enter();
        lock_deep_sleep();
        pwmout_write(&_pwm, value);
        core_util_critical_section_exit();
    }

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
    float read() {
        core_util_critical_section_enter();
        float val = pwmout_read(&_pwm);
        core_util_critical_section_exit();
        return val;
    }

    /** Set the PWM period, specified in seconds (float), keeping the duty cycle the same.
     *
     *  @param seconds Change the period of a PWM signal in seconds (float) without modifying the duty cycle
     *  @note
     *   The resolution is currently in microseconds; periods smaller than this
     *   will be set to zero.
     */
    void period(float seconds) {
        core_util_critical_section_enter();
        pwmout_period(&_pwm, seconds);
        core_util_critical_section_exit();
    }

    /** Set the PWM period, specified in milli-seconds (int), keeping the duty cycle the same.
     *  @param ms Change the period of a PWM signal in milli-seconds without modifying the duty cycle
     */
    void period_ms(int ms) {
        core_util_critical_section_enter();
        pwmout_period_ms(&_pwm, ms);
        core_util_critical_section_exit();
    }

    /** Set the PWM period, specified in micro-seconds (int), keeping the duty cycle the same.
     *  @param us Change the period of a PWM signal in micro-seconds without modifying the duty cycle
     */
    void period_us(int us) {
        core_util_critical_section_enter();
        pwmout_period_us(&_pwm, us);
        core_util_critical_section_exit();
    }

    /** Set the PWM pulsewidth, specified in seconds (float), keeping the period the same.
     *  @param seconds Change the pulse width of a PWM signal specified in seconds (float)
     */
    void pulsewidth(float seconds) {
        core_util_critical_section_enter();
        pwmout_pulsewidth(&_pwm, seconds);
        core_util_critical_section_exit();
    }

    /** Set the PWM pulsewidth, specified in milli-seconds (int), keeping the period the same.
     *  @param ms Change the pulse width of a PWM signal specified in milli-seconds
     */
    void pulsewidth_ms(int ms) {
        core_util_critical_section_enter();
        pwmout_pulsewidth_ms(&_pwm, ms);
        core_util_critical_section_exit();
    }

    /** Set the PWM pulsewidth, specified in micro-seconds (int), keeping the period the same.
     *  @param us Change the pulse width of a PWM signal specified in micro-seconds  
     */
    void pulsewidth_us(int us) {
        core_util_critical_section_enter();
        pwmout_pulsewidth_us(&_pwm, us);
        core_util_critical_section_exit();
    }

    /** A operator shorthand for write()
     *  \sa PwmOut::write()
     */
    PwmOut& operator= (float value) {
        // Underlying call is thread safe
        write(value);
        return *this;
    }

    /** A operator shorthand for write()
     * \sa PwmOut::write()
     */    
    PwmOut& operator= (PwmOut& rhs) {
        // Underlying call is thread safe
        write(rhs.read());
        return *this;
    }

    /** An operator shorthand for read()
     * \sa PwmOut::read()
     */
    operator float() {
        // Underlying call is thread safe
        return read();
    }

protected:
    /** Lock deep sleep only if it is not yet locked */
    void lock_deep_sleep() {
        if (_deep_sleep_locked == false) {
            sleep_manager_lock_deep_sleep();
            _deep_sleep_locked = true;
        }
    }

    /** Unlock deep sleep in case it is locked */
    void unlock_deep_sleep() {
        if (_deep_sleep_locked == true) {
            sleep_manager_unlock_deep_sleep();
            _deep_sleep_locked = false;
        }
    }

    pwmout_t _pwm;
    bool _deep_sleep_locked;
};

} // namespace mbed

#endif

#endif
