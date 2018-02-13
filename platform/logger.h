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

#include <stdio.h>
#include <stdarg.h>
#include "platform/mbed_preprocessor.h"

#ifndef MBED_CONF_MAX_LOG_STR_SIZE
#define MBED_CONF_MAX_LOG_STR_SIZE          128
#endif

#ifndef MBED_CONF_LOG_MAX_BUFFER_SIZE
#define MBED_CONF_LOG_MAX_BUFFER_SIZE       1024
#endif

#define LOG_TOTAL_BUFFER_SIZE       MBED_CONF_LOG_MAX_BUFFER_SIZE
#define LOG_SINGLE_STR_SIZE         MBED_CONF_MAX_LOG_STR_SIZE

#if defined(__ARMCC_VERSION)
#define FILE_NAME_                  __MODULE__
#define FILE_INFO_                  __MODULE__ " " MBED_STRINGIFY(__LINE__) " "
#else
#define FILE_NAME_                  __BASE_FILE__
#define FILE_INFO_                  __BASE_FILE__ " " MBED_STRINGIFY(__LINE__) " "
#endif

#define LOG_LEVEL_ERR_CRITICAL      0
#define LOG_LEVEL_ERR               1
#define LOG_LEVEL_WARN              2
#define LOG_LEVEL_DEBUG             3
#define LOG_LEVEL_INFO              4

// Log-Level Strings
#define LOG_ERR_CRITICAL_        "CRT"
#define LOG_ERR_                 "ERR"
#define LOG_WARN_                "WRN"
#define LOG_DEBUG_               "DBG"
#define LOG_INFO_                "INF"

#define GET_MODULE_ID_(a, b, c, d)   (375 + (a << 1) + (b << 2) + (c >> 2) + (d >> 1))
#define GET_MODULE_ID(x)        (GET_MODULE_ID_(x[0], x[1], x[2], x[3]))

#define SET_MODULE(x)           (GET_MODULE_ID(x) & 0xFF)
#define SET_COUNTER(y)          ((y & 0xFF) << 8)
#define SET_LINE_NUM(z)         ((z & 0xFFFF) << 16)
#define TRACE_ID_(x,y,z)        (SET_MODULE(x) | SET_COUNTER(y) | SET_LINE_NUM(z))

typedef struct trace_id {
    uint32_t t_id;
    uint32_t strLen;
    char *s;
}trace_id_t;

// Macros to log ID based data
#define MBED_LOG_ID_4(...)                           log_buffer_id_data(__VA_ARGS__)
#define MBED_LOG_ID_3(counter, id, args, fmt, ...)   ({volatile static const __attribute__((section(".keep.log_data"))) char str##counter[] = fmt; \
                                                       volatile static const __attribute__((section(".keep.log_data"))) uint32_t len##counter = MBED_STRLEN(fmt); \
                                                       volatile static const __attribute__((section(".keep.log_data"))) uint32_t c##counter = id; \
                                                       MBED_LOG_ID_4(args, id, ##__VA_ARGS__); \
                                                      })
#define MBED_LOG_ID_2(counter, id, ...)               MBED_LOG_ID_3(counter, id, MBED_COUNT_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)
#define MBED_LOG_ID_1(mod, fmt, ll, f, l, c, ...)     MBED_LOG_ID_2(c, TRACE_ID_(mod,c,l), mod " " f " " MBED_STRINGIFY(l) " : " fmt "\n", ##__VA_ARGS__)
                                                      
// Macros to log string data
#define MBED_LOG_STR(...)                          log_buffer_string_data(__VA_ARGS__)
#define MBED_LOG_STR_1(mod, fmt, ll, f, l, ...)    MBED_LOG_STR("%-3.3s %-4.4s %-15s %5d : " fmt "\n", ll, mod, f, l, ##__VA_ARGS__)

void log_buffer_id_data(uint8_t argCount, ...);
void log_buffer_string_data(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif  // LOGGER_INTERNAL_H
/**@}*/

/**@}*/
