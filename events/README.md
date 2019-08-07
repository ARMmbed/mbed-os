## The `mbed-events` library ##

The `mbed-events` library provides a flexible queue for scheduling events.

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

You can use the `mbed-events library` as a normal event loop, or you can
background it on a single hardware timer or even another event loop. It is
both thread and IRQ safe and provides functions for easily composing 
independent event queues.

The `mbed-events` library can act as a drop-in scheduler, provide synchronization
between multiple threads or act as a mechanism for moving events out of 
interrupt contexts.

### Usage ###

The core of the `mbed-events library` is the [EventQueue](EventQueue.h) class,
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
to the underlying event queue. The call functions are thread and IRQ safe,
don't need the underlying loop to be running and provide a mechanism
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

The call functions return an ID that uniquely represents the event in the 
the event queue. You can pass this ID to `EventQueue::cancel` to cancel
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

For a more detailed control of event dispatch, you can manually instantiate
and configure the `Event` class. An `Event` represents an event as
a C++ style function object, and you can directly pass it to other APIs that
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
Event<void(int, int)> event(&queue, printf, "received %d and %d\n");

// Events can be posted multiple times and enqueue gracefully until
// the dispatch function is called.
event.post(1, 2);
event.post(3, 4);
event.post(5, 6);

queue.dispatch();
```

Event queues easily align with module boundaries, where internal state can
be implicitly synchronized through event dispatch. Multiple modules can
use independent event queues but still be composed through the
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

## Underlying C API ##

The `mbed-events` C++ library is the recommended library for scheduling events. However, for occasions where C++ cannot be used for a project (e.g bare metal), the underlying C API `equeue` used by `mbed-events` can be accessed directly.

The `equeue` library is designed as a simple but powerful library for scheduling
events on composable queues.

``` c
#include "equeue.h"
#include <stdio.h>

int main() {
    // creates a queue with space for 32 basic events
    equeue_t queue;
    equeue_create(&queue, 32*EQUEUE_EVENT_SIZE);

    // events can be simple callbacks
    equeue_call(&queue, print, "called immediately");
    equeue_call_in(&queue, 2000, print, "called in 2 seconds");
    equeue_call_every(&queue, 1000, print, "called every 1 seconds");

    // events are executed in equeue_dispatch
    equeue_dispatch(&queue, 3000);

    print("called after 3 seconds");

    equeue_destroy(&queue);
}
```

The equeue library can be used as a normal event loop, or it can be
backgrounded on a single hardware timer or even another event loop. It
is both thread and irq safe, and provides functions for easily composing
multiple queues.

The equeue library can act as a drop-in scheduler, provide synchronization
between multiple threads, or just act as a mechanism for moving events
out of interrupt contexts.

## Documentation ##

The in-depth documentation on specific functions can be found in
[equeue.h](equeue.h).

The core of the equeue library is the `equeue_t` type which represents a
single event queue, and the `equeue_dispatch` function which runs the equeue,
providing the context for executing events.

On top of this, `equeue_call`, `equeue_call_in`, and `equeue_call_every`
provide easy methods for posting events to execute in the context of the
`equeue_dispatch` function.

``` c
#include "equeue.h"
#include "game.h"

equeue_t queue;
struct game game;

// button_isr may be in interrupt context
void button_isr(void) {
    equeue_call(&queue, game_button_update, &game);
}

// a simple user-interface framework
int main() {
    equeue_create(&queue, 4096);
    game_create(&game);

    // call game_screen_udpate at 60 Hz
    equeue_call_every(&queue, 1000/60, game_screen_update, &game);

    // dispatch forever
    equeue_dispatch(&queue, -1);
}
```

In addition to simple callbacks, an event can be manually allocated with
`equeue_alloc` and posted with `equeue_post` to allow passing an arbitrary
amount of context to the execution of the event. This memory is allocated out
of the equeue's buffer, and dynamic memory can be completely avoided.

The equeue allocator is designed to minimize jitter in interrupt contexts as
well as avoid memory fragmentation on small devices. The allocator achieves
both constant-runtime and zero-fragmentation for fixed-size events, however
grows linearly as the quantity of differently-sized allocations increases.

``` c
#include "equeue.h"

equeue_t queue;

// arbitrary data can be moved to a different context
int enet_consume(void *buffer, int size) {
    if (size > 512) {
        size = 512;
    }

    void *data = equeue_alloc(&queue, 512);
    memcpy(data, buffer, size);
    equeue_post(&queue, handle_data_elsewhere, data);

    return size;
}
```

Additionally, in-flight events can be cancelled with `equeue_cancel`. Events
are given unique ids on post, allowing safe cancellation of expired events.

``` c
#include "equeue.h"

equeue_t queue;
int sonar_value;
int sonar_timeout_id;

void sonar_isr(int value) {
    equeue_cancel(&queue, sonar_timeout_id);
    sonar_value = value;
}

void sonar_timeout(void *) {
    sonar_value = -1;
}

void sonar_read(void) {
    sonar_timeout_id = equeue_call_in(&queue, 300, sonar_timeout, 0);
    sonar_start();
}
```

From an architectural standpoint, event queues easily align with module
boundaries, where internal state can be implicitly synchronized through
event dispatch.

On platforms where multiple threads are unavailable, multiple modules
can use independent event queues and still be composed through the
`equeue_chain` function.

``` c
#include "equeue.h"

// run a simultaneous localization and mapping loop in one queue
struct slam {
    equeue_t queue;
};

void slam_create(struct slam *s, equeue_t *target) {
    equeue_create(&s->queue, 4096);
    equeue_chain(&s->queue, target);
    equeue_call_every(&s->queue, 100, slam_filter);
}

// run a sonar with it's own queue
struct sonar {
    equeue_t equeue;
    struct slam *slam;
};

void sonar_create(struct sonar *s, equeue_t *target) {
    equeue_create(&s->queue, 64);
    equeue_chain(&s->queue, target);
    equeue_call_in(&s->queue, 5, sonar_update, s);
}

// all of the above queues can be combined into a single thread of execution
int main() {
    equeue_t queue;
    equeue_create(&queue, 1024);

    struct sonar s1, s2, s3;
    sonar_create(&s1, &queue);
    sonar_create(&s2, &queue);
    sonar_create(&s3, &queue);

    struct slam slam;
    slam_create(&slam, &queue);

    // dispatches events from all of the modules
    equeue_dispatch(&queue, -1);
}
```

## Platform ##

The equeue library has a minimal porting layer that is flexible depending
on the requirements of the underlying platform. Platform specific declarations
and more information can be found in [equeue_platform.h](equeue_platform.h).

## Tests ##

The equeue library uses a set of local tests based on the posix implementation.

Runtime tests are located in [tests.c](tests/tests.c):

``` bash
make test
```

Profiling tests based on rdtsc are located in [prof.c](tests/prof.c):

``` bash
make prof
```

To make profiling results more tangible, the profiler also supports percentage
comparison with previous runs:
``` bash
make prof | tee results.txt
cat results.txt | make prof
```

