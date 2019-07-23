/*
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
#ifndef EVENT_H
#define EVENT_H

#include "events/EventQueue.h"
#include "platform/mbed_assert.h"

namespace events {
/**
 * \addtogroup events-public-api Events
 * \ingroup mbed-os-public
 * @{
 */

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename F>
class Event;

/**
 * \defgroup events_Event Event<void()> class
 * @{
 */

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename... ArgTs>
class Event<void(ArgTs...)> {
public:
    /** Create an event
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     */
    template <typename F>
    Event(EventQueue *q, F f)
    {
        _event = static_cast<struct event *>(
                     equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));

        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &Event::event_post<F>;
            _event->dtor = &Event::event_dtor<F>;

            new (_event + 1) F(f);

            _event->ref = 1;
        }
    }

    /** Copy constructor for events
     */
    Event(const Event &e)
    {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    /** Assignment operator for events
     */
    Event &operator=(const Event &that)
    {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

    /** Destructor for events
     */
    ~Event()
    {
        if (_event) {
            _event->ref -= 1;
            if (_event->ref == 0) {
                _event->dtor(_event);
                equeue_dealloc(_event->equeue, _event);
            }
        }
    }

    /** Configure the delay of an event
     *
     *  @param delay    Millisecond delay before dispatching the event
     */
    void delay(int delay)
    {
        if (_event) {
            _event->delay = delay;
        }
    }

    /** Configure the period of an event
     *
     *  @param period   Millisecond period for repeatedly dispatching an event
     */
    void period(int period)
    {
        if (_event) {
            _event->period = period;
        }
    }

    /** Posts an event onto the underlying event queue
     *
     *  The event is posted to the underlying queue and is executed in the
     *  context of the event queue's dispatch loop.
     *
     *  The post function is IRQ safe and can act as a mechanism for moving
     *  events out of IRQ contexts.
     *
     *  @param args     Arguments to pass to the event
     *  @return         A unique id that represents the posted event and can
     *                  be passed to EventQueue::cancel, or an id of 0 if
     *                  there is not enough memory to allocate the event.
     */
    int post(ArgTs... args) const
    {
        if (!_event) {
            return 0;
        }

        _event->id = _event->post(_event, args...);
        return _event->id;
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param args     Arguments to pass to the event
     */
    void call(ArgTs... args) const
    {
        MBED_UNUSED int id = post(args...);
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param args     Arguments to pass to the event
     */
    void operator()(ArgTs... args) const
    {
        return call(args...);
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Event to call passed as a void pointer
     *  @param args     Arguments to pass to the event
     */
    static void thunk(void *func, ArgTs... args)
    {
        return static_cast<Event *>(func)->call(args...);
    }

    /** Cancels the most recently posted event
     *
     *  Attempts to cancel the most recently posted event. It is safe to call
     *  cancel after an event has already been dispatched.
     *
     *  The cancel function is IRQ safe.
     *
     *  If called while the event queue's dispatch loop is active, the cancel
     *  function does not guarantee that the event will not execute after it
     *  returns, as the event may have already begun executing.
     */
    void cancel() const
    {
        if (_event) {
            equeue_cancel(_event->equeue, _event->id);
        }
    }

private:
    struct event {
        unsigned ref;
        equeue_t *equeue;
        int id;

        int delay;
        int period;

        int (*post)(struct event *, ArgTs... args);
        void (*dtor)(struct event *);

        // F follows
    } *_event;

    // Event attributes
    template <typename F>
    static int event_post(struct event *e, ArgTs... args)
    {
        typedef EventQueue::context<F, ArgTs...> C;
        void *p = equeue_alloc(e->equeue, sizeof(C));
        if (!p) {
            return 0;
        }

        new (p) C(*(F *)(e + 1), args...);
        equeue_event_delay(p, e->delay);
        equeue_event_period(p, e->period);
        equeue_event_dtor(p, &EventQueue::function_dtor<C>);
        return equeue_post(e->equeue, &EventQueue::function_call<C>, p);
    }

    template <typename F>
    static void event_dtor(struct event *e)
    {
        ((F *)(e + 1))->~F();
    }

public:
    /** Create an event
     *  @param q                Event queue to dispatch on
     *  @param f                Function to execute when the event is dispatched
     *  @param context_args     Arguments to bind to the callback, these arguments are
     *                          allocated on an IRQ-safe allocator from the event queue's
     *                          memory pool. Must be type-compatible with bound_args, the
     *                          arguments to the underlying callback.
     */
    template <typename F, typename... ContextArgTs>
    Event(EventQueue *q, F f, ContextArgTs... context_args) :
        Event(q, EventQueue::context<F, ContextArgTs...>(f, context_args...)) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, T *obj, R(T::*method)(B0, ArgTs...), B0 b0) :
        Event(q, mbed::callback(obj, method), b0) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const T *obj, R(T::*method)(B0, ArgTs...) const, B0 b0) :
        Event(q, mbed::callback(obj, method), b0) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, volatile T *obj, R(T::*method)(B0, ArgTs...) volatile, B0 b0) :
        Event(q, mbed::callback(obj, method), b0) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const volatile T *obj, R(T::*method)(B0, ArgTs...) const volatile, B0 b0) :
        Event(q, mbed::callback(obj, method), b0) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, T *obj, R(T::*method)(B0, B1, ArgTs...), B0 b0, B1 b1) :
        Event(q, mbed::callback(obj, method), b0, b1) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const T *obj, R(T::*method)(B0, B1, ArgTs...) const, B0 b0, B1 b1) :
        Event(q, mbed::callback(obj, method), b0, b1) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, volatile T *obj, R(T::*method)(B0, B1, ArgTs...) volatile, B0 b0, B1 b1) :
        Event(q, mbed::callback(obj, method), b0, b1) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const volatile T *obj, R(T::*method)(B0, B1, ArgTs...) const volatile, B0 b0, B1 b1) :
        Event(q, mbed::callback(obj, method), b0, b1) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, T *obj, R(T::*method)(B0, B1, B2, ArgTs...), B0 b0, B1 b1, B2 b2) :
        Event(q, mbed::callback(obj, method), b0, b1, b2) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const T *obj, R(T::*method)(B0, B1, B2, ArgTs...) const, B0 b0, B1 b1, B2 b2) :
        Event(q, mbed::callback(obj, method), b0, b1, b2) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, volatile T *obj, R(T::*method)(B0, B1, B2, ArgTs...) volatile, B0 b0, B1 b1, B2 b2) :
        Event(q, mbed::callback(obj, method), b0, b1, b2) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const volatile T *obj, R(T::*method)(B0, B1, B2, ArgTs...) const volatile, B0 b0, B1 b1, B2 b2) :
        Event(q, mbed::callback(obj, method), b0, b1, b2) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, T *obj, R(T::*method)(B0, B1, B2, B3, ArgTs...), B0 b0, B1 b1, B2 b2, B3 b3) :
        Event(q, mbed::callback(obj, method), b0, b1, b2, b3) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const T *obj, R(T::*method)(B0, B1, B2, B3, ArgTs...) const, B0 b0, B1 b1, B2 b2, B3 b3) :
        Event(q, mbed::callback(obj, method), b0, b1, b2, b3) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, volatile T *obj, R(T::*method)(B0, B1, B2, B3, ArgTs...) volatile, B0 b0, B1 b1, B2 b2, B3 b3) :
        Event(q, mbed::callback(obj, method), b0, b1, b2, b3) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const volatile T *obj, R(T::*method)(B0, B1, B2, B3, ArgTs...) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) :
        Event(q, mbed::callback(obj, method), b0, b1, b2, b3) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, T *obj, R(T::*method)(B0, B1, B2, B3, B4, ArgTs...), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) :
        Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const T *obj, R(T::*method)(B0, B1, B2, B3, B4, ArgTs...) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) :
        Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, volatile T *obj, R(T::*method)(B0, B1, B2, B3, B4, ArgTs...) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) :
        Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4) { }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const volatile T *obj, R(T::*method)(B0, B1, B2, B3, B4, ArgTs...) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) :
        Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4) { }
};

