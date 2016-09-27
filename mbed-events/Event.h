/* events
 * Copyright (c) 2006-2013 ARM Limited
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

#include "EventQueue.h"
#include "mbed_assert.h"

namespace events {

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
    /** Event lifetime
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param a0..a4   Arguments to pass to the callback
     */
    template <typename F>
    Event(EventQueue *q, F f) {
        struct local {
            static int post(struct event *e) {
                typedef EventQueue::context00<F> C;
                struct local {
                    static void call(void *p) { (*static_cast<C*>(p))(); }
                    static void dtor(void *p) { static_cast<C*>(p)->~C(); }
                };

                void *p = equeue_alloc(e->equeue, sizeof(C));
                if (!p) {
                    return 0;
                }

                new (p) C(*reinterpret_cast<F*>(e+1));
                equeue_event_delay(p, e->delay);
                equeue_event_period(p, e->period);
                equeue_event_dtor(p, &local::dtor);
                return equeue_post(e->equeue, &local::call, p);
            }

            static void dtor(struct event *e) {
                reinterpret_cast<F*>(e+1)->~F();
            }
        };

        _event = static_cast<struct event *>(
                equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));
        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &local::post;
            _event->dtor = &local::dtor;

            new (_event+1) F(f);

            _event->ref = 1;
        }
    }

    template <typename F, typename B0>
    Event(EventQueue *q, F f, B0 b0) {
        new (this) Event(q, EventQueue::
                context10<F, B0>(f, b0));
    }

    template <typename F, typename B0, typename B1>
    Event(EventQueue *q, F f, B0 b0, B1 b1) {
        new (this) Event(q, EventQueue::
                context20<F, B0, B1>(f, b0, b1));
    }

    template <typename F, typename B0, typename B1, typename B2>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, EventQueue::
                context30<F, B0, B1, B2>(f, b0, b1, b2));
    }

    template <typename F, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, EventQueue::
                context40<F, B0, B1, B2, B3>(f, b0, b1, b2, b3));
    }

    template <typename F, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, EventQueue::
                context50<F, B0, B1, B2, B3, B4>(f, b0, b1, b2, b3, b4));
    }

    Event(const Event &e) {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    Event &operator=(const Event &that) {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

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
    int post() {
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
    void call() {
        int id = post();
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void operator()() {
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
    void cancel() {
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
};

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename A0>
class Event<void(A0)> {
public:
    /** Event lifetime
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param a0..a4   Arguments to pass to the callback
     */
    template <typename F>
    Event(EventQueue *q, F f) {
        struct local {
            static int post(struct event *e, A0 a0) {
                typedef EventQueue::context10<F, A0> C;
                struct local {
                    static void call(void *p) { (*static_cast<C*>(p))(); }
                    static void dtor(void *p) { static_cast<C*>(p)->~C(); }
                };

                void *p = equeue_alloc(e->equeue, sizeof(C));
                if (!p) {
                    return 0;
                }

                new (p) C(*reinterpret_cast<F*>(e+1), a0);
                equeue_event_delay(p, e->delay);
                equeue_event_period(p, e->period);
                equeue_event_dtor(p, &local::dtor);
                return equeue_post(e->equeue, &local::call, p);
            }

            static void dtor(struct event *e) {
                reinterpret_cast<F*>(e+1)->~F();
            }
        };

        _event = static_cast<struct event *>(
                equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));
        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &local::post;
            _event->dtor = &local::dtor;

            new (_event+1) F(f);

            _event->ref = 1;
        }
    }

    template <typename F, typename B0>
    Event(EventQueue *q, F f, B0 b0) {
        new (this) Event(q, EventQueue::
                context11<F, B0, A0>(f, b0));
    }

    template <typename F, typename B0, typename B1>
    Event(EventQueue *q, F f, B0 b0, B1 b1) {
        new (this) Event(q, EventQueue::
                context21<F, B0, B1, A0>(f, b0, b1));
    }

    template <typename F, typename B0, typename B1, typename B2>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, EventQueue::
                context31<F, B0, B1, B2, A0>(f, b0, b1, b2));
    }

    template <typename F, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, EventQueue::
                context41<F, B0, B1, B2, B3, A0>(f, b0, b1, b2, b3));
    }

    template <typename F, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, EventQueue::
                context51<F, B0, B1, B2, B3, B4, A0>(f, b0, b1, b2, b3, b4));
    }

    Event(const Event &e) {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    Event &operator=(const Event &that) {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

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
    int post(A0 a0) {
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
    void call(A0 a0) {
        int id = post(a0);
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void operator()(A0 a0) {
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
    void cancel() {
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
};

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename A0, typename A1>
class Event<void(A0, A1)> {
public:
    /** Event lifetime
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param a0..a4   Arguments to pass to the callback
     */
    template <typename F>
    Event(EventQueue *q, F f) {
        struct local {
            static int post(struct event *e, A0 a0, A1 a1) {
                typedef EventQueue::context20<F, A0, A1> C;
                struct local {
                    static void call(void *p) { (*static_cast<C*>(p))(); }
                    static void dtor(void *p) { static_cast<C*>(p)->~C(); }
                };

                void *p = equeue_alloc(e->equeue, sizeof(C));
                if (!p) {
                    return 0;
                }

                new (p) C(*reinterpret_cast<F*>(e+1), a0, a1);
                equeue_event_delay(p, e->delay);
                equeue_event_period(p, e->period);
                equeue_event_dtor(p, &local::dtor);
                return equeue_post(e->equeue, &local::call, p);
            }

            static void dtor(struct event *e) {
                reinterpret_cast<F*>(e+1)->~F();
            }
        };

        _event = static_cast<struct event *>(
                equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));
        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &local::post;
            _event->dtor = &local::dtor;

            new (_event+1) F(f);

            _event->ref = 1;
        }
    }

    template <typename F, typename B0>
    Event(EventQueue *q, F f, B0 b0) {
        new (this) Event(q, EventQueue::
                context12<F, B0, A0, A1>(f, b0));
    }

    template <typename F, typename B0, typename B1>
    Event(EventQueue *q, F f, B0 b0, B1 b1) {
        new (this) Event(q, EventQueue::
                context22<F, B0, B1, A0, A1>(f, b0, b1));
    }

    template <typename F, typename B0, typename B1, typename B2>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, EventQueue::
                context32<F, B0, B1, B2, A0, A1>(f, b0, b1, b2));
    }

    template <typename F, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, EventQueue::
                context42<F, B0, B1, B2, B3, A0, A1>(f, b0, b1, b2, b3));
    }

    template <typename F, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, EventQueue::
                context52<F, B0, B1, B2, B3, B4, A0, A1>(f, b0, b1, b2, b3, b4));
    }

    Event(const Event &e) {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    Event &operator=(const Event &that) {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

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
    int post(A0 a0, A1 a1) {
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
    void call(A0 a0, A1 a1) {
        int id = post(a0, a1);
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void operator()(A0 a0, A1 a1) {
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
    void cancel() {
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
};

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename A0, typename A1, typename A2>
class Event<void(A0, A1, A2)> {
public:
    /** Event lifetime
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param a0..a4   Arguments to pass to the callback
     */
    template <typename F>
    Event(EventQueue *q, F f) {
        struct local {
            static int post(struct event *e, A0 a0, A1 a1, A2 a2) {
                typedef EventQueue::context30<F, A0, A1, A2> C;
                struct local {
                    static void call(void *p) { (*static_cast<C*>(p))(); }
                    static void dtor(void *p) { static_cast<C*>(p)->~C(); }
                };

                void *p = equeue_alloc(e->equeue, sizeof(C));
                if (!p) {
                    return 0;
                }

                new (p) C(*reinterpret_cast<F*>(e+1), a0, a1, a2);
                equeue_event_delay(p, e->delay);
                equeue_event_period(p, e->period);
                equeue_event_dtor(p, &local::dtor);
                return equeue_post(e->equeue, &local::call, p);
            }

            static void dtor(struct event *e) {
                reinterpret_cast<F*>(e+1)->~F();
            }
        };

        _event = static_cast<struct event *>(
                equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));
        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &local::post;
            _event->dtor = &local::dtor;

            new (_event+1) F(f);

            _event->ref = 1;
        }
    }

    template <typename F, typename B0>
    Event(EventQueue *q, F f, B0 b0) {
        new (this) Event(q, EventQueue::
                context13<F, B0, A0, A1, A2>(f, b0));
    }

    template <typename F, typename B0, typename B1>
    Event(EventQueue *q, F f, B0 b0, B1 b1) {
        new (this) Event(q, EventQueue::
                context23<F, B0, B1, A0, A1, A2>(f, b0, b1));
    }

    template <typename F, typename B0, typename B1, typename B2>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, EventQueue::
                context33<F, B0, B1, B2, A0, A1, A2>(f, b0, b1, b2));
    }

    template <typename F, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, EventQueue::
                context43<F, B0, B1, B2, B3, A0, A1, A2>(f, b0, b1, b2, b3));
    }

    template <typename F, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, EventQueue::
                context53<F, B0, B1, B2, B3, B4, A0, A1, A2>(f, b0, b1, b2, b3, b4));
    }

    Event(const Event &e) {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    Event &operator=(const Event &that) {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

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
    int post(A0 a0, A1 a1, A2 a2) {
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
    void call(A0 a0, A1 a1, A2 a2) {
        int id = post(a0, a1, a2);
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void operator()(A0 a0, A1 a1, A2 a2) {
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
    void cancel() {
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
};

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename A0, typename A1, typename A2, typename A3>
class Event<void(A0, A1, A2, A3)> {
public:
    /** Event lifetime
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param a0..a4   Arguments to pass to the callback
     */
    template <typename F>
    Event(EventQueue *q, F f) {
        struct local {
            static int post(struct event *e, A0 a0, A1 a1, A2 a2, A3 a3) {
                typedef EventQueue::context40<F, A0, A1, A2, A3> C;
                struct local {
                    static void call(void *p) { (*static_cast<C*>(p))(); }
                    static void dtor(void *p) { static_cast<C*>(p)->~C(); }
                };

                void *p = equeue_alloc(e->equeue, sizeof(C));
                if (!p) {
                    return 0;
                }

                new (p) C(*reinterpret_cast<F*>(e+1), a0, a1, a2, a3);
                equeue_event_delay(p, e->delay);
                equeue_event_period(p, e->period);
                equeue_event_dtor(p, &local::dtor);
                return equeue_post(e->equeue, &local::call, p);
            }

            static void dtor(struct event *e) {
                reinterpret_cast<F*>(e+1)->~F();
            }
        };

        _event = static_cast<struct event *>(
                equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));
        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &local::post;
            _event->dtor = &local::dtor;

            new (_event+1) F(f);

            _event->ref = 1;
        }
    }

    template <typename F, typename B0>
    Event(EventQueue *q, F f, B0 b0) {
        new (this) Event(q, EventQueue::
                context14<F, B0, A0, A1, A2, A3>(f, b0));
    }

    template <typename F, typename B0, typename B1>
    Event(EventQueue *q, F f, B0 b0, B1 b1) {
        new (this) Event(q, EventQueue::
                context24<F, B0, B1, A0, A1, A2, A3>(f, b0, b1));
    }

    template <typename F, typename B0, typename B1, typename B2>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, EventQueue::
                context34<F, B0, B1, B2, A0, A1, A2, A3>(f, b0, b1, b2));
    }

    template <typename F, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, EventQueue::
                context44<F, B0, B1, B2, B3, A0, A1, A2, A3>(f, b0, b1, b2, b3));
    }

    template <typename F, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, EventQueue::
                context54<F, B0, B1, B2, B3, B4, A0, A1, A2, A3>(f, b0, b1, b2, b3, b4));
    }

    Event(const Event &e) {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    Event &operator=(const Event &that) {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

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
    int post(A0 a0, A1 a1, A2 a2, A3 a3) {
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
    void call(A0 a0, A1 a1, A2 a2, A3 a3) {
        int id = post(a0, a1, a2, a3);
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void operator()(A0 a0, A1 a1, A2 a2, A3 a3) {
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
    void cancel() {
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
};

/** Event
 *
 *  Representation of an event for fine-grain dispatch control
 */
template <typename A0, typename A1, typename A2, typename A3, typename A4>
class Event<void(A0, A1, A2, A3, A4)> {
public:
    /** Event lifetime
     *
     *  Constructs an event bound to the specified event queue. The specified
     *  callback acts as the target for the event and is executed in the
     *  context of the event queue's dispatch loop once posted.
     *
     *  @param q        Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param a0..a4   Arguments to pass to the callback
     */
    template <typename F>
    Event(EventQueue *q, F f) {
        struct local {
            static int post(struct event *e, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
                typedef EventQueue::context50<F, A0, A1, A2, A3, A4> C;
                struct local {
                    static void call(void *p) { (*static_cast<C*>(p))(); }
                    static void dtor(void *p) { static_cast<C*>(p)->~C(); }
                };

                void *p = equeue_alloc(e->equeue, sizeof(C));
                if (!p) {
                    return 0;
                }

                new (p) C(*reinterpret_cast<F*>(e+1), a0, a1, a2, a3, a4);
                equeue_event_delay(p, e->delay);
                equeue_event_period(p, e->period);
                equeue_event_dtor(p, &local::dtor);
                return equeue_post(e->equeue, &local::call, p);
            }

            static void dtor(struct event *e) {
                reinterpret_cast<F*>(e+1)->~F();
            }
        };

        _event = static_cast<struct event *>(
                equeue_alloc(&q->_equeue, sizeof(struct event) + sizeof(F)));
        if (_event) {
            _event->equeue = &q->_equeue;
            _event->id = 0;
            _event->delay = 0;
            _event->period = -1;

            _event->post = &local::post;
            _event->dtor = &local::dtor;

            new (_event+1) F(f);

            _event->ref = 1;
        }
    }

    template <typename F, typename B0>
    Event(EventQueue *q, F f, B0 b0) {
        new (this) Event(q, EventQueue::
                context15<F, B0, A0, A1, A2, A3, A4>(f, b0));
    }

    template <typename F, typename B0, typename B1>
    Event(EventQueue *q, F f, B0 b0, B1 b1) {
        new (this) Event(q, EventQueue::
                context25<F, B0, B1, A0, A1, A2, A3, A4>(f, b0, b1));
    }

    template <typename F, typename B0, typename B1, typename B2>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2) {
        new (this) Event(q, EventQueue::
                context35<F, B0, B1, B2, A0, A1, A2, A3, A4>(f, b0, b1, b2));
    }

    template <typename F, typename B0, typename B1, typename B2, typename B3>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Event(q, EventQueue::
                context45<F, B0, B1, B2, B3, A0, A1, A2, A3, A4>(f, b0, b1, b2, b3));
    }

    template <typename F, typename B0, typename B1, typename B2, typename B3, typename B4>
    Event(EventQueue *q, F f, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Event(q, EventQueue::
                context55<F, B0, B1, B2, B3, B4, A0, A1, A2, A3, A4>(f, b0, b1, b2, b3, b4));
    }

    Event(const Event &e) {
        _event = 0;
        if (e._event) {
            _event = e._event;
            _event->ref += 1;
        }
    }

    Event &operator=(const Event &that) {
        if (this != &that) {
            this->~Event();
            new (this) Event(that);
        }

        return *this;
    }

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
    int post(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
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
    void call(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        int id = post(a0, a1, a2, a3, a4);
        MBED_ASSERT(id);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  @param a0..a4   Arguments to pass to the event
     */
    void operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
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
    void cancel() {
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
};


template <typename F>
Event<void()> EventQueue::event(F f) {
    return Event<void()>(this, f);
}

template <typename F, typename A0>
Event<void()> EventQueue::event(F f, A0 a0) {
    return Event<void()>(this, f, a0);
}

template <typename F, typename A0, typename A1>
Event<void()> EventQueue::event(F f, A0 a0, A1 a1) {
    return Event<void()>(this, f, a0, a1);
}

template <typename F, typename A0, typename A1, typename A2>
Event<void()> EventQueue::event(F f, A0 a0, A1 a1, A2 a2) {
    return Event<void()>(this, f, a0, a1, a2);
}

template <typename F, typename A0, typename A1, typename A2, typename A3>
Event<void()> EventQueue::event(F f, A0 a0, A1 a1, A2 a2, A3 a3) {
    return Event<void()>(this, f, a0, a1, a2, a3);
}

template <typename F, typename A0, typename A1, typename A2, typename A3, typename A4>
Event<void()> EventQueue::event(F f, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
    return Event<void()>(this, f, a0, a1, a2, a3, a4);
}

}

#endif