/*
 * Flexible event queue for dispatching events
 *
 * Copyright (c) 2016-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
#include "events/equeue.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// check if the event is allocaded by user - event address is outside queues internal buffer address range
#define EQUEUE_IS_USER_ALLOCATED_EVENT(e) ((q->buffer == NULL) || ((uintptr_t)(e) < (uintptr_t)q->buffer) || ((uintptr_t)(e) > ((uintptr_t)q->slab.data)))

// for user allocated events use event id to track event state
enum {
    EQUEUE_USER_ALLOCATED_EVENT_STATE_INPROGRESS = 1,
    EQUEUE_USER_ALLOCATED_EVENT_STATE_DONE = 0          // event canceled or dispatching done
};

// calculate the relative-difference between absolute times while
// correctly handling overflow conditions
static inline int equeue_tickdiff(unsigned a, unsigned b)
{
    return (int)(unsigned)(a - b);
}

// calculate the relative-difference between absolute times, but
// also clamp to zero, resulting in only non-zero values.
static inline int equeue_clampdiff(unsigned a, unsigned b)
{
    int diff = equeue_tickdiff(a, b);
    return diff > 0 ? diff : 0;
}

// Increment the unique id in an event, hiding the event from cancel
static inline void equeue_incid(equeue_t *q, struct equeue_event *e)
{
    e->id += 1;
    if (((unsigned)e->id << q->npw2) == 0) {
        e->id = 1;
    }
}


// equeue lifetime management
int equeue_create(equeue_t *q, size_t size)
{
    // dynamically allocate the specified buffer
    void *buffer = malloc(size);
    if (!buffer) {
        return -1;
    }

    int err = equeue_create_inplace(q, size, buffer);
    q->allocated = buffer;
    return err;
}

int equeue_create_inplace(equeue_t *q, size_t size, void *buffer)
{
    // setup queue around provided buffer
    // ensure buffer and size are aligned
    if (size >= sizeof(void *)) {
        q->buffer = (void *)(((uintptr_t) buffer + sizeof(void *) -1) & ~(sizeof(void *) -1));
        size -= (char *) q->buffer - (char *) buffer;
        size &= ~(sizeof(void *) -1);
    } else {
        // don't align when size less then pointer size
        // e.g. static queue (size == 1)
        q->buffer = buffer;
    }

    q->allocated = 0;

    q->npw2 = 0;
    for (unsigned s = size; s; s >>= 1) {
        q->npw2++;
    }

    q->chunks = 0;
    q->slab.size = size;
    q->slab.data = q->buffer;

    q->queue = 0;
    equeue_tick_init();
    q->tick = equeue_tick();
    q->generation = 0;
    q->break_requested = false;

    q->background.active = false;
    q->background.update = 0;
    q->background.timer = 0;

    // initialize platform resources
    int err;
    err = equeue_sema_create(&q->eventsema);
    if (err < 0) {
        return err;
    }

    err = equeue_mutex_create(&q->queuelock);
    if (err < 0) {
        return err;
    }

    err = equeue_mutex_create(&q->memlock);
    if (err < 0) {
        return err;
    }

    return 0;
}

void equeue_destroy(equeue_t *q)
{
    // call destructors on pending events
    for (struct equeue_event *es = q->queue; es; es = es->next) {
        for (struct equeue_event *e = es->sibling; e; e = e->sibling) {
            if (e->dtor) {
                e->dtor(e + 1);
            }
        }
        if (es->dtor) {
            es->dtor(es + 1);
        }
    }
    // notify background timer
    if (q->background.update) {
        q->background.update(q->background.timer, -1);
    }

    // clean up platform resources + memory
    equeue_mutex_destroy(&q->memlock);
    equeue_mutex_destroy(&q->queuelock);
    equeue_sema_destroy(&q->eventsema);
    free(q->allocated);
}


// equeue chunk allocation functions
static struct equeue_event *equeue_mem_alloc(equeue_t *q, size_t size)
{
    // add event overhead
    size += sizeof(struct equeue_event);
    size = (size + sizeof(void *) -1) & ~(sizeof(void *) -1);

    equeue_mutex_lock(&q->memlock);

    // check if a good chunk is available
    for (struct equeue_event **p = &q->chunks; *p; p = &(*p)->next) {
        if ((*p)->size >= size) {
            struct equeue_event *e = *p;
            if (e->sibling) {
                *p = e->sibling;
                (*p)->next = e->next;
            } else {
                *p = e->next;
            }

            equeue_mutex_unlock(&q->memlock);
            return e;
        }
    }

    // otherwise allocate a new chunk out of the slab
    if (q->slab.size >= size) {
        struct equeue_event *e = (struct equeue_event *)q->slab.data;
        q->slab.data += size;
        q->slab.size -= size;
        e->size = size;
        e->id = 1;

        equeue_mutex_unlock(&q->memlock);
        return e;
    }

    equeue_mutex_unlock(&q->memlock);
    return 0;
}

static void equeue_mem_dealloc(equeue_t *q, struct equeue_event *e)
{
    equeue_mutex_lock(&q->memlock);

    // stick chunk into list of chunks
    struct equeue_event **p = &q->chunks;
    while (*p && (*p)->size < e->size) {
        p = &(*p)->next;
    }

    if (*p && (*p)->size == e->size) {
        e->sibling = *p;
        e->next = (*p)->next;
    } else {
        e->sibling = 0;
        e->next = *p;
    }
    *p = e;

    equeue_mutex_unlock(&q->memlock);
}

void *equeue_alloc(equeue_t *q, size_t size)
{
    struct equeue_event *e = equeue_mem_alloc(q, size);
    if (!e) {
        return 0;
    }

    e->target = 0;
    e->period = -1;
    e->dtor = 0;

    return e + 1;
}

void equeue_dealloc(equeue_t *q, void *p)
{
    struct equeue_event *e = (struct equeue_event *)p - 1;

    if (e->dtor) {
        e->dtor(e + 1);
    }

    if (EQUEUE_IS_USER_ALLOCATED_EVENT(e)) {
        e->id = EQUEUE_USER_ALLOCATED_EVENT_STATE_DONE;
    } else {
        equeue_mem_dealloc(q, e);
    }
}

void equeue_enqueue(equeue_t *q, struct equeue_event *e, unsigned tick)
{
    e->target = tick + equeue_clampdiff(e->target, tick);
    e->generation = q->generation;

    equeue_mutex_lock(&q->queuelock);

    // find the event slot
    struct equeue_event **p = &q->queue;
    while (*p && equeue_tickdiff((*p)->target, e->target) < 0) {
        p = &(*p)->next;
    }

    // insert at head in slot
    if (*p && (*p)->target == e->target) {
        e->next = (*p)->next;
        if (e->next) {
            e->next->ref = &e->next;
        }
        e->sibling = *p;
        e->sibling->next = 0;
        e->sibling->ref = &e->sibling;
    } else {
        e->next = *p;
        if (e->next) {
            e->next->ref = &e->next;
        }
        e->sibling = 0;
    }

    *p = e;
    e->ref = p;

    // notify background timer
    if ((q->background.update && q->background.active) &&
            (q->queue == e && !e->sibling)) {
        q->background.update(q->background.timer,
                             equeue_clampdiff(e->target, tick));
    }
    equeue_mutex_unlock(&q->queuelock);
}

// equeue scheduling functions
static int equeue_event_id(equeue_t *q, struct equeue_event *e)
{
    // setup event and hash local id with buffer offset for unique id
    return ((unsigned)e->id << q->npw2) | ((unsigned char *)e - q->buffer);
}

static struct equeue_event *equeue_unqueue_by_address(equeue_t *q, struct equeue_event *e)
{
    equeue_mutex_lock(&q->queuelock);
    // clear the event and check if already in-flight
    e->cb = 0;
    e->period = -1;

    int diff = equeue_tickdiff(e->target, q->tick);
    if (diff < 0 || (diff == 0 && e->generation != q->generation)) {
        equeue_mutex_unlock(&q->queuelock);
        return 0;
    }

    // disentangle from queue
    if (e->sibling) {
        e->sibling->next = e->next;
        if (e->sibling->next) {
            e->sibling->next->ref = &e->sibling->next;
        }

        *e->ref = e->sibling;
        e->sibling->ref = e->ref;
    } else {
        *e->ref = e->next;
        if (e->next) {
            e->next->ref = e->ref;
        }
    }
    equeue_mutex_unlock(&q->queuelock);
    return e;
}

static struct equeue_event *equeue_unqueue_by_id(equeue_t *q, int id)
{
    // decode event from unique id and check that the local id matches
    struct equeue_event *e = (struct equeue_event *)
                             &q->buffer[id & ((1u << q->npw2) - 1u)];

    equeue_mutex_lock(&q->queuelock);
    if (e->id != (unsigned)id >> q->npw2) {
        equeue_mutex_unlock(&q->queuelock);
        return 0;
    }

    if (0 == equeue_unqueue_by_address(q, e)) {
        equeue_mutex_unlock(&q->queuelock);
        return 0;
    }

    equeue_incid(q, e);
    equeue_mutex_unlock(&q->queuelock);

    return e;
}

static struct equeue_event *equeue_dequeue(equeue_t *q, unsigned target)
{
    equeue_mutex_lock(&q->queuelock);

    // find all expired events

    if (equeue_tickdiff(q->tick, target) <= 0) {
        q->tick = target;
    }

    struct equeue_event *head = q->queue;
    struct equeue_event **p = &head;
    while (*p && equeue_tickdiff((*p)->target, target) <= 0) {
        p = &(*p)->next;
    }

    q->queue = *p;
    if (q->queue) {
        q->queue->ref = &q->queue;
    }

    *p = 0;

    /* we only increment the generation if events have been taken off the queue
     * as this is the only time cancellation may conflict with dequeueing */
    if (head) {
        q->generation += 1;
    }

    equeue_mutex_unlock(&q->queuelock);

    // reverse and flatten each slot to match insertion order
    struct equeue_event **tail = &head;
    struct equeue_event *ess = head;
    while (ess) {
        struct equeue_event *es = ess;
        ess = es->next;

        struct equeue_event *prev = 0;
        for (struct equeue_event *e = es; e; e = e->sibling) {
            e->next = prev;
            prev = e;
        }

        *tail = prev;
        tail = &es->next;
    }

    return head;
}

