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

#include "Event.h"
#include "EventLoop.h"
#include "mbed_assert.h"

namespace mbed {

Event::Event(void (*func)(void), EventLoop *queue):
    _callback(func), _queue(queue) {
}

Event::Event(const Callback<void()>& cb, EventLoop *queue):
    _callback(), _queue(queue) {
    _callback.attach(cb);
}

Event::Event(void *obj, void (*func)(void*), EventLoop *queue):
    _callback(obj, func), _queue(queue)  {
}

Event::Event(const void *obj, void (*func)(const void*), EventLoop *queue):
    _callback(obj, func), _queue(queue)  {
}

Event::Event(volatile void *obj, void (*func)(volatile void*), EventLoop *queue):
    _callback(obj, func), _queue(queue)  {
}

Event::Event(const volatile void *obj, void (*func)(const volatile void*), EventLoop *queue):
    _callback(obj, func), _queue(queue)  {
}

template<typename T>
Event::Event(T *obj, void (*func)(T*), EventLoop *queue):
    _callback(obj, func), _queue(queue)  {
}

template<typename T>
Event::Event(const T *obj, void (*func)(const T*), EventLoop *queue):
    _callback(obj, func), _queue(queue)  {
}

template<typename T>
Event::Event(volatile T *obj, void (*func)(volatile T*), EventLoop *queue):
    _callback(obj, func), _queue(queue)  {
}

template<typename T>
Event::Event(const volatile T *obj, void (*func)(const volatile T*), EventLoop *queue):
    _callback(obj, func), _queue(queue)  {
}

template<typename T>
Event::Event(T *obj, void (T::*func)(), EventLoop *queue):
    _callback(obj, func), _queue(queue)  {
}

template<typename T>
Event::Event(const T *obj, void (T::*func)() const, EventLoop *queue):
    _callback(obj, func), _queue(queue)  {
}

template<typename T>
Event::Event(volatile T *obj, void (T::*func)() volatile, EventLoop *queue):
    _callback(obj, func), _queue(queue)  {
}

template<typename T>
Event::Event(const volatile T *obj, void (T::*func)() const volatile, EventLoop *queue):
    _callback(obj, func), _queue(queue)  {
}

void Event::attach(void (*func)(), EventLoop *queue) {
    _queue = queue;
    _callback.attach(func);
}

void Event::attach(const Event& evt) {
    _queue = evt._queue;
    _callback.attach(evt._callback);
}

void Event::attach(void *obj, void (*func)(void*), EventLoop *queue) {
    _queue = queue;
    _callback.attach(obj, func);
}

void Event::attach(const void *obj, void (*func)(const void*), EventLoop *queue) {
    _queue = queue;
    _callback.attach(obj, func);
}

void Event::attach(volatile void *obj,  void (*func)(volatile void*), EventLoop *queue) {
    _queue = queue;
    _callback.attach(obj, func);
}

void Event::attach(const volatile void *obj,  void (*func)(const volatile void*), EventLoop *queue) {
    _queue = queue;
    _callback.attach(obj, func);
}

template <typename T>
void Event::attach(T *obj,  void (*func)(T*), EventLoop *queue) {
    _queue = queue;
    _callback.attach(obj, func);
}

template <typename T>
void Event::attach(const T *obj,  void (*func)(const T*), EventLoop *queue) {
    _queue = queue;
    _callback.attach(obj, func);
}

template <typename T>
void Event::attach(volatile T *obj,  void (*func)(volatile T*), EventLoop *queue) {
    _queue = queue;
    _callback.attach(obj, func);
}

template <typename T>
void Event::attach(const volatile T *obj,  void (*func)(const volatile T*), EventLoop *queue) {
    _queue = queue;
    _callback.attach(obj, func);
}

template<typename T>
void Event::attach(T *obj,  void (T::*func)(), EventLoop *queue) {
    _queue = queue;
    _callback.attach(obj, func);
}

template<typename T>
void Event::attach(const T *obj,  void (T::*func)() const, EventLoop *queue) {
    _queue = queue;
    _callback.attach(obj, func);
}

template<typename T>
void Event::attach(volatile T *obj,  void (T::*func)() volatile, EventLoop *queue) {
    _queue = queue;
    _callback.attach(obj, func);
}

template<typename T>
void Event::attach(const volatile T *obj,  void (T::*func)() const volatile, EventLoop *queue) {
    _queue = queue;
    _callback.attach(obj, func);
}

void Event::call() const {
    if (_queue == 0) {
        _callback.call();
    } else {
#ifdef MBED_CONF_RTOS_PRESENT
        MBED_ASSERT(_queue->post(_callback));
#else
        // There's no event queue without the RTOS, so nothing to do here but assert.
        MBED_ASSERT(false);
#endif
    }
}

void Event::operator()() const {
    call();
}

Event::operator bool() const {
    return _callback.operator bool();
}

void Event::thunk(void *func) {
    static_cast<Event*>(func)->call();
}

Event event(void (*func)(void), EventLoop *queue) {
    return Event(func, queue);
}

Event event(volatile void *obj, void (*func)(volatile void*), EventLoop *queue) {
    return Event(obj, func, queue);
}

Event event(const volatile void *obj, void (*func)(const volatile void*), EventLoop *queue) {
    return Event(obj, func, queue);
}

template <typename T>
Event event(T *obj, void (*func)(T*), EventLoop *queue) {
    return Event(obj, func, queue);
}

template <typename T>
Event event(const T *obj, void (*func)(const T*), EventLoop *queue) {
    return Event(obj, func, queue);
}

template <typename T>
Event event(volatile T *obj, void (*func)(volatile T*), EventLoop *queue) {
    return Event(obj, func, queue);
}

template <typename T>
Event event(const volatile T *obj, void (*func)(const volatile T*), EventLoop *queue) {
    return Event(obj, func, queue);
}

template<typename T>
Event event(T *obj, void (T::*func)(), EventLoop *queue) {
    return Event(obj, func, queue);
}

template<typename T>
Event event(const T *obj, void (T::*func)() const, EventLoop *queue) {
    return Event(obj, func, queue);
}

template<typename T>
Event event(volatile T *obj, void (T::*func)() volatile, EventLoop *queue) {
    return Event(obj, func, queue);
}

template<typename T>
Event event(const volatile T *obj, void (T::*func)() const volatile, EventLoop *queue) {
    return Event(obj, func, queue);
}

} // namespace mbed
