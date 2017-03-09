/* events
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
#ifndef EVENT_H
#define EVENT_H

#include "events/EventQueue.h"
#include "platform/mbed_assert.h"

namespace events {
/** \addtogroup events */
/** @{*/

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename F>
class Event;

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <>
class Event<void()> {
public:
    /** Create an event
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param c0..c4   Arguments to bind to the callback, these arguments are
     *                  allocated on an irq-safe allocator from the event queue's
     *                  memory pool. Must be type-compatible with b0..b4, the
     *                  arguments to the underlying callback.
     */
    template <typename F>
    Event(EventQueue *q, F f) {
        _event = static_cast<struct event *>(
                equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));

        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &Event::event_post<F>;
            _event->dtor = &Event::event_dtor<F>;

            new (_event+1) F(f);

            _event->ref = 1;
        }
    }

    /** Copy constructor for events
     */
    Event(const Event &e) {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    /** Assignment operator for events
     */
    Event &operator=(const Event &that) {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

    /** Destructor for events
     */
    ~Event() {
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
    void delay(int delay) {
        if (_event) {
            _event->delay = delay;
        }
    }

    /** Configure the period of an event
     *
     *  @param period   Millisecond period for repeatedly dispatching an event
     */
    void period(int period) {
        if (_event) {
            _event->period = period;
        }
    }

    /** Posts an event onto the underlying event queue
     *
     *  The event is posted to the underlying queue and is executed in the
     *  context of the event queue's dispatch loop.
     *
     *  The post function is irq safe and can act as a mechanism for moving
     *  events out of irq contexts.
     *
     *  @param a0..a4   Arguments to pass to the event
     *  @return         A unique id that represents the posted event and can
     *                  be passed to EventQueue::cancel, or an id of 0 if
     *                  there is not enough memory to allocate the event.
     */
    int post() const {
        if (!_event) {
            return 0;
        }

        _event->id = _event->post(_event);
        return _event->id;
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void call() const {
        MBED_UNUSED int id = post();
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void operator()() const {
        return call();
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Event to call passed as a void pointer
     *  @param a0..a4   Arguments to pass to the event
     */
    static void thunk(void *func) {
        return static_cast<Event*>(func)->call();
    }

    /** Cancels the most recently posted event
     *
     *  Attempts to cancel the most recently posted event. It is safe to call
     *  cancel after an event has already been dispatched.
     *
     *  The cancel function is irq safe.
     *
     *  If called while the event queue's dispatch loop is active, the cancel
     *  function does not garuntee that the event will not execute after it
     *  returns, as the event may have already begun executing.
     */
    void cancel() const {
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

        int (*post)(struct event *);
        void (*dtor)(struct event *);

        // F follows
    } *_event;

    // Event attributes
    template <typename F>
    static int event_post(struct event *e) {
        typedef EventQueue::context00<F> C;
        void *p = equeue_alloc(e->equeue, sizeof(C));
        if (!p) {
            return 0;
        }

        new (p) C(*(F*)(e + 1));
        equeue_event_delay(p, e->delay);
        equeue_event_period(p, e->period);
        equeue_event_dtor(p, &EventQueue::function_dtor<C>);
        return equeue_post(e->equeue, &EventQueue::function_call<C>, p);
    }

    template <typename F>
    static void event_dtor(struct event *e) {
        ((F*)(e + 1))->~F();
    }

public:
    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0>
    Event(EventQueue *q, F f, C0 c0) {
        new (this) Event(q, EventQueue::context10<F, C0>(f, c0));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1>
    Event(EventQueue *q, F f, C0 c0, C1 c1) {
        new (this) Event(q, EventQueue::context20<F, C0, C1>(f, c0, c1));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2) {
        new (this) Event(q, EventQueue::context30<F, C0, C1, C2>(f, c0, c1, c2));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2, C3 c3) {
        new (this) Event(q, EventQueue::context40<F, C0, C1, C2, C3>(f, c0, c1, c2, c3));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
        new (this) Event(q, EventQueue::context50<F, C0, C1, C2, C3, C4>(f, c0, c1, c2, c3, c4));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, T *obj, R (T::*method)(B0), B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0) const, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0) volatile, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0) const volatile, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1), B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1) const, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1) volatile, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1) const volatile, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2), B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2) const, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2) volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2) const volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, B3), B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, B3) const, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, B3) volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, B3) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, B3, B4), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, B3, B4) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }
};

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename A0>
class Event<void(A0)> {
public:
    /** Create an event
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param c0..c4   Arguments to bind to the callback, these arguments are
     *                  allocated on an irq-safe allocator from the event queue's
     *                  memory pool. Must be type-compatible with b0..b4, the
     *                  arguments to the underlying callback.
     */
    template <typename F>
    Event(EventQueue *q, F f) {
        _event = static_cast<struct event *>(
                equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));

        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &Event::event_post<F>;
            _event->dtor = &Event::event_dtor<F>;

            new (_event+1) F(f);

