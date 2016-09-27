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
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_EVENT_ADAPTER_H
#define MBED_EVENT_ADAPTER_H

#include "Callback.h"
#if MBED_CONF_MBED_EVENTS_PRESENT
#include "Event.h"
#endif
#include "mbed_assert.h"
#include <stdint.h>
#include <new>

namespace mbed {

template<typename F>
class EventAdapter;

/** An EventAdapter holds either a Callback<void()> or an Event<void()>. It can be used
 *  for attach()-style functions to specify if the attached callback should run in user
 *  context or in interrupt context. To run in user context, construct an EventAdapter
 *  from an Event object, this will be enqueued in an EventQueue. To run in interrupt
 *  context (without enqueuing), const an EventAdapter from a Callback object. Example:
 *
 *  @code
 *  InterruptIn sw(SW2);
 *  EventQueue queue(128);
 *
 *  void handler(void) {
 *  }
 *
 *  int main() {
 *      sw.rise(queue.event(handler)); // run in user context
 *      sw.rise(callback(handler)); // run in interrupt context
 *      sw.rise(handler); // shortcut for the form above (run in interrupt context)
 *  }
 *  @endcode
 */
template<>
class EventAdapter<void()> {

    typedef Callback<void()> adapter_callback_t;
#if MBED_CONF_MBED_EVENTS_PRESENT
    typedef events::Event<void()> adapter_event_t;
#endif

public:
    /** Create an empty EventAdapter (it contains an empty Callback)
     */
    EventAdapter() {
        construct_callback(adapter_callback_t());
    }

    /** Create an EventAdapter that holds a callback
     *  @param cb The callback
     */
    EventAdapter(const adapter_callback_t &cb) {
        construct_callback(cb);
    }

    /** Create an EventAdapter with a static function callback
     *  @param func     Static function to attach
     */
    EventAdapter(void (*func)()) {
        construct_callback(callback(func));
    }

    /** Create an EventAdapter with a member function callback
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T>
    EventAdapter(T *obj, void (T::*method)()) {
        construct_callback(callback(obj, method));
    }

    /** Create an EventAdapter with a member function callback
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T>
    EventAdapter(const T *obj, void (T::*method)() const) {
        construct_callback(callback(obj, method));
    }

    /** Create an EventAdapter with a member function callback
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T>
    EventAdapter(volatile T *obj, void (T::*method)() volatile) {
        construct_callback(callback(obj, method));
    }

    /** Create an EventAdapter with a member function callback
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T>
    EventAdapter(const volatile T *obj, void (T::*method)() const volatile) {
        construct_callback(callback(obj, method));
    }

    /** Create an EventAdapter with a static function and bound pointer callback
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    EventAdapter(void (*func)(void*), void *arg) {
        construct_callback(callback(func, arg));
    }

    /** Create an EventAdapter with a static function and bound pointer callback
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    EventAdapter(void (*func)(const void*), const void *arg) {
        construct_callback(callback(func, arg));
    }

    /** Create an EventAdapter with a static function and bound pointer callback
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    EventAdapter(void (*func)(volatile void*), volatile void *arg) {
        construct_callback(callback(func, arg));
    }

    /** Create an EventAdapter with a static function and bound pointer callback
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    EventAdapter(void (*func)(const volatile void*), const volatile void *arg) {
        construct_callback(callback(func, arg));
    }

    /** Create an EventAdapter with a static function and bound pointer callback
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    template<typename T>
    EventAdapter(void (*func)(T*), T *arg) {
        construct_callback(callback(func, arg));
    }

    /** Create an EventAdapter with a static function and bound pointer callback
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    template<typename T>
    EventAdapter(void (*func)(const T*), const T *arg) {
        construct_callback(callback(func, arg));
    }

    /** Create an EventAdapter with a static function and bound pointer callback
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    template<typename T>
    EventAdapter(void (*func)(volatile T*), volatile T *arg) {
        construct_callback(callback(func, arg));
    }

    /** Create an EventAdapter with a static function and bound pointer callback
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    template<typename T>
    EventAdapter(void (*func)(const volatile T*), const volatile T *arg) {
        construct_callback(callback(func, arg));
    }

#if MBED_CONF_MBED_EVENTS_PRESENT
    /** Create an EventAdapter that holds an Event
     *  @param evt The event
     */
    EventAdapter(const adapter_event_t &evt) {
        construct_event(evt);
    }
#endif

