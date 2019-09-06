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
#ifndef MUTEX_H
#define MUTEX_H

#include <stdint.h>
#include "rtos/mbed_rtos_types.h"
#include "rtos/mbed_rtos1_types.h"
#include "rtos/mbed_rtos_storage.h"

#include "platform/NonCopyable.h"
#include "platform/ScopedLock.h"
#include "platform/mbed_toolchain.h"

namespace rtos {
/** \addtogroup rtos-public-api */
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
 This is, for example, used to protect access to a shared resource.

 In bare-metal builds, the Mutex class is a dummy, so lock() and unlock() are no-ops.

 @note You cannot use member functions of this class in ISR context. If you require Mutex functionality within
 ISR handler, consider using @a Semaphore.

 @note
 Memory considerations: The mutex control structures are created on the current thread's stack, both for the Mbed OS
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

    /**
      Wait until a Mutex becomes available.

      @return  status code that indicates the execution status of the function:
               @a osOK the mutex has been obtained.

      @note You cannot call this function from ISR context.
      @note This function treats RTOS errors as fatal system errors, so it can only return osOK.
            Use of the return value is deprecated, as the return is expected to become void in the future.
     */
#if MBED_CONF_RTOS_PRESENT
    osStatus lock();
#else
    void lock(); // Value return backwards compatibility not required for non-RTOS
#endif

    /**
      Wait until a Mutex becomes available.

      @deprecated Do not use this function. This function has been replaced with lock(), trylock() and trylock_for() functions.

      @param   millisec  timeout value.
      @return  status code that indicates the execution status of the function:
               @a osOK the mutex has been obtained.
               @a osErrorTimeout the mutex could not be obtained in the given time.
               @a osErrorResource the mutex could not be obtained when no timeout was specified.

      @note You cannot call this function from ISR context.
      @note This function treats RTOS errors as fatal system errors, so it can only return osOK or
            osErrorResource in case when millisec is 0 or osErrorTimeout if millisec is not osWaitForever.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.10.0", "Replaced with lock(), trylock() and trylock_for() functions")
    osStatus lock(uint32_t millisec);

    /** Try to lock the mutex, and return immediately
      @return true if the mutex was acquired, false otherwise.
      @note equivalent to trylock_for(0)

      @note You cannot call this function from ISR context.
     */
    bool trylock();

    /** Try to lock the mutex for a specified time
      @param   millisec  timeout value.
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

    /**
      Unlock the mutex that has previously been locked by the same thread

      @return status code that indicates the execution status of the function:
              @a osOK the mutex has been released.

      @note You cannot call this function from ISR context.
      @note This function treats RTOS errors as fatal system errors, so it can only return osOK.
            Use of the return value is deprecated, as the return is expected to become void in the future.
     */
#if MBED_CONF_RTOS_PRESENT
    osStatus unlock();
#else
    void unlock(); // Value return backwards compatibility not required for non-RTOS
#endif

    /** Get the owner the this mutex
      @return  the current owner of this mutex.

      @note You cannot call this function from ISR context.
     */
    osThreadId_t get_owner();

    /** Mutex destructor
     *
     * @note You cannot call this function from ISR context.
     */
    ~Mutex();

private:
#if MBED_CONF_RTOS_PRESENT
    void constructor(const char *name = nullptr);
    friend class ConditionVariable;

    osMutexId_t               _id;
    mbed_rtos_storage_mutex_t _obj_mem;
    uint32_t                  _count;
#endif
};

#if !MBED_CONF_RTOS_PRESENT
inline Mutex::Mutex()
{
}

inline Mutex::Mutex(const char *)
{
}

inline Mutex::~Mutex()
{
}

inline void Mutex::lock()
{
}

inline bool Mutex::trylock()
{
    return true;
}

inline bool Mutex::trylock_for(uint32_t)
{
    return true;
}

inline bool Mutex::trylock_until(uint64_t)
{
    return true;
}

inline void Mutex::unlock()
{
}
#endif

/** @}*/
/** @}*/
}
#endif
