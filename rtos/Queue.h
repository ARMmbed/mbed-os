/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: MIT
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
#ifndef QUEUE_H
#define QUEUE_H

#include "rtos/mbed_rtos_types.h"
#include "rtos/mbed_rtos1_types.h"
#include "rtos/mbed_rtos_storage.h"
#include "platform/mbed_error.h"
#include "platform/NonCopyable.h"

#if MBED_CONF_RTOS_PRESENT || defined(DOXYGEN_ONLY)

namespace rtos {
/** \addtogroup rtos-public-api */
/** @{*/

/**
 * \defgroup rtos_Queue Queue class
 * @{
 */

/** The Queue class represents a collection of objects that are stored first by
 *  order of priority, and then in first-in, first-out (FIFO) order.
 *
 * You can use a queue when you need to store data and then access it in the same
 * order that it has been stored. The order in which you retrieve the data is in
 * order of descending priority. If multiple elements have the same priority,
 * they are retrieved in FIFO order.
 *
 * The object type stored in the queue can be an integer, pointer or a generic
 * type given by the template parameter T.
 *
 * @tparam T        Specifies the type of elements stored in the queue.
 * @tparam queue_sz Maximum number of messages that you can store in the queue.
 *
 * @note Memory considerations: The queue control structures are created on the
 *       current thread's stack, both for the Mbed OS and underlying RTOS
 *       objects (static or dynamic RTOS memory pools are not being used).
 *
 */
template<typename T, uint32_t queue_sz>
class Queue : private mbed::NonCopyable<Queue<T, queue_sz> > {
public:
    /** Create and initialize a message Queue of objects of the parameterized
     * type `T` and maximum capacity specified by `queue_sz`.
     *
     * @note You cannot call this function from ISR context.
    */
    Queue()
    {
        osMessageQueueAttr_t attr = { 0 };
        attr.mq_mem = _queue_mem;
        attr.mq_size = sizeof(_queue_mem);
        attr.cb_mem = &_obj_mem;
        attr.cb_size = sizeof(_obj_mem);
        _id = osMessageQueueNew(queue_sz, sizeof(T *), &attr);
        MBED_ASSERT(_id);
    }

    /** Queue destructor
     *
     * @note You cannot call this function from ISR context.
     */
    ~Queue()
    {
        osMessageQueueDelete(_id);
    }

    /** Check if the queue is empty.
     *
     * @return True if the queue is empty, false if not
     *
     * @note You may call this function from ISR context.
     */
    bool empty() const
    {
        return osMessageQueueGetCount(_id) == 0;
    }

    /** Check if the queue is full.
     *
     * @return True if the queue is full, false if not
     *
     * @note You may call this function from ISR context.
     */
    bool full() const
    {
        return osMessageQueueGetSpace(_id) == 0;
    }

    /** Get number of queued messages in the queue.
     *
     * @return Number of items in the queue
     *
     * @note You may call this function from ISR context.
     */
    uint32_t count() const
    {
        return osMessageQueueGetCount(_id);
    }

    /** Inserts the given element to the end of the queue.
     *
     * This function puts the message pointed to by `data` into the queue. The
     * parameter `prio` is used to sort the message according to their priority
     * (higher numbers indicate higher priority) on insertion.
     *
     * The timeout indicated by the parameter `millisec` specifies how long the
     * function blocks waiting for the message to be inserted into the
     * queue.
     *
     * The parameter `millisec` can have the following values:
     *  - When the timeout is 0 (the default), the function returns instantly.
     *  - When the timeout is osWaitForever, the function waits for an
     *    infinite time.
     *  - For all other values, the function waits for the given number of
     *    milliseconds.
     *
     * @param  data      Pointer to the element to insert into the queue.
     * @param  millisec  Timeout for the operation to be executed, or 0 in case
     *                   of no timeout. (default: 0)
     * @param  prio      Priority of the operation or 0 in case of default.
     *                   (default: 0)
     *
     * @return Status code that indicates the execution status of the function:
     *         @a osOK              The message has been successfully inserted
     *                              into the queue.
     *         @a osErrorTimeout    The message could not be inserted into the
     *                              queue in the given time.
     *         @a osErrorResource   The message could not be inserted because
     *                              the queue is full.
     *         @a osErrorParameter  Internal error or nonzero timeout specified
     *                              in an ISR.
     *
     * @note You may call this function from ISR context if the millisec
     *       parameter is set to 0.
     *
     */
    osStatus put(T *data, uint32_t millisec = 0, uint8_t prio = 0)
    {
        return osMessageQueuePut(_id, &data, prio, millisec);
    }

    /** Get a message or wait for a message from the queue.
     *
     * This function retrieves a message from the queue. The message is stored
     * in the value field of the returned `osEvent` object.
     *
     * The timeout specified by the parameter `millisec` specifies how long the
     * function waits to retrieve the message from the queue.
     *
     * The timeout parameter can have the following values:
     *  - When the timeout is 0, the function returns instantly.
     *  - When the timeout is osWaitForever (default), the function waits
     *    infinite time until the message is retrieved.
     *  - When the timeout is any other value, the function waits for the
     *    specified time before returning a timeout error.
     *
     * Messages are retrieved in descending priority order. If two messages
     * share the same priority level, they are retrieved in first-in, first-out
     * (FIFO) order.
     *
     * @param   millisec  Timeout value.
     *                    (default: osWaitForever).
     *
     * @return Event information that includes the message in event. Message
     *         value and the status code in event.status:
     *         @a osEventMessage   Message successfully received.
     *         @a osOK             No message is available in the queue, and no
     *                             timeout was specified.
     *         @a osEventTimeout   No message was received before a timeout
     *                             event occurred.
     *         @a osErrorParameter A parameter is invalid or outside of a
     *                             permitted range.
     *
     * @note  You may call this function from ISR context if the millisec
     *        parameter is set to 0.
     */
    osEvent get(uint32_t millisec = osWaitForever)
    {
        osEvent event;
        T *data = nullptr;
        osStatus_t res = osMessageQueueGet(_id, &data, nullptr, millisec);

        switch (res) {
            case osOK:
                event.status = (osStatus)osEventMessage;
                event.value.p = data;
                break;
            case osErrorResource:
                event.status = osOK;
                break;
            case osErrorTimeout:
                event.status = (osStatus)osEventTimeout;
                break;
            case osErrorParameter:
            default:
                event.status = osErrorParameter;
                break;
        }
        event.def.message_id = _id;

        return event;
    }

private:
    osMessageQueueId_t            _id;
    char                          _queue_mem[queue_sz * (sizeof(T *) + sizeof(mbed_rtos_storage_message_t))];
    mbed_rtos_storage_msg_queue_t _obj_mem;
};
/** @}*/
/** @}*/

} // namespace rtos

#endif

#endif // QUEUE_H
