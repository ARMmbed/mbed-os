# Mbed OS clocks

# Table of contents

1. [Mbed OS clocks](#mbed-os-clocks).
1. [Table of contents](#table-of-contents).
    1. [Revision history](#revision-history).
1. [Introduction](#introduction).
    1. [Overview and background](#overview-and-background).
    1. [Requirements and assumptions](#requirements-and-assumptions).
1. [System architecture and high-level design](#system-architecture-and-high-level-design).
    1. [Which clock to use?](#which-clock-to-use).
1. [Detailed design](#detailed-design).
    1. [Accessing Chrono APIs](#accessing-chrono-apis)
    1. [The RTOS clock](#the-rtos-clock)
    1. [High-resolution and low-power clocks](#high-resolution-and-low-power-clocks)
    1. [The real-time clock](#the-real-time-clock)
    1. [Comparison to standard C++ Chrono clocks](#comparison-to-standard-c++-chrono-clocks)
1. [Usage scenarios and examples](#usage-scenarios-and-examples).
    1. [Using the RTOS clock](#using-the-rtos-clock)
    1. [Using the high-resolution clock](#using-the-high-resolution-clock)
    1. [Using the real-time clock](#using-the-real-time-clock)
    1. [Being more generic](#being-more-generic)
1. [Other information](#other-information).
    1. [Deprecations](#deprecations).
    1. [References](#references).

### Revision history

1.0 - Initial revision (Mbed OS 6.0) - Kevin Bracey - 03 Mar 2020

# Introduction

### Overview and background

Mbed OS 6.0 provides a set of timing APIs that use C++'s `<chrono>` library. This provides strong typing for measuring time durations (`std::chrono::duration`) and time points (`std::chrono::time_point`). Previous APIs based on integer types are deprecated.

### Requirements and assumptions

The new API relies on C++14 to provide the library functions, and allow duration constants like `200ms`. Mbed OS 6.0 generally requires a C++14 or later toolchain, so the Chrono API is now possible. Some core APIs are also provided in C form.

# System architecture and high-level design

Mbed OS has four primary central clocks:

- The RTOS clock. This is managed by the RTOS if present, else based on a HAL clock, and has millisecond resolution.
- The high-resolution clock (`us_ticker`). This is provided by the target's HAL, and has approximately microsecond resolution. It does not run in deep sleep, so its users will usually lock deep sleep.
- The low-power clock (`lp_ticker`). This is provided by most targets' HAL, and has sub-millisecond resolution. It continues to run in deep sleep.
- The real-time clock. This is provided by some targets' HAL, often runs when the main CPU does not, and has one-second resolution.

The way these APIs map to hardware varies by target. In some cases, they may use four distinct peripherals, but often they share hardware.

Capabilities and uses of the clocks vary, but they all share the need to describe time points and durations, and as such use the `time_point` and `duration` template types from `<chrono>` to provide strongly-typed APIs.

### Which clock to use?

In many higher-level APIs, you do not directly specify a clock - for example `EventQueue` just takes relative times, with no direct indication of the clock being used. But when implementing such code, you will need to make a choice.

1. As an application, you should by default use the RTOS clock. This is the most general-purpose clock, with the best scheduling capability, and the API is available even in a bare-metal build.

1. If higher-resolution timing is required, then you can use the high-resolution clock. But this does mean locking deep sleep.

1. If real calendar/wall-clock time is required, then you can use the real-time clock. You may want to combine its use with another clock to get better-than-second resolution or event scheduling.

1. The low-power clock should not normally be used to schedule events. It is only really recommended as a slightly-higher-resolution time-point clock than the RTOS millisecond clock (4kHz to 64kHz). But it cannot be easily used for event scheduling as care must be taken to compensate for deep sleep wake latency, which is usually multiple milliseconds. Such latency compensation is included in the RTOS timing system.

`EventQueue` is a case study in API choice - previously it used the RTOS clock in RTOS builds and the low-power clock in bare-metal builds (because the RTOS clock was not available). It now uses the RTOS clock in all builds, meaning it now gets deep sleep wake latency compensation in bare metal builds too.

# Detailed design

### The RTOS clock

**API description**

The RTOS tracks time at millisecond resolution, and this clock is exposed in C++ form as `Kernel::Clock`.

All other RTOS APIs that use time, such as `ThisThread::sleep_for`, `Semaphore::try_acquire_for` and `EventFlags::wait_any_until` use this clock.

Unlike direct use of the low-power clock, RTOS wake events compensate for deep sleep latency. So requesting `ThisThread::sleep_for(10ms)` will wake after 10ms, even if it causes deep sleep and there is a 4ms wake delay from deep sleep. 

The RTOS clock is the best general-purpose clock for most use cases. Subsystems such as `EventQueue` use the RTOS timing APIs internally.

The RTOS clock API and a subset of other RTOS APIs are available in bare-metal builds even when an actual RTOS is not present. 

**RTOS clock generation**

There are three current ways that the system can produce the RTOS clock:

* The RTOS can use a dedicated millisecond ticker such as the SysTick peripheral. This ticker always runs, and deep sleep does not occur while the RTOS is active, unless manually requested by the application. 
* The RTOS can be in "tickless" mode, where ticks and wake-ups from idle periods are generated using the low-power clock. Deep sleep is entered whenever idle, if it is unlocked and there is sufficient time before the next scheduled wake.
* The system is built as "bare-metal", so there is no actual RTOS. A subset of RTOS APIs are implemented using the low-power clock.

When there is an actual RTOS, the timing system is always present in the build, so it's a sunk cost. In a bare-metal build, the timer is not necessarily needed - it will be included in the build and started if used. The separation of APIs into distinct untimed and timed forms like `Mutex::try_lock` and `Mutex::try_lock_for` are intended to help exclude timer code from bare-metal builds.

### High-resolution and low-power clocks

The high-resolution and low-power clocks share a common API and implementation, so are described together.

Both present themselves via a microsecond-based API. The high-resolution clock has at least 250kHz (4us) resolution, and the low-power clock has 4kHz-64kHz (16us-250us) resolution, depending on target.

Aside from the resolution, key differences are:

* The high-resolution clock does not operate in deep sleep, so deep sleep must be locked to keep it running, affecting power consumption. It should normally only be used for measuring short times while the device is active. Unless the high resolution is needed, the RTOS timing APIs are preferable.
* The low-resolution clock permits deep sleep, so clock-timed wake-ups can suffer from deep sleep wake-up latency delays. It does not compensate for these, so a `LowPowerTimeout` requested for 10ms could easily wake up after 14ms. The RTOS timing APIs are usually preferable.

**API description**

At the lowest level, these clocks are obtained by calling `get_us_ticker data`or `get_lp_ticker_data`, and
using the returned `ticker_data_t *` with the C functions in `ticker_api.h`.

The underlying `ticker_data` can describe hardware clocks that can have varying bit-width and frequency. Each clock provides one single-shot interrupt.

The C ticker API functions convert the underlying clock to an absolute 64-bit microsecond timebase, and add a queue for registering multiple single-shot events. Interrupts are automatically scheduled before the underlying clock wraps to update the long-term 64-bit timebase.

The classes `HighResClock`,  and `LowPowerClock` act as Chrono clock interfaces for the two primary clocks.

* `HighResClock`: reads `us_ticker` and provides manual locks for deep sleep; it is steady while locked.
* `LowPowerClock`: reads `lp_ticker`; the clock is inherently steady, but provides dummy lock methods to keep the same API as `HighResClock`.

If using the high-resolution clock, to save power it should only be run when necessary, which means using `HighResClock`'s `lock` and `unlock` methods to indicate when it is required.

Higher-level clock functions are provided via the following classes:

* `Timer` / `LowPowerTimer`: acts like a stopwatch for elapsed time - can be stopped, started and reset
* `Ticker` / `LowPowerTicker`: calls a callback from interrupt context periodically
* `Timeout` / `LowPowerTimeout`: calls a callback from interrupt context once

The classes without the `LowPower` prefix are the high-resolution ones; they should arguably be named `HighResTimer`, and so on.

Unlike direct clock access, those high-level classes always handle deep sleep locking automatically for the high-resolution clock. Deep sleep is locked whenever a high-resolution `Timer` is running, or when a `Ticker` or `Timeout` callback is pending, and it is unlocked when stopped, removed or destroyed.

**Other ticker_data_t clocks**

There is also a generic `TickerDataClock` which provides a Chrono Clock wrapper for a `ticker_data *`, so that `TickerDataClock::time_point` can be used to express absolute times for arbitrary clocks.

Note that the `TickerDataClock` is a non-standard Chrono clock, in that its `now` method is non-static. The Chrono type-checking will not detect that `TickerDataClock::time_point`s for different tickers are not interchangeable. It is generally preferable to use the dedicated `HighResClock` or `LowPowerClock`, but `TickerDataClock` is used by some generic code, to avoid template bloat from separate code for each Clock. 

The base classes `TimerBase`, `TickerBase` and `TimeoutBase` use `TickerDataClock` to provide the core of the high-level implementation, and these are then used to provide the separate strongly-typed high-resolution and low-power classes above.

Systems wishing to provide more HAL clocks beyond the basic two can use the same framework to give that clock its own custom `Clock/Timer/Ticker/Timeout` set of classes, using the base classes.

### The real-time clock

The real-time clock, if available, provides calendar date and time information, at one-second resolution. The RTC can continue to run while the CPU does not.

The C APIs in `rtc_api.h` read and write the time as a C `time_t`.

The class `RealTimeClock` provides a wrapper that acts as a Chrono clock, so that `RealTimeClock::time_point`can be used to express absolute real time.

The methods `to_time_t` and `from_time_t` convert to and from C `time_t`.

At present there are no other APIs using `RealTimeClock`, and no real-time-based wake or alarm facility, so utility is limited, but the class has been created for framework symmetry with the other clocks.

### Comparison to standard C++ Chrono clocks

Mbed OS does not provide the standard C++11 Chrono Clocks, as it is not possible to provide consistent behaviour across the various toolchains and libraries supported. Here's a suggested mapping from the 3 standard C++ Clocks to Mbed OS.

**std::chrono::steady_clock**

`std::chrono::steady_clock` represents a clock for which values of `time_point` never decrease as physical time advances and for which values of `time_point` advance at a steady rate relative to real time.

`Kernel::Clock` can be used in place of `std::chrono::steady_clock` for general use.

For slightly higher timestamp resolution you can use `LowPowerClock`, but if attempting to schedule time events, using RTOS APIs with `Kernel::Clock` will often work better due to the RTOS's deep sleep wake latency compensation.

If high resolution is required, you can use `HighResClock`, as long as the lock is held while in use to keep it steady.

**std::chrono::high_resolution_clock**

`std::chrono::high_resolution_clock` represents the clock with the shortest tick period.

`HighResClock` is the best choice for a high-resolution clock, but it will not run during deep sleep. You can use `HighResClock::lock` to keep it running, or otherwise ensure that deep sleep is locked.

**std::chrono::system_clock**

`std::chrono::system_clock` represents wall clock time from the system-wide realtime clock.

`RealTimeClock` can be used in place of `std::chrono::system_clock`, and provides the same `to_time_t` and `from_time_t` methods. However, unlike many implementations, its resolution is only seconds.

# Usage scenarios and examples

### Accessing Chrono APIs

Using the Chronos API needs a little thought about namespaces. This section covers that; the following sections will assume the complete Chrono API has been imported.

By default C++14 duration literals will be available after including `mbed.h` due to it including files that use `<chrono>` and having a `using namespace std` directive:

```C++
#include "mbed.h"
auto delay = 5ms;
```

If `MBED_NO_GLOBAL_USING_DIRECTIVE` is defined, or you are not including `mbed.h`, you will need your own directive, and possibly own include, to use the literals:

```C++
#include <chrono> 
using namespace std::chrono_literals;
auto delay = 5ms;
```

All Chrono APIs are in the namespace `std::chrono`, so it can be convenient to have a directive to import this complete namespace to reduce typing:

```C++
#include <chrono>
using namespace std::chrono; // provides both types like std::chrono::microseconds and the chrono_literals

microseconds t = 1h;
printf("1 hour is %d us\n", int(t.count()));
```

Namespace directives should only be used in source files to avoid unexpected name collision for users. Header files intended for general use will need to fully qualify their use of Chrono with `std::chrono` prefixes. This also precludes use of literals:

```C++
// In a header file
#include <chrono>
void my_function(std::chrono::microseconds timeout = std::chrono::seconds{1});
```

### Using the RTOS clock

**Basic delays**

```C++
for (;;) {
    LED1 = !LED1;
    ThisThread::sleep_for(500ms);
}
```

**Prefer absolute time**

Using absolute time aids precision, guaranteeing that there is no drift due to code execution time. This applies to all clocks.

```C++
auto next_wake = Kernel::Clock::now();
for (;;) {
    LED1 = !LED1;
    next_wake += 500ms;
    ThisThread::sleep_until(next_wake);
}
```

Periodic callback APIs such as `Ticker` or `EventQueue::call_every` use this mechanism. If manually rescheduling events yourself, you should use the same technique, rather than using `call_in` within a callback.

**Measuring elapsed time via the RTOS**

```C++
auto start = Kernel::Clock::now(); // type will be Kernel::Clock::time_point
do_operation();
milliseconds elapsed_time = Kernel::Clock::now() - start;
printf("elapsed time = %d ms\n", int(elapsed_time.count()));
```

Explicitly stating that `elapsed_time` is `milliseconds` provides a cross-check that the `count()` for the print really is milliseconds. If the clock had lower resolution, it would be implicitly converted to milliseconds. If the clock had higher resolution, it would be a compilation error - see "being more generic" below to handle this.

### Using the high-resolution clock

**Measuring elapsed time using Timer**
```C++
template<class F>
microseconds time_operation(const F &operation)
{
    Timer timer;
    timer.start(); // deep sleep is automatically locked while timer is running
    operation();
    return timer.elapsed_time();
} // Deep sleep automatically unlocked when timer is destroyed.

microseconds print_time = time_operation([] { printf("Hello!\n"); });
printf("Printing that took %d us\n", int(print_time.count()));
```

This example relies on the destruction of the timer to unlock the clock. If using a timer that doesn't get destroyed, you must remember to stop it manually to allow deep sleep to be entered again.

**Recording high-resolution time points**
```C++
HighResClock::lock(); // take lock to ensure clock is steady (by preventing deep sleep)
auto t1 = HighResClock::now();
do_operation1();
auto t2 = HighResClock::now();
do_operation2();
auto t3 = HighResClock::now();
HighResClock::unlock();
microseconds total = t3 - t1;
microseconds stage1 = t2 - t1;
microseconds stage2 = t3 - t2; 
printf("total time   = %d us\n", int(total.count()));
printf("stage 1 time = %d us\n", int(stage1.count()));
printf("stage 2 time = %d us\n", int(stage2.count()));
```

**Blinking an LED at 1kHz**
```C++
Ticker ticker;
ticker.attach([] { LED1 = !LED1; }, 500us);
``` 

### Using the real-time clock

**Logging times**
```C++
auto woken = RealTimeClock::now();
time_t t = RealTimeClock::to_time_t(woken);
printf("Woken at %s", ctime(&t));
do_work();
t = RealTimeClock::to_time_t(woken + 3h);
printf("Sleeping until %s", ctime(&t));
ThisThread::sleep_for(3h);
```

Note that RTOS APIs currently only accept RTOS time points, which is why the above example uses `sleep_for`; you would get a compile error for `ThisThread::sleep_until(woken + 3h)`. If the work took a long time, then the schedule would drift away from a regular 3 hour period. See next example for an absolute-time-based approach.

**Making helpers to combine clocks**

The above example could be adapted to use absolute wake times with a helper function, guaranteeing a regular 3-hour period, regardless of how long the work takes:

```C++
void sleep_until_rtc(RealTimeClock::time_point abs_time)
{
    auto rel_time = abs_time - RealTimeClock::now();
    ThisThread::sleep_until(Kernel::Clock::now() + rel_time);
}

auto woken = RealTimeClock::now();
time_t t = RealTimeClock::to_time_t(woken);
printf("Woken at %s", ctime(&t));
do_work();
auto wake = woken + 3h;
t = RealTimeClock::to_time_t(wake);
printf("Sleeping until %s", ctime(&t));
sleep_until_rtc(wake);
```

Note that `sleep_until_rtc` would not respond to any adjustments made to `RealTimeClock` made while sleeping.

### Being more generic

**Allowing choice of clock**

As the high-resolution and low-power clocks provide the same API, code can be written generically to work with either clock. For example:
```C++
#ifdef USE_HIGHRES_CLOCK // or Clock could be a template parameter
using Clock = HighResClock;
#else
using Clock = LowPowerClock;
#endif
Clock::lock(); // will lock deep sleep if high-res clock, else no-op
auto t1 = Clock::now();
do_operation1();
auto t2 = Clock::now();
do_operation2();
auto t3 = Clock::now();
Clock::unlock();
microseconds total = t3 - t1;
microseconds stage1 = t2 - t1;
microseconds stage2 = t3 - t2; 
printf("total time   = %d us\n", int(total.count()));
printf("stage 1 time = %d us\n", int(stage1.count()));
printf("stage 2 time = %d us\n", int(stage2.count()));
```

This same code would actually also work with `Kernel::Clock` and `RealTimeClock`, due to the care taken with the types, including explicit use of `microseconds`. For more details, see next section.

**Coping with different resolutions**

At present, the clocks have specified periods: seconds, milliseconds or microseconds. Chrono prevents any errors being made by mixing different-resolution clocks at compile time, except when entering or leaving the Chrono domain by constructing a `duration` or using `duration::count()`.

In principle, the periods of these clocks could be changed in future, for example to make `HighResClock` match the native hardware rate. Generic code itself may want to select between RTOS and high-resolution clocks which have different periods. To support both cases you may want to write code that always works and compiles regardless of resolution.

The basic rule is that Chrono conversions are implicit if they don't lose precision. So `milliseconds d = t1 - t2;` will work when `t1` and `t2` have millisecond precision or lower. If they were `RealTimeClock::time_point`s measured in seconds, then there would be an automatic multiplication by 1000 to convert rates. To permit precision-losing conversions, such as dividing by 1000, you need use `duration_cast` or `time_point_cast`.

Guidelines:

* use `auto` for your variables to match the clock you're using; don't convert to a specific representation unless necessary
* use `duration_cast` to round to a particular precision
* use templates to accept any resolution and round (see references for examples)

Here is the high-res timer example from above adjusted to still compile if the high-res timer was increased to higher than microsecond resolution:

```C++
Timer timer;
timer.start();
do_operation();
auto elapsed_time = duration_cast<microseconds>(timer.elapsed_time()); // rounds down
printf("elapsed time = %d us\n", int(elapsed_time.count()));
```

If the timer actually is microseconds, then this cast does not actually need to generate any code. 

# Other information

### Deprecations

Many older C++ APIs using integer or float time repres are deprecated in favour of the Chrono APIs.

These include:

* `Kernel::get_ms_count`
* `Semaphore::try_acquire_for(uint32_t)` and similar RTOS functions
* `Semaphore::try_acquire_until(uint64_t)` and similar RTOS functions
* `Timer::read`
* `Timer::read_ms`
* `Timer::read_us`
* `Timer::read_high_resolution_us`
* `Timer::operator float`
* `Ticker::attach(F, float)`
* `Ticker::attach_us(Callback, us_timestamp_t )`
* `EventQueue::call_in(int, ...)`
* `EventQueue::call_every(int, ...)`

### References

General Chrono background:

* [N2661 - original C++ proposal](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2661.htm)
* [Video: CppCon 2016: Howard Hinnant “A \<chrono\> Tutorial"](https://www.youtube.com/watch?v=P32hvk8b13M)
* [Introduction to std::chrono](http://rachelnertia.github.io/programming/2018/01/07/intro-to-std-chrono/)

