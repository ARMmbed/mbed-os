/*
 * Copyright (c) 2019 ARM Limited
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
#ifndef USER_ALLOCATED_EVENT_H
#define USER_ALLOCATED_EVENT_H

#include "events/EventQueue.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_atomic.h"

namespace events {
/**
 * \addtogroup events-public-api
 * @{
 */
template <typename F, typename A>
class UserAllocatedEvent;

/**
 * \defgroup events_Event UserAllocatedEvent class
 * @{
 */

/** UserAllocatedEvent
 *
 *  Representation of an static event for fine-grain dispatch control.
 *
 *  UserAllocatedEvent provides mechanism for event posting and dispatching
 *  without utilization of queue internal memory. It embeds all underlying
 *  event data and doesn't require any memory allocation while posting and dispatching.
 *  All of these makes it cannot fail due to memory exhaustion while posting
 *
 * Usage:
 * @code
 *  #include "mbed.h"
 *
 *  void handler(int data) { ... }
 *
 *  class Device {
 *     public:
 *     void handler(int data) { ... }
 *  };
 *
 *  Device dev;
 *
 *  // queue with not internal storage for dynamic events
 *  // accepts only user allocated events
 *  static EventQueue queue(0);
 *  // Create events
 *  static auto e1 = make_user_allocated_event(&dev, Device::handler, 2);
 *  static auto e2 = queue.make_user_allocated_event(handler, 3);
 *
 *  int main()
 *  {
 *    e1.call_on(&queue);
 *    e2.call();
 *
 *    queue.dispatch(1);
 *  }
 * @endcode
 */
template <typename F, typename... ArgTs>
class UserAllocatedEvent<F, void(ArgTs...)> {
public:
    typedef EventQueue::context<F, ArgTs...> C;

    /** Create an event
     *
     *  Constructs an event. The specified callback acts as the target
     *  for the event and is executed in the context of the
     *  event queue's dispatch loop once posted.
     *
     *  @param f        Function to execute when the event is dispatched
     *  @param args     Arguments to bind to the callback
     */
    constexpr UserAllocatedEvent(F f,  ArgTs... args) : _e(get_default_equeue_event()), _c(f, args...), _delay(), _period(-1), _equeue(), _post_ref()
    {
    }

    /** Create an event
     *
     *  Constructs an event. The specified callback acts as the target
     *  for the event and is executed in the context of the
     *  event queue's dispatch loop once posted.
     *
     *  @param queue    Event queue to dispatch on
     *  @param f        Function to execute when the event is dispatched
     *  @param args     Arguments to bind to the callback
     */
    constexpr UserAllocatedEvent(EventQueue *queue, F f,  ArgTs... args) : _e(get_default_equeue_event()), _c(f, args...), _delay(), _period(-1), _equeue(&queue->_equeue), _post_ref()
    {
    }

    /** Destructor for events
     */
#if !defined(NDEBUG)
    // Remove the user provided destructor in release to allow constexpr optimization
    // constexpr requires destructor to be trivial and only default one is treated as trivial
    ~UserAllocatedEvent()
    {
        MBED_ASSERT(!_post_ref);
    }
#endif

    /** Posts an event onto the underlying event queue, returning void
    *
    *  The event is posted to the underlying queue and is executed in the
    *  context of the event queue's dispatch loop.
    *
    *  This call cannot fail due queue memory exhaustion
    *  because it doesn't allocate any memory
    *
    *  The post function is IRQ safe and can act as a mechanism for moving
    *  events out of IRQ contexts.
    *
    */
    void call()
    {
        MBED_ASSERT(!_post_ref);
        MBED_ASSERT(_equeue);
        MBED_UNUSED bool status = post();
        MBED_ASSERT(status);
    }

