/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_log Logger functions
 * @{
 */

#ifndef LOGGER_INTERNAL_H
#define LOGGER_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "platform/mbed_preprocessor.h"

#ifndef MBED_CONF_PLATFORM_LOG_MAX_STR_SIZE
#define MBED_CONF_PLATFORM_LOG_MAX_STR_SIZE      128
#endif

#ifndef MBED_CONF_PLATFORM_LOG_MAX_BUFFER_SIZE
#define MBED_CONF_PLATFORM_LOG_MAX_BUFFER_SIZE   512
#endif

#if defined (MBED_ID_BASED_TRACING)
#define LOG_DATA_TYPE_   uint32_t
#else
#define LOG_DATA_TYPE_   char
#endif

#if defined(__ARMCC_VERSION)
#define LOG_FILE_NAME_                  __MODULE__
#else
#define LOG_FILE_NAME_                  __BASE_FILE__
#endif

#define LOG_SINGLE_STR_SIZE_            MBED_CONF_PLATFORM_LOG_MAX_STR_SIZE
#define LOG_BUF_SIZE_                   MBED_CONF_PLATFORM_LOG_MAX_BUFFER_SIZE

// Log-Level Strings
#define LOG_GEN_                 "GEN "
#define LOG_ERR_CRITICAL_        "CRT "
#define LOG_ERR_                 "ERR "
#define LOG_WARN_                "WARN"
#define LOG_DEBUG_               "DBG "
#define LOG_INFO_                "INFO"
#define LOG_TRACE_               "TRAC"

#define GET_LOG_STRING(ll)      ((ll > LOG_LEVEL_DEBUG) ? LOG_TRACE_ : \
                                ((ll > LOG_LEVEL_INFO)  ? LOG_DEBUG_ : \
                                ((ll > LOG_LEVEL_WARN)  ? LOG_INFO_  : \
                                ((ll > LOG_LEVEL_ERR)   ? LOG_WARN_  : \
                                ((ll >= 1)   ? LOG_ERR_   : LOG_GEN_)))))

#define GET_MODULE_ID_(a, b, c, d)   (375 + (a << 1) + (b << 2) + (c >> 2) + (d >> 1))
#define GET_MODULE_ID(x)        (GET_MODULE_ID_(x[0], x[1], x[2], x[3]))

#define SET_MODULE(x)           (GET_MODULE_ID(x) & 0xFF)
#define SET_COUNTER(y)          ((y & 0xFF) << 8)
#define SET_LINE_NUM(z)         ((z & 0xFFFF) << 16)
#define TRACE_ID_(x,y,z)        (SET_MODULE(x) | SET_COUNTER(y) | SET_LINE_NUM(z))

// Macros to log ID based data
#define MBED_LOG_ID_4(...)                            log_id_data(__VA_ARGS__)
// Data dumped in special section is : Unique ID, format string length and format string
#if 0
#define MBED_LOG_ID_3(counter, id, args, fmt, ...)    ({volatile static const __attribute__((section(".keep.log_data"))) char str##counter[] = fmt; \
                                                       volatile static const __attribute__((section(".keep.log_data"))) uint32_t len##counter = MBED_STRLEN(fmt); \
                                                       volatile static const __attribute__((section(".keep.log_data"))) uint32_t c##counter = id; \
                                                       MBED_LOG_ID_4(args, id, ##__VA_ARGS__); \
                                                      })
#else
#define MBED_LOG_ID_3(counter, id, args, fmt, ...)    MBED_LOG_ID_4(args, id, ##__VA_ARGS__);
#endif

#define MBED_LOG_ID_2(counter, id, ...)               MBED_LOG_ID_3(counter, id, MBED_COUNT_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)
#define MBED_LOG_ID_1(mod, fmt, ll, f, l, c, ...)     MBED_LOG_ID_2(c, TRACE_ID_(mod,c,l), "[" mod "][" f "][" MBED_STRINGIFY(l) "]: " fmt, ##__VA_ARGS__)

// Macros to log string data
#define MBED_LOG_STR(...)                             log_str_data(__VA_ARGS__)
#define MBED_LOG_STR_1(mod, fmt, ll, ...)             MBED_LOG_STR("[%-4.4s][%-4.4s]: " fmt, ll, mod, ##__VA_ARGS__)

// Assert string
#define MBED_LOG_ASSERT(...)                          log_assert(__VA_ARGS__)
#define MBED_LOG_ASSERT_1(mod, fmt, ll, f, l, ...)    MBED_LOG_ASSERT("[%-4.4s][%-4.4s][%-15s][%5d]: " fmt, ll, mod, f, l, ##__VA_ARGS__)

#define MBED_LOG_VSTR_2(fmt1, ap)                     log_str_vdata(fmt1, ap)
#define MBED_LOG_VSTR_1(ll, mod, fmt, ap)             MBED_LOG_VSTR_2("[" ll "][" mod "]: " fmt, ap)

/*  Internal function for assertion
 *
 *  This should be used to log assert information, application will termiate post logging
 *
 *  @param format    printf like format
 *  @param ...       variable arguments related to format
 *  @note: Application will terminate after printing assert log.
 */
