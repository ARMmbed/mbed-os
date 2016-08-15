# utest: Asynchronous C++ Test Harness

This test harness allows you to execute a specified series of (asynchronous) C++ test cases with sensible default reporting and useful customization options.

Please note that this is a purposefully lean test harness, only dealing with test execution and providing default reporting handlers. It specifically does not support auto-discovery of test cases and does not provide you with test macros or other convenience functions.
Instead, the macros in the [unity module](https://github.com/ARMmbed/mbed-os/tree/master/features/frameworks/unity) can be used for this purpose. However, you are not required to use these, and can use your own macros if you wish.

Furthermore, test failure recovery through the use of exceptions or `longjmp` is not supported; the test will either continue and ignore failures or die by busy-waiting.

## Theory of Operation

A test specification contains a setup handler, several test cases and a teardown handler.
Each test case contains a textual description, setup, teardown and failure handler as well as the actual test handler.

The order of handler execution is:

1. Test setup handler.
1. For each test case:
    1. Test case setup handler.
    1. Test case execution handler.
    1. (wait for timeout or callback validation in case of an asynchronous test case.)
    1. (repeat test case execution handler if specified.)
    1. Test case teardown handler.
1. Test teardown handler.

## Example

The following example showcases a lot of functionality and proper integration with the [Greentea testing automation framework](https://github.com/ARMmbed/greentea), while making use of the [unity test macros](https://github.com/ARMmbed/mbed-os/tree/master/features/frameworks/unity):

```cpp
#include "mbed-drivers/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

using namespace utest::v1;

void test_simple() {
    TEST_ASSERT_EQUAL(0, 0);
    printf("Simple test called\n");
}

status_t test_repeats_setup(const Case *const source, const size_t index_of_case) {
    // Call the default handler for proper reporting
    status_t status = greentea_case_setup_handler(source, index_of_case);
    printf("Setting up for '%s'\n", source->get_description());
    return status;
}
control_t test_repeats(const size_t call_count) {
    printf("Called for the %u. time\n", call_count);
    TEST_ASSERT_NOT_EQUAL(3, call_count);
    // Specify how often this test is repeated ie. n total calls
    return (call_count < 2) ? CaseRepeatAll : CaseNext;
}

void test_callback_validate() {
    // You may also use assertions here!
    TEST_ASSERT_EQUAL_PTR(0, 0);
    // Validate the callback
    Harness::validate_callback();
}
control_t test_asynchronous() {
    TEST_ASSERT_TRUE_MESSAGE(true, "(true == false) o_O");
    // Set up a callback in the future. This may also be an interrupt!
    minar::Scheduler::postCallback(test_callback_validate).delay(minar::milliseconds(100));
    // Set a 200ms timeout starting from now
    return CaseTimeout(200);
}

control_t test_asynchronous_timeout(const size_t call_count) {
    TEST_ASSERT_TRUE_MESSAGE(true, "(true == false) o_O");
    // Set a 200ms timeout starting from now,
    // but automatically repeat only this handler on timeout.
    if (call_count >= 5) {
        // but after the 5th call, the callback finally gets validated
        minar::Scheduler::postCallback(test_callback_validate).delay(minar::milliseconds(100));
    }
    return CaseRepeatHandlerOnTimeout(200);
}

// Custom setup handler required for proper Greentea support
status_t greentea_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "default_auto");
    // Call the default reporting function
    return greentea_test_setup_handler(number_of_cases);
}

// Specify all your test cases here
Case cases[] = {
    Case("Simple Test", test_simple),
    Case("Repeating Test", test_repeats_setup, test_repeats),
    Case("Asynchronous Test (200ms timeout)", test_asynchronous),
    Case("Asynchronous Timeout Repeat", test_asynchronous_timeout)
};

// Declare your test specification with a custom setup handler
Specification specification(greentea_setup, cases);

void app_start(int, char**)
{   // Run the test specification
    Harness::run(specification);
}
```

Running this test will output the following:

```
{{timeout;20}}
{{host_test_name;default_auto}}
{{description;utest greentea example}}
{{test_id;MBED_OS}}
{{start}}
>>> Running 4 test cases...

>>> Running case #1: 'Simple Test'...
Simple test called
>>> 'Simple Test': 1 passed, 0 failed

>>> Running case #2: 'Repeating Test'...
Setting up for 'Repeating Test'
Called for the 1. time
>>> 'Repeating Test': 1 passed, 0 failed

>>> Running case #2: 'Repeating Test'...
Setting up for 'Repeating Test'
Called for the 2. time
>>> 'Repeating Test': 2 passed, 0 failed

>>> Running case #3: 'Asynchronous Test (200ms timeout)'...
>>> 'Asynchronous Test (200ms timeout)': 1 passed, 0 failed

>>> Running case #4: 'Asynchronous Timeout Repeat'...
>>> failure with reason 'Ignored: Timed Out'
>>> failure with reason 'Ignored: Timed Out'
>>> failure with reason 'Ignored: Timed Out'
>>> failure with reason 'Ignored: Timed Out'
>>> failure with reason 'Ignored: Timed Out'
>>> 'Asynchronous Timeout Repeat': 1 passed, 0 failed

>>> Test cases: 4 passed, 0 failed
{{success}}
{{end}}
```

## Detailed Description

### Handlers

There are five handler types you can, but do not have to, override to customize operation.
Please see the `utest/types.h` file for a detailed description.

1. `status_t test_setup_handler_t(const size_t number_of_cases)`: called before execution of any test case.
1. `void test_teardown_handler_t(const size_t passed, const size_t failed, const failure_t failure)`: called after execution of all test cases, and if testing is aborted.
1. `void test_failure_handler_t(const failure_t failure)`: called whenever a failure occurs anywhere in the specification.
1. `status_t case_setup_handler_t(const Case *const source, const size_t index_of_case)`: called before execution of each test case.
1. `status_t case_teardown_handler_t(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)`: called after execution of each test case, and if testing is aborted.
1. `status_t case_failure_handler_t(const Case *const source, const failure_t reason)`: called whenever a failure occurs during the execution of a test case.

All handlers are defaulted for integration with the [Greentea testing automation framework](https://github.com/ARMmbed/greentea).

### Test Case Handlers

There are three test case handlers:

1. `void case_handler_t(void)`: executes once, if the case setup succeeded.
1. `control_t case_control_handler_t(void)`: executes (asynchronously) as many times as you specify, if the case setup succeeded.
1. `control_t case_call_count_handler_t(const size_t call_count)`: executes (asynchronously) as many times as you specify, if the case setup succeeded.

To specify a test case you must wrap it into a `Case` class: `Case("mandatory description", case_handler)`. You may override the setup, teardown and failure handlers in this wrapper class as well.
The `Case` constructor is overloaded to allow you a comfortable declaration of all your callbacks and the order of arguments is:

1. Description (required).
1. Setup handler (optional).
1. Test case handler (required).
1. Teardown handler (optional).
1. Failure handler (optional).

#### Test Case Attributes

You can modify test case behavior by returning `control_t` modifiers:

- `CaseNext`: never repeats and immediately moves to next test case
- `CaseNoRepeat`: never repeats.
- `CaseRepeatAll`: repeats test case **with** setup and teardown handlers.
- `CaseRepeatHandler`: repeats test case **without** set and teardown handlers.
- `CaseNoTimeout`: immediately moves to next test case.
- `CaseAwait`: waits indefinitely for callback validation (*use with caution*).
- `CaseTimeout(uint32_t ms)`: waits for callback validation for `ms` milliseconds, times out after that (fails with `REASON_TIMEOUT`).
- `CaseRepeatAllOnTimeout(uint32_t ms)`: waits for callback validation for `ms` milliseconds, repeats test case **with** setup and teardown handlers on time out.
- `CaseRepeatHandlerOnTimeout(uint32_t ms)`: waits for callback validation for `ms` milliseconds, repeats test case **without** setup and teardown handlers on time out.

Returning `CaseRepeatAll` from your test case handler tells the test harness to repeat the test handler. You can use the `call_count` (starts counting at 1) to decide when to stop.
By default the setup and teardown handlers are called on every repeated test cases, however, you may only repeat the case handler by returning `CaseRepeatHandler`. To stop the harness from repeating the test case, return `CaseNext`.

For asynchronous test cases, you must return a `CaseTimeout(uint32_t ms)`.
If you want to automatically repeat the test case on a timeout, use `CaseRepeatAllOnTimeout(uint32_t ms)` and `CaseRepeatHandlerOnTimeout(uint32_t ms)`.

To validate your callback, you must call `Harness::validate_callback()` in your asynchronous callback before the timeout fires.
This will schedule the execution of the next test case.

For repeating asynchronous cases, you can "add" both attributes together: `CaseTimeout(200) + CaseRepeatAll` will wait for 200ms for the callback validation and then repeat the test case. See the section on arbitration logic for more details.

Note that you can also add attributes during callback validation, however, only repeat attributes are considered. This allows you to return `CaseTimeout(500)` to wait up to 500ms for the callback validation and delegate the decision to repeat to the time the callback occurs: `Harness::validate_callback(CaseRepeatHandler)`.

Keep in mind that you can only validate a callback once. If you need to wait for several callbacks, you need to write your own helper function that validates the expected callback only when all your custom callbacks arrive.
This custom functionality is purposefully not part of this test harness, you can achieve it externally with additional code.

### Failure Handlers

A failure may occur during any phase of the test. The appropriate failure handler is then called with `failure_t`, which contains the failure reason and location.

The failure reasons are:

- `REASON_NONE`: No failure occurred
- `REASON_UNKNOWN`: An unknown failure occurred
- `REASON_CASES`: A failure occurred in at least one test case
- `REASON_EMPTY_CASE`: The test case contains only empty handlers
- `REASON_TIMEOUT`: An expected asynchronous call timed out
- `REASON_ASSERTION`: An assertion failed
- `REASON_TEST_SETUP`: Test setup failed
- `REASON_TEST_TEARDOWN`: Test teardown failed
- `REASON_CASE_SETUP`: Case setup failed
- `REASON_CASE_HANDLER`: Case handler failed
- `REASON_CASE_TEARDOWN`: Case teardown failed
- `REASON_CASE_INDEX`: Case index returned from test setup or case teardown handler is invalid
- `REASON_SCHEDULER`: Underlying scheduler is not asynchronous

The failure locations are:

- `LOCATION_NONE`: No location information
- `LOCATION_UNKNOWN`: A failure occurred in an unknown location
- `LOCATION_TEST_SETUP`: A failure occurred in the test setup
- `LOCATION_TEST_TEARDOWN`: A failure occurred in the test teardown
- `LOCATION_CASE_SETUP`: A failure occurred in the case setup
- `LOCATION_CASE_HANDLER`: A failure occurred in the case handler
- `LOCATION_CASE_TEARDOWN`: A failure occurred in the case teardown

If the setup or teardown handlers fail, they may return a `STATUS_ABORT` code, which will call the failure handler with the appropriate failure reason (`REASON_CASE_{SETUP|TEARDOWN}`) and failure location (`LOCATION_CASE_{SETUP|TEARDOWN}`).
If the setup handler fails, the test case is never executed. Instead, the teardown handler is called in an attempt to salvage the situation.
Please note that if a teardown handler fails, the system can be considered too unstable to continue testing.

You may also raise a failure manually by calling `Harness::raise_failure(failure_reason_t reason)`. In fact, this is how you can integrate assertion failures from custom test macros, as done with the unity macros, which raise a failure with the `REASON_ASSERTION` reason.

When waiting for an asynchronous callback, if the timeout fires, `REASON_TIMEOUT` is raised.

The failure handler decides whether to continue or abort testing by returning `STATUS_CONTINUE` or `STATUS_ABORT` respectively.
You can also ignore any raised failure by returning `STATUS_IGNORE` and the harness will then not count this failure.
In case of an abort, the test harness dies by busy waiting in a forever loop.
This is needed because we cannot unwind the stack without exception support, and the asynchronous nature of the test harness breaks with using `longjmp`s.

Note that when `REASON_IGNORE` is `OR`ed into the failure reason, the failure handler is expected to return `STATUS_IGNORE`.
This is done automatically for test cases repeating after a timeout, and the default failure handlers also report this failure, but tell the harness to ignore it.
Furthermore, the unity macros may decide to ignore assertion failures as well, in which case the assertion is ignored intentionally.

### Default Handlers

Three sets of default handlers with different behaviors are provided for your convenience:

1. `greentea_abort_handlers` (default): Greentea-style reporting, aborts on the first failure, but requires custom test setup handler.
1. `greentea_continue_handlers`: Greentea-style reporting, always continues testing, but requires custom test setup handler.
1. `verbose_continue_handlers`: always continues testing and reporting, except when a teardown failed.
1. `selftest_handlers`: Greentea-style reporting, but aborts on the first assertion failure raised. This allows the use of unity macros for self testing without recursive failure handler calls.

These default handlers are called when you have not overridden a custom handler, and they only contain reporting functionality and do not modify global state.

You can specify which default handlers you want to use when wrapping your test cases in the `Specification` class:

```cpp
// Declare your test specification with a custom setup handler
// and set the default handlers to the predefined “greentea continue” behavior
Specification specification(greentea_setup, cases, greentea_continue_handlers);
```

### Custom Handlers

You may override any of the default handlers with your own custom handler.

To ignore a handler completely and not call a custom or default handler, you may use the `ignore_handler` hint.
To explicitly invoke the default handler, use the `default_handler` hint.

To use your own custom handler, provide a function with the correct signature for the handler that you want to customize and provide it in your test case wrapper or specification wrapper.
To turn a `failure_t` into a meaningful string use the `stringify(failure_t)` method.

**We strongly recommend that you call the predefined `greentea_*` handlers inside your custom callback, as they report the current condition in a properly formatted fashion.**
By calling these handlers inside your custom callback your unit test does not need to be modified if the test logging needs to be changed in the future.

For the `Specification` class the order of arguments is:

1. Test setup handler (optional).
1. Array of test cases (required).
1. Test teardown handler (optional).
1. Test failure handler (optional).
1. Default handlers (optional).

### Test Case Attribute Arbitration

When adding conflicting modifiers together

- the more restrictive (=shorter) timeout is chosen, but `CaseNoTimeout` always wins arbitration: `CaseNoTimeout` > `CaseTimeout(100)` > `CaseTimeout(200)` > `CaseAwait`.
- the more invasive repeat method is chosen, but `CaseNoRepeat` always wins arbitration: `CaseNoRepeat` > `CaseRepeatAll`/`CaseRepeatAllOnTimeout(ms)` > `CaseRepeatHandler`/`CaseRepeatHandlerOnTimeout(ms)`.
- `CaseNext` always wins arbitration.

The following table shows this arbitration logic in detail:

| + | `CaseNext` | `CaseNoRepeat` | `CaseRepeatAll` | `CaseRepeatHandler` | `CaseNoTimeout` | `CaseAwait` | `CaseTimeout(aa)` | `CaseRepeatAllOnTimeout(aa)` | `CaseRepeatHandlerOnTimeout(aa)`
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| `CaseNext` | no repeat &<br> no timeout
| `CaseNoRepeat` | no repeat &<br> no timeout | no repeat
| `CaseRepeatAll` | no repeat &<br> no timeout | no repeat | repeat all
| `CaseRepeatHandler` | no repeat &<br> no timeout | no repeat | repeat all | repeat handler
| `CaseNoTimeout` | no repeat &<br> no timeout | no repeat &<br> no timeout | repeat all &<br> no timeout | repeat handler &<br> no timeout | no timeout
| `CaseAwait` | no repeat &<br> no timeout | no repeat &<br> infinite timeout | repeat all &<br> infinite timeout | repeat handler &<br> infinite timeout | no timeout | infinite timeout
| `CaseTimeout(bb)` | no repeat &<br> no timeout | no repeat &<br> `bb`ms timeout | repeat all &<br> `bb`ms timeout | repeat handler &<br> `bb`ms timeout | no timeout | `bb`ms timeout | `min(aa,bb)`ms timeout
| `CaseRepeatAllOnTimeout(bb)` | no repeat &<br> no timeout | no repeat &<br> `bb`ms timeout | repeat all on validate & repeat all on `bb`ms timeout | repeat all on validate & repeat all on `bb`ms timeout | repeat all & no timeout | repeat all on `bb`ms timeout |  repeat all on `min(aa,bb)`ms timeout |  repeat all on `min(aa,bb)`ms timeout |
| `CaseRepeatHandlerOnTimeout(bb)` | no repeat &<br> no timeout | no repeat &<br> `bb`ms timeout | repeat all on validate & repeat all on `bb`ms timeout | repeat handler on validate & repeat handler on `bb`ms timeout | repeat handler & no timeout | repeat handler on `bb`ms timeout |  repeat handler on `min(aa,bb)`ms timeout |  repeat all on `min(aa,bb)`ms timeout | repeat handler on `min(aa,bb)`ms timeout

### Atomicity

All handlers execute with interrupts enabled, **except the case failure handler!**.
This means you can write test cases that poll for interrupts to be completed inside any handler, except the failure handler.

If you setup an interrupt that validates its callback using `Harness::validate_callback()` inside a test case and it fires before the test case completed, the validation will be buffered.
If the test case then returns a timeout value, but the callback is already validated, the test harness just continues normally.

### Custom Scheduler

By default, a Timeout object is used for scheduling the harness operations.
In case this is not available you can provide your own custom scheduler implementation and make the harness use it with the `Harness::set_scheduler(your_custom_implementation)` function.

The scheduler requirements are very simple: Execute a `void(void)` function in you main loop (with a delay of *N* ms). Only one function is scheduled by the harness *at any given time*.
Note that you do not need to implement the delay functionality, if your tests do not require timeouts. You will still be able to use repeating test cases, but an error is thrown if your tests attempt to use a timeout, when your underlying scheduler does not support it.

There are two functions you need to implement:

- `void* post_callback(const utest_v1_harness_callback_t callback, const uint32_t delay_ms)`: schedules a `void(void)` callback function in *N* ms.
- `int32_t cancel_callback_t(void *handle)`: cancels an asynchronous callback.

Please see [their doxygen documentation for implementation details](utest/scheduler.h).

### Example Synchronous Scheduler

Here is the most [basic scheduler implementation without any asynchronous support](test/minimal_scheduler/main.cpp). Note that this does not require any hardware support at all, but you cannot use timeouts in your test cases!
```cpp
volatile utest_v1_harness_callback_t minimal_callback;

static void* utest_minimal_post(const utest_v1_harness_callback_t callback, const uint32_t delay_ms) {
    minimal_callback = callback;
    // this scheduler does not support asynchronous callbacks
    return (delay_ms ? NULL : (void*)1);
}
static int32_t utest_minimal_cancel(void*) {
    return -1;  // canceling not supported either
}
static const utest_v1_scheduler_t utest_minimal_scheduler = {utest_minimal_post, utest_minimal_cancel};

// [...] Add your test cases and specification here.

void main() // or whatever your custom entry point is
{
    // You MUST set the custom scheduler before running the specification.
    Harness::set_scheduler(utest_minimal_scheduler);
    Harness::run(specification);

    while(1) {
        if (minimal_callback) {
            // copy the callback and reset the shared memory
            utest_v1_harness_callback_t callback = minimal_callback;
            minimal_callback = NULL;
            callback(); // execute the copied callback
        }
    }
}
```

### Example Asynchronous Scheduler

Here is the a [complete scheduler implementation with any asynchronous support](test/minimal_scheduler_async/main.cpp). Note that this does require at least a hardware timer, in this case we have used `mbed-hal/us_ticker`! Note that you must not execute the callback in the timer interrupt context, but in the main loop context!
```cpp
volatile utest_v1_harness_callback_t minimal_callback;
volatile utest_v1_harness_callback_t ticker_callback;
const ticker_data_t *ticker_data;
ticker_event_t ticker_event;

static void ticker_handler(uint32_t) {
    minimal_callback = ticker_callback; // interrupt context!
}
static void* utest_minimal_post(const utest_v1_harness_callback_t callback, const uint32_t delay_ms) {
    if (delay_ms) {
        ticker_callback = callback;
        ticker_insert_event(ticker_data, &ticker_event, ticker_read(ticker_data) + delay_ms * 1000, 0);
    }
    else minimal_callback = callback;
    return (void*)1;
}
static int32_t utest_minimal_cancel(void*) {
    ticker_remove_event(ticker_data, &ticker_event);
    return 0;   // canceling is supported
}
static const utest_v1_scheduler_t utest_minimal_scheduler = {utest_minimal_post, utest_minimal_cancel};

// [...] Add your test cases and specification here.

void main() // or whatever your custom entry point is
{
    ticker_data = get_us_ticker_data(); // initialize the ticker data.
    ticker_set_handler(ticker_data, ticker_handler);
    // You MUST set the custom scheduler before running the specification.
    Harness::set_scheduler(utest_minimal_scheduler);
    Harness::run(specification);

    while(1) {
        if (minimal_callback) {
            // copy the callback and reset the shared memory
            utest_v1_harness_callback_t callback = minimal_callback;
            minimal_callback = NULL;
            callback(); // execute the copied callback
        }
    }
}
```