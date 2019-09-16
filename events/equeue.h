
/*
 * Flexible event queue for dispatching events
 *
 * Copyright (c) 2016-2019 ARM Limited
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
#ifndef EQUEUE_H
#define EQUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

// Platform specific files
#include "events/internal/equeue_platform.h"

#include <stddef.h>
#include <stdint.h>

/**
 * \addtogroup events-public-api
 * @{
 */

// The minimum size of an event
// This size is guaranteed to fit events created by event_call
#define EQUEUE_EVENT_SIZE (sizeof(struct equeue_event) + 2*sizeof(void*))

// Internal event structure
struct equeue_event {
    unsigned size;
    uint8_t id;
    uint8_t generation;

    struct equeue_event *next;
    struct equeue_event *sibling;
    struct equeue_event **ref;

    unsigned target;
    int period;
    void (*dtor)(void *);

    void (*cb)(void *);
    // data follows
};

// Event queue structure
typedef struct equeue {
    struct equeue_event *queue;
    unsigned tick;
    bool break_requested;
    uint8_t generation;

    unsigned char *buffer;
    unsigned npw2;
    void *allocated;

    struct equeue_event *chunks;
    struct equeue_slab {
        size_t size;
        unsigned char *data;
    } slab;

    struct equeue_background {
        bool active;
        void (*update)(void *timer, int ms);
        void *timer;
    } background;

    equeue_sema_t eventsema;
    equeue_mutex_t queuelock;
    equeue_mutex_t memlock;
} equeue_t;


// Queue lifetime operations
//
// Creates and destroys an event queue. The event queue either allocates a
// buffer of the specified size with malloc or uses a user provided buffer
// if constructed with equeue_create_inplace.
//
// If the event queue creation fails, equeue_create returns a negative,
// platform-specific error code.
int equeue_create(equeue_t *queue, size_t size);
int equeue_create_inplace(equeue_t *queue, size_t size, void *buffer);
void equeue_destroy(equeue_t *queue);

// Dispatch events
//
// Executes events until the specified milliseconds have passed. If ms is
// negative, equeue_dispatch will dispatch events indefinitely or until
// equeue_break is called on this queue.
//
// When called with a finite timeout, the equeue_dispatch function is
// guaranteed to terminate. When called with a timeout of 0, the
// equeue_dispatch does not wait and is irq safe.
void equeue_dispatch(equeue_t *queue, int ms);

// Break out of a running event loop
//
// Forces the specified event queue's dispatch loop to terminate. Pending
// events may finish executing, but no new events will be executed.
void equeue_break(equeue_t *queue);

// Simple event calls
//
// The specified callback will be executed in the context of the event queue's
// dispatch loop. When the callback is executed depends on the call function.
//
// equeue_call       - Immediately post an event to the queue
// equeue_call_in    - Post an event after a specified time in milliseconds
// equeue_call_every - Post an event periodically every milliseconds
//
// All equeue_call functions are irq safe and can act as a mechanism for
// moving events out of irq contexts.
//
// The return value is a unique id that represents the posted event and can
// be passed to equeue_cancel. If there is not enough memory to allocate the
// event, equeue_call returns an id of 0.
int equeue_call(equeue_t *queue, void (*cb)(void *), void *data);
int equeue_call_in(equeue_t *queue, int ms, void (*cb)(void *), void *data);
int equeue_call_every(equeue_t *queue, int ms, void (*cb)(void *), void *data);

// Allocate memory for events
//
// The equeue_alloc function allocates an event that can be manually dispatched
// with equeue_post. The equeue_dealloc function may be used to free an event
// that has not been posted. Once posted, an event's memory is managed by the
// event queue and should not be deallocated.
//
// Both equeue_alloc and equeue_dealloc are irq safe.
//
// The equeue allocator is designed to minimize jitter in interrupt contexts as
// well as avoid memory fragmentation on small devices. The allocator achieves
// both constant-runtime and zero-fragmentation for fixed-size events, however
// grows linearly as the quantity of different sized allocations increases.
//
// The equeue_alloc function returns a pointer to the event's allocated memory
// and acts as a handle to the underlying event. If there is not enough memory
// to allocate the event, equeue_alloc returns null.
void *equeue_alloc(equeue_t *queue, size_t size);
void equeue_dealloc(equeue_t *queue, void *event);

