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
#ifndef MBED_INTERRUPTMANAGER_H
#define MBED_INTERRUPTMANAGER_H

#include "cmsis.h"
#include "platform/CallChain.h"
#include "platform/PlatformMutex.h"
#include <string.h>

namespace mbed {
/** \addtogroup drivers */

/** Use this singleton if you need to chain interrupt handlers.
 *
 * @note Synchronization level: Thread safe
 *
 * Example (for LPC1768):
 * @code
 * #include "InterruptManager.h"
 * #include "mbed.h"
 *
 * Ticker flipper;
 * DigitalOut led1(LED1);
 * DigitalOut led2(LED2);
 *
 * void flip(void) {
 *     led1 = !led1;
 * }
 *
 * void handler(void) {
 *     led2 = !led1;
 * }
 *
 * int main() {
 *     led1 = led2 = 0;
 *     flipper.attach(&flip, 1.0);
 *     InterruptManager::get()->add_handler(handler, TIMER3_IRQn);
 * }
 * @endcode
 * @ingroup drivers
 */
class InterruptManager {
public:
    /** Get the instance of InterruptManager Class
     *
     *  @return the only instance of this class
     */
    static InterruptManager* get();

    /** Destroy the current instance of the interrupt manager
     */
    static void destroy();

    /** Add a handler for an interrupt at the end of the handler list
     *
     *  @param function the handler to add
     *  @param irq interrupt number
     *
     *  @returns
     *  The function object created for 'function'
     */
    pFunctionPointer_t add_handler(void (*function)(void), IRQn_Type irq) {
        // Underlying call is thread safe
        return add_common(function, irq);
    }

    /** Add a handler for an interrupt at the beginning of the handler list
     *
     *  @param function the handler to add
     *  @param irq interrupt number
     *
     *  @returns
     *  The function object created for 'function'
     */
    pFunctionPointer_t add_handler_front(void (*function)(void), IRQn_Type irq) {
        // Underlying call is thread safe
        return add_common(function, irq, true);
    }

    /** Add a handler for an interrupt at the end of the handler list
     *
     *  @param tptr pointer to the object that has the handler function
     *  @param mptr pointer to the actual handler function
     *  @param irq interrupt number
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointer_t add_handler(T* tptr, void (T::*mptr)(void), IRQn_Type irq) {
        // Underlying call is thread safe
        return add_common(tptr, mptr, irq);
    }

    /** Add a handler for an interrupt at the beginning of the handler list
     *
     *  @param tptr pointer to the object that has the handler function
     *  @param mptr pointer to the actual handler function
     *  @param irq interrupt number
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointer_t add_handler_front(T* tptr, void (T::*mptr)(void), IRQn_Type irq) {
        // Underlying call is thread safe
        return add_common(tptr, mptr, irq, true);
    }

    /** Remove a handler from an interrupt
     *
     *  @param handler the function object for the handler to remove
     *  @param irq the interrupt number
     *
     *  @returns
     *  true if the handler was found and removed, false otherwise
     */
    bool remove_handler(pFunctionPointer_t handler, IRQn_Type irq);

private:
    InterruptManager();
    ~InterruptManager();

    void lock();
    void unlock();

    // We declare the copy contructor and the assignment operator, but we don't
    // implement them. This way, if someone tries to copy/assign our instance,
    // he will get an error at compile time.
    InterruptManager(const InterruptManager&);
    InterruptManager& operator =(const InterruptManager&);

    template<typename T>
    pFunctionPointer_t add_common(T *tptr, void (T::*mptr)(void), IRQn_Type irq, bool front=false) {
        _mutex.lock();
        int irq_pos = get_irq_index(irq);
        bool change = must_replace_vector(irq);

        pFunctionPointer_t pf = front ? _chains[irq_pos]->add_front(tptr, mptr) : _chains[irq_pos]->add(tptr, mptr);
        if (change)
            NVIC_SetVector(irq, (uint32_t)&InterruptManager::static_irq_helper);
        _mutex.unlock();
        return pf;
    }

    pFunctionPointer_t add_common(void (*function)(void), IRQn_Type irq, bool front=false);
    bool must_replace_vector(IRQn_Type irq);
    int get_irq_index(IRQn_Type irq);
    void irq_helper();
    void add_helper(void (*function)(void), IRQn_Type irq, bool front=false);
    static void static_irq_helper();

    CallChain* _chains[NVIC_NUM_VECTORS];
    static InterruptManager* _instance;
    PlatformMutex _mutex;
};

} // namespace mbed

#endif
