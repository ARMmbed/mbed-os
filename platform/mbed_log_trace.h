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

#ifndef MBED_LOG_TRACE_H
#define MBED_LOG_TRACE_H

#if defined(MBED_CONF_PLATFORM_LOGGING_ENABLE) && (MBED_CONF_PLATFORM_LOGGING_ENABLE)
#include "platform/mbed_logger.h"

#define MBED_TRACE_MAX_LEVEL                    MBED_CONF_PLATFORM_LOG_MAX_LEVEL

#define tr_error(...)                           MBED_ERR(TRACE_GROUP, ##__VA_ARGS__)
#define tr_err(...)                             MBED_ERR(TRACE_GROUP, ##__VA_ARGS__)

#define tr_warning(...)                         MBED_WARN(TRACE_GROUP, ##__VA_ARGS__)
#define tr_warn(...)                            MBED_WARN(TRACE_GROUP, ##__VA_ARGS__)

#define tr_debug(...)                           MBED_DBG(TRACE_GROUP, ##__VA_ARGS__)

#define tr_info(...)                            MBED_INFO(TRACE_GROUP, ##__VA_ARGS__)

#define tr_cmdline(...)                         MBED_LOG(TRACE_LEVEL_CMD, TRACE_GROUP, ##__VA_ARGS__)


//aliases for the most commonly used functions and the helper functions
#define tracef(dlevel, grp, ...)                mbed_tracef(dlevel, grp, ##__VA_ARGS__)
#define vtracef(dlevel, grp, fmt, ap)           mbed_vtracef(dlevel, grp, fmt, ap)
#define tr_array(buf, len)                      mbed_trace_array(buf, len)
#define tr_ipv6(addr_ptr)                       mbed_trace_ipv6(addr_ptr)
#define tr_ipv6_prefix(prefix, prefix_len)      mbed_trace_ipv6_prefix(prefix, prefix_len)
#define trace_array(buf, len)                   mbed_trace_array(buf, len)
#define trace_ipv6(addr_ptr)                    mbed_trace_ipv6(addr_ptr)
#define trace_ipv6_prefix(prefix, prefix_len)   mbed_trace_ipv6_prefix(prefix, prefix_len)

#define mbed_tracef(dlevel, grp, fmt, ...)      MBED_LOG(dlevel, grp, fmt, ##__VA_ARGS__);
#define mbed_vtracef(dlevel, grp, fmt, ap)      MBED_LOG_VSTR_1(MBED_STRINGIFY(dlevel), grp, MBED_STRINGIFY(fmt), ap)

#ifdef  MBED_ID_BASED_TRACING
#define mbed_trace_array(buf, len)                  "Not supported"
#define mbed_trace_ipv6(addr_ptr)                   "Not supported"
#define mbed_trace_ipv6_prefix(prefix, prefix_len)  "Not supported"
#define mbed_trace_mutex_wait_function_set(...)     ((void) 0)
#define mbed_trace_mutex_release_function_set(...)  ((void) 0)
#else
#define mbed_trace_array(buf, len)                  mbed_log_array(buf, len)
#define mbed_trace_ipv6(addr_ptr)                   mbed_log_ipv6(addr_ptr)
#define mbed_trace_ipv6_prefix(prefix, prefix_len)  mbed_log_ipv6_prefix(prefix, prefix_len)
#define mbed_trace_mutex_wait_function_set(wait)    mbed_log_set_mutex_wait(wait)
#define mbed_trace_mutex_release_function_set(release) mbed_log_set_mutex_release(release)
#endif

// Not supported
#define mbed_trace_init(...)                        ((int) 0)
#define mbed_trace_free(...)                        ((void) 0)
#define mbed_trace_buffer_sizes(...)                ((void) 0)
#define mbed_trace_config_set(...)                  ((void) 0)
#define mbed_trace_config_get(...)                  ((uint8_t) 0)
#define mbed_trace_prefix_function_set(...)         ((void) 0)
#define mbed_trace_suffix_function_set(...)         ((void) 0)
#define mbed_trace_print_function_set(...)          ((void) 0)
#define mbed_trace_cmdprint_function_set(...)       ((void) 0)
#define mbed_trace_exclude_filters_set(...)         ((void) 0)
#define mbed_trace_exclude_filters_get(...)         ((const char *) 0)
#define mbed_trace_include_filters_set(...)         ((void) 0)
#define mbed_trace_include_filters_get(...)         ((const char *) 0)
#define mbed_trace_last(...)                        ((const char *) 0)

