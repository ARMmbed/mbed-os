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
#define eventOS_event_send_at(event, at)       eventOS_event_timer_request_at(event, at)
#define eventOS_event_send_in(event, in)       eventOS_event_timer_request_in(event, in)
#define eventOS_event_send_after(event, after) eventOS_event_timer_request_after(event, after)
#define eventOS_event_send_every(event, every) eventOS_event_timer_request_every(event, every)

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
