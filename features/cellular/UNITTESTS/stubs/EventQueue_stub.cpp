
#include "EventQueue.h"
#include "Callback.h"

using namespace mbed;

namespace events {

EventQueue::EventQueue(unsigned event_size, unsigned char *event_pointer) {
}

EventQueue::~EventQueue() {
}

void EventQueue::dispatch(int ms) {
}

void EventQueue::break_dispatch() {
}

unsigned EventQueue::tick() {
    return 0;
}

void EventQueue::cancel(int id) {
}

void EventQueue::background(Callback<void(int)> update) {
}

void EventQueue::chain(EventQueue *target) {
}

}
