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
#ifndef MBED_INTERRUPTIN_H
#define MBED_INTERRUPTIN_H

#include "platform/platform.h"

#if DEVICE_INTERRUPTIN || defined(DOXYGEN_ONLY)

#include "hal/gpio_api.h"
#include "hal/gpio_irq_api.h"
#include "platform/Callback.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_toolchain.h"
#include "platform/NonCopyable.h"

namespace mbed {
/**
 * \defgroup drivers_InterruptIn InterruptIn class
 * \ingroup drivers-public-api-gpio
 * @{
 */

/** A digital interrupt input, used to call a function on a rising or falling edge
 *
 * @note Synchronization level: Interrupt safe
 *
 * Example:
 * @code
 * // Flash an LED while waiting for events
 *
 * #include "mbed.h"
 *
 * InterruptIn event(p16);
 * DigitalOut led(LED1);
 *
 * void trigger() {
 *     printf("triggered!\n");
 * }
 *
 * int main() {
 *     // register trigger() to be called upon the rising edge of event
 *     event.rise(&trigger);
 *     while(1) {
 *         led = !led;
 *         ThisThread::sleep_for(250);
 *     }
 * }
 * @endcode
 */
class InterruptIn : private NonCopyable<InterruptIn> {

public:

    /** Create an InterruptIn connected to the specified pin
     *
     *  @param pin InterruptIn pin to connect to
     */
    InterruptIn(PinName pin);

    /** Create an InterruptIn connected to the specified pin,
     *  and the pin configured to the specified mode.
     *
     *  @param pin InterruptIn pin to connect to
     *  @param mode Desired Pin mode configuration.
     *  (Valid values could be PullNone, PullDown, PullUp and PullDefault.
     *  See PinNames.h for your target for definitions)
     *
     */
    InterruptIn(PinName pin, PinMode mode);

    virtual ~InterruptIn();

    /** Read the input, represented as 0 or 1 (int)
     *
     *  @returns
     *    An integer representing the state of the input pin,
     *    0 for logical 0, 1 for logical 1
     */
    int read();

    /** An operator shorthand for read()
     */
    operator int();


    /** Attach a function to call when a rising edge occurs on the input
     *
     *  @param func A pointer to a void function, or 0 to set as none
     */
    void rise(Callback<void()> func);

    /** Attach a function to call when a falling edge occurs on the input
     *
     *  @param func A pointer to a void function, or 0 to set as none
     */
    void fall(Callback<void()> func);

    /** Set the input pin mode
     *
     *  @param pull PullUp, PullDown, PullNone, PullDefault
     *  See PinNames.h for your target for definitions)
     */
    void mode(PinMode pull);

    /** Enable IRQ. This method depends on hardware implementation, might enable one
     *  port interrupts. For further information, check gpio_irq_enable().
     */
    void enable_irq();

    /** Disable IRQ. This method depends on hardware implementation, might disable one
     *  port interrupts. For further information, check gpio_irq_disable().
     */
    void disable_irq();

    static void _irq_handler(uint32_t id, gpio_irq_event event);
#if !defined(DOXYGEN_ONLY)
protected:
    gpio_t gpio;
    gpio_irq_t gpio_irq;

    Callback<void()> _rise;
    Callback<void()> _fall;

    void irq_init(PinName pin);
#endif
};

/** @}*/

} // namespace mbed

#endif

#endif
