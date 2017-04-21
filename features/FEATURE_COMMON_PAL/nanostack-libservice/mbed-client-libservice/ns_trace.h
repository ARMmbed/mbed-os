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
 * \file ns_trace.h
 * Trace interface for NanoStack library as well as application.
 * This file provide simple but flexible way to handle software traces.
 * Trace library are abstract layer, which use stdout (printf) by default, 
 * but outputs can be easily redirect to custom function, for example to 
 * store traces to memory or other interfaces.
 *
 *  usage example:
 * \code(main.c:)
 *      #include "ns_trace.h"
 *      #define TRACE_GROUP  "main"
 *
 *      int main(void){
 *          trace_init();   // initialize trace library
 *          tr_debug("this is debug msg");  //print debug message to stdout: "[DBG]
 *          tr_err("this is error msg");
 *          tr_warn("this is warning msg");
 *          tr_info("this is info msg");
 *          return 0;
 *      }
 * \endcode
 *
 */
#ifndef NS_TRACE_H_
#define NS_TRACE_H_
#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NS_TRACE_USE_MBED_TRACE
#if defined(NS_TRACE_USE_MBED_TRACE)

#if defined(HAVE_DEBUG) && !defined(FEA_TRACE_SUPPORT)
#define FEA_TRACE_SUPPORT
#endif

#include "mbed-trace/mbed_trace.h"


/* Convert libTrace calls to mbed-trace calls */
#define trace_init()                        mbed_trace_init()
#define trace_free()                        mbed_trace_free()
#define set_trace_config(config)            mbed_trace_config_set(config)
#define get_trace_config()                  mbed_trace_config_get()
#define set_trace_prefix_function(pref_f)   mbed_trace_prefix_function_set(pref_f)
#define set_trace_suffix_function(suffix_f) mbed_trace_suffix_function_set(suffix_f)
#define set_trace_print_function(print_f)   mbed_trace_print_function_set(print_f)
#define set_trace_cmdprint_function(printf) mbed_trace_cmdprint_function_set(printf)
#define set_trace_exclude_filters(filters)  mbed_trace_exclude_filters_set(filters)
#define set_trace_include_filters(filters)  mbed_trace_include_filters_set(filters)
#define get_trace_exclude_filters()         mbed_trace_exclude_filters_get()
#define get_trace_include_filters()         mbed_trace_include_filters_get()
#define trace_last()                        mbed_trace_last()


/* Definitions for the old functions with no equivalents in mbed-trace. These work without any special init.
 * */
#if defined(FEA_TRACE_SUPPORT) || defined(HAVE_DEBUG) || (defined(YOTTA_CFG) && !defined(NDEBUG)) /*backward compatible*/
#if defined  __GNUC__ || defined __CC_ARM
/** obsolete function */
void debugf(const char *fmt, ...) __attribute__ ((__format__(__printf__, 1, 2)));   //!< obsolete function
void debug(const char *s);                                                          //!< obsolete function
void debug_put(char c);                                                             //!< obsolete function
void debug_hex(uint8_t x);                                                          //!< obsolete function
void debug_int(int i);                                                              //!< obsolete function
void printf_array(const void *buf, uint16_t len);                                   //!< obsolete function
void printf_string(const void *buf, uint16_t len);                                  //!< obsolete function
void printf_ipv6_address(const void *addr);                                         //!< obsolete function
#else //__GNUC__ || __CC_ARM
//obsolete functions:
void debugf(const char *fmt, ...);
void debug(const char *s);
void debug_put(char c);
void debug_hex(uint8_t x);
void debug_int(int i);
void printf_array(const void *buf, uint16_t len);
void printf_string(const void *buf, uint16_t len);
void printf_ipv6_address(const void *addr);
#endif
#else /*FEA_TRACE_SUPPORT*/
// trace functionality not supported
//obsolete
#define debugf(...)                    ((void) 0)
#define debug(s)                       ((void) 0)
#define debug_put(c)                   ((void) 0)
#define debug_hex(x)                   ((void) 0)
#define debug_int(i)                   ((void) 0)
#define printf_array(buf, len)         ((void) 0)
#define printf_string(buf, len)        ((void) 0)
#define printf_ipv6_address(addr)      ((void) 0)

#endif /*FEA_TRACE_SUPPORT*/

#else /* NS_TRACE_USE_MBED_TRACE */

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

