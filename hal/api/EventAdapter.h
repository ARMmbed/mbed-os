#ifndef MBED_EVENT_ADAPTER_H
#define MBED_EVENT_ADAPTER_H

#include "Callback.h"
#include "Event.h"
#include <stdint.h>

namespace mbed {

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
 */
class EventAdapter {

    typedef Callback<void()> adapter_callback_t;
    typedef events::Event<void()> adapter_event_t;

public:
    /** Create an empty EventAdapter (it contains an empty Callback)
     */
    EventAdapter() {
        construct_callback(adapter_callback_t());
    }

    /** Create an EventAdapter that holds a callback
     *  @param cb The callback
     */
    EventAdapter(const adapter_callback_t& cb) {
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

    /** Create an EventAdapter that holds an Event
     *  @param evt The event
     */
    EventAdapter(const adapter_event_t& evt) {
        construct_event(evt);
    }

    EventAdapter(const EventAdapter& adapter) {
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

    EventAdapter& operator =(const EventAdapter& rhs) {
        if (this != &rhs) {
            destroy();
            _attach(rhs);
        }
        return *this;
    }

    /** Attach this adapter to the callback or event in a different adapter
     *  @param adapter The EventAdapter to attach.
     */
    void attach(const EventAdapter& adapter) {
        destroy();
        _attach(adapter);
    }

    /** Call the callback or event in this adapter
     */
    void call() {
        if (_is_cb) {
            get_callback()->call();
        } else {
            get_event()->call();
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
        uint32_t _evt_data[(sizeof(adapter_event_t) + sizeof(uint32_t) - 1) / sizeof(uint32_t)];
    };
    bool  _is_cb;

    void destroy() {
        if (_is_cb) {
            get_callback()->~adapter_callback_t();
        } else {
            get_event()->~adapter_event_t();
        }
    }

    void construct_callback(const adapter_callback_t& cb) {
        _is_cb = true;
        adapter_callback_t *p_cb = reinterpret_cast<adapter_callback_t*>(&_cb_data);
        new (p_cb) adapter_callback_t();
        p_cb->attach(cb);
    }

    void construct_event(const adapter_event_t&evt) {
        _is_cb = false;
        adapter_event_t *p_evt = reinterpret_cast<adapter_event_t*>(&_evt_data);
        new(p_evt) adapter_event_t(evt);
    }

    const adapter_event_t *get_event() const {
        return reinterpret_cast<const adapter_event_t*>(&_evt_data);
    }

    adapter_event_t *get_event() {
        return reinterpret_cast<adapter_event_t*>(&_evt_data);
    }

    const adapter_callback_t *get_callback() const {
        return reinterpret_cast<const adapter_callback_t*>(&_cb_data);
    }

    adapter_callback_t *get_callback()  {
        return reinterpret_cast<adapter_callback_t*>(&_cb_data);
    }

    void _attach(const EventAdapter& adapter) {
        if (adapter._is_cb) {
            construct_callback(*adapter.get_callback());
        } else {
            construct_event(*adapter.get_event());
        }
    }
};

} // namespace mbed

#endif