            _event->ref = 1;
        }
    }

    /** Copy constructor for events
     */
    Event(const Event &e) {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    /** Assignment operator for events
     */
    Event &operator=(const Event &that) {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

    /** Destructor for events
     */
    ~Event() {
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
    void delay(int delay) {
        if (_event) {
            _event->delay = delay;
        }
    }

    /** Configure the period of an event
     *
     *  @param period   Millisecond period for repeatedly dispatching an event
     */
    void period(int period) {
        if (_event) {
            _event->period = period;
        }
    }

    /** Posts an event onto the underlying event queue
     *
     *  The event is posted to the underlying queue and is executed in the
     *  context of the event queue's dispatch loop.
     *
     *  The post function is irq safe and can act as a mechanism for moving
     *  events out of irq contexts.
     *
     *  @param a0..a4   Arguments to pass to the event
     *  @return         A unique id that represents the posted event and can
     *                  be passed to EventQueue::cancel, or an id of 0 if
     *                  there is not enough memory to allocate the event.
     */
    int post(A0 a0) const {
        if (!_event) {
            return 0;
        }

        _event->id = _event->post(_event, a0);
        return _event->id;
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void call(A0 a0) const {
        MBED_UNUSED int id = post(a0);
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void operator()(A0 a0) const {
        return call(a0);
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Event to call passed as a void pointer
     *  @param a0..a4   Arguments to pass to the event
     */
    static void thunk(void *func, A0 a0) {
        return static_cast<Event*>(func)->call(a0);
    }

    /** Cancels the most recently posted event
     *
     *  Attempts to cancel the most recently posted event. It is safe to call
     *  cancel after an event has already been dispatched.
     *
     *  The cancel function is irq safe.
     *
     *  If called while the event queue's dispatch loop is active, the cancel
     *  function does not garuntee that the event will not execute after it
     *  returns, as the event may have already begun executing.
     */
    void cancel() const {
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

        int (*post)(struct event *, A0 a0);
        void (*dtor)(struct event *);

        // F follows
    } *_event;

    // Event attributes
    template <typename F>
    static int event_post(struct event *e, A0 a0) {
        typedef EventQueue::context10<F, A0> C;
        void *p = equeue_alloc(e->equeue, sizeof(C));
        if (!p) {
            return 0;
        }

        new (p) C(*(F*)(e + 1), a0);
        equeue_event_delay(p, e->delay);
        equeue_event_period(p, e->period);
        equeue_event_dtor(p, &EventQueue::function_dtor<C>);
        return equeue_post(e->equeue, &EventQueue::function_call<C>, p);
    }

    template <typename F>
    static void event_dtor(struct event *e) {
        ((F*)(e + 1))->~F();
    }

public:
    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0>
    Event(EventQueue *q, F f, C0 c0) {
        new (this) Event(q, EventQueue::context11<F, C0, A0>(f, c0));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1>
    Event(EventQueue *q, F f, C0 c0, C1 c1) {
        new (this) Event(q, EventQueue::context21<F, C0, C1, A0>(f, c0, c1));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2) {
        new (this) Event(q, EventQueue::context31<F, C0, C1, C2, A0>(f, c0, c1, c2));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2, C3 c3) {
        new (this) Event(q, EventQueue::context41<F, C0, C1, C2, C3, A0>(f, c0, c1, c2, c3));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
        new (this) Event(q, EventQueue::context51<F, C0, C1, C2, C3, C4, A0>(f, c0, c1, c2, c3, c4));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, A0), B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, A0) const, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, A0) volatile, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, A0) const volatile, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, A0), B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, A0) const, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, A0) volatile, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, A0) const volatile, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, A0), B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, A0) const, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, A0) volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, A0) const volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, B3, A0), B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, B3, A0) const, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0) volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }
};

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename A0, typename A1>
class Event<void(A0, A1)> {
public:
    /** Create an event
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param c0..c4   Arguments to bind to the callback, these arguments are
     *                  allocated on an irq-safe allocator from the event queue's
     *                  memory pool. Must be type-compatible with b0..b4, the
     *                  arguments to the underlying callback.
     */
    template <typename F>
    Event(EventQueue *q, F f) {
        _event = static_cast<struct event *>(
                equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));

        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &Event::event_post<F>;
            _event->dtor = &Event::event_dtor<F>;

            new (_event+1) F(f);

            _event->ref = 1;
        }
    }

    /** Copy constructor for events
     */
    Event(const Event &e) {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    /** Assignment operator for events
     */
    Event &operator=(const Event &that) {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

    /** Destructor for events
     */
    ~Event() {
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
    void delay(int delay) {
        if (_event) {
            _event->delay = delay;
        }
    }

    /** Configure the period of an event
     *
     *  @param period   Millisecond period for repeatedly dispatching an event
     */
    void period(int period) {
        if (_event) {
            _event->period = period;
        }
    }

    /** Posts an event onto the underlying event queue
     *
     *  The event is posted to the underlying queue and is executed in the
     *  context of the event queue's dispatch loop.
     *
     *  The post function is irq safe and can act as a mechanism for moving
     *  events out of irq contexts.
     *
     *  @param a0..a4   Arguments to pass to the event
     *  @return         A unique id that represents the posted event and can
     *                  be passed to EventQueue::cancel, or an id of 0 if
     *                  there is not enough memory to allocate the event.
     */
    int post(A0 a0, A1 a1) const {
        if (!_event) {
            return 0;
        }

        _event->id = _event->post(_event, a0, a1);
        return _event->id;
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void call(A0 a0, A1 a1) const {
        MBED_UNUSED int id = post(a0, a1);
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void operator()(A0 a0, A1 a1) const {
        return call(a0, a1);
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Event to call passed as a void pointer
     *  @param a0..a4   Arguments to pass to the event
     */
    static void thunk(void *func, A0 a0, A1 a1) {
        return static_cast<Event*>(func)->call(a0, a1);
    }

    /** Cancels the most recently posted event
     *
     *  Attempts to cancel the most recently posted event. It is safe to call
     *  cancel after an event has already been dispatched.
     *
     *  The cancel function is irq safe.
     *
     *  If called while the event queue's dispatch loop is active, the cancel
     *  function does not garuntee that the event will not execute after it
     *  returns, as the event may have already begun executing.
     */
    void cancel() const {
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

        int (*post)(struct event *, A0 a0, A1 a1);
        void (*dtor)(struct event *);

        // F follows
    } *_event;

    // Event attributes
    template <typename F>
    static int event_post(struct event *e, A0 a0, A1 a1) {
        typedef EventQueue::context20<F, A0, A1> C;
        void *p = equeue_alloc(e->equeue, sizeof(C));
        if (!p) {
            return 0;
        }

        new (p) C(*(F*)(e + 1), a0, a1);
        equeue_event_delay(p, e->delay);
        equeue_event_period(p, e->period);
        equeue_event_dtor(p, &EventQueue::function_dtor<C>);
        return equeue_post(e->equeue, &EventQueue::function_call<C>, p);
    }

    template <typename F>
    static void event_dtor(struct event *e) {
        ((F*)(e + 1))->~F();
    }

public:
    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0>
    Event(EventQueue *q, F f, C0 c0) {
        new (this) Event(q, EventQueue::context12<F, C0, A0, A1>(f, c0));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1>
    Event(EventQueue *q, F f, C0 c0, C1 c1) {
        new (this) Event(q, EventQueue::context22<F, C0, C1, A0, A1>(f, c0, c1));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2) {
        new (this) Event(q, EventQueue::context32<F, C0, C1, C2, A0, A1>(f, c0, c1, c2));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2, C3 c3) {
        new (this) Event(q, EventQueue::context42<F, C0, C1, C2, C3, A0, A1>(f, c0, c1, c2, c3));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
        new (this) Event(q, EventQueue::context52<F, C0, C1, C2, C3, C4, A0, A1>(f, c0, c1, c2, c3, c4));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, A0, A1), B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, A0, A1) const, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, A0, A1) volatile, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, A0, A1) const volatile, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, A0, A1), B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, A0, A1) const, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, A0, A1) volatile, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, A0, A1) const volatile, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, A0, A1), B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, A0, A1) const, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1) volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1) const volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1), B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1) const, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1) volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }
};

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename A0, typename A1, typename A2>
class Event<void(A0, A1, A2)> {
public:
    /** Create an event
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param c0..c4   Arguments to bind to the callback, these arguments are
     *                  allocated on an irq-safe allocator from the event queue's
     *                  memory pool. Must be type-compatible with b0..b4, the
     *                  arguments to the underlying callback.
     */
    template <typename F>
    Event(EventQueue *q, F f) {
        _event = static_cast<struct event *>(
                equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));

        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &Event::event_post<F>;
            _event->dtor = &Event::event_dtor<F>;