//usage macros:
#define tr_info(...)            tracef(TRACE_LEVEL_INFO,    TRACE_GROUP, __VA_ARGS__)   //!< Print info message
#define tr_debug(...)           tracef(TRACE_LEVEL_DEBUG,   TRACE_GROUP, __VA_ARGS__)   //!< Print debug message
#define tr_warning(...)         tracef(TRACE_LEVEL_WARN,    TRACE_GROUP, __VA_ARGS__)   //!< Print warning message
#define tr_warn(...)            tracef(TRACE_LEVEL_WARN,    TRACE_GROUP, __VA_ARGS__)   //!< Alternative warning message
#define tr_error(...)           tracef(TRACE_LEVEL_ERROR,   TRACE_GROUP, __VA_ARGS__)   //!< Print Error Message
#define tr_err(...)             tracef(TRACE_LEVEL_ERROR,   TRACE_GROUP, __VA_ARGS__)   //!< Alternative error message
#define tr_cmdline(...)         tracef(TRACE_LEVEL_CMD,     TRACE_GROUP, __VA_ARGS__)   //!< Special print for cmdline. See more from TRACE_LEVEL_CMD -level

/** Possible to skip all traces in compile time */
#if defined(FEA_TRACE_SUPPORT) || defined(HAVE_DEBUG) || (defined(YOTTA_CFG) && !defined(NDEBUG)) /*backward compatible*/

#if defined  __GNUC__ || defined __CC_ARM
/**
 * Initialize trace functionality. This method must be called from application process.
 * @return 0 when all success, otherwise non zero
 */
int trace_init( void );
/**
 * Free trace memory. This method must be called from application process.
 */
void trace_free( void );
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
 *  set_trace_config( TRACE_ACTIVE_LEVEL_ALL|TRACE_MODE_COLOR );
 * @endcode
 */
void set_trace_config(uint8_t config);
/** get trace configurations 
 * @return trace configuration byte
 */
uint8_t get_trace_config(void);
/**
 * Set trace prefix function
 * pref_f -function return string with null terminated
 * Can be used for e.g. time string
 * e.g.
 *   char* trace_time(){ return "rtc-time-in-string"; }
 *   set_trace_prefix_function( &trace_time );
 */
void set_trace_prefix_function( char* (*pref_f)(size_t) );
/**
 * Set trace suffix function
 * suffix -function return string with null terminated
 * Can be used for e.g. time string
 * e.g.
 *   char* trace_suffix(){ return " END"; }
 *   set_trace_suffix_function( &trace_suffix );
 */
void set_trace_suffix_function(char* (*suffix_f)(void) );
/**
 * Set trace print function
 * By default, trace module print using printf() function,
 * but with this you can write own print function,
 * for e.g. to other IO device.
 */
void set_trace_print_function( void (*print_f)(const char*) );
/**
 * Set trace print function for tr_cmdline()
 */
void set_trace_cmdprint_function( void (*printf)(const char*) );
/**
 * When trace group contains text in filters,
 * trace print will be ignored.
 * e.g.: 
 *  set_trace_exclude_filters("mygr");
 *  tracef(TRACE_ACTIVE_LEVEL_DEBUG, "ougr", "This is not printed");
 */
void set_trace_exclude_filters(char* filters);
/** get trace exclude filters
 */
const char* get_trace_exclude_filters(void);
/**
 * When trace group contains text in filter,
 * trace will be printed.
 * e.g.:
 *  set_trace_include_filters("mygr");
 *  tracef(TRACE_ACTIVE_LEVEL_DEBUG, "mygr", "Hi There");
 *  tracef(TRACE_ACTIVE_LEVEL_DEBUG, "grp2", "This is not printed");
 */
void set_trace_include_filters(char* filters);
/** get trace include filters
 */
const char* get_trace_include_filters(void);
/**
 * General trace function
 * This should be used every time when user want to print out something important thing
 * Usage e.g.
 *   tracef( TRACE_LEVEL_INFO, "mygr", "Hello world!");
 *
 * @param dlevel debug level
 * @param grp    trace group
 * @param fmt    trace format (like printf)
 * @param ...    variable arguments related to fmt
 */
void tracef(uint8_t dlevel, const char* grp, const char *fmt, ...) __attribute__ ((__format__(__printf__, 3, 4)));
/**
 *  Get last trace from buffer
 */
const char* trace_last(void);
/**
 * tracef helping function for convert ipv6
 * table to human readable string.
 * usage e.g.
 * char ipv6[16] = {...}; // ! array length is 16 bytes !
 * tracef(TRACE_LEVEL_INFO, "mygr", "ipv6 addr: %s", trace_ipv6(ipv6));
 *
 * @param add_ptr  IPv6 Address pointer
 * @return temporary buffer where ipv6 is in string format
 */
