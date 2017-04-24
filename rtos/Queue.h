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
#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <string.h>

#include "cmsis_os2.h"
#include "mbed_rtos_storage.h"
#include "platform/mbed_error.h"
#include "mbed_rtos1_types.h"

namespace rtos {
/** \addtogroup rtos */
/** @{*/

/** The Queue class allow to control, send, receive, or wait for messages.
 A message can be a integer or pointer value  to a certain type T that is send
 to a thread or interrupt service routine.
  @tparam  T         data type of a single message element.
  @tparam  queue_sz  maximum number of messages in queue.

 @note
 Memory considerations: The queue control structures will be created on current thread's stack, both for the mbed OS
 and underlying RTOS objects (static or dynamic RTOS memory pools are not being used).
*/
template<typename T, uint32_t queue_sz>
class Queue {
public:
    /** Create and initialize a message Queue. */
    Queue() {
        memset(&_obj_mem, 0, sizeof(_obj_mem));
        memset(&_attr, 0, sizeof(_attr));
        _attr.mq_mem = _queue_mem;
        _attr.mq_size = sizeof(_queue_mem);
        _attr.cb_mem = &_obj_mem;
        _attr.cb_size = sizeof(_obj_mem);
        _id = osMessageQueueNew(queue_sz, sizeof(T*), &_attr);
        MBED_ASSERT(_id);
    }

    /** Put a message in a Queue.
      @param   data      message pointer.
      @param   millisec  timeout value or 0 in case of no time-out. (default: 0)
      @param   prio      priority value or 0 in case of default. (default: 0)
      @return  status code that indicates the execution status of the function.
    */
    osStatus put(T* data, uint32_t millisec=0, uint8_t prio=0) {
        return osMessageQueuePut(_id, &data, prio, millisec);
    }

    /** Get a message or Wait for a message from a Queue.
      @param   millisec  timeout value or 0 in case of no time-out. (default: osWaitForever).
      @return  event information that includes the message and the status code.
    */
    osEvent get(uint32_t millisec=osWaitForever) {
        osEvent event;
        T *data = NULL;
        osStatus_t res = osMessageQueueGet(_id, &data, NULL, millisec);

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
    osMessageQueueAttr_t          _attr;
    char                          _queue_mem[queue_sz * (sizeof(T*) + sizeof(mbed_rtos_storage_message_t))];
    mbed_rtos_storage_msg_queue_t _obj_mem;
};

}
#endif

/** @}*/
