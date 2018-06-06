#include "mbed_assert.h"
#include "mbed_stats.h"
#include "mbed_power_mgmt.h"
#include <string.h>
#include <stdlib.h>

#include "device.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "cmsis_os2.h"
#include "rtos_idle.h"
#elif defined(MBED_STACK_STATS_ENABLED) || defined(MBED_THREAD_STATS_ENABLED) || defined(MBED_CPU_STATS_ENABLED)
#warning Statistics are currently not supported without the rtos.
#endif

#if defined(MBED_CPU_STATS_ENABLED) && (!defined(DEVICE_LPTICKER) || !defined(DEVICE_SLEEP))
#warning CPU statistics are not supported without low power timer support.
#endif

void mbed_stats_cpu_get(mbed_stats_cpu_t *stats)
{
    MBED_ASSERT(stats != NULL);
    memset(stats, 0, sizeof(mbed_stats_cpu_t));
#if defined(MBED_CPU_STATS_ENABLED) && defined(DEVICE_LPTICKER) && defined(DEVICE_SLEEP)
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
    MBED_ASSERT(threads != NULL);

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
    MBED_ASSERT(threads != NULL);

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
#if defined(__CORTEX_M)
    stats->cpu_id = SCB->CPUID;
#endif
#if defined(__IAR_SYSTEMS_ICC__)
    stats->compiler_id = IAR;
    stats->compiler_version = __VER__;
#elif defined(__CC_ARM)
    stats->compiler_id = ARM;
    stats->compiler_version = __ARMCC_VERSION;
#elif defined(__GNUC__)
    stats->compiler_id = GCC_ARM;
    stats->compiler_version = (__GNUC__ * 10000 + __GNUC_MINOR__ * 100);
#endif

#endif
    return;
}