int equeue_post(equeue_t *q, void (*cb)(void *), void *p)
{
    struct equeue_event *e = (struct equeue_event *)p - 1;
    unsigned tick = equeue_tick();
    e->cb = cb;
    e->target = tick + e->target;

    equeue_enqueue(q, e, tick);
    int id = equeue_event_id(q, e);
    equeue_sema_signal(&q->eventsema);
    return id;
}

void equeue_post_user_allocated(equeue_t *q, void (*cb)(void *), void *p)
{
    struct equeue_event *e = (struct equeue_event *)p;
    unsigned tick = equeue_tick();
    e->cb = cb;
    e->target = tick + e->target;
    e->id = EQUEUE_USER_ALLOCATED_EVENT_STATE_INPROGRESS;

    equeue_enqueue(q, e, tick);
    equeue_sema_signal(&q->eventsema);
}

bool equeue_cancel(equeue_t *q, int id)
{
    if (!id) {
        return false;
    }

    struct equeue_event *e = equeue_unqueue_by_id(q, id);
    if (e) {
        equeue_dealloc(q, e + 1);
        return true;
    } else {
        return false;
    }
}

bool equeue_cancel_user_allocated(equeue_t *q, void *e)
{
    if (!e || ((struct equeue_event *)e)->id == EQUEUE_USER_ALLOCATED_EVENT_STATE_DONE) {
        return false;
    }

    struct equeue_event *_e = equeue_unqueue_by_address(q, e);
    if (_e) {
        equeue_dealloc(q, _e + 1);
        return true;
    } else {
        return false;
    }
}