    EventAdapter(const EventAdapter &adapter) {
        _attach(adapter);
    }

    /** Test if a function is attached to this adapter
     */
    operator bool() const {
        if (_is_cb) {
            return get_callback()->operator bool();
        } else {
            return true;
        }
    }

    EventAdapter& operator =(const EventAdapter &rhs) {
        if (this != &rhs) {
            destroy();
            _attach(rhs);
        }
        return *this;
    }

    /** Attach this adapter to the callback or event in a different adapter
     *  @param adapter The EventAdapter to attach.
     */
    void attach(const EventAdapter &adapter) {
        destroy();
        _attach(adapter);
    }

    /** Call the callback or event in this adapter
     */
    void call() {
        if (_is_cb) {
            get_callback()->call();
        } else {
#if MBED_CONF_MBED_EVENTS_PRESENT
            get_event()->call();
#endif
        }
    }

    /** Call the callback or event in this adapter
     */
    void operator ()() {
        call();
    }

    ~EventAdapter() {
        destroy();
    }

private:
    // No unions for non-POD data types in C++03. Need to use placement new to simulate the
    // union between a Callback<void()> and an Event<void()>
    union {
        // Ensure alignment by declaring the storage uint32_t instead of uint8_t
        uint32_t _cb_data[(sizeof(adapter_callback_t) + sizeof(uint32_t) - 1) / sizeof(uint32_t)];
#if MBED_CONF_MBED_EVENTS_PRESENT
        uint32_t _evt_data[(sizeof(adapter_event_t) + sizeof(uint32_t) - 1) / sizeof(uint32_t)];
#endif
    };
    bool  _is_cb;

    void destroy() {
        if (_is_cb) {
            get_callback()->~adapter_callback_t();
        } else {
#if MBED_CONF_MBED_EVENTS_PRESENT
            get_event()->~adapter_event_t();
#endif
        }
    }

    void construct_callback(const adapter_callback_t &cb) {
        _is_cb = true;
        adapter_callback_t *p_cb = reinterpret_cast<adapter_callback_t*>(&_cb_data);
        new (p_cb) adapter_callback_t();
        p_cb->attach(cb);
    }

#if MBED_CONF_MBED_EVENTS_PRESENT
    void construct_event(const adapter_event_t &evt) {
        _is_cb = false;
        adapter_event_t *p_evt = reinterpret_cast<adapter_event_t*>(&_evt_data);
        new(p_evt) adapter_event_t(evt);
    }

    const adapter_event_t *get_event() const {
        MBED_ASSERT(!_is_cb);
        return reinterpret_cast<const adapter_event_t*>(&_evt_data);
    }

    adapter_event_t *get_event() {
        MBED_ASSERT(!_is_cb);
        return reinterpret_cast<adapter_event_t*>(&_evt_data);
    }
#endif

    const adapter_callback_t *get_callback() const {
        MBED_ASSERT(_is_cb);
        return reinterpret_cast<const adapter_callback_t*>(&_cb_data);
    }

    adapter_callback_t *get_callback()  {
        MBED_ASSERT(_is_cb);
        return reinterpret_cast<adapter_callback_t*>(&_cb_data);
    }

    void _attach(const EventAdapter &adapter) {
        if (adapter._is_cb) {
            construct_callback(*adapter.get_callback());
        } else {
#if MBED_CONF_MBED_EVENTS_PRESENT
            construct_event(*adapter.get_event());
#endif
        }
    }
};

} // namespace mbed

#endif // #ifndef MBED_EVENT_ADAPTER_H
