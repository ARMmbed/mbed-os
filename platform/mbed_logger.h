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
 * \defgroup platform_log_api Logging API functions
 *
 * Example: Logging module usage
 * @code
 *
 *  #include "mbed.h"
 *  int main() {
 *      MBED_CRIT("OS", "This is critical error. Action Required: %s", "Restart");
 *      MBED_ERR("OS", "Error performing XYZ operation, errno = %d", -10);
 *      MBED_WARN("Lib", "This is warning message");
 *      MBED_DBG_IF("main", 1, "%d %s ", 1, "hello");
 *      MBED_INFO("main", "%s %s 0x%lx", "world", "!", 2);
 *      MBED_TRACE("lib", "Trace level of debugging for libraries");
 *  }
 *
 * @endcode
 *
 * Example: Prints in interrupt context using logging module
 * @code
 *
 *  #include "mbed.h"
 *
 *  void periodicCallback(void)
 *  {
 *      MBED_WARN("ISR", "Print in interrupt context");
 *  }
 *
 *  int main() {
 *      Ticker ticker1;
 *      ticker1.attach_us(periodicCallback, 500000);
 *      int count = 1;
 *      while(count++) {
 *          wait(0.2);
 *          MBED_DBG_IF("main", (0 == count%10), "Count = %d", count);
 *      }
 *  }
 *
 * @endcode
 *
 * @{
 */
#ifndef MBED_LOGGER_H
#define MBED_LOGGER_H

#include <stdint.h>
#include "platform/logger.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  MBED_ID_BASED_TRACING
#error "ID based tracing is not supported"
#endif

#ifndef MBED_CONF_MAX_LOG_LEVEL
#define MBED_CONF_MAX_LOG_LEVEL            LOG_LEVEL_DEBUG
#endif

/** Critical level log
 *
 *  Critical level of log is equivalent to ASSERTS and it will dump
 *  all previous prints before asserting, use only when you want to assert.
 *
 * @param  mod  Module ID String
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_ERR_CRITICAL
#define MBED_CRIT(mod, fmt, ...)    MBED_LOG_ASSERT_1(mod, fmt, LOG_ERR_CRITICAL_, FILE_NAME_, __LINE__, ##__VA_ARGS__)
#else
#define MBED_CRIT(mod, fmt, ...)
#endif

/** Error level log
 *
 *  Error level of log can be used by OS, library and application to
 *  log all the errors.
 *
 * @param  mod  Module ID String
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_ERR
#if MBED_ID_BASED_TRACING
#define MBED_ERR(mod, fmt, ...)     MBED_LOG_ID_1(mod, fmt, LOG_ERR_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__)
#else
#define MBED_ERR(mod, fmt, ...)     MBED_LOG_STR_1(mod, fmt, LOG_ERR_, ##__VA_ARGS__)
#endif
#else
#define MBED_ERR(mod, fmt, ...)
#endif

/** Warning level log
 *
 *  Warning level of log can be used by OS, library and application to
 *  log all the warning messages.
 *
 * @param  mod  Module ID String
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_WARN
#if MBED_ID_BASED_TRACING
#define MBED_WARN(mod, fmt, ...)    MBED_LOG_ID_1(mod, fmt, LOG_WARN_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__)
#else
#define MBED_WARN(mod, fmt, ...)    MBED_LOG_STR_1(mod, fmt, LOG_WARN_, ##__VA_ARGS__)
#endif
#else
#define MBED_WARN(mod, fmt, ...)
#endif

/** Debug level log
 *
 *  Debugging level is mainly for application. Libraries and OS must
 *  not use this for debug prints, use trace level instead.
 *
 * @param  mod  Module ID String
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_DEBUG
#if MBED_ID_BASED_TRACING
#define MBED_DBG(mod, fmt, ...)     MBED_LOG_ID_1(mod, fmt, LOG_DEBUG_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__)
#else
#define MBED_DBG(mod, fmt, ...)     MBED_LOG_STR_1(mod, fmt, LOG_DEBUG_, ##__VA_ARGS__)
#endif
#else
#define MBED_DBG(mod, fmt, ...)
#endif

/** Debug level log
 *
 *  Debugging level is mainly for application. Libraries and OS must
 *  not use this for debug prints, use trace level instead.
 *
 * @param  mod  Module ID String
 * @param  condition output only if condition is true (!= 0)
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_DEBUG
#if MBED_ID_BASED_TRACING
#define MBED_DBG_IF(mod, condition, fmt, ...)  do { if(condition) \
                                                      { \
                                                         MBED_LOG_ID_1(mod, fmt, LOG_DEBUG_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__); \
                                                      } \
                                               } while(0);
#else
#define MBED_DBG_IF(mod, condition, fmt, ...)  do { if(condition) \
                                                      { \
                                                         MBED_LOG_STR_1(mod, fmt, LOG_DEBUG_, ##__VA_ARGS__);\
                                                      } \
                                               } while(0);
#endif
#else
#define MBED_DBG_IF(mod, condition, fmt, ...)
#endif

/** Information level log
 *
 *  Info level of log can be used by OS, library and application to
 *  log any kind of information.
 *
 * @param  mod  Module ID String
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_INFO
#if MBED_ID_BASED_TRACING
#define MBED_INFO(mod, fmt, ...)   MBED_LOG_ID_1(mod, fmt, LOG_INFO_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__)
#else
#define MBED_INFO(mod, fmt, ...)   MBED_LOG_STR_1(mod, fmt, LOG_INFO_, ##__VA_ARGS__)
#endif
#else
#define MBED_INFO(mod, fmt, ...)
#endif

/** Information level log
 *
 *  Info level of log can be used by OS, library and application to
 *  log any kind of information.
 *
 * @param  mod  Module ID String
 * @param  condition output only if condition is true (!= 0)
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_INFO
#if MBED_ID_BASED_TRACING
#define MBED_INFO_IF(mod, condition, fmt, ...)   do { if(condition) \
                                                      { \
                                                         MBED_LOG_ID_1(mod, fmt, LOG_INFO_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__); \
                                                      } \
                                                 } while(0);
#else
#define MBED_INFO_IF(mod, condition, fmt, ...)   do { if(condition) \
                                                      { \
                                                         MBED_LOG_STR_1(mod, fmt, LOG_INFO_, ##__VA_ARGS__);\
                                                      } \
                                                 } while(0);

#endif
#else
#define MBED_INFO_IF(mod, condition, fmt, ...)
#endif

/** Trace level log
 *
 *  Trace level of log can be used by OS, library to log internal information
 *  used for tracing
 *
 * @param  mod  Module ID String
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_TRACE
#if MBED_ID_BASED_TRACING
#define MBED_TRACE(mod, fmt, ...)   MBED_LOG_ID_1(mod, fmt, LOG_TRACE_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__)
#else
#define MBED_TRACE(mod, fmt, ...)   MBED_LOG_STR_1(mod, fmt, LOG_TRACE_, ##__VA_ARGS__)
#endif
#else
#define MBED_TRACE(mod, fmt, ...)
#endif

/** Trace level log
 *
 *  Debugging level is mainly for application. Libraries and OS must
 *  not use this for debug prints, use trace level instead.
 *
 * @param  mod  Module ID String
 * @param  condition output only if condition is true (!= 0)
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_TRACE
#if MBED_ID_BASED_TRACING
#define MBED_TRACE_IF(mod, condition, fmt, ...)  do { if(condition) \
                                                      { \
                                                         MBED_LOG_ID_1(mod, fmt, LOG_TRACE_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__); \
                                                      } \
                                               } while(0);
#else
#define MBED_TRACE_IF(mod, condition, fmt, ...)  do { if(condition) \
                                                      { \
                                                         MBED_LOG_STR_1(mod, fmt, LOG_TRACE_, ##__VA_ARGS__);\
                                                      } \
                                               } while(0);
#endif
#else
#define MBED_TRACE_IF(mod, condition, fmt, ...)
#endif

/** General trace function
 *
 *  Generic function to print user data, this log level is always
 *  enabled hence should be used to print important information
 *
 * @param ll   Log level
 * @param mod  Module name
 * @param fmt  trace format (like printf)
 * @param ...  variable arguments related to fmt
 */
#if MBED_ID_BASED_TRACING
#define MBED_LOG(ll, mod, fmt, ...)              MBED_LOG_ID_1(mod, fmt, GET_LOG_STRING(ll), FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__)
#else
#define MBED_LOG(ll, mod, fmt, ...)              MBED_LOG_STR_1(mod, fmt, GET_LOG_STRING(ll), ##__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
/** @}*/