/** @}*/


// Convenience functions declared here to avoid cyclic
// dependency between Event and EventQueue
template <typename R, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(R(*func)(ArgTs...))
{
    return Event<void(ArgTs...)>(this, func);
}

template <typename T, typename R, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(T *obj, R(T::*method)(ArgTs...))
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(const T *obj, R(T::*method)(ArgTs...) const)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(volatile T *obj, R(T::*method)(ArgTs...) volatile)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(const volatile T *obj, R(T::*method)(ArgTs...) const volatile)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method));
}

template <typename R, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(mbed::Callback<R(ArgTs...)> cb)
{
    return Event<void(ArgTs...)>(this, cb);
}

template <typename R, typename B0, typename C0, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(R(*func)(B0, ArgTs...), C0 c0)
{
    return Event<void(ArgTs...)>(this, func, c0);
}

template <typename T, typename R, typename B0, typename C0, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(T *obj, R(T::*method)(B0, ArgTs...), C0 c0)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(const T *obj, R(T::*method)(B0, ArgTs...) const, C0 c0)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(volatile T *obj, R(T::*method)(B0, ArgTs...) volatile, C0 c0)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(const volatile T *obj, R(T::*method)(B0, ArgTs...) const volatile, C0 c0)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0);
}

template <typename R, typename B0, typename C0, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(mbed::Callback<R(B0, ArgTs...)> cb, C0 c0)
{
    return Event<void(ArgTs...)>(this, cb, c0);
}

