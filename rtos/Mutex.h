/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
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
#ifndef MUTEX_H
#define MUTEX_H

#include <stdint.h>
#include "cmsis_os2.h"
#include "mbed_rtos1_types.h"
#include "mbed_rtos_storage.h"

#include "platform/NonCopyable.h"
#include "platform/ScopedLock.h"

namespace rtos {
/** \addtogroup rtos */
/** @{*/

class Mutex;
/** Typedef for the mutex lock
 *
 * Usage:
 * @code
 * void foo(Mutex &m) {
 *     ScopedMutexLock lock(m);
 *     // Mutex lock protects code in this block
 * }
 * @endcode
 */
typedef mbed::ScopedLock<Mutex> ScopedMutexLock;

/**
 * \defgroup rtos_Mutex Mutex class
 * @{
 */
 
/** The Mutex class is used to synchronize the execution of threads.
 This is for example used to protect access to a shared resource.

 @note You cannot use member functions of this class in ISR context. If you require Mutex functionality within
 ISR handler, consider using @a Semaphore.

 @note
 Memory considerations: The mutex control structures will be created on current thread's stack, both for the Mbed OS
 and underlying RTOS objects (static or dynamic RTOS memory pools are not being used).
*/
class Mutex : private mbed::NonCopyable<Mutex> {
public:
    /** Create and Initialize a Mutex object
     *
     * @note You cannot call this function from ISR context.
    */
    Mutex();

    /** Create and Initialize a Mutex object

     @param name name to be used for this mutex. It has to stay allocated for the lifetime of the thread.

     @note You cannot call this function from ISR context.
    */
    Mutex(const char *name);

    /** Wait until a Mutex becomes available.
      @param   millisec  timeout value or 0 in case of no time-out. (default: osWaitForever)
      @return  status code that indicates the execution status of the function:
               @a osOK the mutex has been obtained.
               @a osErrorTimeout the mutex could not be obtained in the given time.
               @a osErrorParameter internal error.
               @a osErrorResource the mutex could not be obtained when no timeout was specified.
               @a osErrorISR this function cannot be called from the interrupt service routine.

      @note You cannot call this function from ISR context.
     */
    osStatus lock(uint32_t millisec=osWaitForever);

    /** Try to lock the mutex, and return immediately
      @return true if the mutex was acquired, false otherwise.
      @note equivalent to trylock_for(0)

      @note You cannot call this function from ISR context.
     */
    bool trylock();

    /** Try to lock the mutex for a specified time
      @param   millisec  timeout value or 0 in case of no time-out.
      @return true if the mutex was acquired, false otherwise.
      @note the underlying RTOS may have a limit to the maximum wait time
            due to internal 32-bit computations, but this is guaranteed to work if the
            wait is <= 0x7fffffff milliseconds (~24 days). If the limit is exceeded,
            the lock attempt will time out earlier than specified.

      @note You cannot call this function from ISR context.
     */
    bool trylock_for(uint32_t millisec);

    /** Try to lock the mutex until specified time
      @param   millisec  absolute timeout time, referenced to Kernel::get_ms_count()
      @return true if the mutex was acquired, false otherwise.
      @note the underlying RTOS may have a limit to the maximum wait time
            due to internal 32-bit computations, but this is guaranteed to work if the
            wait is <= 0x7fffffff milliseconds (~24 days). If the limit is exceeded,
            the lock attempt will time out earlier than specified.

      @note You cannot call this function from ISR context.
     */
    bool trylock_until(uint64_t millisec);

    /** Unlock the mutex that has previously been locked by the same thread
      @return status code that indicates the execution status of the function:
              @a osOK the mutex has been released.
              @a osErrorParameter internal error.
              @a osErrorResource the mutex was not locked or the current thread wasn't the owner.
              @a osErrorISR this function cannot be called from the interrupt service routine.

      @note You cannot call this function from ISR context.
     */
    osStatus unlock();

    /** Get the owner the this mutex
      @return  the current owner of this mutex.

      @note You cannot call this function from ISR context.
     */
    osThreadId get_owner();

    /** Mutex destructor
     *
     * @note You cannot call this function from ISR context.
     */
    ~Mutex();

private:
    void constructor(const char *name = NULL);
    friend class ConditionVariable;

    osMutexId_t               _id;
    mbed_rtos_storage_mutex_t _obj_mem;
    uint32_t                  _count;
};
/** @}*/
/** @}*/
}
#endif


