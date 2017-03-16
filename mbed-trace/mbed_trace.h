/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file mbed_trace.h
 * Trace interface for MbedOS applications.
 * This file provide simple but flexible way to handle software traces.
 * Trace library are abstract layer, which use stdout (printf) by default,
 * but outputs can be easily redirect to custom function, for example to
 * store traces to memory or other interfaces.
 *
 *  usage example:
 * \code(main.c:)
 *      #include "mbed_trace.h"
 *      #define TRACE_GROUP  "main"
 *
 *      int main(void){
 *          mbed_trace_init();   // initialize trace library
 *          tr_debug("this is debug msg");  //print debug message to stdout: "[DBG]
 *          tr_info("this is info msg");
 *          tr_warn("this is warning msg");
 *          tr_err("this is error msg");
 *          return 0;
 *      }
 * \endcode
 * Activate with compiler flag: YOTTA_CFG_MBED_TRACE
 * Configure trace line buffer size with compiler flag: YOTTA_CFG_MBED_TRACE_LINE_LENGTH. Default length: 1024.
 * Limit the size of flash by setting MBED_TRACE_MAX_LEVEL value. Default is TRACE_LEVEL_DEBUG (all included)
 *
 */
#ifndef MBED_TRACE_H_
#define MBED_TRACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef YOTTA_CFG
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#else
#include "ns_types.h"
#endif

#include <stdarg.h>

#ifndef YOTTA_CFG_MBED_TRACE
#define YOTTA_CFG_MBED_TRACE 0
#endif

#ifndef YOTTA_CFG_MBED_TRACE_FEA_IPV6
#define YOTTA_CFG_MBED_TRACE_FEA_IPV6 1
#else
#warning YOTTA_CFG_MBED_TRACE_FEA_IPV6 is deprecated and will be removed in the future! Use MBED_CONF_MBED_TRACE_FEA_IPV6 instead.
#define MBED_CONF_MBED_TRACE_FEA_IPV6 YOTTA_CFG_MBED_TRACE_FEA_IPV6
#endif

#ifndef MBED_CONF_MBED_TRACE_ENABLE
#define MBED_CONF_MBED_TRACE_ENABLE 0
#endif

#ifndef MBED_CONF_MBED_TRACE_FEA_IPV6
#define MBED_CONF_MBED_TRACE_FEA_IPV6 1
#endif

/** 3 upper bits are trace modes related,
    and 5 lower bits are trace level configuration */

/** Config mask */
#define TRACE_MASK_CONFIG         0xE0
/** Trace level mask */
#define TRACE_MASK_LEVEL          0x1F

/** plain trace data instead of "headers" */
#define TRACE_MODE_PLAIN          0x80
/** color mode */
#define TRACE_MODE_COLOR          0x40
/** Use print CR before trace line */
#define TRACE_CARRIAGE_RETURN     0x20

/** used to activate all trace levels */
#define TRACE_ACTIVE_LEVEL_ALL    0x1F
/** print all traces same as above */
#define TRACE_ACTIVE_LEVEL_DEBUG  0x1f
/** print info,warn and error traces */
#define TRACE_ACTIVE_LEVEL_INFO   0x0f
/** print warn and error traces */
#define TRACE_ACTIVE_LEVEL_WARN   0x07
/** print only error trace */
#define TRACE_ACTIVE_LEVEL_ERROR  0x03
/** print only cmd line data */
#define TRACE_ACTIVE_LEVEL_CMD    0x01
/** trace nothing  */
#define TRACE_ACTIVE_LEVEL_NONE   0x00

/** this print is some deep information for debug purpose */
#define TRACE_LEVEL_DEBUG         0x10
/** Info print, for general purpose prints */
#define TRACE_LEVEL_INFO          0x08
/** warning prints, which shouldn't causes any huge problems */
#define TRACE_LEVEL_WARN          0x04
/** Error prints, which causes probably problems, e.g. out of mem. */
#define TRACE_LEVEL_ERROR         0x02
/** special level for cmdline. Behaviours like "plain mode" */
#define TRACE_LEVEL_CMD           0x01

#ifndef MBED_TRACE_MAX_LEVEL
#define MBED_TRACE_MAX_LEVEL TRACE_LEVEL_DEBUG
#endif

//usage macros:
#if MBED_TRACE_MAX_LEVEL >= TRACE_LEVEL_DEBUG
#define tr_debug(...)           mbed_tracef(TRACE_LEVEL_DEBUG,   TRACE_GROUP, __VA_ARGS__)   //!< Print debug message
#else
#define tr_debug(...)
#endif

#if MBED_TRACE_MAX_LEVEL >= TRACE_LEVEL_INFO
#define tr_info(...)            mbed_tracef(TRACE_LEVEL_INFO,    TRACE_GROUP, __VA_ARGS__)   //!< Print info message
#else
#define tr_info(...)
#endif