            new (_event+1) F(f);

            _event->ref = 1;
        }
    }

    /** Copy constructor for events
     */
    Event(const Event &e) {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    /** Assignment operator for events
     */
    Event &operator=(const Event &that) {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

    /** Destructor for events
     */
    ~Event() {
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
    void delay(int delay) {
        if (_event) {
            _event->delay = delay;
        }
    }

    /** Configure the period of an event
     *
     *  @param period   Millisecond period for repeatedly dispatching an event
     */
    void period(int period) {
        if (_event) {
            _event->period = period;
        }
    }

    /** Posts an event onto the underlying event queue
     *
     *  The event is posted to the underlying queue and is executed in the
     *  context of the event queue's dispatch loop.
     *
     *  The post function is irq safe and can act as a mechanism for moving
     *  events out of irq contexts.
     *
     *  @param a0..a4   Arguments to pass to the event
     *  @return         A unique id that represents the posted event and can
     *                  be passed to EventQueue::cancel, or an id of 0 if
     *                  there is not enough memory to allocate the event.
     */
    int post(A0 a0, A1 a1, A2 a2) const {
        if (!_event) {
            return 0;
        }

        _event->id = _event->post(_event, a0, a1, a2);
        return _event->id;
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void call(A0 a0, A1 a1, A2 a2) const {
        MBED_UNUSED int id = post(a0, a1, a2);
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void operator()(A0 a0, A1 a1, A2 a2) const {
        return call(a0, a1, a2);
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Event to call passed as a void pointer
     *  @param a0..a4   Arguments to pass to the event
     */
    static void thunk(void *func, A0 a0, A1 a1, A2 a2) {
        return static_cast<Event*>(func)->call(a0, a1, a2);
    }

    /** Cancels the most recently posted event
     *
     *  Attempts to cancel the most recently posted event. It is safe to call
     *  cancel after an event has already been dispatched.
     *
     *  The cancel function is irq safe.
     *
     *  If called while the event queue's dispatch loop is active, the cancel
     *  function does not garuntee that the event will not execute after it
     *  returns, as the event may have already begun executing.
     */
    void cancel() const {
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

        int (*post)(struct event *, A0 a0, A1 a1, A2 a2);
        void (*dtor)(struct event *);

        // F follows
    } *_event;

    // Event attributes
    template <typename F>
    static int event_post(struct event *e, A0 a0, A1 a1, A2 a2) {
        typedef EventQueue::context30<F, A0, A1, A2> C;
        void *p = equeue_alloc(e->equeue, sizeof(C));
        if (!p) {
            return 0;
        }

        new (p) C(*(F*)(e + 1), a0, a1, a2);
        equeue_event_delay(p, e->delay);
        equeue_event_period(p, e->period);
        equeue_event_dtor(p, &EventQueue::function_dtor<C>);
        return equeue_post(e->equeue, &EventQueue::function_call<C>, p);
    }

    template <typename F>
    static void event_dtor(struct event *e) {
        ((F*)(e + 1))->~F();
    }

public:
    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0>
    Event(EventQueue *q, F f, C0 c0) {
        new (this) Event(q, EventQueue::context13<F, C0, A0, A1, A2>(f, c0));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1>
    Event(EventQueue *q, F f, C0 c0, C1 c1) {
        new (this) Event(q, EventQueue::context23<F, C0, C1, A0, A1, A2>(f, c0, c1));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2) {
        new (this) Event(q, EventQueue::context33<F, C0, C1, C2, A0, A1, A2>(f, c0, c1, c2));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2, C3 c3) {
        new (this) Event(q, EventQueue::context43<F, C0, C1, C2, C3, A0, A1, A2>(f, c0, c1, c2, c3));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
        new (this) Event(q, EventQueue::context53<F, C0, C1, C2, C3, C4, A0, A1, A2>(f, c0, c1, c2, c3, c4));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, A0, A1, A2), B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, A0, A1, A2) const, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, A0, A1, A2) volatile, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, A0, A1, A2) const volatile, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, A0, A1, A2), B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, A0, A1, A2) const, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2) volatile, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2) const volatile, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2), B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2) const, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2) volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2) const volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2), B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2) const, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2) volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }
};

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename A0, typename A1, typename A2, typename A3>
class Event<void(A0, A1, A2, A3)> {
public:
    /** Create an event
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param c0..c4   Arguments to bind to the callback, these arguments are
     *                  allocated on an irq-safe allocator from the event queue's
     *                  memory pool. Must be type-compatible with b0..b4, the
     *                  arguments to the underlying callback.
     */
    template <typename F>
    Event(EventQueue *q, F f) {
        _event = static_cast<struct event *>(
                equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));

        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &Event::event_post<F>;
            _event->dtor = &Event::event_dtor<F>;