int equeue_timeleft(equeue_t *q, int id)
{
    int ret = -1;

    if (!id) {
        return -1;
    }

    // decode event from unique id and check that the local id matches
    struct equeue_event *e = (struct equeue_event *)
                             &q->buffer[id & ((1u << q->npw2) - 1u)];

    equeue_mutex_lock(&q->queuelock);
    if (e->id == (unsigned)id >> q->npw2) {
        ret = equeue_clampdiff(e->target, equeue_tick());
    }
    equeue_mutex_unlock(&q->queuelock);
    return ret;
}

int equeue_timeleft_user_allocated(equeue_t *q, void *e)
{
    int ret = -1;

    if (!e) {
        return -1;
    }

    struct equeue_event *_e = (struct equeue_event *)e;
    equeue_mutex_lock(&q->queuelock);
    ret = equeue_clampdiff(_e->target, equeue_tick());
    equeue_mutex_unlock(&q->queuelock);
    return ret;
}

void equeue_break(equeue_t *q)
{
    equeue_mutex_lock(&q->queuelock);
    q->break_requested = true;
    equeue_mutex_unlock(&q->queuelock);
    equeue_sema_signal(&q->eventsema);
}

void equeue_dispatch(equeue_t *q, int ms)
{
    unsigned tick = equeue_tick();
    unsigned timeout = tick + ms;
    q->background.active = false;

    while (1) {
        // collect all the available events and next deadline
        struct equeue_event *es = equeue_dequeue(q, tick);

        // dispatch events
        while (es) {
            struct equeue_event *e = es;
            es = e->next;

            // actually dispatch the callbacks
            void (*cb)(void *) = e->cb;
            if (cb) {
                cb(e + 1);
            }

            // reenqueue periodic events or deallocate
            if (e->period >= 0) {
                e->target += e->period;
                equeue_enqueue(q, e, equeue_tick());
            } else {
                if (!EQUEUE_IS_USER_ALLOCATED_EVENT(e)) {
                    equeue_incid(q, e);
                }
                equeue_dealloc(q, e + 1);
            }
        }

        int deadline = -1;
        tick = equeue_tick();

        // check if we should stop dispatching soon
        if (ms >= 0) {
            deadline = equeue_tickdiff(timeout, tick);
            if (deadline <= 0) {
                // update background timer if necessary
                if (q->background.update) {
                    equeue_mutex_lock(&q->queuelock);
                    if (q->background.update && q->queue) {
                        q->background.update(q->background.timer,
                                             equeue_clampdiff(q->queue->target, tick));
                    }
                    q->background.active = true;
                    equeue_mutex_unlock(&q->queuelock);
                }
                q->break_requested = false;
                return;
            }
        }

        // find closest deadline
        equeue_mutex_lock(&q->queuelock);
        if (q->queue) {
            int diff = equeue_clampdiff(q->queue->target, tick);
            if ((unsigned)diff < (unsigned)deadline) {
                deadline = diff;
            }
        }
        equeue_mutex_unlock(&q->queuelock);

        // wait for events
        equeue_sema_wait(&q->eventsema, deadline);

        // check if we were notified to break out of dispatch
        if (q->break_requested) {
            equeue_mutex_lock(&q->queuelock);
            if (q->break_requested) {
                q->break_requested = false;
                equeue_mutex_unlock(&q->queuelock);
                return;
            }
            equeue_mutex_unlock(&q->queuelock);
        }

        // update tick for next iteration
        tick = equeue_tick();
    }
}


