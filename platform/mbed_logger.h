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
#define MBED_CONF_MAX_LOG_LEVEL             LOG_LEVEL_WARN
#endif

/** Critical level log
 *
 * @param  mod  Module ID String
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_ERR_CRITICAL
#if MBED_ID_BASED_TRACING
#define tr_critical(mod, fmt, ...)    MBED_LOG_ID_1(mod, fmt, LOG_ERR_CRITICAL_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__)
#else
#define tr_critical(mod, fmt, ...)    MBED_LOG_STR_1(mod, fmt, LOG_ERR_CRITICAL_, FILE_NAME_, __LINE__, ##__VA_ARGS__)
#endif
#else
#define tr_critical(mod, ...)
#endif

/** Error level log
 *
 * @param  mod  Module ID String
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_ERR
#if MBED_ID_BASED_TRACING
#define tr_err(mod, fmt, ...)     MBED_LOG_ID_1(mod, fmt, LOG_ERR_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__)
#else
#define tr_err(mod, fmt, ...)     MBED_LOG_STR_1(mod, fmt, LOG_ERR_, FILE_NAME_, __LINE__, ##__VA_ARGS__)
#endif
#else
#define tr_err(mod, ...)
#endif

/** Warning level log
 *
 * @param  mod  Module ID String
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_WARN
#if MBED_ID_BASED_TRACING
#define tr_warn(mod, fmt, ...)    MBED_LOG_ID_1(mod, fmt, LOG_WARN_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__)
#else
#define tr_warn(mod, fmt, ...)    MBED_LOG_STR_1(mod, fmt, LOG_WARN_, FILE_NAME_, __LINE__, ##__VA_ARGS__)
#endif
#else
#define tr_warn(mod, ...)
#endif

/** Debug level log
 *
 * @param  mod  Module ID String
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_DEBUG
#if MBED_ID_BASED_TRACING
#define tr_debug(mod, fmt, ...)     MBED_LOG_ID_1(mod, fmt, LOG_DEBUG_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__)
#else
#define tr_debug(mod, fmt, ...)     MBED_LOG_STR_1(mod, fmt, LOG_DEBUG_, FILE_NAME_, __LINE__, ##__VA_ARGS__)
#endif
#else
#define tr_debug(mod, ...)
#endif

/** Debug level log
 *
 * @param  mod  Module ID String
 * @param  condition output only if condition is true (!= 0)
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_DEBUG
#if MBED_ID_BASED_TRACING
#define tr_debug_if(mod, condition, fmt, ...)  do { if(condition) \
                                                      { \
                                                         MBED_LOG_ID_1(mod, fmt, LOG_DEBUG_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__); \
                                                      } \
                                               } while(0);
#else
#define tr_debug_if(mod, condition, fmt, ...)  do { if(condition) \
                                                      { \
                                                         MBED_LOG_STR_1(mod, fmt, LOG_DEBUG_, FILE_NAME_, __LINE__, ##__VA_ARGS__);\
                                                      } \
                                               } while(0);
#endif
#else
#define tr_debug_if(mod, condition, ...)
#endif

/** Information level log
 *
 * @param  mod  Module ID String
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_INFO
#if MBED_ID_BASED_TRACING
#define tr_info(mod, fmt, ...)   MBED_LOG_ID_1(mod, fmt, LOG_INFO_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__)
#else
#define tr_info(mod, fmt, ...)   MBED_LOG_STR_1(mod, fmt, LOG_INFO_, FILE_NAME_, __LINE__, ##__VA_ARGS__)
#endif
#else
#define tr_info(mod, ...)
#endif

/** Information level log
 *
 * @param  mod  Module ID String
 * @param  condition output only if condition is true (!= 0)
 * @param  fmt  printf-style format string, followed by variables
 **/
#if MBED_CONF_MAX_LOG_LEVEL >= LOG_LEVEL_INFO
#if MBED_ID_BASED_TRACING
#define tr_info_if(mod, condition, fmt, ...)   do { if(condition) \
                                                      { \
                                                         MBED_LOG_ID_1(mod, fmt, LOG_INFO_, FILE_NAME_, __LINE__, __COUNTER__, ##__VA_ARGS__); \
                                                      } \
                                                 } while(0);
#else
#define tr_info_if(mod, condition, fmt, ...)   do { if(condition) \
                                                      { \
                                                         MBED_LOG_STR_1(mod, fmt, LOG_INFO_, FILE_NAME_, __LINE__, ##__VA_ARGS__);\
                                                      } \
                                                 } while(0);

#endif
#else
#define tr_info_if(mod, condition, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
/** @}*/