            new (_event+1) F(f);

            _event->ref = 1;
        }
    }

    /** Copy constructor for events
     */
    Event(const Event &e) {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    /** Assignment operator for events
     */
    Event &operator=(const Event &that) {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

    /** Destructor for events
     */
    ~Event() {
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
    void delay(int delay) {
        if (_event) {
            _event->delay = delay;
        }
    }

    /** Configure the period of an event
     *
     *  @param period   Millisecond period for repeatedly dispatching an event
     */
    void period(int period) {
        if (_event) {
            _event->period = period;
        }
    }

    /** Posts an event onto the underlying event queue
     *
     *  The event is posted to the underlying queue and is executed in the
     *  context of the event queue's dispatch loop.
     *
     *  The post function is irq safe and can act as a mechanism for moving
     *  events out of irq contexts.
     *
     *  @param a0..a4   Arguments to pass to the event
     *  @return         A unique id that represents the posted event and can
     *                  be passed to EventQueue::cancel, or an id of 0 if
     *                  there is not enough memory to allocate the event.
     */
    int post(A0 a0, A1 a1, A2 a2, A3 a3) const {
        if (!_event) {
            return 0;
        }

        _event->id = _event->post(_event, a0, a1, a2, a3);
        return _event->id;
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void call(A0 a0, A1 a1, A2 a2, A3 a3) const {
        MBED_UNUSED int id = post(a0, a1, a2, a3);
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void operator()(A0 a0, A1 a1, A2 a2, A3 a3) const {
        return call(a0, a1, a2, a3);
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Event to call passed as a void pointer
     *  @param a0..a4   Arguments to pass to the event
     */
    static void thunk(void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
        return static_cast<Event*>(func)->call(a0, a1, a2, a3);
    }

    /** Cancels the most recently posted event
     *
     *  Attempts to cancel the most recently posted event. It is safe to call
     *  cancel after an event has already been dispatched.
     *
     *  The cancel function is irq safe.
     *
     *  If called while the event queue's dispatch loop is active, the cancel
     *  function does not garuntee that the event will not execute after it
     *  returns, as the event may have already begun executing.
     */
    void cancel() const {
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

        int (*post)(struct event *, A0 a0, A1 a1, A2 a2, A3 a3);
        void (*dtor)(struct event *);

        // F follows
    } *_event;

    // Event attributes
    template <typename F>
    static int event_post(struct event *e, A0 a0, A1 a1, A2 a2, A3 a3) {
        typedef EventQueue::context40<F, A0, A1, A2, A3> C;
        void *p = equeue_alloc(e->equeue, sizeof(C));
        if (!p) {
            return 0;
        }

        new (p) C(*(F*)(e + 1), a0, a1, a2, a3);
        equeue_event_delay(p, e->delay);
        equeue_event_period(p, e->period);
        equeue_event_dtor(p, &EventQueue::function_dtor<C>);
        return equeue_post(e->equeue, &EventQueue::function_call<C>, p);
    }

    template <typename F>
    static void event_dtor(struct event *e) {
        ((F*)(e + 1))->~F();
    }

public:
    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0>
    Event(EventQueue *q, F f, C0 c0) {
        new (this) Event(q, EventQueue::context14<F, C0, A0, A1, A2, A3>(f, c0));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1>
    Event(EventQueue *q, F f, C0 c0, C1 c1) {
        new (this) Event(q, EventQueue::context24<F, C0, C1, A0, A1, A2, A3>(f, c0, c1));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2) {
        new (this) Event(q, EventQueue::context34<F, C0, C1, C2, A0, A1, A2, A3>(f, c0, c1, c2));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2, C3 c3) {
        new (this) Event(q, EventQueue::context44<F, C0, C1, C2, C3, A0, A1, A2, A3>(f, c0, c1, c2, c3));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
        new (this) Event(q, EventQueue::context54<F, C0, C1, C2, C3, C4, A0, A1, A2, A3>(f, c0, c1, c2, c3, c4));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, A0, A1, A2, A3), B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, A0, A1, A2, A3) const, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3) volatile, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3) const volatile, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3), B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3) const, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3) volatile, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3) const volatile, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3), B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3) const, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3) volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3) const volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3), B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3) const, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3) volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }
};

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename A0, typename A1, typename A2, typename A3, typename A4>
class Event<void(A0, A1, A2, A3, A4)> {
public:
    /** Create an event
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param c0..c4   Arguments to bind to the callback, these arguments are
     *                  allocated on an irq-safe allocator from the event queue's
     *                  memory pool. Must be type-compatible with b0..b4, the
     *                  arguments to the underlying callback.
     */
    template <typename F>
    Event(EventQueue *q, F f) {
        _event = static_cast<struct event *>(
                equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));

        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &Event::event_post<F>;
            _event->dtor = &Event::event_dtor<F>;

