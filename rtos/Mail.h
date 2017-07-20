/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#ifndef MAIL_H
#define MAIL_H

#include <stdint.h>
#include <string.h>

#include "Queue.h"
#include "MemoryPool.h"
#include "cmsis_os2.h"
#include "rtx_lib.h"
#include "mbed_rtos1_types.h"

#include "platform/NonCopyable.h"

using namespace rtos;

namespace rtos {
/** \addtogroup rtos */
/** @{*/

/** The Mail class allow to control, send, receive, or wait for mail.
 A mail is a memory block that is send to a thread or interrupt service routine.
  @tparam  T         data type of a single message element.
  @tparam  queue_sz  maximum number of messages in queue.

 @note
 Memory considerations: The mail data store and control structures will be created on current thread's stack,
 both for the mbed OS and underlying RTOS objects (static or dynamic RTOS memory pools are not being used).
*/
template<typename T, uint32_t queue_sz>
class Mail : private mbed::NonCopyable<Mail<T, queue_sz> > {
public:
    /** Create and Initialise Mail queue. */
    Mail() { };

    /** Allocate a memory block of type T
      @param   millisec  timeout value or 0 in case of no time-out. (default: 0).
      @return  pointer to memory block that can be filled with mail or NULL in case error.
    */
    T* alloc(uint32_t millisec=0) {
        return _pool.alloc();
    }

    /** Allocate a memory block of type T and set memory block to zero.
      @param   millisec  timeout value or 0 in case of no time-out.  (default: 0).
      @return  pointer to memory block that can be filled with mail or NULL in case error.
    */
    T* calloc(uint32_t millisec=0) {
        return _pool.calloc();
    }

    /** Put a mail in the queue.
      @param   mptr  memory block previously allocated with Mail::alloc or Mail::calloc.
      @return  status code that indicates the execution status of the function.
    */
    osStatus put(T *mptr) {
        return _queue.put(mptr);
    }

    /** Get a mail from a queue.
      @param   millisec  timeout value or 0 in case of no time-out. (default: osWaitForever).
      @return  event that contains mail information or error code.
    */
    osEvent get(uint32_t millisec=osWaitForever) {
        osEvent evt = _queue.get(millisec);
        if (evt.status == osEventMessage) {
            evt.status = osEventMail;
        }
        return evt;
    }

    /** Free a memory block from a mail.
      @param   mptr  pointer to the memory block that was obtained with Mail::get.
      @return  status code that indicates the execution status of the function.
    */
    osStatus free(T *mptr) {
        return _pool.free(mptr);
    }

private:
    Queue<T, queue_sz> _queue;
    MemoryPool<T, queue_sz> _pool;
};

}

#endif


/** @}*/
