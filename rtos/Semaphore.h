/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include "rtos/mbed_rtos_types.h"
#include "rtos/mbed_rtos1_types.h"
#include "rtos/mbed_rtos_storage.h"
#include "platform/mbed_toolchain.h"
#include "platform/NonCopyable.h"

namespace rtos {
/** \addtogroup rtos-public-api */
/** @{*/

/**
 * \defgroup rtos_Semaphore Semaphore class
 * @{
 */

/** The Semaphore class is used to manage and protect access to a set of shared resources.
 *
 * @note
 * Memory considerations: The semaphore control structures will be created on current thread's stack, both for the mbed OS
 * and underlying RTOS objects (static or dynamic RTOS memory pools are not being used).
 */
class Semaphore : private mbed::NonCopyable<Semaphore> {
public:
    /** Create and Initialize a Semaphore object used for managing resources.
      @param count      number of available resources; maximum index value is (count-1). (default: 0).

      @note You cannot call this function from ISR context.
    */
    Semaphore(int32_t count = 0);

    /** Create and Initialize a Semaphore object used for managing resources.
      @param  count     number of available resources
      @param  max_count maximum number of available resources

      @note You cannot call this function from ISR context.
    */
    Semaphore(int32_t count, uint16_t max_count);

    /** Wait until a Semaphore resource becomes available.

      @deprecated Do not use this function. This function has been replaced with acquire(), try_acquire() and try_acquire_for() functions.

      @param   millisec  timeout value. (default: osWaitForever).
      @return  number of available tokens, before taking one; or -1 in case of incorrect parameters

      @note You may call this function from ISR context if the millisec parameter is set to 0.
    */
    MBED_DEPRECATED_SINCE("mbed-os-5.13", "Replaced with acquire, try_acquire() and try_acquire_for() functions")
    int32_t wait(uint32_t millisec = osWaitForever);

    /** Wait until a Semaphore resource becomes available.

      @deprecated Do not use this function. This function has been replaced with try_acquire_until().

      @param   millisec  absolute timeout time, referenced to Kernel::get_ms_count()
      @return  number of available tokens, before taking one; or -1 in case of incorrect parameters
      @note the underlying RTOS may have a limit to the maximum wait time
            due to internal 32-bit computations, but this is guaranteed to work if the
            wait is <= 0x7fffffff milliseconds (~24 days). If the limit is exceeded,
            the acquire attempt will time out earlier than specified.

      @note You cannot call this function from ISR context.
    */
    MBED_DEPRECATED_SINCE("mbed-os-5.13", "Replaced with try_acquire_until()")
    int32_t wait_until(uint64_t millisec);

    /** Wait until a Semaphore resource becomes available.

      @note You cannot call this function from ISR context.
    */
    void acquire();

    /** Try to acquire a Semaphore resource, and return immediately
      @return true if a resource was acquired, false otherwise.
      @note equivalent to try_acquire_for(0)

    @note You may call this function from ISR context.
    */
    bool try_acquire();

    /** Wait until a Semaphore resource becomes available.
      @param   millisec  timeout value.
      @return true if a resource was acquired, false otherwise.

      @note You may call this function from ISR context if the millisec parameter is set to 0.
    */
    bool try_acquire_for(uint32_t millisec);

    /** Wait until a Semaphore resource becomes available.
      @param   millisec  absolute timeout time, referenced to Kernel::get_ms_count()
      @return true if a resource was acquired, false otherwise.
      @note the underlying RTOS may have a limit to the maximum wait time
            due to internal 32-bit computations, but this is guaranteed to work if the
            wait is <= 0x7fffffff milliseconds (~24 days). If the limit is exceeded,
            the acquire attempt will time out earlier than specified.

      @note You cannot call this function from ISR context.
    */
    bool try_acquire_until(uint64_t millisec);

    /** Release a Semaphore resource that was obtain with Semaphore::acquire.
      @return status code that indicates the execution status of the function:
              @a osOK the token has been correctly released.
              @a osErrorResource the maximum token count has been reached.
              @a osErrorParameter internal error.

      @note You may call this function from ISR context.
    */
    osStatus release(void);

    /** Semaphore destructor
     *
     * @note You cannot call this function from ISR context.
     */
    ~Semaphore();

private:
    void constructor(int32_t count, uint16_t max_count);

#if MBED_CONF_RTOS_PRESENT
    int32_t _wait(uint32_t millisec);

    osSemaphoreId_t               _id;
    mbed_rtos_storage_semaphore_t _obj_mem;
#else
    static bool semaphore_available(void *);
    int32_t _count;
    uint16_t _max_count;
#endif
};
/** @}*/
/** @}*/
}
#endif