            new (_event+1) F(f);

            _event->ref = 1;
        }
    }

    /** Copy constructor for events
     */
    Event(const Event &e) {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    /** Assignment operator for events
     */
    Event &operator=(const Event &that) {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

    /** Destructor for events
     */
    ~Event() {
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
    void delay(int delay) {
        if (_event) {
            _event->delay = delay;
        }
    }

    /** Configure the period of an event
     *
     *  @param period   Millisecond period for repeatedly dispatching an event
     */
    void period(int period) {
        if (_event) {
            _event->period = period;
        }
    }

    /** Posts an event onto the underlying event queue
     *
     *  The event is posted to the underlying queue and is executed in the
     *  context of the event queue's dispatch loop.
     *
     *  The post function is irq safe and can act as a mechanism for moving
     *  events out of irq contexts.
     *
     *  @param a0..a4   Arguments to pass to the event
     *  @return         A unique id that represents the posted event and can
     *                  be passed to EventQueue::cancel, or an id of 0 if
     *                  there is not enough memory to allocate the event.
     */
    int post(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const {
        if (!_event) {
            return 0;
        }

        _event->id = _event->post(_event, a0, a1, a2, a3, a4);
        return _event->id;
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void call(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const {
        MBED_UNUSED int id = post(a0, a1, a2, a3, a4);
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const {
        return call(a0, a1, a2, a3, a4);
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Event to call passed as a void pointer
     *  @param a0..a4   Arguments to pass to the event
     */
    static void thunk(void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        return static_cast<Event*>(func)->call(a0, a1, a2, a3, a4);
    }

    /** Cancels the most recently posted event
     *
     *  Attempts to cancel the most recently posted event. It is safe to call
     *  cancel after an event has already been dispatched.
     *
     *  The cancel function is irq safe.
     *
     *  If called while the event queue's dispatch loop is active, the cancel
     *  function does not garuntee that the event will not execute after it
     *  returns, as the event may have already begun executing.
     */
    void cancel() const {
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

        int (*post)(struct event *, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4);
        void (*dtor)(struct event *);

        // F follows
    } *_event;

    // Event attributes
    template <typename F>
    static int event_post(struct event *e, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        typedef EventQueue::context50<F, A0, A1, A2, A3, A4> C;
        void *p = equeue_alloc(e->equeue, sizeof(C));
        if (!p) {
            return 0;
        }

        new (p) C(*(F*)(e + 1), a0, a1, a2, a3, a4);
        equeue_event_delay(p, e->delay);
        equeue_event_period(p, e->period);
        equeue_event_dtor(p, &EventQueue::function_dtor<C>);
        return equeue_post(e->equeue, &EventQueue::function_call<C>, p);
    }

    template <typename F>
    static void event_dtor(struct event *e) {
        ((F*)(e + 1))->~F();
    }

public:
    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0>
    Event(EventQueue *q, F f, C0 c0) {
        new (this) Event(q, EventQueue::context15<F, C0, A0, A1, A2, A3, A4>(f, c0));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1>
    Event(EventQueue *q, F f, C0 c0, C1 c1) {
        new (this) Event(q, EventQueue::context25<F, C0, C1, A0, A1, A2, A3, A4>(f, c0, c1));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2) {
        new (this) Event(q, EventQueue::context35<F, C0, C1, C2, A0, A1, A2, A3, A4>(f, c0, c1, c2));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2, C3 c3) {
        new (this) Event(q, EventQueue::context45<F, C0, C1, C2, C3, A0, A1, A2, A3, A4>(f, c0, c1, c2, c3));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    Event(EventQueue *q, F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
        new (this) Event(q, EventQueue::context55<F, C0, C1, C2, C3, C4, A0, A1, A2, A3, A4>(f, c0, c1, c2, c3, c4));
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4), B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4) const, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4) volatile, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4) const volatile, B0 b0) {
        new (this) Event(q, mbed::callback(obj, method), b0);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4), B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4) const, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4) volatile, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4) const volatile, B0 b0, B1 b1) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4), B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4) const, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4) volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4) const volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4), B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4) const, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4) volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create an event
     *  @see Event::Event
     */
    template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, mbed::callback(obj, method), b0, b1, b2, b3, b4);
    }
};



// Convenience functions declared here to avoid cyclic
// dependency between Event and EventQueue
template <typename R>
Event<void()> EventQueue::event(R (*func)()) {
    return Event<void()>(this, func);
}

template <typename T, typename R>
Event<void()> EventQueue::event(T *obj, R (T::*method)()) {
    return Event<void()>(this, mbed::callback(obj, method));
}

template <typename T, typename R>
Event<void()> EventQueue::event(const T *obj, R (T::*method)() const) {
    return Event<void()>(this, mbed::callback(obj, method));
}

template <typename T, typename R>
Event<void()> EventQueue::event(volatile T *obj, R (T::*method)() volatile) {
    return Event<void()>(this, mbed::callback(obj, method));
}

template <typename T, typename R>
Event<void()> EventQueue::event(const volatile T *obj, R (T::*method)() const volatile) {
    return Event<void()>(this, mbed::callback(obj, method));
}

template <typename R>
Event<void()> EventQueue::event(mbed::Callback<R()> cb) {
    return Event<void()>(this, cb);
}

template <typename R, typename B0, typename C0>
Event<void()> EventQueue::event(R (*func)(B0), C0 c0) {
    return Event<void()>(this, func, c0);
}

