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
#ifndef MBED_INTERRUPTIN_H
#define MBED_INTERRUPTIN_H

#include "platform.h"

#if DEVICE_INTERRUPTIN

#include "gpio_api.h"
#include "gpio_irq_api.h"

#include "FunctionPointer.h"
#include "CallChain.h"

namespace mbed {

/** A digital interrupt input, used to call a function on a rising or falling edge
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
 *     event.rise(&trigger);
 *     while(1) {
 *         led = !led;
 *         wait(0.25);
 *     }
 * }
 * @endcode
 */
class InterruptIn {

public:

    /** Create an InterruptIn connected to the specified pin
     *
     *  @param pin InterruptIn pin to connect to
     *  @param name (optional) A string to identify the object
     */
    InterruptIn(PinName pin);
    virtual ~InterruptIn();

     int read();
#ifdef MBED_OPERATORS
    operator int();

#endif

    /** Attach a function to call when a rising edge occurs on the input
     *
     *  @param fptr A pointer to a void function, or 0 to set as none
     *
     *  @returns
     *  The function object created for 'fptr'
     */
    pFunctionPointer_t rise(void (*fptr)(void));

    /** Add a function to be called when a rising edge occurs at the end of the call chain
     *
     *  @param fptr the function to add
     *
     *  @returns
     *  The function object created for 'fptr'
     */
    pFunctionPointer_t rise_add(void (*fptr)(void)) {
        return rise_add_common(fptr);
    }

    /** Add a function to be called when a rising edge occurs at the beginning of the call chain
     *
     *  @param fptr the function to add
     *
     *  @returns
     *  The function object created for 'fptr'
     */
    pFunctionPointer_t rise_add_front(void (*fptr)(void)) {
        return rise_add_common(fptr, true);
    }

    /** Attach a member function to call when a rising edge occurs on the input
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointer_t rise(T* tptr, void (T::*mptr)(void)) {
        _rise.clear();
        pFunctionPointer_t pf = _rise.add(tptr, mptr);
        gpio_irq_set(&gpio_irq, IRQ_RISE, 1);
        return pf;
    }

    /** Add a function to be called when a rising edge occurs at the end of the call chain
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointer_t rise_add(T* tptr, void (T::*mptr)(void)) {
        return rise_add_common(tptr, mptr);
    }

    /** Add a function to be called when a rising edge occurs at the beginning of the call chain
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointer_t rise_add_front(T* tptr, void (T::*mptr)(void)) {
        return rise_add_common(tptr, mptr, true);
    }

    /** Remove a function from the list of functions to be called when a rising edge occurs
     *
     *  @param pf the function object to remove
     *
     *  @returns
     *  true if the function was found and removed, false otherwise
     */
    bool rise_remove(pFunctionPointer_t pf);

    /** Attach a function to call when a falling edge occurs on the input
     *
     *  @param fptr A pointer to a void function, or 0 to set as none
     *
     *  @returns
     *  The function object created for 'fptr'
     */
    pFunctionPointer_t fall(void (*fptr)(void));

     /** Add a function to be called when a falling edge occurs at the end of the call chain
     *
     *  @param fptr the function to add
     *
     *  @returns
     *  The function object created for 'fptr'
     */   
    pFunctionPointer_t fall_add(void (*fptr)(void)) {
        return fall_add_common(fptr);
    }

    /** Add a function to be called when a falling edge occurs at the beginning of the call chain
     *
     *  @param fptr the function to add
     *
     *  @returns
     *  The function object created for 'fptr'
     */
    pFunctionPointer_t fall_add_front(void (*fptr)(void)) {
        return fall_add_common(fptr, true);
    }

    /** Attach a member function to call when a falling edge occurs on the input
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointer_t fall(T* tptr, void (T::*mptr)(void)) {
        _fall.clear();
        pFunctionPointer_t pf = _fall.add(tptr, mptr);
        gpio_irq_set(&gpio_irq, IRQ_FALL, 1);
        return pf;
    }

    /** Add a function to be called when a falling edge occurs at the end of the call chain
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointer_t fall_add(T* tptr, void (T::*mptr)(void)) {
        return fall_add_common(tptr, mptr);
    }

    /** Add a function to be called when a falling edge occurs at the beginning of the call chain
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointer_t fall_add_front(T* tptr, void (T::*mptr)(void)) {
        return fall_add_common(tptr, mptr, true);
    }

    /** Remove a function from the list of functions to be called when a falling edge occurs
     *
     *  @param pf the function object to remove
     *
     *  @returns
     *  true if the function was found and removed, false otherwise
     */
    bool fall_remove(pFunctionPointer_t pf);

    /** Set the input pin mode
     *
     *  @param mode PullUp, PullDown, PullNone
     */
    void mode(PinMode pull);

    static void _irq_handler(uint32_t id, gpio_irq_event event);

protected:
    pFunctionPointer_t rise_add_common(void (*fptr)(void), bool front=false);
    pFunctionPointer_t fall_add_common(void (*fptr)(void), bool front=false);

    template<typename T>
    pFunctionPointer_t rise_add_common(T* tptr, void (T::*mptr)(void), bool front=false) {
        pFunctionPointer_t pf = front ? _rise.add_front(tptr, mptr) : _rise.add(tptr, mptr);
        gpio_irq_set(&gpio_irq, IRQ_RISE, 1);
        return pf;
    }
    template<typename T>
    pFunctionPointer_t fall_add_common(T* tptr, void (T::*mptr)(void), bool front=false) {
        pFunctionPointer_t pf = front ? _fall.add_front(tptr, mptr) : _fall.add(tptr, mptr);
        gpio_irq_set(&gpio_irq, IRQ_FALL, 1);
        return pf;
    }

    gpio_t gpio;
    gpio_irq_t gpio_irq;

    CallChain _rise;
    CallChain _fall;
};

} // namespace mbed

#endif

#endif
