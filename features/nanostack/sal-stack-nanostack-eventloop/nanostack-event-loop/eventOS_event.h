/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef EVENTOS_EVENT_H_
#define EVENTOS_EVENT_H_
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup nanostack-eventloop Nanostack's event system.
 * Small event scheduler and timer system written in C.
 *
 * This event system is originating from project called Nanostack and developed within Arm. Therefore
 * some of the types and names within this library are prefixed with `ns_*` or `arm_*` or `eventOS*`.
 *
 * <h3>Concept</h3>
 *
 * Event loop uses a concept called tasklet, which is just a callback function that receives events.
 * There can be as many as 128 tasklets registered if memory allows. This is only limited by event ID being just 8-bits.
 * Each tasklet is first registered to the event system, which then gives 8 bit ID number for the tasklet.
 *
 * @startuml
 *      package "eventOS" {
 *       [eventOS_event.h] - event_handler_create
 *      }
 *      node "application" {
 *          [tasklet1.cpp] ..> event_handler_create : register
 *          [tasklet1.cpp] - tasklet1
 *          [tasklet2.cpp] ..> event_handler_create : register
 *          [tasklet2.cpp] - tasklet2
 *          [tasklet3.cpp] ..> event_handler_create : register
 *          [tasklet3.cpp] - tasklet3
 *      }
 * @enduml
 *
 * Events are send to a specific tasklet, identified by its ID.
 * Each event is coded into a \ref arm_event_s structure which is then pushed into event loop by calling eventOS_event_send().
 *
 * @startuml
 *      partition tasklet1.cpp {
 *          (*) --> tasklet1
 *      }
 *      partition "eventOS" {
 *          tasklet1 -->[event:\nreceiver: 3\nevent_id: 1] eventOS_event_send
 *      }
 *      partition tasklet3.cpp {
 *          eventOS_event_send -->[event:\nreceiver: 3\nevent_id: 1] tasklet3
 *      }
 * @enduml
 *
 * <h3>Usage</h3>
 *
 * To send or receive events, you first need to register your event handler.
 * \code
 *      // In header
 *      extern uint8_t my_eventhandler_id;
 *      #define INITIALIZATION_EVENT 0
 *      #define MY_EVENT 1
 *
 *      // In my_handler.cpp
 *      void my_event_handler(arm_event_t *e)
 *      {
 *          switch (e->event_type) {
 *              case INITIALIZATION_EVENT:
 *                  // Initialize my module
 *                  break;
 *              case MY_EVENT:
 *                  // Event received
 *                  break;
 *          }
 *      }
 *
 *      // Register the handler
 *      my_eventhandler_id = eventOS_event_handler_create(my_event_handler, INITIALIZATION_EVENT);
 *      if (my_eventhandler_id < 0) {
 *          // fail
 *      }
 * \endcode
 *
 * Each event is basically a \ref arm_event_s structure. You need to fill in the arm_event_s::receiver field.
 * Rest of the fields are optional, and used only by the receiving callback. So you have different options to
 * deliver data to a receiving tasklet. The structure is copied by the event system, so temporary storage may be used,
 * and the structure may be freed after it has been pushed into event system.
 *
 * \code
 *      // Send the event
 *      arm_event_t e = {
 *       .receiver = my_eventhandler_id,
 *       .event_type = MY_EVENT
 *      };
 *
 *      if (eventOS_event_send(e) != 0) {
 *       // fail
 *      }
 * \endcode
 *
 * Where required, event system allows you to delay the event propagation.
 *
 * \code
 *      // Wait 3 seconds before the event
 *      #define MY_DELAY_MS 3000
 *
 *      arm_event_t e = {
 *          .receiver = my_eventhandler_id,
 *          .event_type = MY_EVENT
 *      };
 *
 *      uint32_t delay = eventOS_event_timer_ms_to_ticks(MY_DELAY_MS);
 *      eventOS_event_send_after(e, delay);
 * \endcode
 *
 * \sa eventOS_event.h
 * \sa eventOS_event_send_at
 * \sa eventOS_event_send_in
 * \sa eventOS_event_send_after
 * \sa eventOS_event_send_every
 *
 * <h3>Pre-allocated events</h3>
 *
 * Two options are provided to limit the heap usage. First option is to use recurring events with eventOS_event_send_every(),
 * so your event is only allocated once. This allows you to create application that does not use heap after initialization phase.
 *
 * Second option is to use pre-allocated or statically allocated event structure. In this model you create a space for
 * \ref arm_event_storage structure and send events using eventOS_event_send_user_allocated() call. This is also
 * very robust, as there is no allocation, so the sending of the event will never fail because of lack of memory.
 *
 * \code
 *   static bool pending = false;
 *   static arm_event_storage_t e;
 *   static int8_t foo_tasklet_id;
 *
 *   void notify_foo()
 *   {
 *       if (!pending) {
 *           pending = true;
 *           e.data.receiver = foo_tasklet_id;
 *           e.data.type = MY_EVENT;
 *           eventOS_event_send_user_allocated(&e);
 *      }
 *   }
 *
 *   void foo_event_handler(arm_event_t *e)
 *   {
 *       pending = false;
 *       // ...
 *   }
 *
 * \endcode
 *
 * <h3>Initialization</h3>
 *
 * Event system does not use malloc(), free() or any system heap directly, but uses nsdynmemLIB.h library instead.
 * Event system must first be initialized by callind eventOS_scheduler_init(). This is usually done just after ns_dyn_mem_init() call.
 * Where porting is already provided, these both are initialized in function called ns_hal_init().
 *
 * After initialization, you can start the event loop by calling eventOS_scheduler_run() which will never return. This is usually
 * end of the `main()` function.
 *
 * \code
 * extern void my_event_handler(arm_event_t *e);
 * extern int8_t my_eventhandler_id;
 *
 * void main(void)
 * {
 *     ns_dyn_mem_init(NULL, HEAP_SIZE, NULL, NULL);
 *     eventOS_scheduler_init();
 *     my_eventhandler_id = eventOS_event_handler_create(my_event_handler, INITIALIZATION_EVENT);
 *     eventOS_scheduler_run()
 * }
 * \endcode
 */