#if MBED_TRACE_MAX_LEVEL >= TRACE_LEVEL_WARN
#define tr_warning(...)         mbed_tracef(TRACE_LEVEL_WARN,    TRACE_GROUP, __VA_ARGS__)   //!< Print warning message
#define tr_warn(...)            mbed_tracef(TRACE_LEVEL_WARN,    TRACE_GROUP, __VA_ARGS__)   //!< Alternative warning message
#else
#define tr_warning(...)
#define tr_warn(...)
#endif

#if MBED_TRACE_MAX_LEVEL >= TRACE_LEVEL_ERROR
#define tr_error(...)           mbed_tracef(TRACE_LEVEL_ERROR,   TRACE_GROUP, __VA_ARGS__)   //!< Print Error Message
#define tr_err(...)             mbed_tracef(TRACE_LEVEL_ERROR,   TRACE_GROUP, __VA_ARGS__)   //!< Alternative error message
#else
#define tr_error(...)
#define tr_err(...)
#endif

#define tr_cmdline(...)         mbed_tracef(TRACE_LEVEL_CMD,     TRACE_GROUP, __VA_ARGS__)   //!< Special print for cmdline. See more from TRACE_LEVEL_CMD -level

//aliases for the most commonly used functions and the helper functions
#define tracef(dlevel, grp, ...)                mbed_tracef(dlevel, grp, __VA_ARGS__)       //!< Alias for mbed_tracef()
#define vtracef(dlevel, grp, fmt, ap)           mbed_vtracef(dlevel, grp, fmt, ap)          //!< Alias for mbed_vtracef()
#define tr_array(buf, len)                      mbed_trace_array(buf, len)                  //!< Alias for mbed_trace_array()
#define tr_ipv6(addr_ptr)                       mbed_trace_ipv6(addr_ptr)                   //!< Alias for mbed_trace_ipv6()
#define tr_ipv6_prefix(prefix, prefix_len)      mbed_trace_ipv6_prefix(prefix, prefix_len)  //!< Alias for mbed_trace_ipv6_prefix()
#define trace_array(buf, len)                   mbed_trace_array(buf, len)                  //!< Alias for mbed_trace_array()
#define trace_ipv6(addr_ptr)                    mbed_trace_ipv6(addr_ptr)                   //!< Alias for mbed_trace_ipv6()
#define trace_ipv6_prefix(prefix, prefix_len)   mbed_trace_ipv6_prefix(prefix, prefix_len)  //!< Alias for mbed_trace_ipv6_prefix()


/**
 * Allow specification of default TRACE_GROUP to be used if not specified by application
 */

#ifndef TRACE_GROUP
#ifdef YOTTA_CFG_MBED_TRACE_GROUP
#define TRACE_GROUP_STR_HELPER(x) #x
#define TRACE_GROUP_STR(x) TRACE_GROUP_STR_HELPER(x)
#define TRACE_GROUP TRACE_GROUP_STR(YOTTA_CFG_MBED_TRACE_GROUP)
#endif
#endif

/**
 * Initialize trace functionality
 * @return 0 when all success, otherwise non zero
 */
int mbed_trace_init( void );
/**
 * Free trace memory
 */
void mbed_trace_free( void );
/**
 * Resize buffers (line / tmp ) sizes
 * @param lineLength    new maximum length for trace line (0 = do no resize)
 * @param tmpLength     new maximum length for trace tmp buffer (used for trace_array, etc) (0 = do no resize)
 */
void mbed_trace_buffer_sizes(int lineLength, int tmpLength);
/**
 *  Set trace configurations
 *  Possible parameters:
 *
 *   TRACE_MODE_COLOR
 *   TRACE_MODE_PLAIN (this exclude color mode)
 *   TRACE_CARRIAGE_RETURN (print CR before trace line)
 *
 *   TRACE_ACTIVE_LEVEL_ALL - to activate all trace levels
 *   or TRACE_ACTIVE_LEVEL_DEBUG (alternative)
 *   TRACE_ACTIVE_LEVEL_INFO
 *   TRACE_ACTIVE_LEVEL_WARN
 *   TRACE_ACTIVE_LEVEL_ERROR
 *   TRACE_ACTIVE_LEVEL_CMD
 *   TRACE_LEVEL_NONE - to deactivate all traces
 *
 * @param config  Byte size Bit-mask. Bits are descripted above.
 * usage e.g.
 * @code
 *  mbed_trace_config_set( TRACE_ACTIVE_LEVEL_ALL|TRACE_MODE_COLOR );
 * @endcode
 */
void mbed_trace_config_set(uint8_t config);
/** get trace configurations
 * @return trace configuration byte
 */
