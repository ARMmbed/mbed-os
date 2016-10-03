# About the mbed OS event loop

One of the optional mbed OS features is an event loop mechanism that can be used to defer the execution of code in a different context. In particular, a common uses of an event loop is to postpone the execution of a code sequence from an interrupt handler to an user context. This is useful because of the specific constraints of code that runs in an interrupt handler:

- the execution of certain functions (notably some functions in the C library) is not safe.
- various RTOS objects and functions can't be used from an interrupt context.
- as a general rule, the code needs to finish as fast as possible, to allow other interrupts to be handled.

The event loop offers a solution to these issues in the form of an API that can be used to defer execution of code from the interrupt context to the user context. More generally, the event loop can be used anywhere in a program (not necessarily in an interrupt handler) to defer code execution to a different context.

# Overview of the mbed OS event loop

An event loop has two main components:

1. an **event queue**, used to store events. In mbed OS, *events* are pointers to functions (and optionally function arguments).
2. an **event loop** that extracts events from the queue and executes them.

The mbed OS event queue is implemented by the [mbed-events library](http://github.com/ARMmbed/mbed-os/tree/master/events). It's a good idea to go through the [README of mbed-events](https://github.com/ARMmbed/mbed-os/blob/master/events/README.md), as it shows how to use the event queue.

The event loop must be created and started manually. The simplest way to achieve that is to create a `Thread` and run the event queue's `dispatch` method in the thread:

```
#include "mbed.h"
#include "mbed_events.h"

// Create a queue that can hold a maximum of 32 events
Queue queue(32 * EVENTS_EVENT_SIZE);
// Create a thread that'll run the event queue's dispatch function
Thread t;

int main () {
    // Start the event queue's dispatch thread
    t.start(callback(&queue, &EventQueue::dispatch_forever));
    ...
}
```

Note that although this document assumes the presence of a single event loop in the system, there's nothing preventing the programmer to run more than one event loop, simply by following the create/start pattern above for each of them.

## Using the event loop

Once the event loop is created, it can be used for posting events. Let's consider a very simple example of a program that attaches two interrupt handlers for an InterruptIn object, using the InterruptIn `rise` and `fall` functions. The `rise` handler will run in interrupt context, while the `fall` handler will run in user context (more specifically, in the context of the event loop's thread). The full code for the example can be found below:

```
#include "mbed.h"
#include "mbed_events.h"

DigitalOut led1(LED1);
InterruptIn sw(SW2);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;

void rise_handler(void) {
    printf("rise_handler in context %p\r\n", Thread::gettid());
    // Toggle LED
    led1 = !led1;
}

void fall_handler(void) {
    printf("fall_handler in context %p\r\n", Thread::gettid());
    // Toggle LED
    led1 = !led1;
}

int main() {
    // Start the event queue
    t.start(callback(&queue, &EventQueue::dispatch_forever));
    printf("Starting in context %p\r\n", Thread::gettid());
    // The 'rise' handler will execute in IRQ context
    sw.rise(rise_handler);
    // The 'fall' handler will execute in the context of thread 't'
    sw.fall(queue.event(fall_handler));
}

```

The above code executes two handler functions (`rise_handler` and `fall_handler`) in two different contexts:

1. in interrupt context when a rising edge is detected on `SW2` (`rise_handler`).
2. in the context of the event loop's thread function when a falling edge is detected on `SW2` (`fall_handler`). `queue.event()` is called with `fall_handler` as an argument to specify that `fall_handler` will run in user context instead of interrupt context.

This is the output of the above program on a FRDM-K64F board after resetting the board and pressing the SW2 button twice:

```
Starting in context 0x20002c50
fall_handler in context 0x20002c90
rise_handler in context 0x0
fall_handler in context 0x20002c90
rise_handler in context 0x0
```

The program starts in the context of the thread that runs the `main` function (`0x29992c5`). When the uses presses SW2, `fall_handler` is automatically queued in the event queue, and  it runs later in the context of thread `t` (`0x20002c90`). When the user releases the button, `rise_handler` is executed immediately, and it displays `0x0`, indicating that the code runs in interrupt context.

The code for `rise_handler` is problematic, since it calls `printf` in interrupt context, which is a potentially unsafe operation. Fortunately, this is exactly the kind of problem that event queues can solve. We can make the code safe by running `rise_handler` in user context (like we already do with `fall_handler`) by replacing this line:

```
sw.rise(rise_handler);
```

with this line:

```
sw.rise(queue.event(rise_handler));
```

The code is safe now, but we might've introduced another problem: latency. After the change above, the call to `rise_handler` will be queued, which means that it doesn't run immediately after the interrupt is raised anymore. For this example code, this isn't a problem, but some applications might require the code to respond as fast as possible to an interrupt. Let's assume that `rise_handler` must toggle the LED as quickly as possible in response to the user's action on SW2. To do that, in must run in interrupt context. However, `rise_handler` still needs to print a message indicating that the handler was called, but that's problematic since it's not safe to call `printf` from an interrupt context. The solution is to split `rise_handler` in two parts: the time critical part will run in interrupt context, while the non-critical part (displaying the message) will run in user context. This is easily doable using `queue.call`:

```
void rise_handler_user_context(void) {
    printf("rise_handler_user_context in context %p\r\n", Thread::gettid());
}

void rise_handler(void) {
    // Execute the time critical part first
    led1 = !led1;
    // The rest can execute later in user context (and can contain code that's not interrupt safe).
    // We use the 'queue.call' function to add an event (the call to 'rise_handler_user_context') to the queue.
    queue.call(rise_handler_user_context);
}

```

After replacing the code for `rise_handler` as above, the output of our example becomes:

```
Starting in context 0x20002c50
fall_handler in context 0x20002c90
rise_handler_user_context in context 0x20002c90
fall_handler in context 0x20002c90
rise_handler_user_context in context 0x20002c90
```

The scenario above (splitting an interrupt handler's code into time critical code and non-time critical code) is another common pattern that's easily implemented with event queues. Another thing to learn from this example is that queuing code that's not interrupt safe is not the only thing that event queues can be used for. Any kind of code can be queued and deferred for later execution.

We used `InterruptIn` for the example above, but the same kind of code can be used with any `attach()`-like functions in the SDK. Example include `Serial::attach()`, `Ticker::attach()`, `Ticker::attach_us()`, `Timeout::attach()`.

## Where to go from here

We just scratched the surface of how event queues work in mbed OS. The `EventQueue` and `Event` classes in the `mbed-events` library offer a lot of features that are not covered in this document, including calling functions with arguments, queueing functions to be called after a delay, or queueing functions to be called periodically. The [README of the mbed-events library](https://github.com/ARMmbed/mbed-os/blob/master/events/README.md) shows more ways to use events and event queues. For more details about how the events library is implemented, check [this file](https://github.com/ARMmbed/mbed-os/blob/master/events/equeue/README.md).