    /** Posts an event onto the event queue passed as argument, returning void
    *
    *  The event is posted to the event queue passed as argument
    *  and is executed in the context of the event queue's dispatch loop.
    *
    *  This call cannot fail due queue memory exhaustion
    *  because it doesn't allocate any memory
    *
    *  The post function is IRQ safe and can act as a mechanism for moving
    *  events out of IRQ contexts.
    *
    *  @param queue    Event queue to dispatch on. Will replace earlier bound EventQueue.
    *
    */
    void call_on(EventQueue *queue)
    {
        MBED_ASSERT(!_post_ref);
        MBED_UNUSED bool status = post_on(queue);
        MBED_ASSERT(status);
    }

    /** Posts an event onto the underlying event queue
    *
    *  The event is posted to the event queue passed as argument
    *  and is executed in the context of the event queue's dispatch loop.
    *
    *  This call cannot fail due queue memory exhaustion
    *  because it doesn't allocate any memory
    *
    *  @return     False if the event was already posted
    *              true otherwise
    *
    */
    bool try_call()
    {
        return post();
    }

    /** Posts an event onto the event queue passed as argument,
    *
    *  The event is posted to the underlying queue and is executed in the
    *  context of the event queue's dispatch loop.
    *
    *  This call cannot fail due queue memory exhaustion
    *  because it doesn't allocate any memory
    *
    *  @param queue    Event queue to dispatch on. Will replace earlier bound EventQueue.
    *  @return         False if the event was already posted
    *                  true otherwise
    *
    */
    bool try_call_on(EventQueue *queue)
    {
        return post_on(queue);
    }

    /** Posts an event onto the underlying event queue, returning void
     *
     *  The event is posted to the underlying queue and is executed in the
     *  context of the event queue's dispatch loop.
     *
     *  This call cannot fail due queue memory exhaustion
     *  because it doesn't allocate any memory
     *
     *  The post function is IRQ safe and can act as a mechanism for moving
     *  events out of IRQ contexts.
     *
     */
    void operator()()
    {
        return call();
    }

    /** Configure the delay of an event
     *
     *  @param delay    Millisecond delay before dispatching the event
     */
    void delay(int delay)
    {
        MBED_ASSERT(!_post_ref);
        _delay = delay;
    }

    /** Configure the period of an event
     *
     *  @param period   Millisecond period for repeatedly dispatching an event
     */
    void period(int period)
    {
        MBED_ASSERT(!_post_ref);
        _period = period;
    }

    /** Cancels posted event
     *
     *  Attempts to cancel posted event. It is not safe to call
     *  cancel after an event has already been dispatched.
     *
     *  The cancel function is IRQ safe.
     *
     *  If called while the event queue's dispatch loop is active, the cancel
     *  function does not guarantee that the event will not execute after it
     *  returns, as the event may have already begun executing.
     *
     *  @return     true if event was successfully cancelled
     */
    bool cancel()
    {
        return _post_ref > 0 ? equeue_cancel_user_allocated(_equeue, &_e) : false;
    }


private:
    friend class EventQueue;
    struct equeue_event _e;
    C _c;
    int _delay;
    int _period;
    struct equeue *_equeue;
    uint8_t _post_ref;

    bool post()
    {
        if (_post_ref) {
            return false;
        }
        core_util_atomic_incr_u8(&_post_ref, 1);
        equeue_event_delay(&_e + 1, _delay);
        equeue_event_period(&_e + 1, _period);
        equeue_post_user_allocated(_equeue, &EventQueue::function_call<C>, &_e);
        return true;
    }

    bool post_on(EventQueue *queue)
    {
        MBED_ASSERT(queue);
        if (_post_ref) {
            return false;
        }
        _equeue = &(queue->_equeue);
        core_util_atomic_incr_u8(&_post_ref, 1);
        equeue_event_delay(&_e + 1, _delay);
        equeue_event_period(&_e + 1, _period);
        equeue_post_user_allocated(_equeue, &EventQueue::function_call<C>, &_e);
        return true;
    }

