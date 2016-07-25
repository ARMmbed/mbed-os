# mbed-trace

A general purpose tracing abstraction library for mbed devices.

## Description

The purpose of the library is to provide a light, simple and general tracing solution for mbed devices. By default, it prints traces to `stdout` (usually, a serial port), but the output can also be redirected to other targets. The library was developed using ANSI C language, but it can be used with C++ as well. Currently, there is no C++ wrapper available, but it can be created easily on top of this library.

## Philosophy

* The library needs to be light, fast, simple and abstract.
* Dependencies must be minimal.
* The memory space required by the library is allocated at the initialization only once during the application lifetime.
* No new malloc/free are needed when running the library.
* The trace methods must be as fast as possible.
* After a trace method call, the trace function needs to release the required resources.
* A trace method call produces a single line containing `<level>`, `<group>` and `<message>`
* It must be possible to filter messages on the fly. Compile time filtering is not fully supported yet.

## Compromises

* The traces are stored as ASCII arrays in the flash memory (pretty high memory consumption). Therefore, it is not necessary to:
 * encode/decode the trace messages on the fly (this may take too much CPU time) or 
 * have external dev-env dependencies to encode the traces compile time and an external application to decode the traces. 
* The group name length is limited to four characters. This makes the lines cleaner and it is enough for most use cases for separating the module names. The group name length may not be suitable for a clean human readable format, but still four characters is enough for unique module names.
* The trace function uses `stdout` as the default output target because it goes directly to serial port when initialized. 
* The trace function produces traces like: `[<levl>][grp ]: msg`. This provides an easy way to detect trace prints and separate traces from normal prints (for example with _regex_).
* This approach requires a `sprintf` implementation (`stdio.h`). The memory consumption is pretty high, but it allows an efficient way to format traces.
* The solution is not Interrupt safe. (PRs are more than welcome.)
* The solution is not Thread safe by default. Thread safety for the actual trace calls can be enabled by providing wait and release callback functions that use mutexes defined by the application. 

## Examples of traces

```
[DBG ][abc ]: This is a debug message from module abc<cr><lf>
[ERR ][abc ]: Something goes wrong in module abc<cr><lf>
[WARN][br  ]: Oh no, br warning occurs!<cr><lf>
[INFO][br  ]: Hi there.<cr><lf>
```

## Usage

### Prerequisites

* Initialize the serial port so that `stdout` works. You can verify that the serial port works using the `printf()` function.
 * if you want to redirect the traces somewhere else, see the [trace API](https://github.com/ARMmbed/mbed-trace/blob/master/mbed-trace/mbed_trace.h#L170).
* to activate traces, configure yotta with flag: `YOTTA_CFG_MBED_TRACE` set to 1 or true. Setting the flag to 0 or false disables tracing.
 * By default trace uses 1024 bytes buffer for trace lines, but it can be configure by yotta with: `YOTTA_CFG_MBED_TRACE_LINE_LENGTH`. Default length: 1024.
 * To disable ipv6 convertion, set `YOTTA_CFG_MBED_TRACE_FEA_IPV6 = 0` from yotta configurations.
* If thread safety is needed, configure the wait and release callback functions before initialization so that the protection is in place as soon as the library is initialized. This should usually only be done once in the application's lifetime.
* Call the trace initialization (`mbed_trace_init`) once before using any other APIs. It allocates the trace buffer and initializes the internal variables.
* Define `TRACE_GROUP` in your source code (not in the header!) to use traces. `TRACE_GROUP` is a 1-4 character long char-array (for example `#define TRACE_GROUP "APPL"`). This will be printed on every trace line.

### Traces

When you want to print traces, use the `tr_<level>` macros. The macros behave like `printf()`. For example, `tr_debug("hello %s", "trace")` produces the following trace line: `[DBG ][APPL] hello trace<cr><lf>`.

Available levels:

* debug
* warning
* error
* info
* cmdline (special behavior, should not be used)

Set mutex wait and release functions, if thread safety is needed. Do this before initialization so the functions are immediately available:

```c
mbed_trace_mutex_wait_function_set(my_mutex_wait);
mbed_trace_mutex_release_function_set(my_mutex_release);
```

Initialization (once in application lifetime):

```c
int mbed_trace_init(void);
```

Set output function, `printf` by default:

```c
mbed_trace_print_function_set(printf)
```

### Helping functions

The purpose of the helping functions is to provide simple conversions, for example from an array to C string, so that you can print everything to single trace line.
These must be called inside actual trace calls, e.g. ```tr_debug("My IP6 address: %s", mbed_trace_ipv6(addr));```.

Available conversion functions:
```
char *mbed_trace_ipv6(const void *addr_ptr)
char *mbed_trace_ipv6_prefix(const uint8_t *prefix, uint8_t prefix_len)
char *mbed_trace_array(const uint8_t *buf, uint16_t len)
```

See more in [mbed_trace.h](https://github.com/ARMmbed/mbed-trace/blob/master/mbed-trace/mbed_trace.h).


## Usage example:

```c++
#define YOTTA_CFG_MBED_TRACE 1 //this can be defined also in the yotta configuration file config.json
#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP  "main"

 // These are only necessary if thread safety is needed
static Mutex MyMutex;
static void my_mutex_wait()
{
    MyMutex.lock();
}
static void my_mutex_release()
{
    MyMutex.unlock();
}

int main(void){
    mbed_trace_mutex_wait_function_set( my_mutex_wait ); // only if thread safety is needed
    mbed_trace_mutex_release_function_set( my_mutex_release ); // only if thread safety is needed
    mbed_trace_init();       // initialize the trace library
    tr_debug("this is debug msg");  //-> "[DBG ][main]: this is a debug msg"
    tr_err("this is error msg");    //-> "[ERR ][main]: this is an error msg"
    tr_warn("this is warning msg"); //-> "[WARN][main]: this is a warning msg"
    tr_info("this is info msg");    //-> "[INFO][main]: this is an info msg"
    char arr[] = {30, 31, 32};
    tr_debug("printing array: %s", mbed_trace_array(arr, 3)); //-> "[DBG ][main]: printing array: 01:02:03"
    return 0;
}
```

## Unit tests

To run unit tests

* In Linux:
```
yotta target x86-linux-native
yotta test mbed_trace_test
```

* In Windows:
```
yotta target x86-windows-native
yotta test mbed_trace_test
```
