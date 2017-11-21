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

namespace rtos {
/** \addtogroup rtos */
/** @{*/
/**
 * \defgroup rtos_Mutex Mutex class
 * @{
 */
 
/** The Mutex class is used to synchronize the execution of threads.
 This is for example used to protect access to a shared resource.

 @note
 Memory considerations: The mutex control structures will be created on current thread's stack, both for the mbed OS
 and underlying RTOS objects (static or dynamic RTOS memory pools are not being used).
*/
class Mutex : private mbed::NonCopyable<Mutex> {
public:
    /** Create and Initialize a Mutex object */
    Mutex();

    /** Create and Initialize a Mutex object

     @param name name to be used for this mutex. It has to stay allocated for the lifetime of the thread.
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
     */
    osStatus lock(uint32_t millisec=osWaitForever);

    /** Try to lock the mutex, and return immediately
      @return true if the mutex was acquired, false otherwise.
     */
    bool trylock();

    /** Unlock the mutex that has previously been locked by the same thread
      @return status code that indicates the execution status of the function:
              @a osOK the mutex has been released.
              @a osErrorParameter internal error.
              @a osErrorResource the mutex was not locked or the current thread wasn't the owner.
              @a osErrorISR this function cannot be called from the interrupt service routine.
     */
    osStatus unlock();

    ~Mutex();

private:
    void constructor(const char *name = NULL);

    osMutexId_t               _id;
    osMutexAttr_t             _attr;
    mbed_rtos_storage_mutex_t _obj_mem;
};
/** @}*/
/** @}*/
}
#endif