template <typename T, typename R, typename B0, typename C0>
Event<void()> EventQueue::event(T *obj, R (T::*method)(B0), C0 c0) {
    return Event<void()>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0>
Event<void()> EventQueue::event(const T *obj, R (T::*method)(B0) const, C0 c0) {
    return Event<void()>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0>
Event<void()> EventQueue::event(volatile T *obj, R (T::*method)(B0) volatile, C0 c0) {
    return Event<void()>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0>
Event<void()> EventQueue::event(const volatile T *obj, R (T::*method)(B0) const volatile, C0 c0) {
    return Event<void()>(this, mbed::callback(obj, method), c0);
}

template <typename R, typename B0, typename C0>
Event<void()> EventQueue::event(mbed::Callback<R(B0)> cb, C0 c0) {
    return Event<void()>(this, cb, c0);
}

template <typename R, typename B0, typename B1, typename C0, typename C1>
Event<void()> EventQueue::event(R (*func)(B0, B1), C0 c0, C1 c1) {
    return Event<void()>(this, func, c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1>
Event<void()> EventQueue::event(T *obj, R (T::*method)(B0, B1), C0 c0, C1 c1) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1>
Event<void()> EventQueue::event(const T *obj, R (T::*method)(B0, B1) const, C0 c0, C1 c1) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1>
Event<void()> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1) volatile, C0 c0, C1 c1) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1>
Event<void()> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1) const volatile, C0 c0, C1 c1) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1);
}

template <typename R, typename B0, typename B1, typename C0, typename C1>
Event<void()> EventQueue::event(mbed::Callback<R(B0, B1)> cb, C0 c0, C1 c1) {
    return Event<void()>(this, cb, c0, c1);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2>
Event<void()> EventQueue::event(R (*func)(B0, B1, B2), C0 c0, C1 c1, C2 c2) {
    return Event<void()>(this, func, c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2>
Event<void()> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2), C0 c0, C1 c1, C2 c2) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2>
Event<void()> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2) const, C0 c0, C1 c1, C2 c2) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2>
Event<void()> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2) volatile, C0 c0, C1 c1, C2 c2) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2>
Event<void()> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2) const volatile, C0 c0, C1 c1, C2 c2) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2>
Event<void()> EventQueue::event(mbed::Callback<R(B0, B1, B2)> cb, C0 c0, C1 c1, C2 c2) {
    return Event<void()>(this, cb, c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3>
Event<void()> EventQueue::event(R (*func)(B0, B1, B2, B3), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void()>(this, func, c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3>
Event<void()> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, B3), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3>
Event<void()> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, B3) const, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3>
Event<void()> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, B3) volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3>
Event<void()> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, B3) const volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3>
Event<void()> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3)> cb, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void()>(this, cb, c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4>
Event<void()> EventQueue::event(R (*func)(B0, B1, B2, B3, B4), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void()>(this, func, c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4>
Event<void()> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, B3, B4), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4>
Event<void()> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, B3, B4) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4>
Event<void()> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4>
Event<void()> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void()>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4>
Event<void()> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, B4)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void()>(this, cb, c0, c1, c2, c3, c4);
}

template <typename R, typename A0>
Event<void(A0)> EventQueue::event(R (*func)(A0)) {
    return Event<void(A0)>(this, func);
}

template <typename T, typename R, typename A0>
Event<void(A0)> EventQueue::event(T *obj, R (T::*method)(A0)) {
    return Event<void(A0)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0>
Event<void(A0)> EventQueue::event(const T *obj, R (T::*method)(A0) const) {
    return Event<void(A0)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0>
Event<void(A0)> EventQueue::event(volatile T *obj, R (T::*method)(A0) volatile) {
    return Event<void(A0)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0>
Event<void(A0)> EventQueue::event(const volatile T *obj, R (T::*method)(A0) const volatile) {
    return Event<void(A0)>(this, mbed::callback(obj, method));
}

template <typename R, typename A0>
Event<void(A0)> EventQueue::event(mbed::Callback<R(A0)> cb) {
    return Event<void(A0)>(this, cb);
}

template <typename R, typename B0, typename C0, typename A0>
Event<void(A0)> EventQueue::event(R (*func)(B0, A0), C0 c0) {
    return Event<void(A0)>(this, func, c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0>
Event<void(A0)> EventQueue::event(T *obj, R (T::*method)(B0, A0), C0 c0) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0>
Event<void(A0)> EventQueue::event(const T *obj, R (T::*method)(B0, A0) const, C0 c0) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0>
Event<void(A0)> EventQueue::event(volatile T *obj, R (T::*method)(B0, A0) volatile, C0 c0) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0>
Event<void(A0)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, A0) const volatile, C0 c0) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0);
}

template <typename R, typename B0, typename C0, typename A0>
Event<void(A0)> EventQueue::event(mbed::Callback<R(B0, A0)> cb, C0 c0) {
    return Event<void(A0)>(this, cb, c0);
}

template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0>
Event<void(A0)> EventQueue::event(R (*func)(B0, B1, A0), C0 c0, C1 c1) {
    return Event<void(A0)>(this, func, c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0>
Event<void(A0)> EventQueue::event(T *obj, R (T::*method)(B0, B1, A0), C0 c0, C1 c1) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0>
Event<void(A0)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, A0) const, C0 c0, C1 c1) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0>
Event<void(A0)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, A0) volatile, C0 c0, C1 c1) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0>
Event<void(A0)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, A0) const volatile, C0 c0, C1 c1) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0>
Event<void(A0)> EventQueue::event(mbed::Callback<R(B0, B1, A0)> cb, C0 c0, C1 c1) {
    return Event<void(A0)>(this, cb, c0, c1);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0>
Event<void(A0)> EventQueue::event(R (*func)(B0, B1, B2, A0), C0 c0, C1 c1, C2 c2) {
    return Event<void(A0)>(this, func, c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0>
Event<void(A0)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, A0), C0 c0, C1 c1, C2 c2) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0>
Event<void(A0)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, A0) const, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0>
Event<void(A0)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, A0) volatile, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0>
Event<void(A0)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, A0) const volatile, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0>
Event<void(A0)> EventQueue::event(mbed::Callback<R(B0, B1, B2, A0)> cb, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0)>(this, cb, c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0>
Event<void(A0)> EventQueue::event(R (*func)(B0, B1, B2, B3, A0), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0)>(this, func, c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0>
Event<void(A0)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, B3, A0), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0>
Event<void(A0)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, B3, A0) const, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0>
Event<void(A0)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0) volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0>
Event<void(A0)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0) const volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0>
Event<void(A0)> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, A0)> cb, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0)>(this, cb, c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0>
Event<void(A0)> EventQueue::event(R (*func)(B0, B1, B2, B3, B4, A0), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0)>(this, func, c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0>
Event<void(A0)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0>
Event<void(A0)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0>
Event<void(A0)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0>
Event<void(A0)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0>
Event<void(A0)> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, B4, A0)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0)>(this, cb, c0, c1, c2, c3, c4);
}

