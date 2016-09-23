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
 * WITHOUT WARRANTIES Ovoid CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_EVENT_H
#define MBED_EVENT_H

#ifndef MBED_CONF_RTOS_PRESENT
#warning Event will not use an event queue if the RTOS is not present.
#endif

#include "Callback.h"

namespace mbed {

class EventLoop;

/** An event represents a function call that is executed either immediately
 *  (like a regular function call) or enqueued in an event queue. If enqueued,
 *  the function will run later, in the context of the event queue's thread.
 *  See also EventLoop.h for details and a code example.
 *
 * @Note Synchronization level: Not protected
 */
class Event {
public:
    /** Create a Event with a static function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    Event(void (*func)() = 0, EventLoop *queue = 0);

    /** Create an event from a Callback
     *  @param cb The callback instance
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    Event(const Callback<void()>& cb, EventLoop *queue = 0);

    /** Create a Event with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    Event(void *obj, void (*func)(void*), EventLoop *queue = 0);

    /** Create a Event with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    Event(const void *obj, void (*func)(const void*), EventLoop *queue = 0);

    /** Create a Event with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    Event(volatile void *obj, void (*func)(volatile void*), EventLoop *queue = 0);

    /** Create a Event with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    Event(const volatile void *obj, void (*func)(const volatile void*), EventLoop *queue = 0);

    /** Create a Event with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template<typename T>
    Event(T *obj, void (*func)(T*), EventLoop *queue = 0);

    /** Create a Event with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template<typename T>
    Event(const T *obj, void (*func)(const T*), EventLoop *queue = 0);

    /** Create a Event with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template<typename T>
    Event(volatile T *obj, void (*func)(volatile T*), EventLoop *queue = 0);

    /** Create a Event with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template<typename T>
    Event(const volatile T *obj, void (*func)(const volatile T*), EventLoop *queue = 0);

    /** Create a Event with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template<typename T>
    Event(T *obj, void (T::*func)(), EventLoop *queue = 0);

    /** Create a Event with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template<typename T>
    Event(const T *obj, void (T::*func)() const, EventLoop *queue = 0);

    /** Create a Event with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template<typename T>
    Event(volatile T *obj, void (T::*func)() volatile, EventLoop *queue = 0);

    /** Create a Event with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template<typename T>
    Event(const volatile T *obj, void (T::*func)() const volatile, EventLoop *queue = 0);

    /** Attach a static function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    void attach(void (*func)(), EventLoop *queue = 0);

    /** Attach an Event
     *  ~param evt The Event to attach
     */
    void attach(const Event& evt);

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    void attach(void *obj, void (*func)(void*), EventLoop *queue = 0);

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    void attach(const void *obj, void (*func)(const void*), EventLoop *queue = 0);

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    void attach(volatile void *obj,  void (*func)(volatile void*), EventLoop *queue = 0);

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    void attach(const volatile void *obj,  void (*func)(const volatile void*), EventLoop *queue = 0);

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template <typename T>
    void attach(T *obj,  void (*func)(T*), EventLoop *queue = 0);

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template <typename T>
    void attach(const T *obj,  void (*func)(const T*), EventLoop *queue = 0);

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template <typename T>
    void attach(volatile T *obj,  void (*func)(volatile T*), EventLoop *queue = 0);

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template <typename T>
    void attach(const volatile T *obj,  void (*func)(const volatile T*), EventLoop *queue = 0);

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template<typename T>
    void attach(T *obj,  void (T::*func)(), EventLoop *queue = 0);

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template<typename T>
    void attach(const T *obj,  void (T::*func)() const, EventLoop *queue = 0);

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template<typename T>
    void attach(volatile T *obj,  void (T::*func)() volatile, EventLoop *queue = 0);

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     *  @param queue Pointer to the event queue (NULL to call the event immediately)
     */
    template<typename T>
    void attach(const volatile T *obj,  void (T::*func)() const volatile, EventLoop *queue = 0);

    /** Calls the event. Depending on the call mode, the callback is either
     *  executed immediately (if _queue is NULL) or enqueue in _queue (if
     *  _queue is not NULL).
     */
    void call() const;

    /** Calls the event. Depending on the call mode, the callback is either
     *  executed immediately (if _queue is NULL) or enqueue in _queue (if
     *  _queue is not NULL).
     */
    void operator()() const;

    /** Static thunk for passing as C-style function
     *  @param func Event to call passed as void pointer
     */
    static void thunk(void *func);

    /** Check if the callback instance is attached to a function
     */
    operator bool() const;

private:
    Callback<void()> _callback;
    EventLoop *_queue;
};

/** Create a callback class with type infered from the arguments
 *
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
Event event(void (*func)(void), EventLoop *queue);

/** Create a event class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
Event event(void *obj, void (*func)(void*), EventLoop *queue);
/** Create a event class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
Event event(const void *obj, void (*func)(const void*), EventLoop *queue);

/** Create a event class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
Event event(volatile void *obj, void (*func)(volatile void*), EventLoop *queue);

/** Create a event class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
Event event(const volatile void *obj, void (*func)(const volatile void*), EventLoop *queue);

/** Create a event class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
template <typename T>
Event event(T *obj, void (*func)(T*), EventLoop *queue);

/** Create a event class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
template <typename T>
Event event(const T *obj, void (*func)(const T*), EventLoop *queue);

/** Create a event class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
template <typename T>
Event event(volatile T *obj, void (*func)(volatile T*), EventLoop *queue);

/** Create a event class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
template <typename T>
Event event(const volatile T *obj, void (*func)(const volatile T*), EventLoop *queue);

/** Create a event class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
template<typename T>
Event event(T *obj, void (T::*func)(), EventLoop *queue);

/** Create a event class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
template<typename T>
Event event(const T *obj, void (T::*func)() const, EventLoop *queue);

/** Create a event class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
template<typename T>
Event event(volatile T *obj, void (T::*func)() volatile, EventLoop *queue);

/** Create a event class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @param queue Pointer to the event queue
 *  @return     Callback with infered type
 */
template<typename T>
Event event(const volatile T *obj, void (T::*func)() const volatile, EventLoop *queue);

} // namespace mbed

#endif // #ifndef MBED_EVENT_H
