/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#include "platform/mbed_assert.h"
#include "platform/mbed_stats.h"
#include "platform/mbed_power_mgmt.h"
#include "platform/mbed_version.h"
#include <string.h>
#include <stdlib.h>

#include "device.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "cmsis_os2.h"
#elif defined(MBED_STACK_STATS_ENABLED) || defined(MBED_THREAD_STATS_ENABLED)
#warning Statistics are currently not supported without the rtos.
#endif

#if defined(MBED_CPU_STATS_ENABLED) && (!DEVICE_SLEEP)
#warning CPU statistics are not supported without sleep support.
#endif

void mbed_stats_cpu_get(mbed_stats_cpu_t *stats)
{
    MBED_ASSERT(stats != NULL);
    memset(stats, 0, sizeof(mbed_stats_cpu_t));
#if defined(MBED_CPU_STATS_ENABLED) && DEVICE_LPTICKER && DEVICE_SLEEP
    stats->uptime = mbed_uptime();
    stats->idle_time = mbed_time_idle();
    stats->sleep_time = mbed_time_sleep();
    stats->deep_sleep_time = mbed_time_deepsleep();
#endif
}

// note: mbed_stats_heap_get defined in mbed_alloc_wrappers.cpp
void mbed_stats_stack_get(mbed_stats_stack_t *stats)
{
    MBED_ASSERT(stats != NULL);
    memset(stats, 0, sizeof(mbed_stats_stack_t));

#if defined(MBED_STACK_STATS_ENABLED) && defined(MBED_CONF_RTOS_PRESENT)
    uint32_t thread_n = osThreadGetCount();
    unsigned i;
    osThreadId_t *threads;

    threads = malloc(sizeof(osThreadId_t) * thread_n);
    // Don't fail on lack of memory
    if (!threads) {
        return;
    }

    osKernelLock();
    thread_n = osThreadEnumerate(threads, thread_n);

    for (i = 0; i < thread_n; i++) {
        uint32_t stack_size = osThreadGetStackSize(threads[i]);
        stats->max_size += stack_size - osThreadGetStackSpace(threads[i]);
        stats->reserved_size += stack_size;
        stats->stack_cnt++;
    }
    osKernelUnlock();

    free(threads);
#endif
}

size_t mbed_stats_stack_get_each(mbed_stats_stack_t *stats, size_t count)
{
    MBED_ASSERT(stats != NULL);
    memset(stats, 0, count * sizeof(mbed_stats_stack_t));

    size_t i = 0;

#if defined(MBED_STACK_STATS_ENABLED) && defined(MBED_CONF_RTOS_PRESENT)
    osThreadId_t *threads;

    threads = malloc(sizeof(osThreadId_t) * count);
    // Don't fail on lack of memory
    if (!threads) {
        return 0;
    }

    osKernelLock();
    count = osThreadEnumerate(threads, count);

    for (i = 0; i < count; i++) {
        uint32_t stack_size = osThreadGetStackSize(threads[i]);
        stats[i].max_size = stack_size - osThreadGetStackSpace(threads[i]);
        stats[i].reserved_size = stack_size;
        stats[i].thread_id = (uint32_t)threads[i];
        stats[i].stack_cnt = 1;
    }
    osKernelUnlock();

    free(threads);
#endif

    return i;
}

size_t mbed_stats_thread_get_each(mbed_stats_thread_t *stats, size_t count)
{
    MBED_ASSERT(stats != NULL);
    memset(stats, 0, count * sizeof(mbed_stats_thread_t));
    size_t i = 0;

#if defined(MBED_THREAD_STATS_ENABLED) && defined(MBED_CONF_RTOS_PRESENT)
    osThreadId_t *threads;

    threads = malloc(sizeof(osThreadId_t) * count);
    MBED_ASSERT(threads != NULL);

    osKernelLock();
    count = osThreadEnumerate(threads, count);

    for (i = 0; i < count; i++) {
        stats[i].id = (uint32_t)threads[i];
        stats[i].state = (uint32_t)osThreadGetState(threads[i]);
        stats[i].priority = (uint32_t)osThreadGetPriority(threads[i]);
        stats[i].stack_size = osThreadGetStackSize(threads[i]);
        stats[i].stack_space = osThreadGetStackSpace(threads[i]);
        stats[i].name = osThreadGetName(threads[i]);
    }
    osKernelUnlock();
    free(threads);
#endif
    return i;
}

void mbed_stats_sys_get(mbed_stats_sys_t *stats)
{
    MBED_ASSERT(stats != NULL);
    memset(stats, 0, sizeof(mbed_stats_sys_t));

#if defined(MBED_SYS_STATS_ENABLED)
    stats->os_version = MBED_VERSION;
#if defined(MBED_RAM_START) && defined(MBED_RAM_SIZE)
    stats->ram_start[0] = MBED_RAM_START;
    stats->ram_size[0] = MBED_RAM_SIZE;
#endif
#if defined(MBED_ROM_START) && defined(MBED_ROM_SIZE)
    stats->rom_start[0] = MBED_ROM_START;
    stats->rom_size[0] = MBED_ROM_SIZE;
#endif
#if defined(MBED_RAM1_START) && defined(MBED_RAM1_SIZE)
    stats->ram_start[1] = MBED_RAM1_START;
    stats->ram_size[1] = MBED_RAM1_SIZE;
#endif
#if defined(MBED_RAM2_START) && defined(MBED_RAM2_SIZE)
    stats->ram_start[2] = MBED_RAM2_START;
    stats->ram_size[2] = MBED_RAM2_SIZE;
#endif
#if defined(MBED_RAM3_START) && defined(MBED_RAM3_SIZE)
    stats->ram_start[3] = MBED_RAM3_START;
    stats->ram_size[3] = MBED_RAM3_SIZE;
#endif
#if defined(MBED_ROM1_START) && defined(MBED_ROM1_SIZE)
    stats->rom_start[1] = MBED_ROM1_START;
    stats->rom_size[1] = MBED_ROM1_SIZE;
#endif
#if defined(MBED_ROM2_START) && defined(MBED_ROM2_SIZE)
    stats->rom_start[2] = MBED_ROM2_START;
    stats->rom_size[2] = MBED_ROM2_SIZE;
#endif
#if defined(MBED_ROM3_START) && defined(MBED_ROM3_SIZE)
    stats->rom_start[3] = MBED_ROM3_START;
    stats->rom_size[3] = MBED_ROM3_SIZE;
#endif

#if defined(__CORTEX_M)
    stats->cpu_id = SCB->CPUID;
#endif
#if defined(__IAR_SYSTEMS_ICC__)
    stats->compiler_id = IAR;
    stats->compiler_version = __VER__;
#elif defined(__ARMCC_VERSION)
    stats->compiler_id = ARM;
    stats->compiler_version = __ARMCC_VERSION;
#elif defined(__GNUC__)
    stats->compiler_id = GCC_ARM;
    stats->compiler_version = (__GNUC__ * 10000 + __GNUC_MINOR__ * 100);
#endif

#endif
    return;
}
