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
#ifndef MBED_TICKER_H
#define MBED_TICKER_H

#include "TimerEvent.h"
#include "FunctionPointer.h"
#include "CallChain.h"

namespace mbed {

/** A Ticker is used to call a function at a recurring interval
 *
 *  You can use as many seperate Ticker objects as you require.
 *
 * Example:
 * @code
 * // Toggle the blinking led after 5 seconds
 *
 * #include "mbed.h"
 *
 * Ticker timer;
 * DigitalOut led1(LED1);
 * DigitalOut led2(LED2);
 *
 * int flip = 0;
 *
 * void attime() {
 *     flip = !flip;
 * }
 *
 * int main() {
 *     timer.attach(&attime, 5);
 *     while(1) {
 *         if(flip == 0) {
 *             led1 = !led1;
 *         } else {
 *             led2 = !led2;
 *         }
 *         wait(0.2);
 *     }
 * }
 * @endcode
 */
class Ticker : public TimerEvent {

public:

    /** Attach a function to be called by the Ticker, specifiying the interval in seconds
     *
     *  @param fptr pointer to the function to be called
     *  @param t the time between calls in seconds
     *
     *  @returns
     *  The function object created for 'fptr'
     */
    pFunctionPointer_t attach(void (*fptr)(void), float t) {
        return attach_us(fptr, t * 1000000.0f);
    }

    /** Add a function to be called by the Ticker at the end of the call chain
     *
     *  @param fptr the function to add
     *
     *  @returns
     *  The function object created for 'fptr'
     */
    pFunctionPointer_t add_function(void (*fptr)(void)) {
        return add_function_helper(fptr);
    }

    /** Add a function to be called by the Ticker at the beginning of the call chain
     *
     *  @param fptr the function to add
     *
     *  @returns
     *  The function object created for 'fptr'
     */
    pFunctionPointer_t add_function_front(void (*fptr)(void)) {
        return add_function_helper(fptr, true);
    }

    /** Attach a member function to be called by the Ticker, specifiying the interval in seconds
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *  @param t the time between calls in seconds
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointer_t attach(T* tptr, void (T::*mptr)(void), float t) {
        return attach_us(tptr, mptr, t * 1000000.0f);
    }

    /** Add a function to be called by the Ticker at the end of the call chain
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointer_t add_function(T* tptr, void (T::*mptr)(void)) {
        return add_function_helper(tptr, mptr);
    }

    /** Add a function to be called by the Ticker at the beginning of the call chain
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointer_t add_function_front(T* tptr, void (T::*mptr)(void)) {
        return add_function_helper(tptr, mptr, true);
    }

    /** Attach a function to be called by the Ticker, specifiying the interval in micro-seconds
     *
     *  @param fptr pointer to the function to be called
     *  @param t the time between calls in micro-seconds
     *
     *  @returns
     *  The function object created for 'fptr'
     */
    pFunctionPointer_t attach_us(void (*fptr)(void), unsigned int t) {
        _chain.clear();
        pFunctionPointer_t pf = _chain.add(fptr);
        setup(t);
        return pf;
    }

    /** Attach a member function to be called by the Ticker, specifiying the interval in micro-seconds
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *  @param t the time between calls in micro-seconds
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointer_t attach_us(T* tptr, void (T::*mptr)(void), unsigned int t) {
        _chain.clear();
        pFunctionPointer_t pf = _chain.add(tptr, mptr);
        setup(t);
        return pf;
    }

    /** Detach the function
     */
    void detach();

    /** Remove a function from the Ticker's call chain
     *
     *  @param pf the function object to remove
     *
     *  @returns
     *  true if the function was found and removed, false otherwise
     */
    bool remove_function(pFunctionPointer_t pf) {
        bool res = _chain.remove(pf);
        if (res && _chain.size() == 0)
            detach();
        return res;
    }

protected:
    void setup(unsigned int t);
    pFunctionPointer_t add_function_helper(void (*fptr)(void), bool front=false);
    virtual void handler();

    template<typename T>
    pFunctionPointer_t add_function_helper(T* tptr, void (T::*mptr)(void), bool front=false) {
        if (_chain.size() == 0)
            return NULL;
        return front ? _chain.add_front(tptr, mptr) : _chain.add(tptr, mptr);
    }

    unsigned int _delay;
    CallChain _chain;
};

} // namespace mbed

#endif
