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
#include "mbed_rtos_storage.h"
#include "mbed_rtos1_types.h"

#include "platform/NonCopyable.h"

#ifndef MBED_NO_GLOBAL_USING_DIRECTIVE
using namespace rtos;
#endif

namespace rtos {
/** \addtogroup rtos */
/** @{*/
/**
 * \defgroup rtos_Mail Mail class
 * @{
 */

/** The Mail class allows you to control, send, receive or wait for mail.
 * A mail is a memory block that is sent to a thread or interrupt service routine (ISR).
 * @tparam  T         Data type of a single mail message element.
 * @tparam  queue_sz  Maximum number of mail messages in queue.
 *
 * @note
 * Memory considerations: The mail data store and control structures are part of this class - they do not (themselves)
 * allocate memory on the heap, both for the Mbed OS and underlying RTOS objects (static or dynamic RTOS memory
 * pools are not being used).
 */
template<typename T, uint32_t queue_sz>
class Mail : private mbed::NonCopyable<Mail<T, queue_sz> > {
public:
    /** Create and initialize Mail queue.
     *
     * @note You cannot call this function from ISR context.
     */
    Mail() { };

    /** Check if the mail queue is empty.
     *
     * @return State of queue.
     * @retval true  Mail queue is empty.
     * @retval false Mail queue contains mail.
     *
     * @note You may call this function from ISR context.
     */
    bool empty() const
    {
        return _queue.empty();
    }

    /** Check if the mail queue is full.
     *
     * @return State of queue.
     * @retval true  Mail queue is full.
     * @retval false Mail queue is not full.
     *
     * @note You may call this function from ISR context.
     */
    bool full() const
    {
        return _queue.full();
    }

    /** Allocate a memory block of type T.
     *
     * @param   millisec  Not used.
     *
     * @return  Pointer to memory block that you can fill with mail or NULL in case error.
     *
     * @note You may call this function from ISR context.
     */
    T *alloc(uint32_t millisec = 0)
    {
        return _pool.alloc();
    }

    /** Allocate a memory block of type T, and set memory block to zero.
     *
     * @param   millisec  Not used.
     *
     * @return  Pointer to memory block that you can fill with mail or NULL in case error.
     *
     * @note You may call this function from ISR context.
     */
    T *calloc(uint32_t millisec = 0)
    {
        return _pool.calloc();
    }

    /** Put a mail in the queue.
     *
     * @param   mptr  Memory block previously allocated with Mail::alloc or Mail::calloc.
     *
     * @return  Status code that indicates the execution status of the function (osOK on success).
     *
     * @note You may call this function from ISR context.
     */
    osStatus put(T *mptr)
    {
        return _queue.put(mptr);
    }

    /** Get a mail from the queue.
     *
     * @param millisec Timeout value (default: osWaitForever).
     *
     * @return Event that contains mail information or error code.
     * @retval osEventMessage   Message received.
     * @retval osOK             No mail is available (and no timeout was specified).
     * @retval osEventTimeout   No mail has arrived during the given timeout period.
     * @retval osErrorParameter A parameter is invalid or outside of a permitted range.
     *
     * @note You may call this function from ISR context if the millisec parameter is set to 0.
     */
    osEvent get(uint32_t millisec = osWaitForever)
    {
        osEvent evt = _queue.get(millisec);
        if (evt.status == osEventMessage) {
            evt.status = osEventMail;
        }
        return evt;
    }

    /** Free a memory block from a mail.
     *
     * @param mptr Pointer to the memory block that was obtained with Mail::get.
     *
     * @return Status code that indicates the execution status of the function (osOK on success).
     *
     * @note You may call this function from ISR context.
     */
    osStatus free(T *mptr)
    {
        return _pool.free(mptr);
    }

private:
    Queue<T, queue_sz> _queue;
    MemoryPool<T, queue_sz> _pool;
};

/** @}*/
/** @}*/

}

#endif



