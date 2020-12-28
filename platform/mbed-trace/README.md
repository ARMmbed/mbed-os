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
* The trace function uses `stdout` as the default output target because it goes directly to serial port in mbed-os.
* The trace function produces traces like: `[<levl>][grp ]: msg`. This provides an easy way to detect trace prints and separate traces from normal prints (for example with _regex_).
* This approach requires a `sprintf` implementation (`stdio.h`). The memory consumption is pretty high, but it allows an efficient way to format traces.
* The solution is not Interrupt safe. ([PRs](https://github.com/ARMmbed/mbed-trace/pulls) are more than welcome.)
* The solution is not thread safe by default. Thread safety for the actual trace calls can be enabled by providing wait and release callback functions that use mutexes defined by the application.

## Examples of traces

```
[DBG ][abc ]: This is a debug message from module abc<cr><lf>
[INFO][br  ]: Hi there.<cr><lf>
[WARN][br  ]: Oh no, br warning occurs!<cr><lf>
[ERR ][abc ]: Something goes wrong in module abc<cr><lf>
```

## Usage

### Prerequisites

* Initialize the serial port so that `stdout` works. You can verify that the serial port works using the `printf()` function.
    * If you want to redirect the traces somewhere else, see the [trace API](https://github.com/ARMmbed/mbed-trace/blob/master/mbed-trace/mbed_trace.h#L245).
* To enable the tracing API:
    * With yotta: set `YOTTA_CFG_MBED_TRACE` to 1 or true. Setting the flag to 0 or false disables tracing.
    * [With mbed OS 5](#enabling-the-tracing-api-in-mbed-os-5)
* By default, trace uses 1024 bytes buffer for trace lines, but you can change it by setting the configuration macro `MBED_TRACE_LINE_LENGTH` to the desired value.
* To disable the IPv6 conversion:
    * With yotta: set `YOTTA_CFG_MBED_TRACE_FEA_IPV6 = 0`.
    * With mbed OS 5: set `MBED_CONF_MBED_TRACE_FEA_IPV6 = 0`.
* If thread safety is needed, configure the wait and release callback functions before initialization to enable the protection. Usually, this needs to be done only once in the application's lifetime.
    * If [helping functions](#helping-functions) are used the mutex must be **recursive** (counting) so it can be acquired from a single thread repeatedly.
* Call the trace initialization (`mbed_trace_init`) once before using any other APIs. It allocates the trace buffer and initializes the internal variables.
* Define `TRACE_GROUP` in your **source code (not in the header)** to use traces. It is a 1-4 characters long char-array (for example `#define TRACE_GROUP "APPL"`). This will be printed on every trace line.

### Enabling the tracing API in mbed OS 5

* Set `MBED_CONF_MBED_TRACE_ENABLE` to 1 or true

To do so, add the following to your mbed_app.json:

```json
{
    "target_overrides": {
        "*": {
            "mbed-trace.enable": 1
        }
    }
}
```

Do not forget to fulfill the other [prerequisites](#prerequisites)!

([Click here for more information on the configuration system](https://docs.mbed.com/docs/mbed-os-api/en/latest/config_system/))


## Examples

* [mbed-os-5](example/mbed-os-5)
* [linux](example/linux)

### Traces

When you want to print traces, use the `tr_<level>` macros. The macros behave like `printf()`. For example,
`tr_debug("hello %s", "trace")` produces the following trace line: `[DBG ][APPL] hello trace<cr><lf>`.

Available levels:

* debug
* info
* warning
* error
* cmdline (special behavior, should not be used)

For the thread safety, set the mutex wait and release functions. You need do this before the initialization to have the functions available right away:

```c
mbed_trace_mutex_wait_function_set(my_mutex_wait);
mbed_trace_mutex_release_function_set(my_mutex_release);
```

Initialization (once in application's lifetime):

```c
int mbed_trace_init(void);
```

Set the output function, `printf` by default:

```c
mbed_trace_print_function_set(printf)
```

### Tracing level

Run time tracing level is set using `mbed_trace_set_config()` function. Possible levels and examples how to set them is presented below.

```c
//mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
//mbed_trace_config_set(TRACE_ACTIVE_LEVEL_DEBUG); // (same as ALL)
mbed_trace_config_set(TRACE_ACTIVE_LEVEL_INFO);
//mbed_trace_config_set(TRACE_ACTIVE_LEVEL_WARN);
//mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ERROR);
//mbed_trace_config_set(TRACE_ACTIVE_LEVEL_CMD);
//mbed_trace_config_set(TRACE_ACTIVE_LEVEL_NONE);
```

Build time optimization can be done with `MBED_TRACE_MAX_LEVEL` definition. Setting max level to `TRACE_LEVEL_DEBUG` includes all traces to the build. Setting max level to `TRACE_LEVEL_INFO` includes all but `tr_debug()` traces to the build. Other maximum tracing levels follow the same behavior and no messages above the selected level are included in the build.

```c
#define MBED_TRACE_MAX_LEVEL TRACE_LEVEL_DEBUG
#define MBED_TRACE_MAX_LEVEL TRACE_LEVEL_INFO
#define MBED_TRACE_MAX_LEVEL TRACE_LEVEL_WARN
#define MBED_TRACE_MAX_LEVEL TRACE_LEVEL_ERROR
#define MBED_TRACE_MAX_LEVEL TRACE_LEVEL_CMD
```

In Mbed OS, the build time maximum tracing level can be set through `mbed_app.json` as shown below.

```
{
   "target_overrides":{
      "*":{
         "mbed-trace.enable": true,
         "mbed-trace.max-level": "TRACE_LEVEL_INFO"
      }
   }
}
```

When max-level is not set by the application, default level is INFO.

### Helping functions

The purpose of the helping functions is to provide simple conversions,
for example from an array to C string, so that you can print everything to single trace line.
They must be called inside the actual trace calls, for example:

```
tr_debug("My IP6 address: %s", mbed_trace_ipv6(addr));
```

Available conversion functions:

```
char *mbed_trace_ipv6(const void *addr_ptr)
char *mbed_trace_ipv6_prefix(const uint8_t *prefix, uint8_t prefix_len)
char *mbed_trace_array(const uint8_t *buf, uint16_t len)
```

See more in [mbed_trace.h](https://github.com/ARMmbed/mbed-trace/blob/master/mbed-trace/mbed_trace.h).


## Usage example:

```c++
#define MBED_CONF_MBED_TRACE_ENABLE 1 //this could be defined also in the mbed-cli configuration file mbed_app.json
#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP  "main"

 // These are necessary only if thread safety is needed
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
    tr_info("this is info msg");    //-> "[INFO][main]: this is an info msg"
    tr_warn("this is warning msg"); //-> "[WARN][main]: this is a warning msg"
    tr_err("this is error msg");    //-> "[ERR ][main]: this is an error msg"
    char arr[] = {30, 31, 32};
    tr_debug("printing array: %s", mbed_trace_array(arr, 3)); //-> "[DBG ][main]: printing array: 01:02:03"
    return 0;
}
```

## Run-time trace group filtering

The trace groups you have defined using the `TRACE_GROUP` macro in your .c/.cpp files can be used to control tracing at run-time.

| Function                          | Explanation                                                  |
|-----------------------------------|--------------------------------------------------------------|
|`mbed_trace_include_filters_get()` | Get the exclusion filter list string.                        |
|`mbed_trace_include_filters_set()` | Set trace list to include only the traces matching the list. |
|`mbed_trace_exclude_filters_get()` | Get the inclusion filter list string.                        |
|`mbed_trace_exclude_filters_set()` | Set trace list to exclude the traces matching the list.      |

The filter list is a null terminated string of comma (`,`) separated trace group names. The default maximum length of the string is 24 characters, including the terminating null. Length can be changed by defining the macro `DEFAULT_TRACE_FILTER_LENGTH`. Exclude and include filters can be combined freely as they both have their own filtering list.

The matching is done simply using `strstr()` from  C standard libraries.

### Examples of trace group filtering

Assuming we have 4 modules called "MAIN", "HELP", "CALC" and "PRNT" we could use the filters in the following ways.

#### Inclusion filter

To include only "MAIN" and "CALC" traces to the trace prints, we can do:

```
    mbed_trace_include_filters_set("MAIN,CALC");
```

This would print out only the traces from "MAIN" and "CALC", since they are the trace groups matching the filter list. Trace groups "HELP" and "PRNT" would not be printed out at all.

## Exclusion filter

```
    mbed_trace_exclude_filters_set("HELP,PRNT");
```

This would exclue trace groups "HELP" and "PRNT" out of trace printing, thus leaving only prints from "MAIN" and "CALC" visible in the tracing.

### Reset filter

```
    mbed_trace_include_filters_set(NULL);
```

This would reset the inclusion filters back to nothing and assuming no exclusion filter is in place either, all trace groups prints would get printed.


## Unit tests

To run unit tests:

* In Linux

```
yotta target x86-linux-native
yotta test mbed_trace_test
```

* In Mac

```
yotta target x86-osx-native
yotta test mbed_trace_test
```

* In Windows

```
yotta target x86-windows-native
yotta test mbed_trace_test
```
