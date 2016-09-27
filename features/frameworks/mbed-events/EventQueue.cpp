#include "EventQueue.h"

#include "mbed_events.h"
#include "mbed.h"


EventQueue::EventQueue(unsigned event_size, unsigned char *event_pointer) {
    if (!event_pointer) {
        equeue_create(&_equeue, event_size);
    } else {
        equeue_create_inplace(&_equeue, event_size, event_pointer);
    }
}

EventQueue::~EventQueue() {
    equeue_destroy(&_equeue);
}

void EventQueue::dispatch(int ms) {
    return equeue_dispatch(&_equeue, ms);
}

void EventQueue::break_dispatch() {
    return equeue_break(&_equeue);
}

unsigned EventQueue::tick() {
    return equeue_tick();
}

void EventQueue::cancel(int id) {
    return equeue_cancel(&_equeue, id);
}

void EventQueue::background(Callback<void(int)> update) {
    _update = update;

    if (_update) {
        equeue_background(&_equeue, &Callback<void(int)>::thunk, &_update);
    } else {
        equeue_background(&_equeue, 0, 0);
    }
}

void EventQueue::chain(EventQueue *target) {
    if (target) {
        equeue_chain(&_equeue, &target->_equeue);
    } else {
        equeue_chain(&_equeue, 0);
    }
}
