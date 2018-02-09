/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef CELLULAR_LOG_H_
#define CELLULAR_LOG_H_

#include <stdint.h>
#ifdef FEATURE_COMMON_PAL
#include <stdarg.h>
#endif // FEATURE_COMMON_PAL
#include "PinNames.h"
#include "us_ticker_api.h"
#include "mbed_debug.h"

namespace mbed {

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

#define TRACE_GROUP "cellular"

#ifndef MBED_TRACE_MAX_LEVEL
#define MBED_TRACE_MAX_LEVEL TRACE_LEVEL_ERROR
#endif

#ifdef FEATURE_COMMON_PAL
#include "mbed-trace/mbed_trace.h"
#define log_debug tr_debug
#define log_info tr_info
#define log_warn tr_warn
#define log_error tr_error
#else
extern uint32_t cellular_log_time;
#if (MBED_TRACE_MAX_LEVEL >= TRACE_LEVEL_DEBUG)
#define log_debug(format, ...) do { debug("DEBUG" " [" TRACE_GROUP " %ums] " format "\r\n", (us_ticker_read()-cellular_log_time) / 1000L, ## __VA_ARGS__); } while (0)
#else
#define log_debug(...)
#endif
#if (MBED_TRACE_MAX_LEVEL >= TRACE_LEVEL_INFO)
#define log_info(format, ...)  do { debug("INFO" " [" TRACE_GROUP " %ums] " format "\r\n", (us_ticker_read()-cellular_log_time) / 1000L, ## __VA_ARGS__); } while (0)
#else
#define log_info(...)
#endif
#if (MBED_TRACE_MAX_LEVEL >= TRACE_LEVEL_WARN)
#define log_warn(format, ...)  do { debug("WARN" " [" TRACE_GROUP " %ums] " format "\r\n", (us_ticker_read()-cellular_log_time) / 1000L, ## __VA_ARGS__); } while (0)
#else
#define log_warn(...)
#endif
#if (MBED_TRACE_MAX_LEVEL >= TRACE_LEVEL_ERROR)
#define log_error(format, ...) do { debug("ERROR" " [" TRACE_GROUP " %ums] " format "\r\n", (us_ticker_read()-cellular_log_time) / 1000L, ## __VA_ARGS__); } while (0)
#else
#define log_error(...)
#endif
#endif

extern void log_init(PinName tx, PinName rx, int baud);

} // namespace mbed

#endif // CELLULAR_LOG_H_
