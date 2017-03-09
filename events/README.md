## The mbed-events library ##

The mbed-events library provides a flexible queue for scheduling events.

``` cpp
#include "mbed_events.h"
#include <stdio.h>

int main() {
    // creates a queue with the default size
    EventQueue queue;

    // events are simple callbacks
    queue.call(printf, "called immediately\n");
    queue.call_in(2000, printf, "called in 2 seconds\n");
    queue.call_every(1000, printf, "called every 1 seconds\n");

    // events are executed by the dispatch method
    queue.dispatch();
}
```

The mbed-events library can be used as a normal event loop, or it can be
backgrounded on a single hardware timer or even another event loop. It is
both thread and irq safe, and provides functions for easily composing 
independent event queues.

The mbed-events library can act as a drop-in scheduler, provide synchronization
between multiple threads, or just act as a mechanism for moving events out of 
interrupt contexts.

### Usage ###

The core of the mbed-events library is the [EventQueue](EventQueue.h) class,
which represents a single event queue. The `EventQueue::dispatch` function
runs the queue, providing the context for executing events.

``` cpp
// Creates an event queue enough buffer space for 32 Callbacks. This
// is the default if no argument was provided. Alternatively the size
// can just be specified in bytes.
EventQueue queue(32*EVENTS_EVENT_SIZE);

// Events can be posted to the underlying event queue with dynamic
// context allocated from the specified buffer
queue.call(printf, "hello %d %d %d %d\n", 1, 2, 3, 4);
queue.call(&serial, &Serial::printf, "hi\n");

// The dispatch function provides the context for the running the queue
// and can take a millisecond timeout to run for a fixed time or to just
// dispatch any pending events
queue.dispatch();
```

The EventQueue class provides several call functions for posting events
to the underlying event queue. The call functions are thread and irq safe,
don't need the underlying loop to be running, and provide an easy mechanism
for moving events out of interrupt contexts.

``` cpp
// Simple call function registers events to be called as soon as possible
queue.call(doit);
queue.call(printf, "called immediately\n");

// The call_in function registers events to be called after a delay
// specified in milliseconds
queue.call_in(2000, doit_in_two_seconds);
queue.call_in(300, printf, "called in 0.3 seconds\n");

// The call_every function registers events to be called repeatedly
// with a period specified in milliseconds
queue.call_every(2000, doit_every_two_seconds);
queue.call_every(400, printf, "called every 0.4 seconds\n");
```

The call functions return an id that uniquely represents the event in the 
the event queue. This id can be passed to `EventQueue::cancel` to cancel
an in-flight event.

``` cpp
// The event id uniquely represents the event in the queue
int id = queue.call_in(100, printf, "will this work?\n");

// If there was not enough memory necessary to allocate the event,
// an id of 0 is returned from the call functions
if (id) {
    error("oh no!");
}

// Events can be cancelled as long as they have not been dispatched. If the
// event has already expired, cancel has no side-effects.
queue.cancel(id);
```

For a more fine-grain control of event dispatch, the `Event` class can be
manually instantiated and configured. An `Event` represents an event as
a C++ style function object and can be directly passed to other APIs that
expect a callback.

``` cpp
// Creates an event bound to the specified event queue
EventQueue queue;
Event<void()> event(&queue, doit);

// The event can be manually configured for special timing requirements
// specified in milliseconds
event.delay(10);
event.period(10000);

// Posted events are dispatched in the context of the queue's
// dispatch function
queue.dispatch();

// Events can also pass arguments to the underlying callback when both
// initially constructed and posted.
Event<void(int, int)> event(&queue, printf, "recieved %d and %d\n");

// Events can be posted multiple times and enqueue gracefully until
// the dispatch function is called.
event.post(1, 2);
event.post(3, 4);
event.post(5, 6);

queue.dispatch();
```

Event queues easily align with module boundaries, where internal state can
be implicitly synchronized through event dispatch. Multiple modules can
use independent event queues, but still be composed through the
`EventQueue::chain` function.

``` cpp
// Create some event queues with pending events
EventQueue a;
a.call(printf, "hello from a!\n");

EventQueue b;
b.call(printf, "hello from b!\n");

EventQueue c;
c.call(printf, "hello from c!\n");

// Chain c and b onto a's event queue. Both c and b will be dispatched
// in the context of a's dispatch function.
c.chain(&a);
b.chain(&a);

// Dispatching a will in turn dispatch b and c, printing hello from
// all three queues
a.dispatch();
```


