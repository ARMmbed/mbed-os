## The equeue library ##

The equeue library is designed as a simple but powerful library for scheduling
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

