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
#include <limits.h>
#include <stdio.h>

#if DEVICE_SLEEP

// deep sleep locking counter. A target is allowed to deep sleep if counter == 0
static uint16_t deep_sleep_lock = 0U;

#ifdef MBED_SLEEP_STATS_ENABLED

// Length of the identifier extracted from the driver name to store for logging.
#define IDENTIFIER_WIDTH 7
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

static size_t sleep_tracker_find_index(const char *const filename)
{
    char temp[IDENTIFIER_WIDTH];
    strncpy(temp, filename, IDENTIFIER_WIDTH);
    temp[IDENTIFIER_WIDTH - 1] = '\0';

    // Search for the a driver matching the current name and return it's index
    for (int i = 0; i < STATISTIC_COUNT; ++i) {
        if (strcmp(sleep_stats[i].identifier, temp) == 0) {
            return i;
        }
    }

    // If no driver was found currently in the structure, find the first array
    // index that hasn't been used by a driver and fill it, then return the
    // index.
    for (int i = 0; i < STATISTIC_COUNT; ++i) {
        if (sleep_stats[i].identifier[0] == '\0') {
            core_util_critical_section_enter();
            strncpy(sleep_stats[i].identifier, temp, sizeof(temp));
            core_util_critical_section_exit();

            return i;
        }
    }

    // Panic if there are no free indexes left to track with
    MBED_ASSERT(true);

    return -1;
}

static void sleep_tracker_print_stats(void)
{
    for (int i = 0; i < STATISTIC_COUNT; ++i) {
        if (sleep_stats[i].count == 0) {
            continue;
        }

        if (sleep_stats[i].identifier[0] == '\0') {
            return;
        }

        printf("[id: %s, count: %u]\r\n", sleep_stats[i].identifier,
                                          sleep_stats[i].count);
    }
}

void sleep_tracker_lock(const char* const filename, int line)
{
    const char* const stripped_path = strip_path(filename);
    size_t index = sleep_tracker_find_index(stripped_path);

    core_util_atomic_incr_u8(&sleep_stats[index].count, 1);

    printf("LOCK: %s, ln: %i, lock count: %u\r\n", stripped_path, line, deep_sleep_lock);
}

void sleep_tracker_unlock(const char* const filename, int line)
{
    const char* const stripped_path = strip_path(filename);
    size_t index = sleep_tracker_find_index(stripped_path);

    core_util_atomic_decr_u8(&sleep_stats[index].count, 1);

    printf("UNLOCK: %s, ln: %i, lock count: %u\r\n", stripped_path, line, deep_sleep_lock);
    sleep_tracker_print_stats();
}

#endif // MBED_SLEEP_STATS_ENABLED

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
