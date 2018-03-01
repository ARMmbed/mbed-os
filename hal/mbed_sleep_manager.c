/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "mbed_assert.h"
#include "mbed_power_mgmt.h"
#include "mbed_critical.h"
#include "sleep_api.h"
#include "mbed_error.h"
#include "mbed_debug.h"
#include <limits.h>
#include <stdio.h>

#if DEVICE_SLEEP

// deep sleep locking counter. A target is allowed to deep sleep if counter == 0
static uint16_t deep_sleep_lock = 0U;

#ifdef MBED_SLEEP_TRACING_ENABLED

// Length of the identifier extracted from the driver name to store for logging.
#define IDENTIFIER_WIDTH 15
// Number of drivers that can be stored in the structure
#define STATISTIC_COUNT  10

typedef struct sleep_statistic {
    char identifier[IDENTIFIER_WIDTH];
    uint8_t count;
} sleep_statistic_t;

static sleep_statistic_t sleep_stats[STATISTIC_COUNT];

static const char* strip_path(const char* const filename)
{
    char *output = strrchr(filename, '/');

    if (output != NULL) {
        return output + 1;
    }

    output = strrchr(filename, '\\');

    if (output != NULL) {
        return output + 1;
    }

    return filename;
}

static sleep_statistic_t* sleep_tracker_find(const char *const filename)
{
    char temp[IDENTIFIER_WIDTH];
    strncpy(temp, filename, IDENTIFIER_WIDTH);
    temp[IDENTIFIER_WIDTH - 1] = '\0';

    // Search for the a driver matching the current name and return it's index
    for (int i = 0; i < STATISTIC_COUNT; ++i) {
        if (strcmp(sleep_stats[i].identifier, temp) == 0) {
            return &sleep_stats[i];
        }
    }

    return NULL;
}

static sleep_statistic_t* sleep_tracker_add(const char* const filename)
{
    char temp[IDENTIFIER_WIDTH];
    strncpy(temp, filename, IDENTIFIER_WIDTH);
    temp[IDENTIFIER_WIDTH - 1] = '\0';

    for (int i = 0; i < STATISTIC_COUNT; ++i) {
        if (sleep_stats[i].identifier[0] == '\0') {
            core_util_critical_section_enter();
            strncpy(sleep_stats[i].identifier, temp, sizeof(temp));
            core_util_critical_section_exit();

            return &sleep_stats[i];
        }
    }

    debug("No free indexes left to use in mbed sleep tracker.\r\n");

    return NULL;
}

static void sleep_tracker_print_stats(void)
{
    debug("Sleep locks held:\r\n");
    for (int i = 0; i < STATISTIC_COUNT; ++i) {
        if (sleep_stats[i].count == 0) {
            continue;
        }

        if (sleep_stats[i].identifier[0] == '\0') {
            return;
        }

        debug("[id: %s, count: %u]\r\n", sleep_stats[i].identifier,
                                          sleep_stats[i].count);
    }
}

void sleep_tracker_lock(const char* const filename, int line)
{
    const char* const stripped_path = strip_path(filename);

    sleep_statistic_t* stat = sleep_tracker_find(stripped_path);

    // Entry for this driver does not exist, create one.
    if (stat == NULL) {
        stat = sleep_tracker_add(stripped_path);
    }

    core_util_atomic_incr_u8(&stat->count, 1);

    debug("LOCK: %s, ln: %i, lock count: %u\r\n", stripped_path, line, deep_sleep_lock);
}

void sleep_tracker_unlock(const char* const filename, int line)
{
    const char* const stripped_path = strip_path(filename);
    sleep_statistic_t* stat = sleep_tracker_find(stripped_path);

    // Entry for this driver does not exist, something went wrong.
    if (stat == NULL) {
        debug("Unlocking sleep for driver that was not previously locked: %s, ln: %i\r\n", stripped_path, line);
        return;
    }

    core_util_atomic_decr_u8(&stat->count, 1);

    debug("UNLOCK: %s, ln: %i, lock count: %u\r\n", stripped_path, line, deep_sleep_lock);
}

#endif // MBED_SLEEP_TRACING_ENABLED

void sleep_manager_lock_deep_sleep_internal(void)
{
    core_util_critical_section_enter();
    if (deep_sleep_lock == USHRT_MAX) {
        core_util_critical_section_exit();
        error("Deep sleep lock would overflow (> USHRT_MAX)");
    }
    core_util_atomic_incr_u16(&deep_sleep_lock, 1);
    core_util_critical_section_exit();
}

void sleep_manager_unlock_deep_sleep_internal(void)
{
    core_util_critical_section_enter();
    if (deep_sleep_lock == 0) {
        core_util_critical_section_exit();
        error("Deep sleep lock would underflow (< 0)");
    }
    core_util_atomic_decr_u16(&deep_sleep_lock, 1);
    core_util_critical_section_exit();
}

bool sleep_manager_can_deep_sleep(void)
{
    return deep_sleep_lock == 0 ? true : false;
}

void sleep_manager_sleep_auto(void)
{
#ifdef MBED_SLEEP_TRACING_ENABLED
    sleep_tracker_print_stats();
#endif
    core_util_critical_section_enter();
// debug profile should keep debuggers attached, no deep sleep allowed
#ifdef MBED_DEBUG
    hal_sleep();
#else
    if (sleep_manager_can_deep_sleep()) {
        hal_deepsleep();
    } else {
        hal_sleep();
    }
#endif
    core_util_critical_section_exit();
}

#else

// locking is valid only if DEVICE_SLEEP is defined
// we provide empty implementation

void sleep_manager_lock_deep_sleep_internal(void)
{

}

void sleep_manager_unlock_deep_sleep_internal(void)
{

}

bool sleep_manager_can_deep_sleep(void)
{
    // no sleep implemented
    return false;
}

#endif