template <typename R, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(R (*func)(A0, A1)) {
    return Event<void(A0, A1)>(this, func);
}

template <typename T, typename R, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(T *obj, R (T::*method)(A0, A1)) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(const T *obj, R (T::*method)(A0, A1) const) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(volatile T *obj, R (T::*method)(A0, A1) volatile) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(const volatile T *obj, R (T::*method)(A0, A1) const volatile) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method));
}

template <typename R, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(mbed::Callback<R(A0, A1)> cb) {
    return Event<void(A0, A1)>(this, cb);
}

template <typename R, typename B0, typename C0, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(R (*func)(B0, A0, A1), C0 c0) {
    return Event<void(A0, A1)>(this, func, c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(T *obj, R (T::*method)(B0, A0, A1), C0 c0) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(const T *obj, R (T::*method)(B0, A0, A1) const, C0 c0) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(volatile T *obj, R (T::*method)(B0, A0, A1) volatile, C0 c0) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, A0, A1) const volatile, C0 c0) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0);
}

template <typename R, typename B0, typename C0, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(mbed::Callback<R(B0, A0, A1)> cb, C0 c0) {
    return Event<void(A0, A1)>(this, cb, c0);
}

template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(R (*func)(B0, B1, A0, A1), C0 c0, C1 c1) {
    return Event<void(A0, A1)>(this, func, c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(T *obj, R (T::*method)(B0, B1, A0, A1), C0 c0, C1 c1) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, A0, A1) const, C0 c0, C1 c1) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, A0, A1) volatile, C0 c0, C1 c1) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, A0, A1) const volatile, C0 c0, C1 c1) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(mbed::Callback<R(B0, B1, A0, A1)> cb, C0 c0, C1 c1) {
    return Event<void(A0, A1)>(this, cb, c0, c1);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(R (*func)(B0, B1, B2, A0, A1), C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1)>(this, func, c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, A0, A1), C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, A0, A1) const, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1) volatile, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1) const volatile, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(mbed::Callback<R(B0, B1, B2, A0, A1)> cb, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1)>(this, cb, c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(R (*func)(B0, B1, B2, B3, A0, A1), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1)>(this, func, c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1) const, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1) volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1) const volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, A0, A1)> cb, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1)>(this, cb, c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(R (*func)(B0, B1, B2, B3, B4, A0, A1), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1)>(this, func, c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1>
Event<void(A0, A1)> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, B4, A0, A1)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1)>(this, cb, c0, c1, c2, c3, c4);
}

template <typename R, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(R (*func)(A0, A1, A2)) {
    return Event<void(A0, A1, A2)>(this, func);
}

template <typename T, typename R, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(T *obj, R (T::*method)(A0, A1, A2)) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(const T *obj, R (T::*method)(A0, A1, A2) const) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(volatile T *obj, R (T::*method)(A0, A1, A2) volatile) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(const volatile T *obj, R (T::*method)(A0, A1, A2) const volatile) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method));
}

template <typename R, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(mbed::Callback<R(A0, A1, A2)> cb) {
    return Event<void(A0, A1, A2)>(this, cb);
}

template <typename R, typename B0, typename C0, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(R (*func)(B0, A0, A1, A2), C0 c0) {
    return Event<void(A0, A1, A2)>(this, func, c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(T *obj, R (T::*method)(B0, A0, A1, A2), C0 c0) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(const T *obj, R (T::*method)(B0, A0, A1, A2) const, C0 c0) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(volatile T *obj, R (T::*method)(B0, A0, A1, A2) volatile, C0 c0) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, A0, A1, A2) const volatile, C0 c0) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0);
}

template <typename R, typename B0, typename C0, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(mbed::Callback<R(B0, A0, A1, A2)> cb, C0 c0) {
    return Event<void(A0, A1, A2)>(this, cb, c0);
}

template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(R (*func)(B0, B1, A0, A1, A2), C0 c0, C1 c1) {
    return Event<void(A0, A1, A2)>(this, func, c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(T *obj, R (T::*method)(B0, B1, A0, A1, A2), C0 c0, C1 c1) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, A0, A1, A2) const, C0 c0, C1 c1) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2) volatile, C0 c0, C1 c1) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2) const volatile, C0 c0, C1 c1) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(mbed::Callback<R(B0, B1, A0, A1, A2)> cb, C0 c0, C1 c1) {
    return Event<void(A0, A1, A2)>(this, cb, c0, c1);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(R (*func)(B0, B1, B2, A0, A1, A2), C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2)>(this, func, c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2), C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2) const, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2) volatile, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2) const volatile, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(mbed::Callback<R(B0, B1, B2, A0, A1, A2)> cb, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2)>(this, cb, c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(R (*func)(B0, B1, B2, B3, A0, A1, A2), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2)>(this, func, c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2) const, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2) volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2) const volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, A0, A1, A2)> cb, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2)>(this, cb, c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(R (*func)(B0, B1, B2, B3, B4, A0, A1, A2), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2)>(this, func, c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2>
Event<void(A0, A1, A2)> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, B4, A0, A1, A2)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2)>(this, cb, c0, c1, c2, c3, c4);
}