uint8_t mbed_trace_config_get(void);
/**
 * Set trace prefix function
 * pref_f -function return string with null terminated
 * Can be used for e.g. time string
 * e.g.
 *   char* trace_time(){ return "rtc-time-in-string"; }
 *   mbed_trace_prefix_function_set( &trace_time );
 */
void mbed_trace_prefix_function_set( char* (*pref_f)(size_t) );
/**
 * Set trace suffix function
 * suffix -function return string with null terminated
 * Can be used for e.g. time string
 * e.g.
 *   char* trace_suffix(){ return " END"; }
 *   mbed_trace_suffix_function_set( &trace_suffix );
 */
void mbed_trace_suffix_function_set(char* (*suffix_f)(void) );
/**
 * Set trace print function
 * By default, trace module print using printf() function,
 * but with this you can write own print function,
 * for e.g. to other IO device.
 */
void mbed_trace_print_function_set( void (*print_f)(const char*) );
/**
 * Set trace print function for tr_cmdline()
 */
void mbed_trace_cmdprint_function_set( void (*printf)(const char*) );
/**
 * Set trace mutex wait function
 * By default, trace calls are not thread safe.
 * If thread safety is required this can be used to set a callback function that will be called before each trace call.
 * The specific implementation is up to the application developer, but the mutex must count so it can
 * be acquired from a single thread repeatedly.
 */
void mbed_trace_mutex_wait_function_set(void (*mutex_wait_f)(void));
/**
 * Set trace mutex release function
 * By default, trace calls are not thread safe.
 * If thread safety is required this can be used to set a callback function that will be called before returning from
 * each trace call. The specific implementation is up to the application developer, but the mutex must count so it can
 * be acquired from a single thread repeatedly.
 */
void mbed_trace_mutex_release_function_set(void (*mutex_release_f)(void));
/**
 * When trace group contains text in filters,
 * trace print will be ignored.
 * e.g.:
 *  mbed_trace_exclude_filters_set("mygr");
 *  mbed_tracef(TRACE_ACTIVE_LEVEL_DEBUG, "ougr", "This is not printed");
 */
void mbed_trace_exclude_filters_set(char* filters);
/** get trace exclude filters
 */
const char* mbed_trace_exclude_filters_get(void);
/**
 * When trace group contains text in filter,
 * trace will be printed.
 * e.g.:
 *  set_trace_include_filters("mygr");
 *  mbed_tracef(TRACE_ACTIVE_LEVEL_DEBUG, "mygr", "Hi There");
 *  mbed_tracef(TRACE_ACTIVE_LEVEL_DEBUG, "grp2", "This is not printed");
 */
void mbed_trace_include_filters_set(char* filters);
/** get trace include filters
 */
const char* mbed_trace_include_filters_get(void);
/**
 * General trace function
 * This should be used every time when user want to print out something important thing
 * Usage e.g.
 *   mbed_tracef( TRACE_LEVEL_INFO, "mygr", "Hello world!");
 *
 * @param dlevel debug level
 * @param grp    trace group
 * @param fmt    trace format (like printf)
 * @param ...    variable arguments related to fmt
 */
#if defined(__GNUC__) || defined(__CC_ARM)
void mbed_tracef(uint8_t dlevel, const char* grp, const char *fmt, ...) __attribute__ ((__format__(__printf__, 3, 4)));
#else
void mbed_tracef(uint8_t dlevel, const char* grp, const char *fmt, ...);
#endif
/**
 * General trace function
 * This should be used every time when user want to print out something important thing
 * and vprintf functionality is desired
 * Usage e.g.
 *   va_list ap;
 *   va_start (ap, fmt);
 *   mbed_vtracef( TRACE_LEVEL_INFO, "mygr", fmt, ap );
 *   va_end (ap);
 *
 * @param dlevel debug level
 * @param grp    trace group
 * @param fmt    trace format (like vprintf)
 * @param ap     variable arguments list (like vprintf)
 */
#if defined(__GNUC__) || defined(__CC_ARM)
void mbed_vtracef(uint8_t dlevel, const char* grp, const char *fmt, va_list ap) __attribute__ ((__format__(__printf__, 3, 0)));
#else
void mbed_vtracef(uint8_t dlevel, const char* grp, const char *fmt, va_list ap);
#endif


/**
 *  Get last trace from buffer
 */
const char* mbed_trace_last(void);
#if MBED_CONF_MBED_TRACE_FEA_IPV6 == 1
/**
 * mbed_tracef helping function for convert ipv6
 * table to human readable string.
 * usage e.g.
 * char ipv6[16] = {...}; // ! array length is 16 bytes !
 * mbed_tracef(TRACE_LEVEL_INFO, "mygr", "ipv6 addr: %s", mbed_trace_ipv6(ipv6));
 *
 * @param add_ptr  IPv6 Address pointer
 * @return temporary buffer where ipv6 is in string format
 */