/**
 * \file eventOS_event.h
 * \ingroup nanostack-eventloop
 * \brief Nanostack's event loop.
 */

#include "ns_types.h"
#include "ns_list.h"

/**
 * \enum arm_library_event_priority_e
 * \brief Event Priority level.
 */
typedef enum arm_library_event_priority_e {
    ARM_LIB_HIGH_PRIORITY_EVENT = 0, /**< High Priority Event (Function CB) */
    ARM_LIB_MED_PRIORITY_EVENT = 1, /**< Medium Priority (Timer) */
    ARM_LIB_LOW_PRIORITY_EVENT = 2, /*!*< Normal Event and ECC / Security */
} arm_library_event_priority_e;

/**
 * \struct arm_event_s
 * \brief Event structure.
 */
typedef struct arm_event_s {
    int8_t receiver; /**< Event handler Tasklet ID */
    int8_t sender; /**< Event sender Tasklet ID */
    uint8_t event_type; /**< This will be typecast arm_library_event_type_e, arm_internal_event_type_e or application specific define */
    uint8_t event_id; /**< Timer ID, NWK interface ID or application specific ID */
    void *data_ptr; /**< Application could share data pointer tasklet to tasklet */
    arm_library_event_priority_e priority;
    uintptr_t event_data;
} arm_event_t;

/* Backwards compatibility */
typedef arm_event_t arm_event_s;

/**
 * \struct arm_event_storage
 * \brief Event structure storage, including list link.

@startuml

partition "Event loop" {
(*) -->[event created] "UNQUEUED"
"UNQUEUED" -->[event_core_write()] "QUEUED"
"QUEUED" -->[event_core_read()] "RUNNING"
"RUNNING" ->[event_core_free_push()] "UNQUEUED"
}

partition "system_timer.c" {
    "UNQUEUED:timer" -->[eventOS_event_send_timer_allocated()] "QUEUED"
}
@enduml

 */