template <typename R, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(R (*func)(A0, A1, A2, A3)) {
    return Event<void(A0, A1, A2, A3)>(this, func);
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(T *obj, R (T::*method)(A0, A1, A2, A3)) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(const T *obj, R (T::*method)(A0, A1, A2, A3) const) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(volatile T *obj, R (T::*method)(A0, A1, A2, A3) volatile) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(const volatile T *obj, R (T::*method)(A0, A1, A2, A3) const volatile) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method));
}

template <typename R, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(mbed::Callback<R(A0, A1, A2, A3)> cb) {
    return Event<void(A0, A1, A2, A3)>(this, cb);
}

template <typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(R (*func)(B0, A0, A1, A2, A3), C0 c0) {
    return Event<void(A0, A1, A2, A3)>(this, func, c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(T *obj, R (T::*method)(B0, A0, A1, A2, A3), C0 c0) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(const T *obj, R (T::*method)(B0, A0, A1, A2, A3) const, C0 c0) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3) volatile, C0 c0) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3) const volatile, C0 c0) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0);
}

template <typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(mbed::Callback<R(B0, A0, A1, A2, A3)> cb, C0 c0) {
    return Event<void(A0, A1, A2, A3)>(this, cb, c0);
}

template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(R (*func)(B0, B1, A0, A1, A2, A3), C0 c0, C1 c1) {
    return Event<void(A0, A1, A2, A3)>(this, func, c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3), C0 c0, C1 c1) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3) const, C0 c0, C1 c1) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3) volatile, C0 c0, C1 c1) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3) const volatile, C0 c0, C1 c1) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(mbed::Callback<R(B0, B1, A0, A1, A2, A3)> cb, C0 c0, C1 c1) {
    return Event<void(A0, A1, A2, A3)>(this, cb, c0, c1);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(R (*func)(B0, B1, B2, A0, A1, A2, A3), C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2, A3)>(this, func, c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3), C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3) const, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3) volatile, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3) const volatile, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(mbed::Callback<R(B0, B1, B2, A0, A1, A2, A3)> cb, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2, A3)>(this, cb, c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(R (*func)(B0, B1, B2, B3, A0, A1, A2, A3), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2, A3)>(this, func, c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3) const, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3) volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3) const volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, A0, A1, A2, A3)> cb, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2, A3)>(this, cb, c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(R (*func)(B0, B1, B2, B3, B4, A0, A1, A2, A3), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2, A3)>(this, func, c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2, A3)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3>
Event<void(A0, A1, A2, A3)> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, B4, A0, A1, A2, A3)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2, A3)>(this, cb, c0, c1, c2, c3, c4);
}

template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(R (*func)(A0, A1, A2, A3, A4)) {
    return Event<void(A0, A1, A2, A3, A4)>(this, func);
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(T *obj, R (T::*method)(A0, A1, A2, A3, A4)) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(const T *obj, R (T::*method)(A0, A1, A2, A3, A4) const) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(volatile T *obj, R (T::*method)(A0, A1, A2, A3, A4) volatile) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method));
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(const volatile T *obj, R (T::*method)(A0, A1, A2, A3, A4) const volatile) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method));
}

template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(mbed::Callback<R(A0, A1, A2, A3, A4)> cb) {
    return Event<void(A0, A1, A2, A3, A4)>(this, cb);
}

template <typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(R (*func)(B0, A0, A1, A2, A3, A4), C0 c0) {
    return Event<void(A0, A1, A2, A3, A4)>(this, func, c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4), C0 c0) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(const T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4) const, C0 c0) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4) volatile, C0 c0) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0);
}

template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4) const volatile, C0 c0) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0);
}

template <typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(mbed::Callback<R(B0, A0, A1, A2, A3, A4)> cb, C0 c0) {
    return Event<void(A0, A1, A2, A3, A4)>(this, cb, c0);
}

template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(R (*func)(B0, B1, A0, A1, A2, A3, A4), C0 c0, C1 c1) {
    return Event<void(A0, A1, A2, A3, A4)>(this, func, c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4), C0 c0, C1 c1) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4) const, C0 c0, C1 c1) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4) volatile, C0 c0, C1 c1) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4) const volatile, C0 c0, C1 c1) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1);
}

template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(mbed::Callback<R(B0, B1, A0, A1, A2, A3, A4)> cb, C0 c0, C1 c1) {
    return Event<void(A0, A1, A2, A3, A4)>(this, cb, c0, c1);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(R (*func)(B0, B1, B2, A0, A1, A2, A3, A4), C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2, A3, A4)>(this, func, c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4), C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4) const, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4) volatile, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4) const volatile, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(mbed::Callback<R(B0, B1, B2, A0, A1, A2, A3, A4)> cb, C0 c0, C1 c1, C2 c2) {
    return Event<void(A0, A1, A2, A3, A4)>(this, cb, c0, c1, c2);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(R (*func)(B0, B1, B2, B3, A0, A1, A2, A3, A4), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2, A3, A4)>(this, func, c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4) const, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4) volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4) const volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, A0, A1, A2, A3, A4)> cb, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Event<void(A0, A1, A2, A3, A4)>(this, cb, c0, c1, c2, c3);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(R (*func)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2, A3, A4)>(this, func, c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2, A3, A4)>(this, mbed::callback(obj, method), c0, c1, c2, c3, c4);
}

template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void(A0, A1, A2, A3, A4)> EventQueue::event(mbed::Callback<R(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Event<void(A0, A1, A2, A3, A4)>(this, cb, c0, c1, c2, c3, c4);
}

}

#endif

/** @}*/