// Configure an allocated event
//
// equeue_event_delay  - Millisecond delay before dispatching an event
// equeue_event_period - Millisecond period for repeating dispatching an event
// equeue_event_dtor   - Destructor to run when the event is deallocated
void equeue_event_delay(void *event, int ms);
void equeue_event_period(void *event, int ms);
void equeue_event_dtor(void *event, void (*dtor)(void *));

// Post an event onto the event queue
//
// The equeue_post function takes a callback and a pointer to an event
// allocated by equeue_alloc. The specified callback will be executed in the
// context of the event queue's dispatch loop with the allocated event
// as its argument.
//
// The equeue_post function is irq safe and can act as a mechanism for
// moving events out of irq contexts.
//
// The return value is a unique id that represents the posted event and can
// be passed to equeue_cancel.
int equeue_post(equeue_t *queue, void (*cb)(void *), void *event);

// Post an user allocated event onto the event queue
//
// The equeue_post_user_allocated function takes a callback and a pointer
// to an event allocated by user. The specified callback will be executed
// in the context of the event queue's dispatch loop with the allocated
// event as its argument.
//
// The equeue_post_user_allocated function is irq safe and can act as
// a mechanism for moving events out of irq contexts.
void equeue_post_user_allocated(equeue_t *queue, void (*cb)(void *), void *event);

// Cancel an in-flight event
//
// Attempts to cancel an event referenced by the unique id returned from
// equeue_call or equeue_post. It is safe to call equeue_cancel after an event
// has already been dispatched.
//
// The equeue_cancel function is irq safe.
//
// If called while the event queue's dispatch loop is active in another thread,
// equeue_cancel does not guarantee that the event will not execute after it returns as
// the event may have already begun executing.
// Returning true guarantees that cancel succeeded and event will not execute.
// Returning false if invalid id or already started executing.
bool equeue_cancel(equeue_t *queue, int id);

// Cancel an in-flight user allocated event
//
// Attempts to cancel an event referenced by its address.
// It is safe to call equeue_cancel_user_allocated after an event
// has already been dispatched.
//
// The equeue_cancel_user_allocated function is irq safe.
//
// If called while the event queue's dispatch loop is active,
// equeue_cancel_user_allocated does not guarantee that the event
// will not not execute after it returns as the event may have
// already begun executing.
bool equeue_cancel_user_allocated(equeue_t *queue, void *event);

// Query how much time is left for delayed event
//
//  If event is delayed, this function can be used to query how much time
//  is left until the event is due to be dispatched.
//
//  This function is irq safe.
//
int equeue_timeleft(equeue_t *q, int id);

// Query how much time is left for delayed user allocated event
//
//  If event is delayed, this function can be used to query how much time
//  is left until the event is due to be dispatched.
//
//  This function is irq safe.
//
int equeue_timeleft_user_allocated(equeue_t *q, void *event);

// Background an event queue onto a single-shot timer
//
// The provided update function will be called to indicate when the queue
// should be dispatched. A negative timeout will be passed to the update
// function when the timer is no longer needed.
//
// Passing a null update function disables the existing timer.
//
// The equeue_background function allows an event queue to take advantage
// of hardware timers or even other event loops, allowing an event queue to
// be effectively backgrounded.
void equeue_background(equeue_t *queue,
                       void (*update)(void *timer, int ms), void *timer);

// Chain an event queue onto another event queue
//
// After chaining a queue to a target, calling equeue_dispatch on the
// target queue will also dispatch events from this queue. The queues
// use their own buffers and events must be managed independently.
//
// Passing a null queue as the target will unchain the existing queue.
//
// The equeue_chain function allows multiple equeues to be composed, sharing
// the context of a dispatch loop while still being managed independently.
//
// If the event queue chaining fails, equeue_chain returns a negative,
// platform-specific error code.
int equeue_chain(equeue_t *queue, equeue_t *target);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif
