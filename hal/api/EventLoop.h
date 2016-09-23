/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#ifndef MBED_EVENT_LOOP_H
#define MBED_EVENT_LOOP_H

#ifdef MBED_CONF_RTOS_PRESENT

#include <stdint.h>
#include "rtos.h"
#include "Callback.h"
#include "Event.h"

namespace mbed {

typedef Callback<void()> queue_event_t;

/** An event queue is a queue that holds callbacks. These callbacks are removed
 *  from the queue and executed by the 'dispatch' function. 'dispatch' waits for
 *  callbacks to be available in an infinite loop. 'dispatch' runs in a separate
 *  thread, which means that all the callbacks in the queue will run in the
 *  EventLoop's 'dispatch' thread context. This can be used to defer execution
 *  of functions in interupt contexts by adding them to the EventLoop (using
 *  'post') instead of running them directly (see also the 'Event' class).
 *
 *  @note The event queue must be started manually after it is created by calling
 *        its 'start' method.
 *
 *  Example:
 *
 *  @code
 *  #include "mbed.h"
 *  #include "EventLoop.h"
 *
 *  EventLoop queue(4);
 *  InterruptIn sw(SW2);
 *  DigitalOut led(LED1);
 *
 *  static void printer() {
 *      // Run 'printf' in user context
 *      printf("LED toggled.\n");
 *  }
 *
 *  static void handler() {
 *      // This handler will run in user context, in the context of the thread
 *      // created by calling 'start' (below).
 *      led = !led;
 *      printf("LED toggled.\n");
 *  }
 *
 *  int main() {
 *      queue.start();
 *      sw.rise(event(handler, &queue));
 *      Thread::wait(osWaitForever);
 *  }
 *  @endcode
 */
class EventLoop {
public:
    /** Create an EventLoop instance.
     *  @param size The maximum number of callbacks in the queue.
     *  @param priority The priority of the event thread function. (default: osPriorityNormal).
     *  @param stack_size The stack size (in bytes) of the event thread function. (default: DEFAULT_STACK_SIZE).
     *  @param stack_pointer Pointer to the stack area to be used by the event thread function (default: NULL).
     */
    EventLoop(uint32_t size, osPriority priority=osPriorityNormal,
               uint32_t stack_size=DEFAULT_STACK_SIZE, unsigned char *stack_pointer=NULL);

    /** Post a callback in the queue.
     *  @param evt the callback to post.
     *  @returns true if the callback was added to the queue, false otherwise.
     */
    bool post(const queue_event_t& evt);

    /** Starts the event queue thread
     */
    void start();

    /** Stops the event queue thread
     */
    void stop();

private:
    DynamicMail<queue_event_t> _queue;
    Thread _thread;

    void dispatch(void);
};

} // namespace mbed

#else // #ifdef MBED_CONF_RTOS_PRESENT

#warning The event queue is not implemented when the RTOS is not present.

#endif // #ifdef MBED_CONF_RTOS_PRESENT

#endif // #ifndef MBED_EVENT_LOOP_H