// event functions
void equeue_event_delay(void *p, int ms)
{
    struct equeue_event *e = (struct equeue_event *)p - 1;
    e->target = ms;
}

void equeue_event_period(void *p, int ms)
{
    struct equeue_event *e = (struct equeue_event *)p - 1;
    e->period = ms;
}

void equeue_event_dtor(void *p, void (*dtor)(void *))
{
    struct equeue_event *e = (struct equeue_event *)p - 1;
    e->dtor = dtor;
}


// simple callbacks
struct ecallback {
    void (*cb)(void *);
    void *data;
};

static void ecallback_dispatch(void *p)
{
    struct ecallback *e = (struct ecallback *)p;
    e->cb(e->data);
}

int equeue_call(equeue_t *q, void (*cb)(void *), void *data)
{
    struct ecallback *e = equeue_alloc(q, sizeof(struct ecallback));
    if (!e) {
        return 0;
    }

    e->cb = cb;
    e->data = data;
    return equeue_post(q, ecallback_dispatch, e);
}

int equeue_call_in(equeue_t *q, int ms, void (*cb)(void *), void *data)
{
    struct ecallback *e = equeue_alloc(q, sizeof(struct ecallback));
    if (!e) {
        return 0;
    }

    equeue_event_delay(e, ms);
    e->cb = cb;
    e->data = data;
    return equeue_post(q, ecallback_dispatch, e);
}

int equeue_call_every(equeue_t *q, int ms, void (*cb)(void *), void *data)
{
    struct ecallback *e = equeue_alloc(q, sizeof(struct ecallback));
    if (!e) {
        return 0;
    }

    equeue_event_delay(e, ms);
    equeue_event_period(e, ms);
    e->cb = cb;
    e->data = data;
    return equeue_post(q, ecallback_dispatch, e);
}


// backgrounding
void equeue_background(equeue_t *q,
                       void (*update)(void *timer, int ms), void *timer)
{
    equeue_mutex_lock(&q->queuelock);
    if (q->background.update) {
        q->background.update(q->background.timer, -1);
    }

    q->background.update = update;
    q->background.timer = timer;

    if (q->background.update && q->queue) {
        q->background.update(q->background.timer,
                             equeue_clampdiff(q->queue->target, equeue_tick()));
    }
    q->background.active = true;
    equeue_mutex_unlock(&q->queuelock);
}

struct equeue_chain_context {
    equeue_t *q;
    equeue_t *target;
    int id;
};

static void equeue_chain_dispatch(void *p)
{
    equeue_dispatch((equeue_t *)p, 0);
}

static void equeue_chain_update(void *p, int ms)
{
    struct equeue_chain_context *c = (struct equeue_chain_context *)p;
    (void)equeue_cancel(c->target, c->id);

    if (ms >= 0) {
        c->id = equeue_call_in(c->target, ms, equeue_chain_dispatch, c->q);
    } else {
        equeue_dealloc(c->q, c);
    }
}

int equeue_chain(equeue_t *q, equeue_t *target)
{
    if (!target) {
        equeue_background(q, 0, 0);
        return 0;
    }

    struct equeue_chain_context *c = equeue_alloc(q,
                                                  sizeof(struct equeue_chain_context));
    if (!c) {
        return -1;
    }

    c->q = q;
    c->target = target;
    c->id = 0;

    equeue_background(q, equeue_chain_update, c);
    return 0;
}