char* trace_ipv6(const void *addr_ptr);
/**
 * tracef helping function for print ipv6 prefix
 * usage e.g.
 * char ipv6[16] = {...}; // ! array length is 16 bytes !
 * tracef(TRACE_LEVEL_INFO, "mygr", "ipv6 addr: %s", trace_ipv6_prefix(ipv6, 4));
 *
 * @param prefix        IPv6 Address pointer
 * @param prefix_len    prefix length
 * @return temporary buffer where ipv6 is in string format
 */
char* trace_ipv6_prefix(const uint8_t *prefix, uint8_t prefix_len);
/**
 * tracef helping function for convert hex-array to string.
 * usage e.g.
 *  char myarr[] = {0x10, 0x20};
 *  tracef(TRACE_LEVEL_INFO, "mygr", "arr: %s", trace_array(myarr, 2));
 *
 * @param buf  hex array pointer
 * @param len  buffer length
 * @return temporary buffer where string copied
 */
char* trace_array(const uint8_t* buf, uint16_t len);


/*
 * obsolete - only because of backward compatible reason
 * As soon as all these functions are replaced by new tracef() function, these can be removed.
 */

/** obsolete function */
void debugf(const char *fmt, ...) __attribute__ ((__format__(__printf__, 1, 2)));   //!< obsolete function
void debug(const char *s);                                                          //!< obsolete function
void debug_put(char c);                                                             //!< obsolete function
void debug_hex(uint8_t x);                                                          //!< obsolete function
void debug_int(int i);                                                              //!< obsolete function
void printf_array(const void *buf, uint16_t len);                                   //!< obsolete function
void printf_string(const void *buf, uint16_t len);                                  //!< obsolete function
void printf_ipv6_address(const void *addr);                                         //!< obsolete function

#else //__GNUC__ || __CC_ARM
int trace_init( void );
void trace_free( void );
void set_trace_config(uint8_t config);
void set_trace_prefix_function( char* (*pref_f)(size_t) );
void set_trace_print_function( void (*print_f)(const char*) );
void set_trace_cmdprint_function( void (*printf)(const char*) );
void set_trace_exclude_filters(char* filters);
const char* get_trace_exclude_filters(void);
void set_trace_include_filters(char* filters);
const char* get_trace_include_filters(void);
void tracef(uint8_t dlevel, const char* grp, const char *fmt, ...);
char* trace_ipv6(const void *addr_ptr);
char* trace_array(const uint8_t* buf, uint16_t len);
char* trace_ipv6_prefix(const uint8_t *prefix, uint8_t prefix_len);

//obsolete functions:
void debugf(const char *fmt, ...);
void debug(const char *s);
void debug_put(char c);
void debug_hex(uint8_t x);
void debug_int(int i);
void printf_array(const void *buf, uint16_t len);
void printf_string(const void *buf, uint16_t len);
void printf_ipv6_address(const void *addr);

#endif


#else /*FEA_TRACE_SUPPORT*/

// trace functionality not supported
#define trace_init(...)                ((int) 0)
#define trace_free(...)                ((void) 0)
#define set_trace_config(...)          ((void) 0)
#define set_trace_prefix_function(...) ((void) 0)
#define set_trace_print_function(...)  ((void) 0)
#define set_trace_cmdprint_function(...)  ((void) 0)
#define set_trace_exclude_filters(...) ((void) 0)
#define set_trace_include_filters(...) ((void) 0)
#define get_trace_exclude_filters(...) ((const char*) 0)
#define get_trace_include_filters(...) ((const char*) 0)

#define tracef(...)                    ((void) 0)
#define trace_ipv6(...)                ((char*) 0)
#define trace_array(...)               ((char*) 0)
#define trace_ipv6_prefix(...)         ((char*) 0)

//obsolete
#define debugf(...)                    ((void) 0)
#define debug(s)                       ((void) 0)
#define debug_put(c)                   ((void) 0)
#define debug_hex(x)                   ((void) 0)
#define debug_int(i)                   ((void) 0)
#define printf_array(buf, len)         ((void) 0)
#define printf_string(buf, len)        ((void) 0)
#define printf_ipv6_address(addr)      ((void) 0)

#endif /*FEA_TRACE_SUPPORT*/
#endif /* NS_TRACE_USE_MBED_TRACE */

#ifdef __cplusplus
}
#endif
#endif /* NS_TRACE_H_ */