typedef struct arm_event_storage {
    arm_event_s data;
    enum {
        ARM_LIB_EVENT_STARTUP_POOL,
        ARM_LIB_EVENT_DYNAMIC,
        ARM_LIB_EVENT_USER,
        ARM_LIB_EVENT_TIMER,
    } allocator;
    enum {
        ARM_LIB_EVENT_UNQUEUED,
        ARM_LIB_EVENT_QUEUED,
        ARM_LIB_EVENT_RUNNING,
    } state;
    ns_list_link_t link;
} arm_event_storage_t;

/**
 * \brief Send event to event scheduler.
 *
 * \param event pointer to pushed event.
 *
 * Event data is copied by the call, and this copy persists until the
 * recipient's callback function returns. The callback function is passed
 * a pointer to a copy of the data, not the original pointer.
 *
 * \return 0 Event push OK
 * \return -1 Memory allocation Fail
 */
extern int8_t eventOS_event_send(const arm_event_t *event);

/* Alternate names for timer function from eventOS_event_timer.h;
 * implementations may one day merge */
#define eventOS_event_send_at(event, at)       eventOS_event_timer_request_at(event, at)       ///< \copydoc eventOS_event_timer_request_at
#define eventOS_event_send_in(event, in)       eventOS_event_timer_request_in(event, in)       ///< \copydoc eventOS_event_timer_request_in
#define eventOS_event_send_after(event, after) eventOS_event_timer_request_after(event, after) ///< \copydoc eventOS_event_timer_request_after
#define eventOS_event_send_every(event, every) eventOS_event_timer_request_every(event, every) ///< \copydoc eventOS_event_timer_request_every

/**
 * \brief Send user-allocated event to event scheduler.
 *
 * \param event pointer to pushed event storage.
 *
 * The event structure is not copied by the call, the event system takes
 * ownership and it is threaded directly into the event queue. This avoids the
 * possibility of event sending failing due to memory exhaustion.
 *
 * event->data must be filled in on entry - the rest of the structure (link and
 * allocator) need not be.
 *
 * The structure must remain valid until the recipient is called - the
 * event system passes ownership to the receiving event handler, who may then
 * invalidate it, or send it again.
 *
 * The recipient receives a pointer to the arm_event_t data member of the
 * event - it can use NS_CONTAINER_OF() to get a pointer to the original
 * event passed to this call, or to its outer container.
 *
 * It is a program error to send a user-allocated event to a non-existent task.
 */
extern void eventOS_event_send_user_allocated(arm_event_storage_t *event);

/**
 * \brief Event handler callback register
 *
 * Function will register and allocate unique event id handler
 *
 * \param handler_func_ptr function pointer for event handler
 * \param init_event_type generated event type for init purpose
 *
 * \return >= 0 Unique event ID for this handler
 * \return < 0 Register fail
 *
 * */
extern int8_t eventOS_event_handler_create(void (*handler_func_ptr)(arm_event_t *), uint8_t init_event_type);

/**
 * Cancel an event.
 *
 * Queued events are removed from the event-loop queue and/or the timer queue.
 *
 * Passing a NULL pointer is allowed, and does nothing.
 *
 * Event pointers are valid from the time they are queued until the event
 * has finished running or is cancelled.
 *
 * Cancelling a currently-running event is only useful to stop scheduling
 * it if it is on a periodic timer; it has no other effect.
 *
 * Cancelling an already-cancelled or already-run single-shot event
 * is undefined behaviour.
 *
 * \param event Pointer to event handle or NULL.
 */
extern void eventOS_cancel(arm_event_storage_t *event);

#ifdef __cplusplus
}
#endif
#endif /* EVENTOS_EVENT_H_ */