#else // MBED_CONF_PLATFORM_LOGGING_ENABLE

#include "mbed_trace.h"
#include "platform/logger_internal.h"

#define LOG_LEVEL_ERR_CRITICAL   0x1
#define LOG_LEVEL_ERR            0x2
#define LOG_LEVEL_WARN           0x4
#define LOG_LEVEL_INFO           0x8
#define LOG_LEVEL_DEBUG          0x10

#ifndef MBED_CONF_PLATFORM_LOG_MAX_LEVEL
#define MBED_CONF_PLATFORM_LOG_MAX_LEVEL  LOG_LEVEL_INFO
#endif

#if MBED_CONF_PLATFORM_LOG_MAX_LEVEL >= LOG_LEVEL_ERR_CRITICAL
#define MBED_CRIT(mod, fmt, ...)    MBED_LOG_ASSERT_1(mod, fmt, LOG_ERR_CRITICAL_, LOG_FILE_NAME_, __LINE__, ##__VA_ARGS__)
#else
#define MBED_CRIT(mod, fmt, ...)
#endif

#if MBED_CONF_PLATFORM_LOG_MAX_LEVEL >= LOG_LEVEL_ERR
#define MBED_ERR(mod, fmt, ...)     mbed_tracef(LOG_LEVEL_ERR, mod, fmt, ##__VA_ARGS__)
#else
#define MBED_ERR(mod, fmt, ...)
#endif

#if MBED_CONF_PLATFORM_LOG_MAX_LEVEL >= LOG_LEVEL_WARN
#define MBED_WARN(mod, fmt, ...)    mbed_tracef(LOG_LEVEL_WARN, mod, fmt, ##__VA_ARGS__)
#else
#define MBED_WARN(mod, fmt, ...)
#endif

#if MBED_CONF_PLATFORM_LOG_MAX_LEVEL >= LOG_LEVEL_DEBUG
#define MBED_DBG(mod, fmt, ...)     mbed_tracef(LOG_LEVEL_DEBUG, mod, fmt, ##__VA_ARGS__)
#else
#define MBED_DBG(mod, fmt, ...)
#endif

#if MBED_CONF_PLATFORM_LOG_MAX_LEVEL >= LOG_LEVEL_DEBUG
#define MBED_DBG_IF(mod, condition, fmt, ...)  do { if(condition) \
                                                      { \
                                                        mbed_tracef(LOG_LEVEL_DEBUG, mod, fmt, ##__VA_ARGS__);\
                                                      } \
                                               } while(0);
#else
#define MBED_DBG_IF(mod, condition, fmt, ...)
#endif

#if MBED_CONF_PLATFORM_LOG_MAX_LEVEL >= LOG_LEVEL_INFO
#define MBED_INFO(mod, fmt, ...)   mbed_tracef(LOG_LEVEL_INFO, mod, fmt, ##__VA_ARGS__)
#else
#define MBED_INFO(mod, fmt, ...)
#endif

#if MBED_CONF_PLATFORM_LOG_MAX_LEVEL >= LOG_LEVEL_INFO
#define MBED_INFO_IF(mod, condition, fmt, ...)   do { if(condition) \
                                                      { \
                                                         mbed_tracef(LOG_LEVEL_INFO, mod, fmt, ##__VA_ARGS__);\
                                                      } \
                                                 } while(0);

#else
#define MBED_INFO_IF(mod, condition, fmt, ...)
#endif

#define MBED_LOG(ll, mod, fmt, ...)     mbed_tracef(ll, mod, fmt, ##__VA_ARGS__)

#endif
#endif // MBED_LOG_TRACE_H
