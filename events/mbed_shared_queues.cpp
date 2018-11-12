/* events
 * Copyright (c) 2017 ARM Limited
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

#include "events/mbed_shared_queues.h"

#ifdef MBED_CONF_RTOS_PRESENT
#include "rtos/Thread.h"
using rtos::Thread;
#endif

using namespace events;

namespace mbed {

#ifdef MBED_CONF_RTOS_PRESENT
/* Create an event queue, and start the thread that dispatches it. Static
 * variables mean this happens once the first time each template instantiation
 * is called. This is currently instantiated no more than twice.
 */
template
<osPriority Priority, size_t QueueSize, size_t StackSize>
EventQueue *do_shared_event_queue_with_thread(const char *name)
{
    static uint64_t queue_buffer[QueueSize / sizeof(uint64_t)];
    static EventQueue queue(sizeof queue_buffer, (unsigned char *) queue_buffer);

    static uint64_t stack[StackSize / sizeof(uint64_t)];
    static Thread thread(Priority, StackSize, (unsigned char *) stack, name);

    Thread::State state = thread.get_state();
    if (state == Thread::Inactive || state == Thread::Deleted) {
        osStatus status = thread.start(callback(&queue, &EventQueue::dispatch_forever));
        MBED_ASSERT(status == osOK);
        if (status != osOK) {
            return NULL;
        }
    }

    return &queue;
}
#endif

EventQueue *mbed_event_queue()
{
#if MBED_CONF_EVENTS_SHARED_DISPATCH_FROM_APPLICATION || !defined MBED_CONF_RTOS_PRESENT
    /* Only create the EventQueue, but no dispatching thread */
    static unsigned char queue_buffer[MBED_CONF_EVENTS_SHARED_EVENTSIZE];
    static EventQueue queue(sizeof queue_buffer, queue_buffer);

    return &queue;
#else
    return do_shared_event_queue_with_thread<osPriorityNormal, MBED_CONF_EVENTS_SHARED_EVENTSIZE, MBED_CONF_EVENTS_SHARED_STACKSIZE>("shared_event_queue");
#endif
}

#ifdef MBED_CONF_RTOS_PRESENT
EventQueue *mbed_highprio_event_queue()
{
    return do_shared_event_queue_with_thread<osPriorityHigh, MBED_CONF_EVENTS_SHARED_HIGHPRIO_EVENTSIZE, MBED_CONF_EVENTS_SHARED_HIGHPRIO_STACKSIZE>("shared_highprio_event_queue");
}
#endif

}