    static void event_dtor(void *p)
    {
        UserAllocatedEvent<F, void(ArgTs...)> *instance = (UserAllocatedEvent<F, void(ArgTs...)> *)(((equeue_event *)p) - 1);
        core_util_atomic_decr_u8(&instance->_post_ref, 1);
        MBED_ASSERT(!instance->_post_ref);
    }

    constexpr static equeue_event get_default_equeue_event()
    {
        return equeue_event{ 0, 0, 0, NULL, NULL, NULL, 0, -1, &UserAllocatedEvent::event_dtor, NULL };
    }

public:

    /** Create an event
     *  @see UserAllocatedEvent::UserAllocatedEvent
     */
    template <typename T, typename R>
    constexpr UserAllocatedEvent(T *obj, R(T::*method)(ArgTs...), ArgTs... args) :
        UserAllocatedEvent(mbed::callback(obj, method), args...) { }

    /** Create an event
     *  @see UserAllocatedEvent::UserAllocatedEvent
     */
    template <typename T, typename R>
    constexpr UserAllocatedEvent(EventQueue *q, T *obj, R(T::*method)(ArgTs...), ArgTs... args) :
        UserAllocatedEvent(q, mbed::callback(obj, method), args...) { }

    /** Create an event
     *  @see UserAllocatedEvent::UserAllocatedEvent
     */
    template <typename T, typename R>
    constexpr UserAllocatedEvent(const T *obj, R(T::*method)(ArgTs...) const, ArgTs... args) :
        UserAllocatedEvent(mbed::callback(obj, method), args...) { }

    /** Create an event
     *  @see UserAllocatedEvent::UserAllocatedEvent
     */
    template <typename T, typename R>
    constexpr UserAllocatedEvent(EventQueue *q, const T *obj, R(T::*method)(ArgTs...) const, ArgTs... args) :
        UserAllocatedEvent(q, mbed::callback(obj, method), args...) { }

    /** Create an event
     *  @see UserAllocatedEvent::UserAllocatedEvent
     */
    template <typename T, typename R>
    constexpr UserAllocatedEvent(volatile T *obj, R(T::*method)(ArgTs...) volatile, ArgTs... args) :
        UserAllocatedEvent(mbed::callback(obj, method), args...) { }

    /** Create an event
     *  @see UserAllocatedEvent::UserAllocatedEvent
     */
    template <typename T, typename R>
    constexpr UserAllocatedEvent(EventQueue *q, volatile T *obj, R(T::*method)(ArgTs...) volatile, ArgTs... args) :
        UserAllocatedEvent(q, mbed::callback(obj, method), args...) { }

    /** Create an event
     *  @see UserAllocatedEvent::UserAllocatedEvent
     */
    template <typename T, typename R>
    constexpr UserAllocatedEvent(const volatile T *obj, R(T::*method)(ArgTs...) const volatile, ArgTs... args) :
        UserAllocatedEvent(mbed::callback(obj, method), args...) { }

    /** Create an event
     *  @see UserAllocatedEvent::UserAllocatedEvent
     */
    template <typename T, typename R>
    constexpr UserAllocatedEvent(EventQueue *q, const volatile T *obj, R(T::*method)(ArgTs...) const volatile, ArgTs... args) :
        UserAllocatedEvent(q, mbed::callback(obj, method), args...) { }
};

// Convenience functions declared here to avoid cyclic
// dependency between Event and EventQueue
template <typename F, typename... ArgTs>
UserAllocatedEvent<F, void(ArgTs...)> EventQueue::make_user_allocated_event(F f, ArgTs... args)
{
    return UserAllocatedEvent<F, void(ArgTs...)>(this, f, args...);
}

template <typename T, typename R, typename... ArgTs>
UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)> EventQueue::make_user_allocated_event(T *obj, R(T::*method)(ArgTs... args), ArgTs... args)
{
    return UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)>(this, mbed::callback(obj, method), args...);
}

