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
 
/**
 * \file mbed_trace_internal.h
 * Trace interface for MbedOS applications.
 * This file provide simple but flexible way to handle software traces.
 * Trace library are abstract layer, which use stdout (printf) by default,
 * but outputs can be easily redirect to custom function, for example to
 * store traces to memory or other interfaces.
 *
 *  usage example:
 * \code(main.c:)
 *      #include "mbed.h"
 *      #define TRACE_GROUP  "main"
 *
 *      int main(void){ 
 *          tr_debug("this is debug msg");
 *          tr_info("this is info msg");
 *          tr_warn("this is warning msg");
 *          tr_err("this is error msg");
 *          return 0;
 *      }
 * \endcode
 *
 * @{
 */
 
#ifndef MBED_TRACE_INTERNAL_H
#define MBED_TRACE_INTERNAL_H

#ifndef MBED_CONF_EXTERNAL_MBED_TRACE_ENABLED
#include "platform/mbed_logger.h"

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
#else
#define mbed_trace_array(buf, len)                  mbed_log_array(buf, len)
#define mbed_trace_ipv6(addr_ptr)                   mbed_log_ipv6(addr_ptr)
#define mbed_trace_ipv6_prefix(prefix, prefix_len)  mbed_log_ipv6_prefix(prefix, prefix_len)
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
#define mbed_trace_mutex_wait_function_set(...)     ((void) 0)
#define mbed_trace_mutex_release_function_set(...)  ((void) 0)
#define mbed_trace_exclude_filters_set(...)         ((void) 0)
#define mbed_trace_exclude_filters_get(...)         ((const char *) 0)
#define mbed_trace_include_filters_set(...)         ((void) 0)
#define mbed_trace_include_filters_get(...)         ((const char *) 0)
#define mbed_trace_last(...)                        ((const char *) 0)

#endif 
#endif
