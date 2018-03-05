
/** \addtogroup events */
/** @{*/
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
#ifndef MBED_SHARED_QUEUES_H
#define MBED_SHARED_QUEUES_H

#include "events/EventQueue.h"

namespace mbed {

/**
 * Return a pointer to an EventQueue, on which normal tasks can be queued.
 *
 * All calls to this return the same EventQueue - it and its dispatch thread
 * are created on the first call to this function. The dispatch thread
 * runs at default priority (currently osPriorityNormal).
 *
 * The EventQueue returned may be used to call() Events, or to chain() other
 * EventQueues so that they are run in the same context.
 *
 * Events (or chained EventQueues) executing on the normal event queue should
 * normally take less than 10ms to execute, to avoid starving other users. As
 * such, users can expect that event latency will typically be 10ms or less,
 * but could occasionally be significantly higher if many events are queued.
 *
 * If an RTOS is not present or the configuration option
 * `events.shared-dispatch-from-application` is set to true, then this
 * does not create a dedicated dispatch thread - instead the application is
 * expected to run the EventQueue's dispatch, eg from main. This is necessary
 * for the event loop to work without an RTOS, or an RTOS system can can save
 * memory by reusing the main stack.
 *
 * @note
 * mbed_event_queue is not itself IRQ safe. To use the mbed_event_queue in
 * interrupt context, you must first call `mbed_event_queue()` in threaded
 * context and store the pointer for later use.
 *
 * @return pointer to event queue
 */
events::EventQueue *mbed_event_queue();

#ifdef MBED_CONF_RTOS_PRESENT
/**
 * Return a pointer to an EventQueue, on which small high-priority tasks can
 * be queues, such as simple deferrals from interrupt.
 *
 * All calls to this return the same EventQueue - it and its thread are
 * created on the first call to this function. The dispatch thread
 * runs at a high priority (currently osPriorityHigh).
 *
 * The EventQueue returned may be used to call() Events, or to chain() other
 * EventQueues so that they are run in the same context.
 *
 * Events (or chained EventQueues) executing on the high-priority event queue
 * should normally take less than 100us to execute, to avoid starving other
 * users. As such, users can expect that event latency will typically be 100us
 * or less, but could occasionally be significantly higher if many events are
 * queued.
 *
 * @note
 * mbed_highprio_event_queue is not itself IRQ safe. To use the
 * mbed_highprio_event_queue in interrupt context, you must first call
 * `mbed_event_queue()` in threaded context and store the pointer for
 * later use.
 *
 * @return pointer to high-priority event queue
 */

events::EventQueue *mbed_highprio_event_queue();

#endif // MBED_CONF_RTOS_PRESENT

};

#endif

/** @}*/