template <typename T, typename R, typename... ArgTs>
UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)> EventQueue::make_user_allocated_event(const T *obj, R(T::*method)(ArgTs... args) const, ArgTs... args)
{
    return UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)>(this, mbed::callback(obj, method), args...);
}

template <typename T, typename R, typename... ArgTs>
UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)> EventQueue::make_user_allocated_event(volatile T *obj, R(T::*method)(ArgTs... args) volatile, ArgTs... args)
{
    return UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)>(this, mbed::callback(obj, method), args...);
}

template <typename T, typename R, typename... ArgTs>
UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)> EventQueue::make_user_allocated_event(const volatile T *obj, R(T::*method)(ArgTs... args) const volatile, ArgTs... args)
{
    return UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)>(this, mbed::callback(obj, method), args...);
}


/** Creates a UserAllocatedEvent object, deducing the target type from the types of arguments
 *
 *  UserAllocatedEvent doesn't utilize EventQueue internal memory,
 *  therefore it can be posted on the queue without being afraid
 *  of post fail due to queue memory exhaustion
 *
 *  @return  UserAllocatedEvent object instance
 *
 */
template <typename F, typename... ArgTs>
constexpr UserAllocatedEvent<F, void(ArgTs...)> make_user_allocated_event(F f, ArgTs... args)
{
    return UserAllocatedEvent<F, void(ArgTs...)>(f, args...);
}

/** Creates a UserAllocatedEvent object, deducing the target type from the types of arguments
 *
 *  UserAllocatedEvent doesn't utilize EventQueue internal memory,
 *  therefore it can be posted on the queue without being afraid
 *  of post fail due to queue memory exhaustion
 *
 *  @return  UserAllocatedEvent object instance
 *
 */
template <typename T, typename R, typename... ArgTs>
constexpr UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)> make_user_allocated_event(T *obj, R(T::*method)(ArgTs... args), ArgTs... args)
{
    return UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)>(mbed::callback(obj, method), args...);
}

/** Creates a UserAllocatedEvent object, deducing the target type from the types of arguments
 *
 *  UserAllocatedEvent doesn't utilize EventQueue internal memory,
 *  therefore it can be posted on the queue without being afraid
 *  of post fail due to queue memory exhaustion
 *
 *  @return  UserAllocatedEvent object instance
 *
 */
template <typename T, typename R, typename... ArgTs>
constexpr UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)> make_user_allocated_event(const T *obj, R(T::*method)(ArgTs... args) const, ArgTs... args)
{
    return UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)>(mbed::callback(obj, method), args...);
}

/** Creates a UserAllocatedEvent object, deducing the target type from the types of arguments
 *
 *  UserAllocatedEvent doesn't utilize EventQueue internal memory,
 *  therefore it can be posted on the queue without being afraid
 *  of post fail due to queue memory exhaustion
 *
 *  @return  UserAllocatedEvent object instance
 *
 */
template <typename T, typename R, typename... ArgTs>
constexpr UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)> make_user_allocated_event(volatile T *obj, R(T::*method)(ArgTs... args) volatile, ArgTs... args)
{
    return UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)>(mbed::callback(obj, method), args...);
}

/** Creates a UserAllocatedEvent object, deducing the target type from the types of arguments
 *
 *  UserAllocatedEvent doesn't utilize EventQueue internal memory,
 *  therefore it can be posted on the queue without being afraid
 *  of post fail due to queue memory exhaustion
 *
 *  @return  UserAllocatedEvent object instance
 *
 */
template <typename T, typename R, typename... ArgTs>
constexpr UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)> make_user_allocated_event(const volatile T *obj, R(T::*method)(ArgTs... args) const volatile, ArgTs... args)
{
    return UserAllocatedEvent<mbed::Callback<void(ArgTs...)>, void(ArgTs...)>(mbed::callback(obj, method), args...);
}

/** @}*/

/** @}*/
}
#endif