char* mbed_trace_ipv6(const void *addr_ptr);
/**
 * mbed_tracef helping function for print ipv6 prefix
 * usage e.g.
 * char ipv6[16] = {...}; // ! array length is 16 bytes !
 * mbed_tracef(TRACE_LEVEL_INFO, "mygr", "ipv6 addr: %s", mbed_trace_ipv6_prefix(ipv6, 4));
 *
 * @param prefix        IPv6 Address pointer
 * @param prefix_len    prefix length
 * @return temporary buffer where ipv6 is in string format
 */
char* mbed_trace_ipv6_prefix(const uint8_t *prefix, uint8_t prefix_len);
#endif
/**
 * mbed_tracef helping function for convert hex-array to string.
 * usage e.g.
 *  char myarr[] = {0x10, 0x20};
 *  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "arr: %s", mbed_trace_array(myarr, 2));
 *
 * @param buf  hex array pointer
 * @param len  buffer length
 * @return temporary buffer where string copied
 * if array as string not fit to temp buffer, this function write '*' as last character,
 * which indicate that buffer is too small for array.
 */
char* mbed_trace_array(const uint8_t* buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* MBED_TRACE_H_ */

/* These macros are outside the inclusion guard so they will be re-evaluated for every inclusion of the header.
 * If tracing is disabled, the dummies will hide the real functions. The real functions can still be reached by
 * surrounding the name of the function with brackets, e.g. "(mbed_tracef)(dlevel, grp, "like so");"
 * */
#if defined(FEA_TRACE_SUPPORT) || MBED_CONF_MBED_TRACE_ENABLE || YOTTA_CFG_MBED_TRACE || (defined(YOTTA_CFG) && !defined(NDEBUG))
// Make sure FEA_TRACE_SUPPORT is always set whenever traces are enabled.
#ifndef FEA_TRACE_SUPPORT
#define FEA_TRACE_SUPPORT
#endif
// undefine dummies, revealing the real functions
#undef MBED_TRACE_DUMMIES_DEFINED
#undef mbed_trace_init
#undef mbed_trace_free
#undef mbed_trace_buffer_sizes
#undef mbed_trace_config_set
#undef mbed_trace_config_get
#undef mbed_trace_prefix_function_set
#undef mbed_trace_suffix_function_set
#undef mbed_trace_print_function_set
#undef mbed_trace_cmdprint_function_set
#undef mbed_trace_mutex_wait_function_set
#undef mbed_trace_mutex_release_function_set
#undef mbed_trace_exclude_filters_set
#undef mbed_trace_exclude_filters_get
#undef mbed_trace_include_filters_set
#undef mbed_trace_include_filters_get
#undef mbed_tracef
#undef mbed_vtracef
#undef mbed_trace_last
#undef mbed_trace_ipv6
#undef mbed_trace_ipv6_prefix
#undef mbed_trace_array

#elif !defined(MBED_TRACE_DUMMIES_DEFINED)
// define dummies, hiding the real functions
#define MBED_TRACE_DUMMIES_DEFINED
#define mbed_trace_init(...)                        ((void) 0)
#define mbed_trace_free(...)                        ((void) 0)
#define mbed_trace_buffer_sizes(...)                ((void) 0)
#define mbed_trace_config_set(...)                  ((void) 0)
#define mbed_trace_config_get(...)                  ((uint8_t) 0)
#define mbed_trace_prefix_function_set(...)         ((void) 0)
#define mbed_trace_suffix_function_set(...)         ((void) 0)
#define mbed_trace_print_function_set(...)          ((void) 0)
#define mbed_trace_cmdprint_function_set(...)       ((void) 0)
#define mbed_trace_mutex_wait_function_set(...)     ((void) 0)
#define mbed_trace_mutex_release_function_set(...)  ((void) 0)
#define mbed_trace_exclude_filters_set(...)         ((void) 0)
#define mbed_trace_exclude_filters_get(...)         ((const char *) 0)
#define mbed_trace_include_filters_set(...)         ((void) 0)
#define mbed_trace_include_filters_get(...)         ((const char *) 0)
#define mbed_trace_last(...)                        ((const char *) 0)
#define mbed_tracef(...)                            ((void) 0)
#define mbed_vtracef(...)                           ((void) 0)
/**
 * These helper functions accumulate strings in a buffer that is only flushed by actual trace calls. Using these
 * functions outside trace calls could cause the buffer to overflow.
 */
#define mbed_trace_ipv6(...)                dont_use_trace_helpers_outside_trace_calls
#define mbed_trace_ipv6_prefix(...)         dont_use_trace_helpers_outside_trace_calls
#define mbed_trace_array(...)               dont_use_trace_helpers_outside_trace_calls

#endif /* FEA_TRACE_SUPPORT */
