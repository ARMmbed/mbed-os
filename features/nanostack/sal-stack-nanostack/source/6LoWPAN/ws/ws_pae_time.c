/*
 * Copyright (c) 2020-2021, Pelion and affiliates.
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
#include "Service_Libs/utils/ns_time.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wst"

// Wednesday, January 1, 2020 0:00:00 GMT
#define CURRENT_TIME_INIT_VALUE        1577836800

#define SECONDS_IN_WEEK                (7 * 24 * 60 * 60)

static uint64_t ws_pae_current_time = CURRENT_TIME_INIT_VALUE;

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

uint64_t ws_pae_time_old_or_new_select(uint64_t old_time, uint64_t new_time)
{
    // If current time is more than one week in the past use the stored time
    if (old_time > SECONDS_IN_WEEK && new_time < old_time - SECONDS_IN_WEEK) {
        return old_time;
    }

    return new_time;
}

bool ws_pae_time_old_and_new_validate(uint64_t old_time, uint64_t new_time)
{
    /* If new time is more than one week in the past or more than a month in the
       future the old time is not valid */
    if ((old_time > SECONDS_IN_WEEK && new_time < old_time - SECONDS_IN_WEEK) ||
            new_time > (old_time + SYSTEM_TIME_MAXIMUM_DIFF)) {
        return false;
    }

    return true;
}

uint64_t ws_pae_current_time_get(void)
{
    if (!ns_time_system_time_acquired_get()) {
        return ws_pae_current_time;
    }

    uint64_t new_time;
    if (ns_time_system_time_read(&new_time) == 0) {
        new_time = ws_pae_time_old_or_new_select(ws_pae_current_time, new_time);
        return new_time;
    }

    return ws_pae_current_time;
}

void ws_pae_current_time_update(uint16_t seconds)
{
    ws_pae_current_time += seconds;
}

int8_t ws_pae_stored_time_check_and_set(uint64_t stored_time)
{
    uint64_t new_system_time;

    tr_debug("Stored time check and set: %"PRIi64, stored_time);

    if (!ns_time_system_time_acquired_get()) {
        ws_pae_current_time = stored_time;
        return stored_time;
    }

    if (ns_time_system_time_read(&new_system_time) == 0) {
        // Use either stored time or current time as reference when calculating lifetimes
        ws_pae_current_time = ws_pae_time_old_or_new_select(stored_time, new_system_time);
    }
    return 0;
}

int8_t ws_pae_current_time_check_and_set(uint64_t current_time)
{
    uint64_t new_system_time;

    tr_debug("Current time check and set: %"PRIi64, current_time);

    if (ns_time_system_time_read(&new_system_time) == 0) {
        // Use either stored time or current time as reference when calculating lifetimes
        ws_pae_current_time = ws_pae_time_old_or_new_select(current_time, new_system_time);
    }
    return 0;
}

#endif /* HAVE_WS */

