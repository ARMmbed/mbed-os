/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "ns_time_api.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_pae_time.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wst"

// Wednesday, January 1, 2020 0:00:00 GMT
#define CURRENT_TIME_INIT_VALUE        1577836800

// Increment two hours in addition to maximum storing interval
#define CURRENT_TIME_INCREMENT_VALUE   (2 * 3600)

static uint64_t current_time = CURRENT_TIME_INIT_VALUE;
static ns_time_api_system_time_callback *system_time_callback = NULL;

uint16_t ws_pae_time_to_short_convert(uint32_t time)
{
    uint16_t short_time;
    time_format_t format;

    if (time < STIME_TIME_MAX) {
        short_time = time;
        format = TIME_FORMAT_SECONDS;
    } else if (time < (STIME_TIME_MAX * 60)) {
        short_time = time / 60;
        format = TIME_FORMAT_MINUTES;
    } else if (time < (STIME_TIME_MAX * 3600)) {
        short_time = time / 3600;
        format = TIME_FORMAT_HOURS;
    } else {
        short_time = time / 86400;
        format = TIME_FORMAT_DAYS;
    }

    short_time |= ((uint16_t) format) << STIME_TIME_BITS;

    return short_time;
}

uint32_t ws_pae_time_from_short_convert(uint16_t short_time)
{
    uint32_t time;

    time_format_t format = short_time >> STIME_TIME_BITS;

    short_time &= STIME_TIME_MASK;

    if (format == TIME_FORMAT_SECONDS) {
        time = short_time;
    } else if (format == TIME_FORMAT_MINUTES) {
        time = short_time * 60;
    } else if (format == TIME_FORMAT_HOURS) {
        time = short_time * 3600;
    } else {
        time = short_time * 86400;
    }

    return time;
}

bool ws_pae_time_from_short_time_compare(uint16_t short_time1, uint16_t short_time2)
{
    uint32_t time1 = ws_pae_time_from_short_convert(short_time1);
    uint32_t time2 = ws_pae_time_from_short_convert(short_time2);

    // Calculate difference
    uint32_t difference;
    if (time1 > time2) {
        difference = time1 - time2;
    } else {
        difference = time2 - time1;
    }

    // Allow variable difference to be regarded as same based on format
    if (STIME_FORMAT_GET(short_time1) == TIME_FORMAT_DAYS || STIME_FORMAT_GET(short_time2) == TIME_FORMAT_DAYS) {
        if (difference > 2 * 24 * 3600) { // Two days
            return false;
        }
    } else if (STIME_FORMAT_GET(short_time1) == TIME_FORMAT_HOURS || STIME_FORMAT_GET(short_time2) == TIME_FORMAT_HOURS) {
        if (difference > 2 * 3600) { // Two hours
            return false;
        }
    } else if (STIME_FORMAT_GET(short_time1) == TIME_FORMAT_MINUTES || STIME_FORMAT_GET(short_time2) == TIME_FORMAT_MINUTES) {
        if (difference > 5 * 60) { // 5 minutes
            return false;
        }
    } else {
        if (difference > 10) { // 10 seconds
            return false;
        }
    }

    return true;
}

int8_t ws_pae_time_diff_calc(uint64_t curr_time, uint64_t comp_time, uint32_t *time_diff, bool future)
{
    int32_t difference;
    *time_diff = 0;
    // Comparison time is in future
    if (curr_time < comp_time) {
        if (!future) {
            // Do not allow times in future
            return -1;
        }
        difference = comp_time - curr_time;
    } else {
        // Comparison time is in past
        if (future) {
            // Do not allow times in past
            return -1;
        }
        difference = curr_time - comp_time;
    }

    // If difference is more two years time is invalid
    if (difference > SEC_MAXIMUM_LIFETIME) {
        return -1;
    }

    *time_diff = difference;

    return 0;
}

uint64_t ws_pae_current_time_get(void)
{
    if (system_time_callback) {
        return system_time_callback();
    }

    return current_time;
}

void ws_pae_current_time_update(uint16_t seconds)
{
    current_time += seconds;
}

int8_t ws_pae_current_time_set(uint64_t time)
{
    current_time = time;

    if (system_time_callback) {
        uint64_t system_time = system_time_callback();
        // System time has gone backwards
        if (system_time < current_time || system_time > current_time + SYSTEM_TIME_MAXIMUM_DIFF) {
            tr_error("FATAL: system time less than reference time or more than 12 months in future: %"PRIi64" reference time: %"PRIi64, system_time, current_time);
            return -1;
        }
    } else {
        current_time += FRAME_COUNTER_STORE_FORCE_INTERVAL + CURRENT_TIME_INCREMENT_VALUE;
    }

    return 0;
}

void ns_time_api_system_time_callback_set(ns_time_api_system_time_callback callback)
{
    system_time_callback = callback;
}

#endif /* HAVE_WS */

