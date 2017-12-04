#include "mbed_stats.h"
#include <string.h>
#include <stdlib.h>
#include "mbed_assert.h"

#if MBED_CONF_RTOS_PRESENT
#include "cmsis_os2.h"
#endif

// note: mbed_stats_heap_get defined in mbed_alloc_wrappers.cpp

void mbed_stats_stack_get(mbed_stats_stack_t *stats)
{
    memset(stats, 0, sizeof(mbed_stats_stack_t));

#if MBED_STACK_STATS_ENABLED && MBED_CONF_RTOS_PRESENT
    uint32_t thread_n = osThreadGetCount();
    unsigned i;
    osThreadId_t *threads;

    threads = malloc(sizeof(osThreadId_t) * thread_n);
    MBED_ASSERT(threads != NULL);

    osKernelLock();
    thread_n = osThreadEnumerate(threads, thread_n);

    for(i = 0; i < thread_n; i++) {
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
    memset(stats, 0, count*sizeof(mbed_stats_stack_t));
    size_t i = 0;

#if MBED_STACK_STATS_ENABLED && MBED_CONF_RTOS_PRESENT
    osThreadId_t *threads;

    threads = malloc(sizeof(osThreadId_t) * count);
    MBED_ASSERT(threads != NULL);

    osKernelLock();
    count = osThreadEnumerate(threads, count);

    for(i = 0; i < count; i++) {
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

#if MBED_STACK_STATS_ENABLED && !MBED_CONF_RTOS_PRESENT
#warning Stack statistics are currently not supported without the rtos.
#endif