#if defined(__GNUC__) || defined(__CC_ARM)
void log_assert(const char *format, ...) __attribute__ ((__format__(__printf__, 1, 2)));
#else
void log_assert(const char *format, ...);
#endif

/*  Internal binary data logging function.
 *
 *  This should be used to log binary data.
 *
 *  @param argCount  Count of following variable arguments
 *  @param args      variable arguments (uint32_t)
 *  @note Only uint32_t type variable arguments accepted.
 */
void log_id_data(uint32_t argCount, ...);

/*  Internal string data logging function
 *
 *  This should be used to log string data.
 *
 *  @param format    printf like format
 *  @param ...       variable arguments related to format
 */
#if defined(__GNUC__) || defined(__CC_ARM)
void log_str_data(const char *format, ...) __attribute__ ((__format__(__printf__, 1, 2)));
#else
void log_str_data(const char *format, ...);
#endif

/*  Internal string data logging function
 *
 *  This should be used to log string data like vprintf.
 *
 *  @param format    vprintf like format
 *  @param args      variable arguments
 */
void log_str_vdata(const char *format, va_list args);

/*  Logging lock
 *
 *  Thread safety is achieved using mutex lock/unlock, lock is applied in each log call.
 *  If external library needs to synchronize with mbed logging, set the locking functions
 *  in `mbed_log_set_mutex_wait` and `mbed_log_set_mutex_release`
 */
void mbed_log_lock(void);

/*  Logging unlock
 *
 *  Thread safety is achieved using mutex lock/unlock, lock is applied in each log call.
 *  If external library needs to synchronize with mbed logging, set the locking functions
 *  in `mbed_log_set_mutex_wait` and `mbed_log_set_mutex_release`
 */
void mbed_log_unlock(void);

/*  Logging unlock all
 *
 *  Few helper functions passed as argument in Logging API's need additional lock
 *  which should be released only when entire format string is passed. Unlock all
 *  does unlocking of all locks using a counter.
 */
void mbed_log_unlock_all(void);

typedef void (*mbed_log_mutex_fptr)(void);
/*  Logging Custom lock
 *
 *  Function pointer to application lock can be used instead of mbed logging libraries
 *  default lock
 *
 *  @param  mutex_wait_f  Function pointer to mutex wait function
 */
void mbed_log_set_mutex_wait(mbed_log_mutex_fptr mutex_wait_f);

/*  Logging Custom Unlock
 *
 *  Function pointer to application unlock can be used instead of mbed logging libraries
 *  default unlock/release
 *
 *  @param  mutex_release_f  Function pointer to mutex release function
 */
void mbed_log_set_mutex_release(mbed_log_mutex_fptr mutex_release_f);

#if defined(NDEBUG) || !defined(MBED_CONF_RTOS_PRESENT)
#define log_reset(...)                   ((void) 0)
#define log_buffer_data(...)             ((void) 0)
#define log_disable_time_capture(...)    ((void) 0)
#define log_enable_time_capture(...)     ((void) 0)
#else

/*  Reset to default state in logging
 *
 *  Resets start pointer of external buffer (if used), does not clear buffer.
 *  Enables time capture info
 */
void log_reset(void);

/*  Enables logging in external buffer
 *
 *  If valid buffer pointer is provided, all log data will be dumped to
 *  buffer and not UART. Application should allocate buffer equivalent to
 *  `MBED_CONF_PLATFORM_LOG_MAX_BUFFER_SIZE` size. Data will be over-written when
 *  full.
 *
 *  @param str - Buffer pointer
 */
void log_buffer_data(char *str);

/*  Disables time logging for each log call
 *
 *  Time information will not be captured for logs
 */
void log_disable_time_capture(void);

/*  Enables time logging for each log call
 *
 *  Time information will be captured for logs
 */
void log_enable_time_capture(void);

#endif

#if defined(NDEBUG) || defined(MBED_ID_BASED_TRACING)
#define mbed_log_array(...)              ((const char *) 0)
#define mbed_log_ipv6(...)               ((const char *) 0)
#define mbed_log_ipv6_prefix(...)        ((const char *) 0)
#else

/*  Helper to log array info
 *
 *  Helper function to convert hex array to string. If array as string does
 *  not fit in allocated buffer, '*' will be appended to end of string.
 *
 *  @param  buf  array pointer
 *  @param  len  buffer length
 *  @note  Will not print buffer info, if used in ISR
 */
char* mbed_log_array(const uint8_t *buf, uint32_t len);

/*  Helper to log ipv6 info
 *
 *  Helper function to convert ipv6 table to human readable string.
 *
 *  @param  add_ptr  IPv6 Address pointer
 *  @return  pointer to converted string
 *  @note  Will not print buffer info, if used in ISR
 */
char* mbed_log_ipv6(const void *addr_ptr);

/*  Helper to log ipv6 prefix
 *
 *  Helper function to convert ipv6 table to human readable string.
 *
 *  @param  add_ptr  IPv6 Address pointer
 *  @param  prefix_len  prefix length
 *  @return  pointer to converted string
 *  @note  Will not print buffer info, if used in ISR
 */
char* mbed_log_ipv6_prefix(const uint8_t *prefix, uint32_t prefix_len);
#endif

#ifdef __cplusplus
}
#endif

#endif  // LOGGER_INTERNAL_H
/**@}*/

/**@}*/