template <typename R, typename B0, typename B1, typename C0, typename C1, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(R(*func)(B0, B1, ArgTs...), C0 c0, C1 c1)
{
    return Event<void(ArgTs...)>(this, func, c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(T *obj, R(T::*method)(B0, B1, ArgTs...), C0 c0, C1 c1)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(const T *obj, R(T::*method)(B0, B1, ArgTs...) const, C0 c0, C1 c1)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(volatile T *obj, R(T::*method)(B0, B1, ArgTs...) volatile, C0 c0, C1 c1)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(const volatile T *obj, R(T::*method)(B0, B1, ArgTs...) const volatile, C0 c0, C1 c1)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename R, typename B0, typename B1, typename C0, typename C1, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(mbed::Callback<R(B0, B1, ArgTs...)> cb, C0 c0, C1 c1)
{
    return Event<void(ArgTs...)>(this, cb, c0, c1);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(R(*func)(B0, B1, B2, ArgTs...), C0 c0, C1 c1, C2 c2)
{
    return Event<void(ArgTs...)>(this, func, c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(T *obj, R(T::*method)(B0, B1, B2, ArgTs...), C0 c0, C1 c1, C2 c2)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(const T *obj, R(T::*method)(B0, B1, B2, ArgTs...) const, C0 c0, C1 c1, C2 c2)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(volatile T *obj, R(T::*method)(B0, B1, B2, ArgTs...) volatile, C0 c0, C1 c1, C2 c2)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(const volatile T *obj, R(T::*method)(B0, B1, B2, ArgTs...) const volatile, C0 c0, C1 c1, C2 c2)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(mbed::Callback<R(B0, B1, B2, ArgTs...)> cb, C0 c0, C1 c1, C2 c2)
{
    return Event<void(ArgTs...)>(this, cb, c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(R(*func)(B0, B1, B2, B3, ArgTs...), C0 c0, C1 c1, C2 c2, C3 c3)
{
    return Event<void(ArgTs...)>(this, func, c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(T *obj, R(T::*method)(B0, B1, B2, B3, ArgTs...), C0 c0, C1 c1, C2 c2, C3 c3)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(const T *obj, R(T::*method)(B0, B1, B2, B3, ArgTs...) const, C0 c0, C1 c1, C2 c2, C3 c3)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(volatile T *obj, R(T::*method)(B0, B1, B2, B3, ArgTs...) volatile, C0 c0, C1 c1, C2 c2, C3 c3)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(const volatile T *obj, R(T::*method)(B0, B1, B2, B3, ArgTs...) const volatile, C0 c0, C1 c1, C2 c2, C3 c3)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, ArgTs...)> cb, C0 c0, C1 c1, C2 c2, C3 c3)
{
    return Event<void(ArgTs...)>(this, cb, c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(R(*func)(B0, B1, B2, B3, B4, ArgTs...), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4)
{
    return Event<void(ArgTs...)>(this, func, c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(T *obj, R(T::*method)(B0, B1, B2, B3, B4, ArgTs...), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(const T *obj, R(T::*method)(B0, B1, B2, B3, B4, ArgTs...) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(volatile T *obj, R(T::*method)(B0, B1, B2, B3, B4, ArgTs...) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(const volatile T *obj, R(T::*method)(B0, B1, B2, B3, B4, ArgTs...) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4)
{
    return Event<void(ArgTs...)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename... ArgTs>
Event<void(ArgTs...)> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, B4, ArgTs...)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4)
{
    return Event<void(ArgTs...)>(this, cb, c0, c1, c2, c3, c4);
}

/** @}*/
}
#endif
